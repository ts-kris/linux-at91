/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Microchip SFR (Special Function Registers) registers for SAMA7G5 family.
 *
 * Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries
 *
 * Author: Cristian Birsan <cristian.birsan@microchip.com>
 */

#ifndef _LINUX_MFD_SYSCON_AT91_SAMA7G5_SFR_H
#define _LINUX_MFD_SYSCON_AT91_SAMA7G5_SFR_H

#define SAMA7G5_SFR_OHCIICR	0x00	/* OHCI INT Configuration Register */
#define SAMA7G5_SFR_OHCIISR	0x04	/* OHCI INT Status Register */
/* 0x08 ~ 0xe3: Reserved */
#define SAMA7G5_SFR_WPMR	0xe4	/* Write Protection Mode Register */
#define SAMA7G5_SFR_WPSR	0xe4	/* Write Protection Status Register */
/* 0xec ~ 0x200b: Reserved */
#define SAMA7G5_SFR_DEBUG	0x200c	/* Debug Register */
/* 0x2010 ~ 0x2027: Reserved */
#define SAMA7G5_SFR_HSS_AXI_QOS	0x2028	/* HSS AXI QOS Register */
#define SAMA7G5_SFR_UDDRC	0x202c  /* UDDRC Register */
#define SAMA7G5_SFR_CAN_SRAM_SEL        0x2030	/* CAN SRAM Select. Register */
/* 0x2034 ~ 0x203f: Reserved */
#define SAMA7G5_SFR_UTMI0R0	0x2040	/* UTMI0 Configuration Register */
#define SAMA7G5_SFR_UTMI0R1	0x2044	/* UTMI1 Configuration Register */
#define SAMA7G5_SFR_UTMI0R2	0x2048	/* UTMI2 Configuration Register */

/* Field definitions */
#define SAMA7G5_SFR_OHCIICR_ARIE		BIT(0)
#define SAMA7G5_SFR_OHCIICR_APPSTART		BIT(1)
#define SAMA7G5_SFR_OHCIICR_USB_SUSP(x)		BIT(8 + (x))
#define SAMA7G5_SFR_OHCIICR_USB_SUSPEND		GENMASK(10, 8)

#define SAMA7G5_SFR_OHCIISR_RIS(x)		BIT(x)

#define SAMA7G5_SFR_WPMR_WPEN			BIT(0)
#define SAMA7G5_SFR_WPMR_KEY			0x53465200 /* SFR in ASCII*/
#define SAMA7G5_SFR_WPMR_WPKEY_MASK		GENMASK(31, 8)

#define SAMA7G5_SFR_WPSR_WPSRC_MASK		GENMASK(23, 8)
#define SAMA7G5_SFR_WPSR_WPVS_MASK		BIT(0)

#define SAMA7G5_SFR_CAN_SRAM_UPPER(x)		BIT(x)

#define SAMA7G5_SFR_UTMI_RX_VBUS		BIT(25) /* VBUS Valid bit */
#define SAMA7G5_SFR_UTMI_RX_TXPREEMPAMPTUNE_1X	BIT(23) /* TXPREEMPAMPTUNE 1x */

#endif /* _LINUX_MFD_SYSCON_AT91_SAMA7G5_SFR_H */
