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

#define LOG_TAG "MtkCam/Shot"
//
#include <dlfcn.h>
//
#include <Log.h>
#include <common.h>
using namespace NSCam;
#include <v1/IParamsManager.h>
//
#include <Shot/IShot.h>
#include <cutils/properties.h>
//
using namespace android;
using namespace NSShot;


/******************************************************************************
 *
 ******************************************************************************/
#define MY_LOGV(fmt, arg...)        CAM_LOGV("(%d)[%s] " fmt, ::gettid(), __FUNCTION__, ##arg)
#define MY_LOGD(fmt, arg...)        CAM_LOGD("(%d)[%s] " fmt, ::gettid(), __FUNCTION__, ##arg)
#define MY_LOGI(fmt, arg...)        CAM_LOGI("(%d)[%s] " fmt, ::gettid(), __FUNCTION__, ##arg)
#define MY_LOGW(fmt, arg...)        CAM_LOGW("(%d)[%s] " fmt, ::gettid(), __FUNCTION__, ##arg)
#define MY_LOGE(fmt, arg...)        CAM_LOGE("(%d)[%s] " fmt, ::gettid(), __FUNCTION__, ##arg)
#define MY_LOGA(fmt, arg...)        CAM_LOGA("(%d)[%s] " fmt, ::gettid(), __FUNCTION__, ##arg)
#define MY_LOGF(fmt, arg...)        CAM_LOGF("(%d)[%s] " fmt, ::gettid(), __FUNCTION__, ##arg)
//
#define MY_LOGV_IF(cond, ...)       do { if ( (cond) ) { MY_LOGV(__VA_ARGS__); } }while(0)
#define MY_LOGD_IF(cond, ...)       do { if ( (cond) ) { MY_LOGD(__VA_ARGS__); } }while(0)
#define MY_LOGI_IF(cond, ...)       do { if ( (cond) ) { MY_LOGI(__VA_ARGS__); } }while(0)
#define MY_LOGW_IF(cond, ...)       do { if ( (cond) ) { MY_LOGW(__VA_ARGS__); } }while(0)
#define MY_LOGE_IF(cond, ...)       do { if ( (cond) ) { MY_LOGE(__VA_ARGS__); } }while(0)
#define MY_LOGA_IF(cond, ...)       do { if ( (cond) ) { MY_LOGA(__VA_ARGS__); } }while(0)
#define MY_LOGF_IF(cond, ...)       do { if ( (cond) ) { MY_LOGF(__VA_ARGS__); } }while(0)


////////////////////////////////////////////////////////////////////////////////
//  Function Prototype.
////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 *  Normal Shot
 ******************************************************************************/
extern "C"
sp<IShot>
createInstance_NormalShot(
    char const*const    pszShotName,
    uint32_t const      u4ShotMode,
    int32_t const       i4OpenId
);


/******************************************************************************
 *  Continuous Shot
 ******************************************************************************/
extern "C"
sp<IShot>
createInstance_ContinuousShot(
    char const*const    pszShotName,
    uint32_t const      u4ShotMode,
    int32_t const       i4OpenId
);


/******************************************************************************
 *  Stereo Shot
 ******************************************************************************/
extern "C"
sp<IShot>
createInstance_StereoShot(
    char const*const    pszShotName,
    uint32_t const      u4ShotMode,
    int32_t const       i4OpenId
);


#if 0
/******************************************************************************
 *  Bss Shot
 ******************************************************************************/
extern "C"
sp<IShot>
createInstance_BestShot(
    char const*const    pszShotName,
    uint32_t const      u4ShotMode,
    int32_t const       i4OpenId
);
#endif

#if 0
/******************************************************************************
 *  Ev Shot
 ******************************************************************************/
extern "C"
sp<IShot>
createInstance_EvShot(
    char const*const    pszShotName,
    uint32_t const      u4ShotMode,
    int32_t const       i4OpenId
);
#endif

#if 0
/******************************************************************************
 *  HDR Shot
 ******************************************************************************/
extern "C"
sp<IShot>
createInstance_HdrShot(
    char const*const    pszShotName,
    uint32_t const      u4ShotMode,
    int32_t const       i4OpenId
);
#endif

/******************************************************************************
 *  FB Shot
 ******************************************************************************/
typedef struct FACE_BEAUTY_SHOT_INFO {
    int32_t iSmoothLevel;
    int32_t iSkinColor;
    int32_t iEnlargeEyeLevel;
    int32_t iSlimFaceLevel;
    int32_t iExtremeBeauty;
    typedef struct
    {
        int32_t x;
        int32_t y;
    } FACE_BEAUTY_POS;
    Vector<FACE_BEAUTY_POS> iFacePos;
};
extern "C"
sp<IShot>
createInstance_FaceBeautyShot(
    char const*const    pszShotName,
    uint32_t const      u4ShotMode,
    int32_t const       i4OpenId,
    FACE_BEAUTY_SHOT_INFO const* pInfo
);

#if 1
/******************************************************************************
 *  Engineer Shot
 ******************************************************************************/
extern "C"
sp<IShot>
createInstance_EngShot(
    char const*const    pszShotName,
    uint32_t const      u4ShotMode,
    int32_t const       i4OpenId
);
#endif

/******************************************************************************
 *  CapBuf Shot
 ******************************************************************************/
extern "C"
sp<IShot>
createInstance_CapBufShot(
    char const*const    pszShotName,
    uint32_t const      u4ShotMode,
    int32_t const       i4OpenId
);

////////////////////////////////////////////////////////////////////////////////
//  Shot Factory
////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 * @brief Create a shot instance.
 *
 * @details
 *
 * @note
 *
 * @param[out] rpShot: Reference to a pointer to a newly-created IShot instance.
 *
 * @param[in] u4ShotMode: Shot Mode (defined in EShotMode)
 *
 * @param[in] i4OpenId: Open id: 0, 1, 2, and so on.
 *
 * @param[in] pParamsMgr: Pointer to IParamsManager instance.
 *
 * @return
 *      true indicates success; false indicates failure.
 *
 ******************************************************************************/
bool
createShotInstance(
    sp<IShot>&          rpShot,
    uint32_t const      u4ShotMode,
    int32_t const       i4OpenId,
    sp<IParamsManager>  pParamsMgr
)
{
    if  ( rpShot != 0 )
    {
        if  ( rpShot->getShotMode() == u4ShotMode )
        {
            MY_LOGI("No change ShotMode: <%#x>", u4ShotMode);
            return  true;
        }
        else
        {
            MY_LOGI("Change ShotMode: <%#x> -> <%#x>", rpShot->getShotMode(), u4ShotMode);
            rpShot = NULL;
        }
    }
    //
    //
    char value[PROPERTY_VALUE_MAX] = {'\0'};
    switch  ( u4ShotMode )
    {
    //  Normal Shot
    case eShotMode_NormalShot:
        rpShot = createInstance_NormalShot("NormalShot", u4ShotMode, i4OpenId);
        break;
    case eShotMode_ZsdMfllShot:
        {
            rpShot = createInstance_NormalShot("NormalShot", u4ShotMode, i4OpenId);
        }
        break;

    //
    //  Smile-detection Shot
    case eShotMode_SmileShot:
        rpShot = createInstance_NormalShot("SmileShot", u4ShotMode, i4OpenId);
        break;
    //
    //  Auto-scene-detection Shot
    case eShotMode_AsdShot:
        rpShot = createInstance_NormalShot("AsdShot", u4ShotMode, i4OpenId);
        break;
    //
    //  Engineer mode Shot
    case eShotMode_EngShot:
        rpShot = createInstance_EngShot("EngShot", u4ShotMode, i4OpenId);
        break;
    //
    //  Continuous Shot Ncc
    case eShotMode_ContinuousShot:
        rpShot = createInstance_ContinuousShot("ContinuousShot", u4ShotMode, i4OpenId);
        break;

    //  Continuous Shot Cc
    case eShotMode_ContinuousShotCc:
        rpShot = createInstance_ContinuousShot("ContinuousShotCc", u4ShotMode, i4OpenId);
        break;
    //
#if 0
    //  Best Shot
    case eShotMode_BestShot:
        rpShot = createInstance_BestShot("BestShot", u4ShotMode, i4OpenId);
        break;
#endif
    //
    //  Ev-bracketshot Shot
#if 0
    case eShotMode_EvShot:
        rpShot = createInstance_EvShot("EvShot", u4ShotMode, i4OpenId);
        break;
#endif
    //
    //  High-dynamic-range Shot
#if 0
    case eShotMode_ZsdHdrShot:
    case eShotMode_HdrShot:
        rpShot = createInstance_HdrShot("hdr", u4ShotMode, i4OpenId);
        break;
#endif
    //
    //  Zero-shutter-delay Shot
    case eShotMode_ZsdShot:
        rpShot = createInstance_CapBufShot("ZSD", u4ShotMode, i4OpenId);
        break;
    //
    //  Video Snap Shot
    case eShotMode_VideoSnapShot:
        rpShot = createInstance_CapBufShot("VSS", u4ShotMode, i4OpenId);
        break;
    //
    //  Face-beautifier Shot
    case eShotMode_FaceBeautyShot: {
        FACE_BEAUTY_SHOT_INFO info;
        info.iSmoothLevel = pParamsMgr->getInt(MtkCameraParameters::KEY_FB_SMOOTH_LEVEL);
        info.iSkinColor = pParamsMgr->getInt(MtkCameraParameters::KEY_FB_SKIN_COLOR);
        info.iEnlargeEyeLevel = pParamsMgr->getInt(MtkCameraParameters::KEY_FB_ENLARGE_EYE);
        info.iSlimFaceLevel = pParamsMgr->getInt(MtkCameraParameters::KEY_FB_SLIM_FACE);
        if(::strcmp(pParamsMgr->getStr(MtkCameraParameters::KEY_FB_EXTREME_BEAUTY), MtkCameraParameters::TRUE) == 0)
        {
            info.iExtremeBeauty = true;
        }
        else
        {
            info.iExtremeBeauty = false;
        }
        Vector<IParamsManager::FACE_BEAUTY_POS> gFacePos;
        pParamsMgr->getFaceBeautyBeautifiedPosition(gFacePos);

        for (Vector<IParamsManager::FACE_BEAUTY_POS>::iterator it = gFacePos.begin();
                it != gFacePos.end();
                it++)
        {
            FACE_BEAUTY_SHOT_INFO::FACE_BEAUTY_POS pos;
            pos.x = it->x;
            pos.y = it->y;
            info.iFacePos.push_back(pos);
        }
        rpShot = createInstance_FaceBeautyShot("FBShot", u4ShotMode, i4OpenId, &info);
        }break;
    //
    //  Multi-motion Shot
//    case eShotMode_MultiMotionShot:
//        break;
    //
    default: {
            MY_LOGW("Unsupported ShotMode<%#x>", u4ShotMode);
            char const szShotName[] = "TestShot";
            String8 const s8ShotInstFactory = String8::format("createInstance_%s", szShotName);
            //
            void* pCreateInstance = ::dlsym(RTLD_DEFAULT, s8ShotInstFactory.string());
            if  ( ! pCreateInstance ) {
                MY_LOGE("Not exist: %s", s8ShotInstFactory.string());
            }
            else {
                rpShot =reinterpret_cast<sp<IShot> (*)(char const*const, uint32_t const, int32_t const)>
                            (pCreateInstance)(szShotName, u4ShotMode, i4OpenId)
                            ;
            }
        }break;
    }
    //
    return  (rpShot != 0);
}

