/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Microchip RSTC (Reset controller) register offsets and bit definitions.
 *
 * Copyright (C) 2020 Microchip Technology, Inc. and its subsidiaries
 *
 * Author: Eugen Hristev <eugen.hristev@microchip.com>
 */

#ifndef _LINUX_MFD_SYSCON_MICROCHIP_RSTC_H
#define _LINUX_MFD_SYSCON_MICROCHIP_RSTC_H

#define AT91_RSTC_GRSTR			0xE4
#define		AT91_GRSTR_USB_RST1	BIT(4)
#define		AT91_GRSTR_USB_RST2	BIT(5)
#define		AT91_GRSTR_USB_RST3	BIT(6)

#endif /* _LINUX_MFD_SYSCON_MICROCHIP_RSTC_H */
