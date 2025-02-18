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

#ifndef _MTK_MMSDK_DEVICE_EFFECT_H_
#define _MTK_MMSDK_DEVICE_EFFECT_H_

#include <pthread.h>
#include <hardware/hardware.h>
#include <effect_device.h>

namespace NSMMSdkHAL {
//
class EffectDevice {
    public:
        // id is used to distinguish cameras. 0 <= id < NUM_CAMERAS.
        // module is a handle to the HAL module, used when the device is opened.
        EffectDevice();
        ~EffectDevice();

        // Common MMSDK Device Operations (see <hardware/mmsdk_common.h>)
        int open(const hw_module_t *module, hw_device_t **device);
        int close();

        //
        bool createEffect(char const* effect_name, 
                          eEFFECT_MODE eMode, void *cookie);                  

        //
        char* getEffectName(void *cookie); 

        //
        bool setParameters(const char *param, 
                            const void *value, void *cookie);  

        //
        bool apply(NSCam::IImageBuffer const *srcImage, 
                   NSCam::IImageBuffer const *destImage,
                   void *cookie
                  ); 

        //
        bool release(void *cookie); 

    protected:  
        bool allocBuf(NSCam::IImageBuffer **pBuf, NSCam::MSize const & imgSize, int const format); 
        bool freeBuf(NSCam::IImageBuffer **pBuf); 
        bool imageTransform(NSCam::IImageBuffer const *pSrcImage, NSCam::IImageBuffer const *pDestImage); 
        bool applyFBEffect(NSCam::IImageBuffer const *pSrcImage, NSCam::IImageBuffer const *pDestImage); 

    private:
        // effect device handle returned to framework for use
        effect_device_t mDevice;

        // effect device operations handle shared by all devices
        const static effect_device_ops sOps;
 
        //[FIXME] should use facebeauty name 
        int mSmoothLevel; 
        int mSkinColor; 
        int mEnlargeEyeLevel; 
        int mSlimFaceLevel; 
        int mMode; 

};
} // namespace NSMMSdkHAL

#endif // _MTK_MMSDK_DEVICE_EFFECT_H_
