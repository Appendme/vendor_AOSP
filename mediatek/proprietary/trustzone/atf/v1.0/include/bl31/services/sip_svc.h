/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2014. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#ifndef __SIP_SVC_H__
#define __SIP_SVC_H__

/*******************************************************************************
 * Defines for runtime services func ids
 ******************************************************************************/
/* SMC32 ID range from 0x82000000 to 0x82000FFF */
/* SMC64 ID range from 0xC2000000 to 0xC2000FFF */

/* For MTK SMC from Trustonic TEE */
/* 0x82000000 - 0x820000FF & 0xC2000000 - 0xC20000FF */
#define MTK_SIP_TBASE_HWUID_AARCH32       0x82000000

/* For MTK SMC from Boot-loader */
/* 0x82000100 - 0x820001FF & 0xC2000100 - 0xC20001FF */
#define MTK_SIP_BL_INIT_AARCH32           0x82000100
#define MTK_SIP_BL_INIT_AARCH64           0xC2000100
#define MTK_SIP_LK_DAPC_INIT_AARCH32      0x82000101
#define MTK_SIP_LK_DAPC_INIT_AARCH64      0xC2000101
#define MTK_SIP_LK_MD_REG_WRITE_AARCH32   0x82000102
#define MTK_SIP_LK_MD_REG_WRITE_AARCH64   0xC2000102
#define MTK_SIP_LK_RPMB_INIT_AARCH32      0x82000103
#define MTK_SIP_LK_RPMB_INIT_AARCH64      0xC2000103
#define MTK_SIP_LK_RPMB_UNINIT_AARCH32    0x82000104
#define MTK_SIP_LK_RPMB_UNINIT_AARCH64    0xC2000104
#define MTK_SIP_LK_RPMB_HMAC_AARCH32      0x82000105
#define MTK_SIP_LK_RPMB_HMAC_AARCH64      0xC2000105


/* For MTK SMC from Kernel */
/* 0x82000200 - 0x820002FF & 0xC2000200 - 0xC20002FF */
#define MTK_SIP_KERNEL_BOOT_AARCH32        0x82000200
#define MTK_SIP_KERNEL_BOOT_AARCH64        0xC2000200
#define MTK_SIP_KERNEL_MCUSYS_WRITE_AARCH32 0x82000201
#define MTK_SIP_KERNEL_MCUSYS_WRITE_AARCH64 0xC2000201
#define MTK_SIP_KERNEL_MCUSYS_ACCESS_COUNT_AARCH32 0x82000202
#define MTK_SIP_KERNEL_MCUSYS_ACCESS_COUNT_AARCH64 0xC2000202
#define MTK_SIP_KERNEL_L2_SHARING_AARCH32  0x82000203
#define MTK_SIP_KERNEL_L2_SHARING_AARCH64  0xC2000203
#define MTK_SIP_KERNEL_WDT_AARCH32 0x82000204
#define MTK_SIP_KERNEL_WDT_AARCH64 0xC2000204
#define MTK_SIP_KERNEL_GIC_DUMP_AARCH32 0x82000205
#define MTK_SIP_KERNEL_GIC_DUMP_AARCH64 0xC2000205
#define MTK_SIP_KERNEL_DAPC_INIT_AARCH32 0x82000206
#define MTK_SIP_KERNEL_DAPC_INIT_AARCH64 0xC2000206
#define MTK_SIP_KERNEL_EMIMPU_WRITE_AARCH32 0x82000207
#define MTK_SIP_KERNEL_EMIMPU_WRITE_AARCH64 0xC2000207
#define MTK_SIP_KERNEL_EMIMPU_READ_AARCH32 0x82000208
#define MTK_SIP_KERNEL_EMIMPU_READ_AARCH64 0xC2000208
#define MTK_SIP_KERNEL_EMIMPU_SET_AARCH32 0x82000209
#define MTK_SIP_KERNEL_EMIMPU_SET_AARCH64 0xC2000209

/* FIQ migration: new smc for setting gic itarget register*/
#define MTK_SIP_KERNEL_SECURE_IRQ_MIGRATE	0x8200020A

#define MTK_SIP_KERNEL_SCP_RESET_AARCH32 0x8200020B
#define MTK_SIP_KERNEL_SCP_RESET_AARCH64 0xC200020B

/* For MTK SMC Reserved */
/* 0x82000300 - 0x82000FFF & 0xC2000300 - 0xC2000FFF */

/* astone added for debug purpose */
#define MTK_SIP_KERNEL_MSG_AARCH32 0x820002ff
#define MTK_SIP_KERNEL_MSG_AARCH64 0xC20002ff

//UDI reserve 0x3a0 ~ 0x3af
#define MTK_SIP_KERNEL_UDI_JTAG_CLOCK_AARCH32 0x820003A0
#define MTK_SIP_KERNEL_UDI_JTAG_CLOCK_AARCH64 0xC20003A0

//iDVFS reserve 0x3b0 ~ 0x3cf
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSENABLE_AARCH32                 0x820003B0
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSENABLE_AARCH64                 0xC20003B0
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSDISABLE_AARCH32                0x820003B1
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSDISABLE_AARCH64                0xC20003B1
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSFREQ_AARCH32                   0x820003B2
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSFREQ_AARCH64                   0xC20003B2
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSCHANNEL_AARCH32                0x820003B3
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSCHANNEL_AARCH64                0xC20003B3
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSSWAVG_AARCH32                  0x820003B4
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSSWAVG_AARCH64                  0xC20003B4
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSSWAVGSTATUS_AARCH32            0x820003B5
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSSWAVGSTATUS_AARCH64            0xC20003B5
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSSWMODE_AARCH32                 0x820003B6
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSSWMODE_AARCH64                 0xC20003B6
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSSLOWMODE_AARCH32               0x820003B7
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSSLOWMODE_AARCH64               0xC20003B7
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLSETFREQ_AARCH32             0x820003B8
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLSETFREQ_AARCH64             0xC20003B8
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLDISABLE_AARCH32             0x820003B9
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLDISABLE_AARCH64             0xC20003B9
                                                                    
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLGETFREQ_AARCH32             0x820003BA //below is add
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLGETFREQ_AARCH64             0xC20003BA
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLSETPOSDIV_AARCH32           0x820003BB
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLSETPOSDIV_AARCH64           0xC20003BB
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLGETPOSDIV_AARCH32           0x820003BC
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLGETPOSDIV_AARCH64           0xC20003BC
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLSETPCW_AARCH32              0x820003BD
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLSETPCW_AARCH64              0xC20003BD
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLGETPCW_AARCH32              0x820003BE
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSPLLGETPCW_AARCH64              0xC20003BE
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSSRAMLDOSET_AARCH32             0x820003BF
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSSRAMLDOSET_AARCH64             0xC20003BF
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSSRAMLDOGET_AARCH32             0x820003C0
#define MTK_SIP_KERNEL_IDVFS_BIGIDVFSSRAMLDOGET_AARCH64             0xC20003C0


//OCP
#define MTK_SIP_KERNEL_OCP_WRITE_AARCH32 0x8200035E
#define MTK_SIP_KERNEL_OCP_WRITE_AARCH64 0xC200035E
#define MTK_SIP_KERNEL_OCP_READ_AARCH32 0x8200035F
#define MTK_SIP_KERNEL_OCP_READ_AARCH64 0xC200035F
#define MTK_SIP_KERNEL_BIGOCPCONFIG_AARCH32 0x82000360
#define MTK_SIP_KERNEL_BIGOCPCONFIG_AARCH64 0xC2000360
#define MTK_SIP_KERNEL_BIGOCPSETTARGET_AARCH32 0x82000361
#define MTK_SIP_KERNEL_BIGOCPSETTARGET_AARCH64 0xC2000361
#define MTK_SIP_KERNEL_BIGOCPENABLE1_AARCH32 0x82000362
#define MTK_SIP_KERNEL_BIGOCPENABLE1_AARCH64 0xC2000362
#define MTK_SIP_KERNEL_BIGOCPENABLE0_AARCH32 0x82000363
#define MTK_SIP_KERNEL_BIGOCPENABLE0_AARCH64 0xC2000363
#define MTK_SIP_KERNEL_BIGOCPDISABLE_AARCH32 0x82000364
#define MTK_SIP_KERNEL_BIGOCPDISABLE_AARCH64 0xC2000364
#define MTK_SIP_KERNEL_BIGOCPINTLIMIT_AARCH32 0x82000365
#define MTK_SIP_KERNEL_BIGOCPINTLIMIT_AARCH64 0xC2000365
#define MTK_SIP_KERNEL_BIGOCPINTENDIS_AARCH32 0x82000366
#define MTK_SIP_KERNEL_BIGOCPINTENDIS_AARCH64 0XC2000366
#define MTK_SIP_KERNEL_BIGOCPINTCLR_AARCH32 0X82000367
#define MTK_SIP_KERNEL_BIGOCPINTCLR_AARCH64 0XC2000367
//#define MTK_SIP_KERNEL_BIGOCPINTSTATUS_AARCH32 0X82000368
//#define MTK_SIP_KERNEL_BIGOCPINTSTATUS_AARCH64 0XC2000368
#define MTK_SIP_KERNEL_BIGOCPCAPTURE1_AARCH32 0X82000369
#define MTK_SIP_KERNEL_BIGOCPCAPTURE1_AARCH64 0XC2000369
#define MTK_SIP_KERNEL_BIGOCPCAPTURE0_AARCH32 0X8200036A
#define MTK_SIP_KERNEL_BIGOCPCAPTURE0_AARCH64 0XC200036A
//#define MTK_SIP_KERNEL_BIGOCPCAPTURESTATUS_AARCH32 0X8200036B
//#define MTK_SIP_KERNEL_BIGOCPCAPTURESTATUS_AARCH64 0XC200036B
#define MTK_SIP_KERNEL_BIGOCPCLKAVG_AARCH32 0X8200036C
#define MTK_SIP_KERNEL_BIGOCPCLKAVG_AARCH64 0XC200036C
//#define MTK_SIP_KERNEL_BIGOCPCLKAVGSTATUS_AARCH32 0X8200036D
//#define MTK_SIP_KERNEL_BIGOCPCLKAVGSTATUS_AARCH64 0XC200036D
#define MTK_SIP_KERNEL_LITTLEOCPCONFIG_AARCH32 0X8200036E
#define MTK_SIP_KERNEL_LITTLEOCPCONFIG_AARCH64 0XC200036E
#define MTK_SIP_KERNEL_LITTLEOCPSETTARGET_AARCH32 0X8200036F
#define MTK_SIP_KERNEL_LITTLEOCPSETTARGET_AARCH64 0XC200036F
#define MTK_SIP_KERNEL_LITTLEOCPENABLE_AARCH32 0X82000370
#define MTK_SIP_KERNEL_LITTLEOCPENABLE_AARCH64 0XC2000370
#define MTK_SIP_KERNEL_LITTLEOCPDISABLE_AARCH32 0X82000371
#define MTK_SIP_KERNEL_LITTLEOCPDISABLE_AARCH64 0XC2000371
#define MTK_SIP_KERNEL_LITTLEOCPDVFSSET_AARCH32 0X82000372
#define MTK_SIP_KERNEL_LITTLEOCPDVFSSET_AARCH64 0XC2000372
#define MTK_SIP_KERNEL_LITTLEOCPINTLIMIT_AARCH32 0X82000373
#define MTK_SIP_KERNEL_LITTLEOCPINTLIMIT_AARCH64 0XC2000373
#define MTK_SIP_KERNEL_LITTLEOCPINTENDIS_AARCH32 0X82000374
#define MTK_SIP_KERNEL_LITTLEOCPINTENDIS_AARCH64 0XC2000374
#define MTK_SIP_KERNEL_LITTLEOCPINTCLR_AARCH32 0X82000375
#define MTK_SIP_KERNEL_LITTLEOCPINTCLR_AARCH64 0XC2000375
//#define MTK_SIP_KERNEL_LITTLEOCPINTSTATUS_AARCH32 0X82000376
//#define MTK_SIP_KERNEL_LITTLEOCPINTSTATUS_AARCH64 0XC2000376
#define MTK_SIP_KERNEL_LITTLEOCPCAPTURE00_AARCH32 0X82000377
#define MTK_SIP_KERNEL_LITTLEOCPCAPTURE00_AARCH64 0XC2000377
#define MTK_SIP_KERNEL_LITTLEOCPCAPTURE10_AARCH32 0X82000378
#define MTK_SIP_KERNEL_LITTLEOCPCAPTURE10_AARCH64 0XC2000378
#define MTK_SIP_KERNEL_LITTLEOCPCAPTURE11_AARCH32 0X82000379
#define MTK_SIP_KERNEL_LITTLEOCPCAPTURE11_AARCH64 0XC2000379

//#define MTK_SIP_KERNEL_LITTLEOCPCAPTUREGET_AARCH32 0X8200037A
//#define MTK_SIP_KERNEL_LITTLEOCPCAPTUREGET_AARCH64 0XC200037A
#define MTK_SIP_KERNEL_BIGSRAMLDOENABLE_AARCH32 0X82000380
#define MTK_SIP_KERNEL_BIGSRAMLDOENABLE_AARCH64 0XC2000380
#define MTK_SIP_KERNEL_BIGDREQHWEN_AARCH32 0X82000381
#define MTK_SIP_KERNEL_BIGDREQHWEN_AARCH64 0XC2000381
#define MTK_SIP_KERNEL_BIGDREQSWEN_AARCH32 0X82000382
#define MTK_SIP_KERNEL_BIGDREQSWEN_AARCH64 0XC2000382
#define MTK_SIP_KERNEL_BIGDREQGET_AARCH32 0X82000383
#define MTK_SIP_KERNEL_BIGDREQGET_AARCH64 0XC2000383
#define MTK_SIP_KERNEL_LITTLEDREQSWEN_AARCH32 0X82000384
#define MTK_SIP_KERNEL_LITTLEDREQSWEN_AARCH64 0XC2000384
#define MTK_SIP_KERNEL_LITTLEDREQGET_AARCH32 0X82000385
#define MTK_SIP_KERNEL_LITTLEDREQGET_AARCH64 0XC2000385

/*
 * Number of SIP calls (above) implemented.
 */
#define MTK_SIP_SVC_NUM_CALLS             2

/*******************************************************************************
 * Defines for SIP Service queries
 ******************************************************************************/
/* 0x82000000 - 0x8200FEFF is SIP service calls */
#define MTK_SIP_SVC_CALL_COUNT      0x8200ff00
#define MTK_SIP_SVC_UID             0x8200ff01
/* 0x8200ff02 is reserved */
#define MTK_SIP_SVC_VERSION         0x8200ff03
/* 0x8200ff04 - 0x8200FFFF is reserved for future expansion */

/* MTK SIP Service Calls version numbers */
#define MTK_SIP_VERSION_MAJOR		0x0
#define MTK_SIP_VERSION_MINOR		0x1

/* The macros below are used to identify SIP calls from the SMC function ID */
/* SMC32 ID range from 0x82000000 to 0x82000FFF */
/* SMC64 ID range from 0xC2000000 to 0xC2000FFF */
#define SIP_FID_MASK                0xf000u
#define SIP_FID_VALUE               0u
#define is_sip_fid(_fid) \
	(((_fid) & SIP_FID_MASK) == SIP_FID_VALUE)

#endif /* __SIP_SVC_H__ */
