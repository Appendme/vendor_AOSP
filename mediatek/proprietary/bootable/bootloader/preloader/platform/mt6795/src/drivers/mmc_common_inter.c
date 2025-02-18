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
 * MediaTek Inc. (C) 2010. All rights reserved.
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

#include "typedefs.h"
#include "platform.h"
#include "mmc_core.h"
#include "nand_core.h"
#include "addr_trans.h"
#include "dram_buffer.h"

#define MMC_HOST_ID                 0
#define BUF_BLK_NUM                 4   /* 4 * 512bytes = 2KB */

/**************************************************************************
*  DEBUG CONTROL
**************************************************************************/

/**************************************************************************
*  MACRO DEFINITION
**************************************************************************/

/**************************************************************************
*  EXTERNAL DECLARATION
**************************************************************************/
extern struct nand_chip g_nand_chip;
#define STORAGE_BUFFER_SIZE 0x10000

static blkdev_t g_mmc_bdev;

#ifdef MTK_EMMC_SUPPORT  
static addr_trans_info_t g_emmc_addr_trans[EMMC_PART_END];
#endif

static addr_trans_tbl_t g_addr_trans_tbl;
u64 g_emmc_size = 0;
u64 g_emmc_user_size = 0;

static int mmc_switch_part(u32 part_id)
{
    int err = MMC_ERR_NONE;
    struct mmc_card *card;
    struct mmc_host *host;    
    u8 part = (u8)part_id;
    u8 cfg;
    u8 *ext_csd;

    card = mmc_get_card(MMC_HOST_ID);
    host = mmc_get_host(MMC_HOST_ID);
    
    if (!card) 
        return MMC_ERR_INVALID;

    ext_csd = &card->raw_ext_csd[0];

    if (mmc_card_mmc(card) && ext_csd[EXT_CSD_REV] >= 3) {
#ifdef MTK_EMMC_SUPPORT     
        if (part_id == EMMC_PART_USER)
            part = EXT_CSD_PART_CFG_DEFT_PART;
        else if(part_id == EMMC_PART_BOOT1)
            part = EXT_CSD_PART_CFG_BOOT_PART_1;
        else if(part_id == EMMC_PART_BOOT2)
            part = EXT_CSD_PART_CFG_BOOT_PART_2;

        cfg = card->raw_ext_csd[EXT_CSD_PART_CFG];

        /* already set to specific partition */
        if (part == (cfg & 0x7))
            return MMC_ERR_NONE;

        cfg = (cfg & ~0x7) | part;

        err = mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CFG, cfg);

        if (err == MMC_ERR_NONE) {
            err = mmc_read_ext_csd(host, card);
            if (err == MMC_ERR_NONE) {
                ext_csd = &card->raw_ext_csd[0];
                if (ext_csd[EXT_CSD_PART_CFG] != cfg)
                    err = MMC_ERR_FAILED;
            }
        }
#endif
    }
    if(err)
        printf("[%s]: failed to switch part_id:%d, part:%d, err=%d\n", __func__, part_id, part, err);
    return err;    
}

#ifdef MTK_EMMC_SUPPORT     
static int mmc_virt_to_phys(u32 virt_blknr, u32 *phys_blknr, u32 *part_id)
{
    int ret;
    virt_addr_t virt;
    phys_addr_t phys;

    virt.addr = virt_blknr;

    ret = virt_to_phys_addr(&g_addr_trans_tbl, &virt, &phys);

    if (phys.id == -1)
        phys.id = EMMC_PART_USER;

    *phys_blknr = (ret == 0) ? phys.addr : virt_blknr; /* in 512B unit */
    *part_id    = (ret == 0) ? phys.id : EMMC_PART_USER;

    return ret;
}

static int mmc_phys_to_virt(u32 phys_blknr, u32 part_id, u32 *virt_blknr)
{
    int ret;
    virt_addr_t virt;
    phys_addr_t phys;

    phys.addr = phys_blknr;
    phys.id   = part_id;

    ret = phys_to_virt_addr(&g_addr_trans_tbl, &phys, &virt);

    *virt_blknr = (ret == 0) ? virt.addr : phys_blknr; /* in 512B unit */

    return ret;
}

/* unit-test */
#if 0
void mmc_addr_trans_test(void)
{
    u32 i, virt_blknr, phys_blknr, part_id;

    virt_blknr = 0;
    for (i = 0; i < EMMC_PART_END; i++) {
        mmc_virt_to_phys(virt_blknr - 1, &phys_blknr, &part_id); 
        printf("[EMMC] Virt: 0x%x --> Phys: 0x%x Part: %d\n",
            virt_blknr - 1, phys_blknr, part_id);
        mmc_virt_to_phys(virt_blknr, &phys_blknr, &part_id);
        printf("[EMMC] Virt: 0x%x --> Phys: 0x%x Part: %d\n",
            virt_blknr, phys_blknr, part_id);
        mmc_virt_to_phys(virt_blknr + 1, &phys_blknr, &part_id);
        printf("[EMMC] Virt: 0x%x --> Phys: 0x%x Part: %d\n",
            virt_blknr + 1, phys_blknr, part_id);        
        virt_blknr += g_emmc_addr_trans[i].len;
    }
    mmc_virt_to_phys(virt_blknr - 1, &phys_blknr, &part_id);
    printf("[EMMC] Virt: 0x%x --> Phys: 0x%x Part: %d\n",
        virt_blknr - 1, phys_blknr, part_id);    
    mmc_virt_to_phys(virt_blknr, &phys_blknr, &part_id);
    printf("[EMMC] Virt: 0x%x --> Phys: 0x%x Part: %d\n",
        virt_blknr, phys_blknr, part_id);
    mmc_virt_to_phys(virt_blknr + 1, &phys_blknr, &part_id);
    printf("[EMMC] Virt: 0x%x --> Phys: 0x%x Part: %d\n",
        virt_blknr + 1, phys_blknr, part_id);

    phys_blknr = 0;
    for (i = 0; i < EMMC_PART_END; i++) {
        mmc_phys_to_virt(phys_blknr, i, &virt_blknr);
        mmc_phys_to_virt(phys_blknr + 1, i, &virt_blknr);

        mmc_phys_to_virt(phys_blknr + g_emmc_addr_trans[i].len, i, &virt_blknr);
        mmc_phys_to_virt(phys_blknr + g_emmc_addr_trans[i].len + 1, i, &virt_blknr);
    }
}
#endif
#define mmc_virt_switch(vbn,pbn)    \
do{ u32 pid; \
    if (mmc_virt_to_phys(vbn, pbn, &pid) == MMC_ERR_NONE) \
        mmc_switch_part(pid); \
}while(0)
    
#else
#define mmc_virt_to_phys(vbn,pbn,pid)       do{}while(0)
#define mmc_phys_to_virt(pbn,pid,vbn)       do{}while(0)
#define mmc_virt_switch(vbn,pbn)            do{}while(0)
//#define mmc_addr_trans_tbl_init(card,bdev)  do{}while(0)
#endif

static int mmc_addr_trans_tbl_init(struct mmc_card *card, blkdev_t *bdev)
{
    u32 wpg_sz;
    u8 *ext_csd;

    memset(&g_addr_trans_tbl, 0, sizeof(addr_trans_tbl_t));

    ext_csd = &card->raw_ext_csd[0];
    bdev->offset = 0;

    if (mmc_card_mmc(card) && ext_csd[EXT_CSD_REV] >= 3) {
#ifdef MTK_EMMC_SUPPORT          
        u64 size[EMMC_PART_END];
        u32 i;

        if ((ext_csd[EXT_CSD_ERASE_GRP_DEF] & EXT_CSD_ERASE_GRP_DEF_EN)
            && (ext_csd[EXT_CSD_HC_WP_GPR_SIZE] > 0)) {
            wpg_sz = 512 * 1024 * ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] * 
                ext_csd[EXT_CSD_HC_WP_GPR_SIZE];
        } else {
            wpg_sz = card->csd.write_prot_grpsz;
        }
            
        size[EMMC_PART_BOOT1] = ext_csd[EXT_CSD_BOOT_SIZE_MULT] * 128 * 1024;
        size[EMMC_PART_BOOT2] = ext_csd[EXT_CSD_BOOT_SIZE_MULT] * 128 * 1024;
        size[EMMC_PART_RPMB]  = ext_csd[EXT_CSD_RPMB_SIZE_MULT] * 128 * 1024;
        size[EMMC_PART_GP1]   = ext_csd[EXT_CSD_GP1_SIZE_MULT + 2] * 256 * 256 +
                                ext_csd[EXT_CSD_GP1_SIZE_MULT + 1] * 256 +
                                ext_csd[EXT_CSD_GP1_SIZE_MULT + 0];
        size[EMMC_PART_GP2]   = ext_csd[EXT_CSD_GP2_SIZE_MULT + 2] * 256 * 256 +
                                ext_csd[EXT_CSD_GP2_SIZE_MULT + 1] * 256 +
                                ext_csd[EXT_CSD_GP2_SIZE_MULT + 0];
        size[EMMC_PART_GP3]   = ext_csd[EXT_CSD_GP3_SIZE_MULT + 2] * 256 * 256 +
                                ext_csd[EXT_CSD_GP3_SIZE_MULT + 1] * 256 +
                                ext_csd[EXT_CSD_GP3_SIZE_MULT + 0];
        size[EMMC_PART_GP4]   = ext_csd[EXT_CSD_GP4_SIZE_MULT + 2] * 256 * 256 +
                                ext_csd[EXT_CSD_GP4_SIZE_MULT + 1] * 256 +
                                ext_csd[EXT_CSD_GP4_SIZE_MULT + 0];
        size[EMMC_PART_USER]  = (u64)card->blklen * card->nblks;

        size[EMMC_PART_GP1] *= wpg_sz;
        size[EMMC_PART_GP2] *= wpg_sz;
        size[EMMC_PART_GP3] *= wpg_sz;
        size[EMMC_PART_GP4] *= wpg_sz;

        for (i = EMMC_PART_BOOT1; i < EMMC_PART_END; i++) {
            g_emmc_addr_trans[i].id  = i;
            g_emmc_addr_trans[i].len = size[i] / 512; /* in 512B unit */
            	g_emmc_size += size[i];
        }
        g_emmc_user_size = size[EMMC_PART_USER];
        /* determine user area offset */
        for (i = EMMC_PART_BOOT1; i < EMMC_PART_USER; i++) {
            bdev->offset += size[i];
        }
        bdev->offset /= bdev->blksz; /* in blksz unit */
        
        g_addr_trans_tbl.num  = EMMC_PART_END;
        g_addr_trans_tbl.info = &g_emmc_addr_trans[0];
#endif
    } else {
        g_addr_trans_tbl.num  = 0;
        g_addr_trans_tbl.info = NULL;
    }

    return 0;
}

static int mmc_bread(blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf, u32 part_id)
{    
    struct mmc_host *host = (struct mmc_host*)bdev->priv;

    //mmc_virt_switch(blknr, (u32*)&blknr);
    mmc_switch_part(part_id);
    return mmc_block_read(host->id, (unsigned long)blknr, blks, (unsigned long*)buf);
}

static int mmc_bwrite(blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf, u32 part_id)
{
    struct mmc_host *host = (struct mmc_host*)bdev->priv;    

    //mmc_virt_switch(blknr, (u32*)&blknr);
    mmc_switch_part(part_id);
    return mmc_block_write(host->id, (unsigned long)blknr, blks, (unsigned long*)buf);
}

// ==========================================================
// MMC Common Interface - Init
// ==========================================================
u32 mmc_init_device(void)
{
    int ret;
    struct mmc_card *card;

    if (!blkdev_get(BOOTDEV_SDMMC)) {

        ret = mmc_init(MMC_HOST_ID);
        if (ret != 0) {
            printf("[SD0] init card failed\n");
            return ret;
        }

        memset(&g_mmc_bdev, 0, sizeof(blkdev_t));

        card = mmc_get_card(MMC_HOST_ID);

        g_mmc_bdev.blksz   = MMC_BLOCK_SIZE;
        g_mmc_bdev.erasesz = MMC_BLOCK_SIZE;
        g_mmc_bdev.blks    = card->nblks;
        g_mmc_bdev.bread   = mmc_bread;
        g_mmc_bdev.bwrite  = mmc_bwrite;
        g_mmc_bdev.type    = BOOTDEV_SDMMC;
        g_mmc_bdev.blkbuf  = NULL;
        g_mmc_bdev.priv    = (void*)mmc_get_host(MMC_HOST_ID);

        mmc_addr_trans_tbl_init(card, &g_mmc_bdev);

        blkdev_register(&g_mmc_bdev);
    }
    return 0;
}

u32 mmc_get_device_id(u8 *id, u32 len,u32 *fw_len)
{
    u8 buf[16]; /* CID = 128 bits */
    struct mmc_card *card;
    u8 buf_sanid[512];
    u8 i;

    if (0 != mmc_init_device())
        return -1;

    card = mmc_get_card(MMC_HOST_ID);
    
    buf[0] = (card->raw_cid[0] >> 24) & 0xFF; /* Manufacturer ID */
    buf[1] = (card->raw_cid[0] >> 16) & 0xFF; /* Reserved(6)+Card/BGA(2) */
    buf[2] = (card->raw_cid[0] >> 8 ) & 0xFF; /* OEM/Application ID */
    buf[3] = (card->raw_cid[0] >> 0 ) & 0xFF; /* Product name [0] */
    buf[4] = (card->raw_cid[1] >> 24) & 0xFF; /* Product name [1] */
    buf[5] = (card->raw_cid[1] >> 16) & 0xFF; /* Product name [2] */
    buf[6] = (card->raw_cid[1] >> 8 ) & 0xFF; /* Product name [3] */
    buf[7] = (card->raw_cid[1] >> 0 ) & 0xFF; /* Product name [4] */
    buf[8] = (card->raw_cid[2] >> 24) & 0xFF; /* Product name [5] */
    buf[9] = (card->raw_cid[2] >> 16) & 0xFF; /* Product revision */
    buf[10] =(card->raw_cid[2] >> 8 ) & 0xFF; /* Serial Number [0] */
    buf[11] =(card->raw_cid[2] >> 0 ) & 0xFF; /* Serial Number [1] */
    buf[12] =(card->raw_cid[3] >> 24) & 0xFF; /* Serial Number [2] */
    buf[13] =(card->raw_cid[3] >> 16) & 0xFF; /* Serial Number [3] */
    buf[14] =(card->raw_cid[3] >> 8 ) & 0xFF; /* Manufacturer date */
    buf[15] =(card->raw_cid[3] >> 0 ) & 0xFF; /* CRC7 + stuff bit*/
    *fw_len = 1;
    
    /* sandisk */	
    if(buf[0] == 0x45){
        /* before v4.41 */
        if (card->raw_ext_csd[EXT_CSD_REV] <= 5) {
            if (0 == mmc_get_sandisk_fwid(MMC_HOST_ID,buf_sanid)){
                *fw_len = 6;
            }
        } else if (card->raw_ext_csd[EXT_CSD_REV] == 6) { /* v4.5, v4.51 */
            /* do nothing, same as other vendor */
        } else if (card->raw_ext_csd[EXT_CSD_REV] == 7) { /* v5.0 */
            /* sandisk only use 6 bytes */
            *fw_len = 6;
            for (i==0; i<6; i++) {
                buf_sanid[32+i] = card->raw_ext_csd[EXT_CSD_FIRMWARE_VERSION+i];
            }
        }
    }

	len = len > 16 ? 16 : len;
    memcpy(id, buf, len);
	if(*fw_len == 6)
		memcpy(id+len,buf_sanid+32,6);
	else
		*(id+len) = buf[9];
    return 0;
}

static int mmc_get_boot_part(u32 *bootpart)
{
    struct mmc_card *card;
    struct mmc_host *host;
    int err = MMC_ERR_NONE;
    u8 *ext_csd;
    u8 part_config;

    if (!bootpart)
        return -1;

    card = mmc_get_card(MMC_HOST_ID);
    host = mmc_get_host(MMC_HOST_ID);

    if(!card || !host)
        return -1;
    err = mmc_read_ext_csd(host, card);

    if (err == MMC_ERR_NONE) {
        ext_csd = &card->raw_ext_csd[0];

        part_config = (ext_csd[EXT_CSD_PART_CFG] >> 3) & 0x07;
        if (part_config == 1) {
            *bootpart = EMMC_PART_BOOT1;
        }
        else if (part_config == 2) {
            *bootpart = EMMC_PART_BOOT2;
        }
        else if(part_config == 7) {
            *bootpart = EMMC_PART_USER;
        }
        else {
            print("[PL MTK] fail to get boot part\n");
            return -1;
        }
    }

    return err;
}

int mmc_bread_boot(blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf)
{
    u32 pid, id;
    struct mmc_host *host;

    if(bdev && bdev->priv){
        host = (struct mmc_host*)bdev->priv;
        id = host->id;
    }else {
        printf("[%s]: invalid bdev, force the host id to 0\n", __func__, pid);
        id = 0;
    }

    if(mmc_get_boot_part(&pid)){
        printf("[%s]: invalid pid=%d\n", __func__, pid);
        return -1;
    }

    mmc_switch_part(pid);

    return mmc_block_read(id, (unsigned long)blknr, blks, (unsigned long*)buf);
}

int mmc_bwrite_boot(blkdev_t *bdev, u32 blknr, u32 blks, u8 *buf)
{
    u32 pid, id;
    struct mmc_host *host;

    if(bdev && bdev->priv){
        host = (struct mmc_host*)bdev->priv;
        id = host->id;
    }else {
        id = 0;
    }

    if(mmc_get_boot_part(&pid)){
        printf("[%s]: invalid pid=%d\n", __func__, pid);
        return -1;
    }

    mmc_switch_part(pid);

    return mmc_block_write(id, (unsigned long)blknr, blks, (unsigned long*)buf);
}

