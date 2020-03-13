/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Header file for the Atmel Extensible DMA Controller (aka XDMAC on AT91 systems)
 *
 * Copyright (C) 2014-2020 Microchip Technology, Inc. and its subsidiaries
 *
 */
#ifndef AT_XDMAC_REGS_H
#define	AT_XDMAC_REGS_H

/* Register map offsets compared to older versions of this regmap */
#if IS_ENABLED(CONFIG_AT_XDMAC_SAMA7G5)
#define AT_XDMAC_SAMA7G5_OFF1	0x8
#define AT_XDMAC_SAMA7G5_OFF2	0xC
#define AT_XDMAC_SAMA7G5_OFF3	0x10
#else
#define AT_XDMAC_SAMA7G5_OFF1	0x0
#define AT_XDMAC_SAMA7G5_OFF2	0x0
#define AT_XDMAC_SAMA7G5_OFF3	0x0
#endif

/* Global registers */
#define AT_XDMAC_GTYPE		0x00	/* Global Type Register */
#define		AT_XDMAC_NB_CH(i)	(((i) & 0x1F) + 1)		/* Number of Channels Minus One */
#define		AT_XDMAC_FIFO_SZ(i)	(((i) >> 5) & 0x7FF)		/* Number of Bytes */
#define		AT_XDMAC_NB_REQ(i)	((((i) >> 16) & 0x3F) + 1)	/* Number of Peripheral Requests Minus One */
#define AT_XDMAC_GCFG		0x04	/* Global Configuration Register */
#define		AT_XDMAC_WRHP(i)	(((i) & 0xF) << 4)
#define		AT_XDMAC_WRMP(i)	(((i) & 0xF) << 8)
#define		AT_XDMAC_WRLP(i)	(((i) & 0xF) << 12)
#define		AT_XDMAC_RDHP(i)	(((i) & 0xF) << 16)
#define		AT_XDMAC_RDMP(i)	(((i) & 0xF) << 20)
#define		AT_XDMAC_RDLP(i)	(((i) & 0xF) << 24)
#define		AT_XDMAC_RDSG(i)	(((i) & 0xF) << 28)
#if IS_ENABLED(CONFIG_AT_XDMAC_SAMA7G5)
#define AT_XDMAC_GCFG_M2M	(AT_XDMAC_RDLP(0xF) | AT_XDMAC_WRLP(0xF))
#define AT_XDMAC_GCFG_P2M	(AT_XDMAC_RDSG(0x1) | AT_XDMAC_RDHP(0x3) | \
				AT_XDMAC_WRHP(0x5))
#else
#define AT_XDMAC_GCFG_M2M	0
#define AT_XDMAC_GCFG_P2M	0
#endif
#define AT_XDMAC_GWAC		0x08	/* Global Weighted Arbiter Configuration Register */
#define		AT_XDMAC_PW0(i)		(((i) & 0xF) << 0)
#define		AT_XDMAC_PW1(i)		(((i) & 0xF) << 4)
#define		AT_XDMAC_PW2(i)		(((i) & 0xF) << 8)
#define		AT_XDMAC_PW3(i)		(((i) & 0xF) << 12)
#if IS_ENABLED(CONFIG_AT_XDMAC_SAMA7G5)
#define AT_XDMAC_GWAC_M2M	0
#define AT_XDMAC_GWAC_P2M	(AT_XDMAC_PW0(0xF) | AT_XDMAC_PW2(0xF))
#else
#define AT_XDMAC_GWAC_M2M	0
#define AT_XDMAC_GWAC_P2M	0
#endif
#define AT_XDMAC_GIE		0x0C	/* Global Interrupt Enable Register */
#define AT_XDMAC_GID		0x10	/* Global Interrupt Disable Register */
#define AT_XDMAC_GIM		0x14	/* Global Interrupt Mask Register */
#define AT_XDMAC_GIS		0x18	/* Global Interrupt Status Register */
#define AT_XDMAC_GE		0x1C	/* Global Channel Enable Register */
#define AT_XDMAC_GD		0x20	/* Global Channel Disable Register */
#define AT_XDMAC_GS		0x24	/* Global Channel Status Register */
/* Global Channel Read Suspend Register */
#define AT_XDMAC_GRS		(0x28 + AT_XDMAC_SAMA7G5_OFF1)
/* Global Write Suspend Register */
#define AT_XDMAC_GWS		(0x2C + AT_XDMAC_SAMA7G5_OFF2)
/* Global Channel Read Write Suspend Register */
#define AT_XDMAC_GRWS		(0x30 + AT_XDMAC_SAMA7G5_OFF3)
/* Global Channel Read Write Resume Register */
#define AT_XDMAC_GRWR		(0x34 + AT_XDMAC_SAMA7G5_OFF3)
/* Global Channel Software Request Register */
#define AT_XDMAC_GSWR		(0x38 + AT_XDMAC_SAMA7G5_OFF3)
/* Global channel Software Request Status Register */
#define AT_XDMAC_GSWS		(0x3C + AT_XDMAC_SAMA7G5_OFF3)
/* Global Channel Software Flush Request Register */
#define AT_XDMAC_GSWF		(0x40 + AT_XDMAC_SAMA7G5_OFF3)
#define AT_XDMAC_VERSION	0xFFC	/* XDMAC Version Register */

/* Channel relative registers offsets */
#define AT_XDMAC_CIE		0x00	/* Channel Interrupt Enable Register */
#define		AT_XDMAC_CIE_BIE	BIT(0)	/* End of Block Interrupt Enable Bit */
#define		AT_XDMAC_CIE_LIE	BIT(1)	/* End of Linked List Interrupt Enable Bit */
#define		AT_XDMAC_CIE_DIE	BIT(2)	/* End of Disable Interrupt Enable Bit */
#define		AT_XDMAC_CIE_FIE	BIT(3)	/* End of Flush Interrupt Enable Bit */
#define		AT_XDMAC_CIE_RBEIE	BIT(4)	/* Read Bus Error Interrupt Enable Bit */
#define		AT_XDMAC_CIE_WBEIE	BIT(5)	/* Write Bus Error Interrupt Enable Bit */
#define		AT_XDMAC_CIE_ROIE	BIT(6)	/* Request Overflow Interrupt Enable Bit */
#define AT_XDMAC_CID		0x04	/* Channel Interrupt Disable Register */
#define		AT_XDMAC_CID_BID	BIT(0)	/* End of Block Interrupt Disable Bit */
#define		AT_XDMAC_CID_LID	BIT(1)	/* End of Linked List Interrupt Disable Bit */
#define		AT_XDMAC_CID_DID	BIT(2)	/* End of Disable Interrupt Disable Bit */
#define		AT_XDMAC_CID_FID	BIT(3)	/* End of Flush Interrupt Disable Bit */
#define		AT_XDMAC_CID_RBEID	BIT(4)	/* Read Bus Error Interrupt Disable Bit */
#define		AT_XDMAC_CID_WBEID	BIT(5)	/* Write Bus Error Interrupt Disable Bit */
#define		AT_XDMAC_CID_ROID	BIT(6)	/* Request Overflow Interrupt Disable Bit */
#define AT_XDMAC_CIM		0x08	/* Channel Interrupt Mask Register */
#define		AT_XDMAC_CIM_BIM	BIT(0)	/* End of Block Interrupt Mask Bit */
#define		AT_XDMAC_CIM_LIM	BIT(1)	/* End of Linked List Interrupt Mask Bit */
#define		AT_XDMAC_CIM_DIM	BIT(2)	/* End of Disable Interrupt Mask Bit */
#define		AT_XDMAC_CIM_FIM	BIT(3)	/* End of Flush Interrupt Mask Bit */
#define		AT_XDMAC_CIM_RBEIM	BIT(4)	/* Read Bus Error Interrupt Mask Bit */
#define		AT_XDMAC_CIM_WBEIM	BIT(5)	/* Write Bus Error Interrupt Mask Bit */
#define		AT_XDMAC_CIM_ROIM	BIT(6)	/* Request Overflow Interrupt Mask Bit */
#define AT_XDMAC_CIS		0x0C	/* Channel Interrupt Status Register */
#define		AT_XDMAC_CIS_BIS	BIT(0)	/* End of Block Interrupt Status Bit */
#define		AT_XDMAC_CIS_LIS	BIT(1)	/* End of Linked List Interrupt Status Bit */
#define		AT_XDMAC_CIS_DIS	BIT(2)	/* End of Disable Interrupt Status Bit */
#define		AT_XDMAC_CIS_FIS	BIT(3)	/* End of Flush Interrupt Status Bit */
#define		AT_XDMAC_CIS_RBEIS	BIT(4)	/* Read Bus Error Interrupt Status Bit */
#define		AT_XDMAC_CIS_WBEIS	BIT(5)	/* Write Bus Error Interrupt Status Bit */
#define		AT_XDMAC_CIS_ROIS	BIT(6)	/* Request Overflow Interrupt Status Bit */
#define AT_XDMAC_CSA		0x10	/* Channel Source Address Register */
#define AT_XDMAC_CDA		0x14	/* Channel Destination Address Register */
#define AT_XDMAC_CNDA		0x18	/* Channel Next Descriptor Address Register */
#if IS_ENABLED(CONFIG_AT_XDMAC_SAMA7G5)
#define		AT_XDMAC_CNDA_NDAIF(i)	0x0
#else
#define		AT_XDMAC_CNDA_NDAIF(i)	((i) & 0x1)			/* Channel x Next Descriptor Interface */
#endif
#define		AT_XDMAC_CNDA_NDA(i)	((i) & 0xfffffffc)		/* Channel x Next Descriptor Address */
#define AT_XDMAC_CNDC		0x1C	/* Channel Next Descriptor Control Register */
#define		AT_XDMAC_CNDC_NDE		(0x1 << 0)		/* Channel x Next Descriptor Enable */
#define		AT_XDMAC_CNDC_NDSUP		(0x1 << 1)		/* Channel x Next Descriptor Source Update */
#define		AT_XDMAC_CNDC_NDDUP		(0x1 << 2)		/* Channel x Next Descriptor Destination Update */
#define		AT_XDMAC_CNDC_NDVIEW_NDV0	(0x0 << 3)		/* Channel x Next Descriptor View 0 */
#define		AT_XDMAC_CNDC_NDVIEW_NDV1	(0x1 << 3)		/* Channel x Next Descriptor View 1 */
#define		AT_XDMAC_CNDC_NDVIEW_NDV2	(0x2 << 3)		/* Channel x Next Descriptor View 2 */
#define		AT_XDMAC_CNDC_NDVIEW_NDV3	(0x3 << 3)		/* Channel x Next Descriptor View 3 */
#define AT_XDMAC_CUBC		0x20	/* Channel Microblock Control Register */
#define AT_XDMAC_CBC		0x24	/* Channel Block Control Register */
#define AT_XDMAC_CC		0x28	/* Channel Configuration Register */
#define		AT_XDMAC_CC_TYPE	(0x1 << 0)	/* Channel Transfer Type */
#define			AT_XDMAC_CC_TYPE_MEM_TRAN	(0x0 << 0)	/* Memory to Memory Transfer */
#define			AT_XDMAC_CC_TYPE_PER_TRAN	(0x1 << 0)	/* Peripheral to Memory or Memory to Peripheral Transfer */
#define		AT_XDMAC_CC_MBSIZE_MASK	(0x3 << 1)
#define			AT_XDMAC_CC_MBSIZE_SINGLE	(0x0 << 1)
#define			AT_XDMAC_CC_MBSIZE_FOUR		(0x1 << 1)
#define			AT_XDMAC_CC_MBSIZE_EIGHT	(0x2 << 1)
#define			AT_XDMAC_CC_MBSIZE_SIXTEEN	(0x3 << 1)
#define		AT_XDMAC_CC_DSYNC	(0x1 << 4)	/* Channel Synchronization */
#define			AT_XDMAC_CC_DSYNC_PER2MEM	(0x0 << 4)
#define			AT_XDMAC_CC_DSYNC_MEM2PER	(0x1 << 4)
#define		AT_XDMAC_CC_PROT	(0x1 << 5)	/* Channel Protection */
#define			AT_XDMAC_CC_PROT_SEC		(0x0 << 5)
#define			AT_XDMAC_CC_PROT_UNSEC		(0x1 << 5)
#define		AT_XDMAC_CC_SWREQ	(0x1 << 6)	/* Channel Software Request Trigger */
#define			AT_XDMAC_CC_SWREQ_HWR_CONNECTED	(0x0 << 6)
#define			AT_XDMAC_CC_SWREQ_SWR_CONNECTED	(0x1 << 6)
#define		AT_XDMAC_CC_MEMSET	(0x1 << 7)	/* Channel Fill Block of memory */
#define			AT_XDMAC_CC_MEMSET_NORMAL_MODE	(0x0 << 7)
#define			AT_XDMAC_CC_MEMSET_HW_MODE	(0x1 << 7)
#define		AT_XDMAC_CC_CSIZE(i)	((0x7 & (i)) << 8)	/* Channel Chunk Size */
#define		AT_XDMAC_CC_DWIDTH_OFFSET	11
#define		AT_XDMAC_CC_DWIDTH_MASK	(0x3 << AT_XDMAC_CC_DWIDTH_OFFSET)
#define		AT_XDMAC_CC_DWIDTH(i)	((0x3 & (i)) << AT_XDMAC_CC_DWIDTH_OFFSET)	/* Channel Data Width */
#define			AT_XDMAC_CC_DWIDTH_BYTE		0x0
#define			AT_XDMAC_CC_DWIDTH_HALFWORD	0x1
#define			AT_XDMAC_CC_DWIDTH_WORD		0x2
#define			AT_XDMAC_CC_DWIDTH_DWORD	0x3
#if IS_ENABLED(CONFIG_AT_XDMAC_SAMA7G5)
#define		AT_XDMAC_CC_SIF(i)	0	/* Channel Source Interface Identifier */
#define		AT_XDMAC_CC_DIF(i)	0	/* Channel Destination Interface Identifier */
#else
#define		AT_XDMAC_CC_SIF(i)	((0x1 & (i)) << 13)	/* Channel Source Interface Identifier */
#define		AT_XDMAC_CC_DIF(i)	((0x1 & (i)) << 14)	/* Channel Destination Interface Identifier */
#endif
#define		AT_XDMAC_CC_SAM_MASK	(0x3 << 16)	/* Channel Source Addressing Mode */
#define			AT_XDMAC_CC_SAM_FIXED_AM	(0x0 << 16)
#define			AT_XDMAC_CC_SAM_INCREMENTED_AM	(0x1 << 16)
#define			AT_XDMAC_CC_SAM_UBS_AM		(0x2 << 16)
#define			AT_XDMAC_CC_SAM_UBS_DS_AM	(0x3 << 16)
#define		AT_XDMAC_CC_DAM_MASK	(0x3 << 18)	/* Channel Source Addressing Mode */
#define			AT_XDMAC_CC_DAM_FIXED_AM	(0x0 << 18)
#define			AT_XDMAC_CC_DAM_INCREMENTED_AM	(0x1 << 18)
#define			AT_XDMAC_CC_DAM_UBS_AM		(0x2 << 18)
#define			AT_XDMAC_CC_DAM_UBS_DS_AM	(0x3 << 18)
#define		AT_XDMAC_CC_INITD	(0x1 << 21)	/* Channel Initialization Terminated (read only) */
#define			AT_XDMAC_CC_INITD_TERMINATED	(0x0 << 21)
#define			AT_XDMAC_CC_INITD_IN_PROGRESS	(0x1 << 21)
#define		AT_XDMAC_CC_RDIP	(0x1 << 22)	/* Read in Progress (read only) */
#define			AT_XDMAC_CC_RDIP_DONE		(0x0 << 22)
#define			AT_XDMAC_CC_RDIP_IN_PROGRESS	(0x1 << 22)
#define		AT_XDMAC_CC_WRIP	(0x1 << 23)	/* Write in Progress (read only) */
#define			AT_XDMAC_CC_WRIP_DONE		(0x0 << 23)
#define			AT_XDMAC_CC_WRIP_IN_PROGRESS	(0x1 << 23)
#define		AT_XDMAC_CC_PERID(i)	(0x7f & (i) << 24)	/* Channel Peripheral Identifier */
#define AT_XDMAC_CDS_MSP	0x2C	/* Channel Data Stride Memory Set Pattern */
#define AT_XDMAC_CSUS		0x30	/* Channel Source Microblock Stride */
#define AT_XDMAC_CDUS		0x34	/* Channel Destination Microblock Stride */

#define AT_XDMAC_CHAN_REG_BASE	(0x50 + AT_XDMAC_SAMA7G5_OFF3)	/* Channel registers base address */

/* Microblock control members */
#define AT_XDMAC_MBR_UBC_UBLEN_MAX	0xFFFFFFUL	/* Maximum Microblock Length */
#define AT_XDMAC_MBR_UBC_NDE		(0x1 << 24)	/* Next Descriptor Enable */
#define AT_XDMAC_MBR_UBC_NSEN		(0x1 << 25)	/* Next Descriptor Source Update */
#define AT_XDMAC_MBR_UBC_NDEN		(0x1 << 26)	/* Next Descriptor Destination Update */
#define AT_XDMAC_MBR_UBC_NDV0		(0x0 << 27)	/* Next Descriptor View 0 */
#define AT_XDMAC_MBR_UBC_NDV1		(0x1 << 27)	/* Next Descriptor View 1 */
#define AT_XDMAC_MBR_UBC_NDV2		(0x2 << 27)	/* Next Descriptor View 2 */
#define AT_XDMAC_MBR_UBC_NDV3		(0x3 << 27)	/* Next Descriptor View 3 */

#define AT_XDMAC_MAX_CHAN	0x20
#define AT_XDMAC_MAX_CSIZE	16	/* 16 data */
#define AT_XDMAC_MAX_DWIDTH	8	/* 64 bits */
#define AT_XDMAC_RESIDUE_MAX_RETRIES	5

#endif
