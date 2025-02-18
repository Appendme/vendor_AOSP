/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/
#ifndef MTK_STREAM_ID_H
#define MTK_STREAM_ID_H
namespace NSMtkStreamId
{
    typedef enum StreamIDs
    {
        STREAM_ID_PASS1_RESIZE          = 1,
        STREAM_ID_PASS1_FULLSIZE,
        STREAM_ID_PASS2_PICTUREYUV,
        STREAM_ID_PASS2_THUMBNAILYUV,
        //
        STREAM_ID_PASS2_OUT1,
        STREAM_ID_PASS2_OUT2,
        STREAM_ID_PASS2_OUT3,
        //
        STREAM_ID_JPG,
        //
        STREAM_ID_METADATA_CONTROL_APP,
        STREAM_ID_METADATA_CONTROL_HAL, // 10
        STREAM_ID_METADATA_RESULT_P1_APP,
        STREAM_ID_METADATA_RESULT_P1_HAL,
        STREAM_ID_METADATA_RESULT_P2_APP,
        STREAM_ID_METADATA_RESULT_P2_HAL,
        STREAM_ID_METADATA_RESULT_JPG_APP,
        STREAM_ID_METADATA_RESULT_JPG_HAL,
        //
        STREAM_ID_RAW16_OUT,

        //////////////////////////////////
        // VSDOF
        //////////////////////////////////
        STREAM_ID_METADATA_CONTROL_APP_MAIN2,
        STREAM_ID_METADATA_CONTROL_HAL_MAIN2,
        // P1Node (Main2)
        STREAM_ID_METADATA_RESULT_P1_APP_MAIN2, // 20
        STREAM_ID_METADATA_RESULT_P1_HAL_MAIN2,
        STREAM_ID_PASS1_FULLSIZE_MAIN2,
        STREAM_ID_PASS1_RESIZE_MAIN2,
        // DepthMapNode
        STREAM_ID_METADATA_RESULT_DEPTH_APP,
        STREAM_ID_METADATA_RESULT_DEPTH_HAL,
        STREAM_ID_DEPTHMAPNODE_DEPTHMAPYUV,
        STREAM_ID_DEPTHMAPNODE_MAINIMAGEYUV,
        STREAM_ID_DEPTHMAPNODE_MAINIMAGE_CAPYUV,
        STREAM_ID_DEPTHMAPNODE_MY_SYUV,
        STREAM_ID_DEPTHMAPNODE_DMWYUV,  // 30
        STREAM_ID_DEPTHMAPNODE_JPSMAIN1YUV,
        STREAM_ID_DEPTHMAPNODE_JPSMAIN2YUV,
        STREAM_ID_DEPTHMAPNODE_FDYUV,
        // BokehNode
        STREAM_ID_METADATA_RESULT_BOKEH_APP,
        STREAM_ID_METADATA_RESULT_BOKEH_HAL,
        STREAM_ID_BOKEHNODE_CLEANIMAGEYUV,
        STREAM_ID_BOKEHNODE_RECORDYUV,
        STREAM_ID_BOKEHNODE_PREVIEWYUV,
        STREAM_ID_BOKEHNODE_RESULTYUV,
        // DualImageTransfromNode
        STREAM_ID_METADATA_RESULT_DUALIMAGETRANSFORM_APP,   //40
        STREAM_ID_METADATA_RESULT_DUALIMAGETRANSFORM_HAL,
        STREAM_ID_DUALIMAGETRANSFORM_JPSYUV,
        // Jpeg
        STREAM_ID_JPG_Bokeh,
        STREAM_ID_METADATA_RESULT_JPG_BOKEH_APP,
        STREAM_ID_JPG_JPS,
        STREAM_ID_METADATA_RESULT_JPG_JPS_APP,
    }StreamIDs;

    typedef enum EPipe_CB
    {
        EPIPE_CB_NONE = 0,
        EPIPE_CB_IMG_JPG
    }EPipe_CB;
}
#endif  //_MTK_CAMERA_INCLUDE_CAMNODE_STREAM_ID_H_
