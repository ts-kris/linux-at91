// SPDX-License-Identifier: GPL-2.0
//
// Copyright (C) 2019 Microchip Technology Inc.
// Copyright (C) 2019 Claudiu Beznea (claudiu.beznea@microchip.com)

#include <linux/clk.h>
#include <linux/clockchips.h>
#include <linux/interrupt.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/sched_clock.h>
#include <linux/slab.h>

#define MCHP_PIT64_CR		0x00	/* Control Register */
#define MCHP_PIT64_CR_START	BIT(0)
#define MCHP_PIT64_CR_SWRST	BIT(8)

#define MCHP_PIT64_MR		0x04	/* Mode Register */
#define MCHP_PIT64_MR_CONT	BIT(0)
#define MCHP_PIT64_MR_SGCLK	BIT(3)
#define MCHP_PIT64_MR_SMOD	BIT(4)
#define MCHP_PIT64_MR_PRES	GENMASK(11, 8)

#define MCHP_PIT64_LSB_PR	0x08	/* LSB Period Register */

#define MCHP_PIT64_MSB_PR	0x0C	/* MSB Period Register */

#define MCHP_PIT64_IER		0x10	/* Interrupt Enable Register */
#define MCHP_PIT64_IER_PERIOD	BIT(0)
#define MCHP_PIT64_IER_OVRE	BIT(1)
#define MCHP_PIT64_IER_SECE	BIT(4)

#define MCHP_PIT64_IDR		0x14	/* Interrupt Disable Register */
#define MCHP_PIT64_IDR_PERIOD	BIT(0)
#define MCHP_PIT64_IDR_OVRE	BIT(1)
#define MCHP_PIT64_IDR_SECE	BIT(4)

#define MCHP_PIT64_IMR		0x18	/* Interrupt Mask Register */
#define MCHP_PIT64_IMR_PERIOD	BIT(0)
#define MCHP_PIT64_IMR_OVRE	BIT(1)
#define MCHP_PIT64_IMR_SECE	BIT(4)

#define MCHP_PIT64_ISR		0x1C	/* Interrupt Status Register */
#define MCHP_PIT64_ISR_PERIOD	BIT(0)
#define MCHP_PIT64_ISR_OVRE	BIT(1)
#define MCHP_PIT64_ISR_SECE	BIT(4)

#define MCHP_PIT64_TLSBR	0x20	/* Timer LSB Register */

#define MCHP_PIT64_TMSBR	0x24	/* Timer MSB Register */

#define MCHP_PRES_MAX		0x10
#define MCHP_PIT64_CS_RATE	2500000UL	/* 2.5 MHz */
#define MCHP_PIT64_CE_RATE	2500000UL	/* 2.5 MHz */
#define MCHP_PIT64_LSBMASK	GENMASK_ULL(31, 0)
#define MCHP_PIT64_PRESCALER(p)	(MCHP_PIT64_MR_PRES & ((p) << 8))

#define mchp_pit64_name		"pit64"

enum mchp_pit64_mode {
	MCHP_PIT64_MODE_CLKSRC,
	MCHP_PIT64_MODE_CLKEVT,
};

struct mchp_pit64_common_data {
	void __iomem *base;
	struct clk *clk;
	u64 cycles;
	u8 pres;
};

struct mchp_pit64_clksrc_data {
	struct clocksource *clksrc;
	struct mchp_pit64_common_data cd;
};

struct mchp_pit64_clkevt_data {
	struct clock_event_device *clkevt;
	struct mchp_pit64_common_data cd;
};

static struct mchp_pit64_data {
	struct mchp_pit64_clksrc_data *csd;
	struct mchp_pit64_clkevt_data *ced;
} data;

static inline u32 mchp_pit64_read(void __iomem *base, u32 offset)
{
	return readl_relaxed(base + offset);
}

static inline void mchp_pit64_write(void __iomem *base, u32 offset, u32 val)
{
	writel_relaxed(val, base + offset);
}

static inline u64 mchp_pit64_get_period(void __iomem *base)
{
	u32 lsb, msb;

	/* LSB must be read first to guarantee an atomic read of the 64 bit
	 * timer.
	 */
	lsb = mchp_pit64_read(base, MCHP_PIT64_TLSBR);
	msb = mchp_pit64_read(base, MCHP_PIT64_TMSBR);

	return (((u64)msb << 32) | lsb);
}

static inline void mchp_pit64_set_period(void __iomem *base, u64 cycles)
{
	u32 lsb, msb;

	lsb = cycles & MCHP_PIT64_LSBMASK;
	msb = cycles >> 32;

	/* LSB must be write last to guarantee an atomic update of the timer
	 * even when SMOD=1.
	 */
	mchp_pit64_write(base, MCHP_PIT64_MSB_PR, msb);
	mchp_pit64_write(base, MCHP_PIT64_LSB_PR, lsb);
}

static inline void mchp_pit64_reset(struct mchp_pit64_common_data *data,
				    u32 mode, bool irq_ena)
{
	mode |= MCHP_PIT64_PRESCALER(data->pres - 1);

	mchp_pit64_write(data->base, MCHP_PIT64_CR, MCHP_PIT64_CR_SWRST);
	mchp_pit64_write(data->base, MCHP_PIT64_MR, mode);
	mchp_pit64_set_period(data->base, data->cycles);
	if (irq_ena)
		mchp_pit64_write(data->base, MCHP_PIT64_IER,
				 MCHP_PIT64_IER_PERIOD);
	mchp_pit64_write(data->base, MCHP_PIT64_CR, MCHP_PIT64_CR_START);
}

static u64 mchp_pit64_read_clk(struct clocksource *cs)
{
	return mchp_pit64_get_period(data.csd->cd.base);
}

static u64 mchp_sched_read_clk(void)
{
	return mchp_pit64_get_period(data.csd->cd.base);
}

static struct clocksource mchp_clksrc = {
	.name = mchp_pit64_name,
	.mask = CLOCKSOURCE_MASK(64),
	.flags = CLOCK_SOURCE_IS_CONTINUOUS,
	.rating = 250,
	.read = mchp_pit64_read_clk,
};

static int mchp_pit64_clkevt_shutdown(struct clock_event_device *cedev)
{
	mchp_pit64_write(data.ced->cd.base, MCHP_PIT64_CR, MCHP_PIT64_CR_SWRST);

	return 0;
}

static int mchp_pit64_clkevt_set_periodic(struct clock_event_device *cedev)
{
	mchp_pit64_reset(&data.ced->cd, MCHP_PIT64_MR_CONT, true);

	return 0;
}

static int mchp_pit64_clkevt_set_oneshot(struct clock_event_device *cedev)
{
	mchp_pit64_reset(&data.ced->cd, MCHP_PIT64_MR_SMOD, true);

	return 0;
}

static int mchp_pit64_clkevt_set_next_event(unsigned long evt,
					    struct clock_event_device *cedev)
{
	mchp_pit64_set_period(data.ced->cd.base, evt);
	mchp_pit64_write(data.ced->cd.base, MCHP_PIT64_CR, MCHP_PIT64_CR_START);

	return 0;
}

static void mchp_pit64_clkevt_suspend(struct clock_event_device *cedev)
{
	mchp_pit64_write(data.ced->cd.base, MCHP_PIT64_CR, MCHP_PIT64_CR_SWRST);
	clk_disable_unprepare(data.ced->cd.clk);
}

static void mchp_pit64_clkevt_resume(struct clock_event_device *cedev)
{
	u32 mode = MCHP_PIT64_MR_SMOD;

	if (clk_prepare_enable(data.ced->cd.clk)) {
		pr_err("clkevt: Failed to enable clk on resume!\n");
		return;
	}

	if (clockevent_state_periodic(data.ced->clkevt))
		mode = MCHP_PIT64_MR_CONT;

	mchp_pit64_reset(&data.ced->cd, mode, true);
}

static struct clock_event_device mchp_clkevt = {
	.name = mchp_pit64_name,
	.features = CLOCK_EVT_FEAT_ONESHOT | CLOCK_EVT_FEAT_PERIODIC,
	.rating = 250,
	.set_state_shutdown = mchp_pit64_clkevt_shutdown,
	.set_state_periodic = mchp_pit64_clkevt_set_periodic,
	.set_state_oneshot = mchp_pit64_clkevt_set_oneshot,
	.set_next_event = mchp_pit64_clkevt_set_next_event,
	.suspend = mchp_pit64_clkevt_suspend,
	.resume = mchp_pit64_clkevt_resume,
};

static irqreturn_t mchp_pit64_interrupt(int irq, void *dev_id)
{
	struct mchp_pit64_clkevt_data *irq_data = dev_id;

	if (data.ced != irq_data)
		return IRQ_NONE;

	if (mchp_pit64_read(irq_data->cd.base, MCHP_PIT64_ISR) &
	    MCHP_PIT64_ISR_PERIOD) {
		irq_data->clkevt->event_handler(irq_data->clkevt);
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

static u16 __init mchp_pit64_pres_compute(unsigned long clk_rate,
					  unsigned long long max_rate)
{
	u32 tmp, pres;

	for (pres = 0; pres < MCHP_PRES_MAX; pres++) {
		tmp = clk_rate / (pres + 1);
		pr_err("%s(): pres=%u, clk_rate=%lu, max_rate=%llu\n", __func__, pres, clk_rate, max_rate);
		if (tmp <= max_rate)
			break;
	}

	/*
	 * Use the last prescaller in case we don't locate one for max_rate Hz.
	 */
	if (pres == MCHP_PRES_MAX)
		pres = MCHP_PRES_MAX - 1;

	return pres;
}

static int __init mchp_pit64_dt_init_clksrc(void __iomem *base, struct clk *clk)
{
	struct mchp_pit64_clksrc_data *csd;
	unsigned long clk_rate;
	int ret;

	csd = kzalloc(sizeof(*csd), GFP_KERNEL);
	if (!csd)
		return -ENOMEM;

	csd->cd.base = base;
	csd->cd.clk = clk;

	ret = clk_prepare_enable(csd->cd.clk);
	if (ret) {
		pr_err("clksrc: Failed to enable PIT64's clock!\n");
		goto free;
	}

	clk_rate = clk_get_rate(csd->cd.clk);
	csd->cd.pres = mchp_pit64_pres_compute(clk_rate, MCHP_PIT64_CS_RATE);
	clk_rate = clk_rate / csd->cd.pres;
	csd->cd.cycles = ULLONG_MAX;

	mchp_pit64_reset(&csd->cd, MCHP_PIT64_MR_CONT, false);

	data.csd = csd;

	csd->clksrc = &mchp_clksrc;

	ret = clocksource_register_hz(csd->clksrc, clk_rate);
	if (ret) {
		pr_err("clksrc: Failed to register PIT64 clocksource!\n");
		goto clk_unprepare;
	}

	sched_clock_register(mchp_sched_read_clk, 64, clk_rate);

	return 0;

clk_unprepare:
	clk_disable_unprepare(csd->cd.clk);
free:
	kfree(csd);
	data.csd = NULL;

	return ret;
}

static int __init mchp_pit64_dt_init_clkevt(void __iomem *base, struct clk *clk,
					    u32 irq)
{
	struct mchp_pit64_clkevt_data *ced;
	unsigned long clk_rate;
	int ret;

	ced = kzalloc(sizeof(*ced), GFP_KERNEL);
	if (!ced)
		return -ENOMEM;

	ced->cd.base = base;
	ced->cd.clk = clk;

	ret = clk_prepare_enable(ced->cd.clk);
	if (ret) {
		pr_err("clkevt: Failed to enable PIT64's clock!\n");
		goto free;
	}

	clk_rate = clk_get_rate(ced->cd.clk);
	ced->cd.pres = mchp_pit64_pres_compute(clk_rate, MCHP_PIT64_CE_RATE);
	clk_rate = clk_rate / ced->cd.pres;
	ced->cd.cycles = DIV_ROUND_CLOSEST(clk_rate, HZ);

	ret = request_irq(irq, mchp_pit64_interrupt, IRQF_TIMER, "pit64_tick",
			  ced);
	if (ret) {
		pr_err("clkevt: Failed to setup PIT64 IRQ\n");
		goto clk_unprepare;
	}

	data.ced = ced;

	/* Set up and register clockevents. */
	ced->clkevt = &mchp_clkevt;
	ced->clkevt->cpumask = cpumask_of(0);
	ced->clkevt->irq = irq;
	clockevents_config_and_register(ced->clkevt, clk_rate, 1, ULONG_MAX);

	return 0;

clk_unprepare:
	clk_disable_unprepare(ced->cd.clk);
free:
	kfree(ced);
	data.ced = NULL;

	return ret;
}

static int __init mchp_pit64_dt_init(struct device_node *node,
				     enum mchp_pit64_mode mode)
{
	const char *name = node->name ? node->name : "pit64";
	void __iomem *base;
	struct clk *clk;
	u32 irq;
	int ret;

	base = of_iomap(node, 0);
	if (!base) {
		pr_err("%s: Could not map PIT64 address!\n", name);
		return -ENXIO;
	}

	clk = of_clk_get(node, 0);
	if (IS_ERR(clk)) {
		pr_err("%s: Failed to get PIT64's clock!\n", name);
		ret = PTR_ERR(clk);
		goto unmap;
	}

	switch (mode) {
	case MCHP_PIT64_MODE_CLKSRC:
		if (data.csd) {
			ret = -EBUSY;
			goto clock_put;
		}

		ret = mchp_pit64_dt_init_clksrc(base, clk);
		if (ret)
			goto clock_put;

		break;

	case MCHP_PIT64_MODE_CLKEVT:
		if (data.ced) {
			ret = -EBUSY;
			goto clock_put;
		}

		irq = irq_of_parse_and_map(node, 0);
		if (!irq) {
			pr_err("%s: Failed to get PIT64 clockevent IRQ!\n",
			       name);
			ret = -ENODEV;
			goto clock_put;
		}

		ret = mchp_pit64_dt_init_clkevt(base, clk, irq);
		if (ret)
			goto irq_unmap;

		break;

	default:
		ret = -EINVAL;
		goto clock_put;
	}

	return 0;

irq_unmap:
	irq_dispose_mapping(irq);
clock_put:
	clk_put(clk);
unmap:
	iounmap(base);

	return ret;
}

static int __init mchp_pit64_clksrc_dt_init(struct device_node *node)
{
	return mchp_pit64_dt_init(node, MCHP_PIT64_MODE_CLKSRC);
}

static int __init mchp_pit64_clkevt_dt_init(struct device_node *node)
{
	return mchp_pit64_dt_init(node, MCHP_PIT64_MODE_CLKEVT);
}

CLOCKSOURCE_OF_DECLARE(mchp_pit64_clksrc, "microchip,pit64-clksrc",
		       mchp_pit64_clksrc_dt_init);
CLOCKSOURCE_OF_DECLARE(mchp_pit64_clkevt, "microchip,pit64-clkevt",
		       mchp_pit64_clkevt_dt_init);
