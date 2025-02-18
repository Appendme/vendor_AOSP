/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
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
TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/
#define LOG_TAG "ae_cct_feature"

#ifndef ENABLE_MY_LOG
    #define ENABLE_MY_LOG       (1)
#endif

//#include <CamDefs.h>
#include <aaa_types.h>
#include <aaa_error_code.h>
#include <aaa_log.h>
#include <aaa_hal_if.h>
//#include <aaa_hal.h>
#include <camera_custom_nvram.h>
#include <awb_param.h>
#include <ae_param.h>
#include <af_param.h>
#include <flash_param.h>
//#include <sensor_hal.h>
#include <nvram_drv.h>
#include <nvram_drv_mgr.h>
#include <ae_tuning_custom.h>
#include <isp_mgr.h>
#include <ispdrv_mgr.h>
#include <isp_tuning_mgr.h>
#include <aaa_sensor_mgr.h>
#include "ae_mgr_if.h"
#include "ae_mgr.h"
#include <cct_feature.h>
#include <nvbuf_util.h>
#include <flash_mgr.h>

using namespace NS3Av3;
using namespace NSIspTuningv3;

static MUINT32 g_u4PreviewFlareOffset;
static MUINT32 g_u4CaptureFlareOffset;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::
CCTOPAEEnable()
{
    MY_LOG("[ACDK_CCT_V2_OP_AE_ENABLE_AUTO_RUN]\n");

    enableAE();

    return S_AE_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::
CCTOPAEDisable()
{
    MY_LOG("[ACDK_CCT_V2_OP_AE_DISABLE_AUTO_RUN]\n");

    disableAE();

    return S_AE_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOPAEGetEnableInfo(
    MINT32 *a_pEnableAE,
    MUINT32 *a_pOutLen
)
{
    MY_LOG("[ACDK_CCT_OP_AE_GET_ENABLE_INFO]\n");

    *a_pEnableAE = m_bEnableAE;

    *a_pOutLen = sizeof(MINT32);

    MY_LOG("AE Enable = %d\n", *a_pEnableAE);

    return S_AE_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOPSetAETargetValue(
    MUINT32 u4AETargetValue
)
{
    MY_LOG("[ACDK_CCT_OP_DEV_AE_SET_TARGET]\n");

    if(m_pIAeAlgo != NULL) {
        m_pIAeAlgo->setAETargetValue((MUINT32)u4AETargetValue);
    } else {
        MY_LOG("The AE algo class is NULL (AE target:%d)\n", u4AETargetValue);
    }

    MY_LOG("[AE Target] = %d\n", u4AETargetValue);

    return S_AE_OK;

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOPAESetSceneMode(
    MINT32 a_AEScene
)
{
    MY_LOG("[ACDK_CCT_OP_DEV_AE_SET_SCENE_MODE]\n");

    setSceneMode((MUINT32) a_AEScene);

    MY_LOG("[AE Scene] = %d\n", a_AEScene);

    return S_AE_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOPAEGetAEScene(
    MINT32 *a_pAEScene,
    MUINT32 *a_pOutLen
)
{
    MY_LOG("[ACDK_CCT_V2_OP_AE_GET_SCENE_MODE]\n");

    *a_pAEScene = m_eAEScene;

    *a_pOutLen = sizeof(MINT32);

    MY_LOG("[AE Scene] = %d\n", *a_pAEScene);

    return S_AE_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOPAESetMeteringMode(
    MINT32 a_AEMeteringMode
)
{
    MY_LOG("[ACDK_CCT_V2_OP_AE_SET_METERING_MODE]\n");

    setAEMeteringMode((MUINT32)a_AEMeteringMode);

    MY_LOG("[AE Metering Mode] = %d\n", a_AEMeteringMode);

    return S_AE_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOPAEApplyExpParam(
    MVOID *a_pAEExpParam
)
{
    MY_LOG("[ACDK_CCT_V2_OP_AE_APPLY_EXPO_INFO]\n");

    ACDK_AE_MODE_CFG_T *pAEExpParam = (ACDK_AE_MODE_CFG_T *)a_pAEExpParam;
    MUINT32 u4AFEGain = 0, u4IspGain = 1024, u4BinningRatio = 1;
    AE_INFO_T rAEInfo2ISP;

    // Set exposure mode
    m_rAEOutput.rPreviewMode.u4ExposureMode = pAEExpParam->u4ExposureMode;
    // Set exposure time
    m_rAEOutput.rPreviewMode.u4Eposuretime = pAEExpParam->u4Eposuretime;

    (m_rAEOutput.rPreviewMode.u4ExposureMode == eAE_EXPO_TIME) ? (AAASensorMgr::getInstance().setSensorExpTime((ESensorDev_T)m_eSensorDev, m_rAEOutput.rPreviewMode.u4Eposuretime))
                                                   : (AAASensorMgr::getInstance().setSensorExpLine((ESensorDev_T)m_eSensorDev, m_rAEOutput.rPreviewMode.u4Eposuretime));

    // Set sensor gain
    if (pAEExpParam->u4GainMode == 0) { // AfeGain and isp gain
        u4AFEGain = pAEExpParam->u4AfeGain;
        u4IspGain = pAEExpParam->u4IspGain;
    }
    else { // ISO
        if(m_p3ANVRAM != NULL) {
            u4AFEGain = (pAEExpParam->u4ISO * 1024) / m_p3ANVRAM->rAENVRAM.rDevicesInfo.u4MiniISOGain;
        } else {
            MY_LOG("NVRAM is NULL\n");
            u4AFEGain = 1024;
        }
        u4IspGain = 1024;
    }

    m_rAEOutput.rPreviewMode.u4AfeGain = u4AFEGain;
    AAASensorMgr::getInstance().setSensorGain((ESensorDev_T)m_eSensorDev, m_rAEOutput.rPreviewMode.u4AfeGain);
    m_rAEOutput.rPreviewMode.u4IspGain = u4IspGain;
    IspTuningMgr::getInstance().setISPAEGain((ESensorDev_T)m_eSensorDev, MFALSE, m_rAEOutput.rPreviewMode.u4IspGain>>1);

    // Set flare
    if((m_pIAeAlgo != NULL)&&(!pAEExpParam->bFlareAuto)) {
        MY_LOG("[CCTOPAEApplyExpParam] u2FlareValue = %d u2CaptureFlareValue:%d\n", pAEExpParam->u2FlareValue, pAEExpParam->u2CaptureFlareValue);    
        m_pIAeAlgo->SetPreviewFlareValue( pAEExpParam->u2FlareValue);
        m_pIAeAlgo->SetCaptureFlareValue(pAEExpParam->u2CaptureFlareValue);
    } else {
        MY_LOG("[CCTOPAEApplyExpParam] m_pIAeAlgo = %d bFlareAuto:%d\n", m_pIAeAlgo, pAEExpParam->bFlareAuto);        
    }

    m_rAEOutput.rPreviewMode.i2FlareOffset = pAEExpParam->u2FlareValue;
    m_rAEOutput.rPreviewMode.i2FlareGain = (MINT16)(FLARE_SCALE_UNIT * FLARE_OFFSET_DOMAIN / (FLARE_OFFSET_DOMAIN  - pAEExpParam->u2FlareValue));
    IspTuningMgr::getInstance().setIspFlareGainOffset((ESensorDev_T)m_eSensorDev, m_rAEOutput.rPreviewMode.i2FlareGain, (-1*m_rAEOutput.rPreviewMode.i2FlareOffset));

    // Update capture exposure time/gain/flare
    if(m_p3ANVRAM != NULL) {
        if(m_p3ANVRAM->rAENVRAM.rDevicesInfo.u4Cap2PreRatio <= 300) {
            u4BinningRatio = 4;
        } else if(m_p3ANVRAM->rAENVRAM.rDevicesInfo.u4Cap2PreRatio <= 450) {
            u4BinningRatio = 3;
        } else if(m_p3ANVRAM->rAENVRAM.rDevicesInfo.u4Cap2PreRatio <= 768) {
            u4BinningRatio = 2;
        } else {
            u4BinningRatio = 1;
        }
    } else {
        MY_LOG("NVRAM is NULL\n");
        u4BinningRatio = 1;
    }

    m_rAEOutput.rCaptureMode[0].u4ExposureMode = pAEExpParam->u4ExposureMode;
    m_rAEOutput.rCaptureMode[0].u4Eposuretime = (pAEExpParam->u4Eposuretime)*u4BinningRatio;
    m_rAEOutput.rCaptureMode[0].u4AfeGain = u4AFEGain;
    m_rAEOutput.rCaptureMode[0].u4IspGain = u4IspGain;
    m_rAEOutput.rCaptureMode[0].i2FlareOffset = pAEExpParam->u2CaptureFlareValue;
    m_rAEOutput.rCaptureMode[0].i2FlareGain = (MINT16)(FLARE_SCALE_UNIT * FLARE_OFFSET_DOMAIN / (FLARE_OFFSET_DOMAIN - pAEExpParam->u2CaptureFlareValue));

    // backup the capture parameters.
    m_rAEOutput.rCaptureMode[2].u4ExposureMode = m_rAEOutput.rCaptureMode[0].u4ExposureMode;
    m_rAEOutput.rCaptureMode[2].u4Eposuretime = m_rAEOutput.rCaptureMode[0].u4Eposuretime;
    m_rAEOutput.rCaptureMode[2].u4AfeGain = m_rAEOutput.rCaptureMode[0].u4AfeGain;
    m_rAEOutput.rCaptureMode[2].u4IspGain = m_rAEOutput.rCaptureMode[0].u4IspGain;
    m_rAEOutput.rCaptureMode[2].i2FlareOffset = m_rAEOutput.rCaptureMode[0].i2FlareOffset;
    m_rAEOutput.rCaptureMode[2].i2FlareGain = m_rAEOutput.rCaptureMode[0].i2FlareGain;

    m_bIsAutoFlare = pAEExpParam->bFlareAuto;
    g_u4PreviewFlareOffset = (MUINT32)pAEExpParam->u2FlareValue;
    g_u4CaptureFlareOffset = (MUINT32)m_rAEOutput.rCaptureMode[0].i2FlareOffset;

    rAEInfo2ISP.u4Eposuretime = m_rAEOutput.rPreviewMode.u4Eposuretime;
    rAEInfo2ISP.i2FlareOffset  = m_rAEOutput.rPreviewMode.i2FlareOffset;
    rAEInfo2ISP.u4AfeGain = m_rAEOutput.rPreviewMode.u4AfeGain;
    rAEInfo2ISP.u4IspGain = m_rAEOutput.rPreviewMode.u4IspGain;
    rAEInfo2ISP.u4RealISOValue = pAEExpParam->u4ISO;
    IspTuningMgr::getInstance().setAEInfo((ESensorDev_T)m_eSensorDev, rAEInfo2ISP);

    MY_LOG("[Exp Time] = %d\n", pAEExpParam->u4ExposureMode);
    MY_LOG("[Exp Time] = %d\n", pAEExpParam->u4Eposuretime);
    MY_LOG("[ISO] = %d\n", pAEExpParam->u4ISO);
    MY_LOG("[AFE Gain] = %d\n", m_rAEOutput.rPreviewMode.u4AfeGain);
    MY_LOG("[Isp Gain] = %d\n", m_rAEOutput.rPreviewMode.u4IspGain);
    MY_LOG("[PV Flare] = %d\n", pAEExpParam->u2FlareValue);
    MY_LOG("[PV Flare Gain] = %d\n", pAEExpParam->u2FlareGain);
    MY_LOG("[CAP Flare] = %d\n", pAEExpParam->u2CaptureFlareValue);
    MY_LOG("[CAP Flare Gain] = %d\n", pAEExpParam->u2CaptureFlareGain);
    MY_LOG("[Flare Auto] = %d\n", pAEExpParam->bFlareAuto);

    return S_AE_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOSetCaptureParams(
    MVOID *a_pAEExpParam
)
{
    MY_LOG("[CCTOSetCaptureParams]\n");

    AE_MODE_CFG_T cap_mode;
    ACDK_AE_MODE_CFG_T *pcfg_in = (ACDK_AE_MODE_CFG_T *)a_pAEExpParam;

    cap_mode.u4ExposureMode = pcfg_in->u4ExposureMode;
    cap_mode.u4Eposuretime = pcfg_in->u4Eposuretime;

    // Set sensor gain
    if (pcfg_in->u4GainMode == 0) { // AfeGain and isp gain
        cap_mode.u4AfeGain = pcfg_in->u4AfeGain;
        cap_mode.u4IspGain = pcfg_in->u4IspGain;
    }
    else { // ISO
        if(m_p3ANVRAM != NULL) {
            cap_mode.u4AfeGain = (pcfg_in->u4ISO * 1024) / m_p3ANVRAM->rAENVRAM.rDevicesInfo.u4MiniISOGain;
        } else {
            MY_LOG("NVRAM is NULL\n");
            cap_mode.u4AfeGain = 1024;
        }
        cap_mode.u4IspGain = 1024;
    }

    cap_mode.u2FrameRate = pcfg_in->u2FrameRate;
    cap_mode.u4RealISO = pcfg_in->u4ISO;

    // Set flare
    if((m_pIAeAlgo != NULL)&&(!pcfg_in->bFlareAuto))
        m_pIAeAlgo->SetCaptureFlareValue(pcfg_in->u2CaptureFlareValue);

    cap_mode.i2FlareOffset = pcfg_in->u2CaptureFlareValue;
    cap_mode.i2FlareGain = (MINT16)(FLARE_SCALE_UNIT * FLARE_OFFSET_DOMAIN / (FLARE_OFFSET_DOMAIN - pcfg_in->u2CaptureFlareValue)); //pcfg_in->u2CaptureFlareGain;

    m_rAEOutput.rCaptureMode[0].i2FlareOffset = cap_mode.i2FlareOffset;
    m_rAEOutput.rCaptureMode[0].i2FlareGain = cap_mode.i2FlareGain;

    m_bIsAutoFlare = pcfg_in->bFlareAuto;

    MY_LOG("[CCTOSetCaptureParams] -- Cap. Exp Mode = %d\n", cap_mode.u4ExposureMode);
    MY_LOG("[CCTOSetCaptureParams] -- Cap. Exp Time = %d\n", cap_mode.u4Eposuretime);
    MY_LOG("[CCTOSetCaptureParams] -- Cap. Gain Mode = %d\n", pcfg_in->u4GainMode);
    MY_LOG("[CCTOSetCaptureParams] -- Cap. Afe Gain = %d\n", cap_mode.u4AfeGain);
    MY_LOG("[CCTOSetCaptureParams] -- Cap. Isp Gain = %d\n", cap_mode.u4IspGain);
    MY_LOG("[CCTOSetCaptureParams] -- Cap. Frame Rate = %d\n", cap_mode.u2FrameRate);
    MY_LOG("[CCTOSetCaptureParams] -- Cap. ISO = %d\n", cap_mode.u4RealISO);
    MY_LOG("[CCTOSetCaptureParams] -- Cap. Flare Offset = %d\n", cap_mode.i2FlareOffset);
    MY_LOG("[CCTOSetCaptureParams] -- Cap. Flare Gain = %d\n", cap_mode.i2FlareGain);
    MY_LOG("[CCTOSetCaptureParams] -- Flare Auto = %d\n", m_bIsAutoFlare);

    updateCaptureParams(cap_mode);

    return S_AE_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOGetCaptureParams(
    MVOID *a_pAEExpParam
)
{
    MY_LOG("[CCTOGetCaptureParams]\n");

    ACDK_AE_MODE_CFG_T *pout_cfg = (ACDK_AE_MODE_CFG_T *)a_pAEExpParam;
    AE_MODE_CFG_T ae_mode;

    getCaptureParams(ae_mode);

    MY_LOG("[CCTOGetCaptureParams] -- Cap. Exp Mode = %d\n", ae_mode.u4ExposureMode);
    MY_LOG("[CCTOGetCaptureParams] -- Cap Exp Time = %d\n", ae_mode.u4Eposuretime);
    MY_LOG("[CCTOGetCaptureParams] -- Cap Afe Gain = %d\n", ae_mode.u4AfeGain);
    MY_LOG("[CCTOGetCaptureParams] -- Cap Isp Gain = %d\n", ae_mode.u4IspGain);
    MY_LOG("[CCTOGetCaptureParams] -- Cap Frame Rate = %d\n", ae_mode.u2FrameRate);
    MY_LOG("[CCTOGetCaptureParams] -- Cap ISO = %d\n", ae_mode.u4RealISO);
    MY_LOG("[CCTOGetCaptureParams] -- Cap Flare Offset = %d\n", ae_mode.i2FlareOffset);
    MY_LOG("[CCTOGetCaptureParams] -- Cap Flare Gain = %d\n", ae_mode.i2FlareGain);
    MY_LOG("[CCTOGetCaptureParams] -- Flare Auto = %d\n", m_bIsAutoFlare);

    pout_cfg->u4ExposureMode = ae_mode.u4ExposureMode;
    pout_cfg->u4Eposuretime = ae_mode.u4Eposuretime;
    pout_cfg->u4GainMode = 0;
    pout_cfg->u4AfeGain = ae_mode.u4AfeGain;
    pout_cfg->u4IspGain = ae_mode.u4IspGain;
    pout_cfg->u4ISO = ae_mode.u4RealISO;
    pout_cfg->u2FrameRate = ae_mode.u2FrameRate;
    pout_cfg->u2CaptureFlareGain = m_rAEOutput.rCaptureMode[0].i2FlareGain;
    pout_cfg->u2CaptureFlareValue = m_rAEOutput.rCaptureMode[0].i2FlareOffset;

    if(m_bIsAutoFlare)
        pout_cfg->bFlareAuto = MTRUE;
    else
        pout_cfg->bFlareAuto = MFALSE;

    return S_AE_OK;
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOPAESetFlickerMode(
    MINT32 a_AEFlickerMode
)
{
    MY_LOG("[ACDK_CCT_V2_OP_AE_SELECT_BAND]\n");

    setAEFlickerMode((MUINT32) a_AEFlickerMode);

    MY_LOG("[AE Flicker Mode] = %d\n", a_AEFlickerMode);

    return S_AE_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOPAEGetExpParam(
    MVOID *a_pAEExpParamIn,
    MVOID *a_pAEExpParamOut,
    MUINT32 *a_pOutLen
)
{
    MY_LOG("[ACDK_CCT_V2_OP_AE_GET_AUTO_EXPO_PARA]\n");

    ACDK_AE_MODE_CFG_T *pAEExpParamIn = (ACDK_AE_MODE_CFG_T *)a_pAEExpParamIn;
    ACDK_AE_MODE_CFG_T *pAEExpParamOut = (ACDK_AE_MODE_CFG_T *)a_pAEExpParamOut;

    pAEExpParamOut->u4GainMode = pAEExpParamIn->u4GainMode;
    MY_LOG("[Gain Mode] = %d\n", pAEExpParamOut->u4GainMode);
    pAEExpParamOut->u4AfeGain = m_rAEOutput.rPreviewMode.u4AfeGain;
    MY_LOG("[AFE Gain] = %d\n", pAEExpParamOut->u4AfeGain);
    pAEExpParamOut->u4IspGain = m_rAEOutput.rPreviewMode.u4IspGain;
    MY_LOG("[Isp Gain] = %d\n", pAEExpParamOut->u4IspGain);
    if(m_p3ANVRAM != NULL) {
        pAEExpParamOut->u4ISO = m_rAEOutput.rPreviewMode.u4IspGain*(((m_rAEOutput.rPreviewMode.u4AfeGain * m_p3ANVRAM->rAENVRAM.rDevicesInfo.u4MiniISOGain) + 512) / 1024) / 1024;
    } else {
        MY_LOG("NVRAM is NULL\n");
        pAEExpParamOut->u4ISO =  m_rAEOutput.rPreviewMode.u4IspGain*(((m_rAEOutput.rPreviewMode.u4AfeGain * 100) + 512) / 1024) / 1024;
    }
    MY_LOG("[ISO] = %d\n", pAEExpParamOut->u4ISO);

    pAEExpParamOut->u4ExposureMode = m_rAEOutput.rPreviewMode.u4ExposureMode;
    MY_LOG("[Exp Mode] = %d\n", pAEExpParamOut->u4ExposureMode);
    pAEExpParamOut->u4Eposuretime = m_rAEOutput.rPreviewMode.u4Eposuretime;
    MY_LOG("[Exp Time] = %d\n", pAEExpParamOut->u4Eposuretime);

    if(m_bIsAutoFlare) {
        pAEExpParamOut->u2FlareValue = (MUINT16)m_rAEOutput.rPreviewMode.i2FlareOffset;
        pAEExpParamOut->u2FlareGain = (MUINT16)m_rAEOutput.rPreviewMode.i2FlareGain;
        pAEExpParamOut->u2CaptureFlareValue = (MUINT16)m_rAEOutput.rCaptureMode[0].i2FlareOffset;
        pAEExpParamOut->u2CaptureFlareGain = (MUINT16)m_rAEOutput.rCaptureMode[0].i2FlareGain;
    } else {
        pAEExpParamOut->u2FlareValue = (MUINT16)g_u4PreviewFlareOffset;
        pAEExpParamOut->u2FlareGain = (MUINT16)(FLARE_SCALE_UNIT * FLARE_OFFSET_DOMAIN / (FLARE_OFFSET_DOMAIN - g_u4PreviewFlareOffset));
        pAEExpParamOut->u2CaptureFlareValue = (MUINT16)g_u4CaptureFlareOffset;
        pAEExpParamOut->u2CaptureFlareGain = (MUINT16)(FLARE_SCALE_UNIT * FLARE_OFFSET_DOMAIN / (FLARE_OFFSET_DOMAIN - g_u4CaptureFlareOffset));
    }
    MY_LOG("[PV Flare] = %d\n", pAEExpParamOut->u2FlareValue);
    MY_LOG("[PV Flare Gain] = %d\n", pAEExpParamOut->u2FlareGain);
    MY_LOG("[CAP Flare] = %d\n", pAEExpParamOut->u2CaptureFlareValue);
    MY_LOG("[CAP Flare Gain] = %d\n", pAEExpParamOut->u2CaptureFlareGain);

    pAEExpParamOut->bFlareAuto = m_bIsAutoFlare;
    MY_LOG("[Flare Auto] = %d\n", m_bIsAutoFlare);

    *a_pOutLen = sizeof(ACDK_AE_MODE_CFG_T);


    return S_AE_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOPAEGetFlickerMode(
    MINT32 *a_pAEFlickerMode,
    MUINT32 *a_pOutLen
)
{
    MY_LOG("[ACDK_CCT_V2_OP_AE_GET_BAND]\n");

    *a_pAEFlickerMode = m_eAEFlickerMode;

    *a_pOutLen = sizeof(MUINT32);

    MY_LOG("[AE Flicker Mode] = %d\n", *a_pAEFlickerMode);

    return S_AE_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOPAEGetMeteringMode(
    MINT32 *a_pAEMEteringMode,
    MUINT32 *a_pOutLen
)
{
    MY_LOG("[ACDK_CCT_V2_OP_AE_GET_METERING_RESULT]\n");

    *a_pAEMEteringMode = m_eAEMeterMode;

    *a_pOutLen = sizeof(MUINT32);

    MY_LOG("[AE Metering Mode] = %d\n", *a_pAEMEteringMode);

    return S_AE_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOPAEApplyNVRAMParam(
    MVOID *a_pAENVRAM
)
{

    AE_NVRAM_T *pAENVRAM = reinterpret_cast<AE_NVRAM_T*>(a_pAENVRAM);

    MY_LOG("[ACDK_CCT_OP_DEV_AE_APPLY_INFO]\n");

    m_rAEInitInput.rAENVRAM = *pAENVRAM;

    m_pIAeAlgo->updateAEParam(&m_rAEInitInput);

    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAEGetNVRAMParam(
    MVOID *a_pAENVRAM,
    MUINT32 *a_pOutLen
)
{
    MY_LOG("[ACDK_CCT_OP_DEV_AE_GET_INFO]\n");

    AE_NVRAM_T *pAENVRAM = reinterpret_cast<AE_NVRAM_T*>(a_pAENVRAM);

    getNvramData(m_eSensorDev);

    if(pAENVRAM != NULL) {
        *pAENVRAM = m_p3ANVRAM->rAENVRAM;
    } else {
        MY_LOG("NVRAM is NULL\n");
    }

    *a_pOutLen = sizeof(AE_NVRAM_T);

    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAESaveNVRAMParam(
)
{
    MUINT32 u4SensorID;
    CAMERA_DUAL_CAMERA_SENSOR_ENUM eSensorEnum;
    MRESULT err = S_AE_OK;
  
    MY_LOG("[ACDK_CCT_OP_DEV_AE_SAVE_INFO_NVRAM]\n");

    m_p3ANVRAM->rAENVRAM = m_rAEInitInput.rAENVRAM;

    err = NvBufUtil::getInstance().write(CAMERA_NVRAM_DATA_3A, m_eSensorDev);
    if (err != 0)
    {
        MY_ERR("NvBufUtil::getInstance().write\n");
        return E_NVRAM_BAD_PARAM;
    }
    
    return err;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32
AeMgr::CCTOPAEApplyPlineNVRAM(
    MVOID *a_pAEPlineNVRAM
)
{

    AE_PLINETABLE_T *pAEPlineNVRAM = reinterpret_cast<AE_PLINETABLE_T*>(a_pAEPlineNVRAM);

    MY_LOG("[ACDK_CCT_OP_DEV_AE_APPLY_PLINE] \n");

    m_rAEInitInput.rAEPlineTable = pAEPlineNVRAM;
    bApplyAEPlineTableByCCT = MTRUE;

    MY_LOG("Mapping0[0] = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->sAEScenePLineMapping.sAESceneMapping[0].eAEScene);
    MY_LOG("Mapping0[0] = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[0]);
    MY_LOG("Mapping0[0] = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[1]);
    MY_LOG("Mapping0[0] = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[2]);
    MY_LOG("Mapping0[0] = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[3]);
    MY_LOG("Mapping0[0] = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[4]);
    MY_LOG("Mapping0[0] = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[5]);
    MY_LOG("Mapping0[0] = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[6]);
    MY_LOG("Mapping0[0] = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[7]);
    MY_LOG("Mapping0[0] = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[8]);
    MY_LOG("Mapping0[0] = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[9]);
    MY_LOG("Mapping0[0] = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[10]);

    MY_LOG("Preview ID = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].eID);
    MY_LOG("Preview Total Idx = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].u4TotalIndex);
    MY_LOG("Preview StrobeBV = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].i4StrobeTrigerBV);
    MY_LOG("Preview i4MaxBV = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].i4MaxBV);
    MY_LOG("Preview i4MinBV = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].i4MinBV);
    MY_LOG("Preview ISO = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].ISOSpeed);
    MY_LOG("Preview Exp = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[0].u4Eposuretime);
    MY_LOG("Preview SGain = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[0].u4AfeGain);
    MY_LOG("Preview IGain = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[0].u4IspGain);
    MY_LOG("Preview Exp = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[1].u4Eposuretime);
    MY_LOG("Preview SGain = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[1].u4AfeGain);
    MY_LOG("Preview IGain = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[1].u4IspGain);
    MY_LOG("Preview Exp = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[2].u4Eposuretime);
    MY_LOG("Preview SGain = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[2].u4AfeGain);
    MY_LOG("Preview IGain = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[2].u4IspGain);
    MY_LOG("Preview Exp = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[3].u4Eposuretime);
    MY_LOG("Preview SGain = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[3].u4AfeGain);
    MY_LOG("Preview IGain = %d\n", ((AE_PLINETABLE_T*)pAEPlineNVRAM)->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[3].u4IspGain);

    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAEGetPlineNVRAM(
    MVOID *a_pAEPlineNVRAM,
    MUINT32 *a_pOutLen
)
{
    MY_LOG("[ACDK_CCT_OP_DEV_AE_GET_PLINE]\n");

    AE_PLINETABLE_T *pAEPlineNVRAM = reinterpret_cast<AE_PLINETABLE_T*>(a_pAEPlineNVRAM);

    getNvramData(m_eSensorDev);

    if(pAEPlineNVRAM != NULL) {
        *pAEPlineNVRAM = *m_pAEPlineTable;
    } else {
        MY_LOG("AE Pline NVRAM is NULL\n");
    }

    *a_pOutLen = sizeof(AE_PLINETABLE_T);

    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAESavePlineNVRAM(
)
{
    MUINT32 u4SensorID;
    CAMERA_DUAL_CAMERA_SENSOR_ENUM eSensorEnum;
    MRESULT err = S_AE_OK;
  
    MY_LOG("[ACDK_CCT_OP_DEV_AE_SAVE_PLINE]\n");

    MY_LOG("Mapping2[0] = %d\n", m_rAEInitInput.rAEPlineTable->sAEScenePLineMapping.sAESceneMapping[0].eAEScene);
    MY_LOG("Mapping2[0] = %d\n", m_rAEInitInput.rAEPlineTable->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[0]);
    MY_LOG("Mapping2[0] = %d\n", m_rAEInitInput.rAEPlineTable->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[1]);
    MY_LOG("Mapping2[0] = %d\n", m_rAEInitInput.rAEPlineTable->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[2]);
    MY_LOG("Mapping2[0] = %d\n", m_rAEInitInput.rAEPlineTable->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[3]);
    MY_LOG("Mapping2[0] = %d\n", m_rAEInitInput.rAEPlineTable->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[4]);
    MY_LOG("Mapping2[0] = %d\n", m_rAEInitInput.rAEPlineTable->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[5]);
    MY_LOG("Mapping2[0] = %d\n", m_rAEInitInput.rAEPlineTable->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[6]);
    MY_LOG("Mapping2[0] = %d\n", m_rAEInitInput.rAEPlineTable->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[7]);
    MY_LOG("Mapping2[0] = %d\n", m_rAEInitInput.rAEPlineTable->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[8]);
    MY_LOG("Mapping2[0] = %d\n", m_rAEInitInput.rAEPlineTable->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[9]);
    MY_LOG("Mapping2[0] = %d\n", m_rAEInitInput.rAEPlineTable->sAEScenePLineMapping.sAESceneMapping[0].ePLineID[10]);

    MY_LOG("Preview ID = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].eID);
    MY_LOG("Preview Total Idx = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].u4TotalIndex);
    MY_LOG("Preview StrobeBV = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].i4StrobeTrigerBV);
    MY_LOG("Preview i4MaxBV = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].i4MaxBV);
    MY_LOG("Preview i4MinBV = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].i4MinBV);
    MY_LOG("Preview ISO = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].ISOSpeed);
    MY_LOG("Preview Exp = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[0].u4Eposuretime);
    MY_LOG("Preview SGain = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[0].u4AfeGain);
    MY_LOG("Preview IGain = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[0].u4IspGain);
    MY_LOG("Preview Exp = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[1].u4Eposuretime);
    MY_LOG("Preview SGain = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[1].u4AfeGain);
    MY_LOG("Preview IGain = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[1].u4IspGain);
    MY_LOG("Preview Exp = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[2].u4Eposuretime);
    MY_LOG("Preview SGain = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[2].u4AfeGain);
    MY_LOG("Preview IGain = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[2].u4IspGain);
    MY_LOG("Preview Exp = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[3].u4Eposuretime);
    MY_LOG("Preview SGain = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[3].u4AfeGain);
    MY_LOG("Preview IGain = %d\n", m_rAEInitInput.rAEPlineTable->AEPlineTable.sPlineTable[0].sTable60Hz.sPlineTable[3].u4IspGain);


    bApplyAEPlineTableByCCT = MFALSE;
    memcpy(m_pAEPlineTable, m_rAEInitInput.rAEPlineTable, sizeof(AE_PLINETABLE_T));

#if 0    
    {
        char fileName[64];
        sprintf(fileName, "/sdcard/AEPline.bin");
        FILE *fp = fopen(fileName, "w");
        if (NULL == fp)
        {
            MY_ERR("fail to open file to save img: %s", fileName);
            MINT32 err = mkdir("/sdcard/", S_IRWXU | S_IRWXG | S_IRWXO);
            MY_LOG("err = %d", err);
            return MFALSE;
        }    
        MY_LOG("%s\n", fileName);
        fwrite(m_pAEPlineTable, 1, sizeof(AE_PLINETABLE_T), fp);
        fclose(fp);
    }
#endif    
    err = NvBufUtil::getInstance().write(CAMERA_DATA_AE_PLINETABLE, m_eSensorDev);
    if (err != 0)
    {
        MY_ERR("NvBufUtil::getInstance().write\n");
        return E_NVRAM_BAD_PARAM;
    }
    
    return err;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAEGetCurrentEV(
    MINT32 *a_pAECurrentEV,
    MUINT32 *a_pOutLen
)
{
    MY_LOG("[ACDK_CCT_OP_DEV_AE_GET_EV_CALIBRATION]\n");

    *a_pAECurrentEV = m_BVvalue;
    *a_pOutLen = sizeof(MINT32);

    MY_LOG("[AE Current EV] = %d\n", *a_pAECurrentEV);

    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAELockExpSetting(
)
{
    MY_LOG("[ACDK_CCT_OP_AE_LOCK_EXPOSURE_SETTING]\n");

    m_bLockExposureSetting = TRUE;

    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAEUnLockExpSetting(
)
{
    MY_LOG("[ACDK_CCT_OP_AE_UNLOCK_EXPOSURE_SETTING]\n");

    m_bLockExposureSetting = FALSE;

    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAEGetIspOB(
    MUINT32 *a_pIspOB,
    MUINT32 *a_pOutLen
)
{
    MY_LOG("[ACDK_CCT_OP_AE_GET_ISP_OB]\n");
/*
    *a_pIspOB = ISP_BITS(m_pIspReg, CAM_RGBOFF, OFF00);

    *a_pOutLen = sizeof(MUINT32);

    AAA_CCTOP_LOG("[OB] = %d\n", *a_pIspOB);
*/
    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAESetIspOB(
    MUINT32 a_IspOB
)
{
    MY_LOG("[ACDK_CCT_OP_AE_SET_ISP_OB]\n");
/*
    NSNvram::BufIF<NVRAM_CAMERA_ISP_PARAM_STRUCT>*const pBufIF = m_pNvramDrvObj->getBufIF< NVRAM_CAMERA_ISP_PARAM_STRUCT>();

    NVRAM_CAMERA_ISP_PARAM_STRUCT* pIspParam = pBufIF->getRefBuf(DUAL_CAMERA_MAIN_SENSOR, m_u4SensorID);

    ISP_NVRAM_OB_T* pOB = &pIspParam->ISPRegs.OB[0];

    pOB->rgboff.bits.OFF11 = a_IspOB;
    pOB->rgboff.bits.S11 = 1; // negative
    pOB->rgboff.bits.OFF10 = a_IspOB;
    pOB->rgboff.bits.S10 = 1; // negative
	pOB->rgboff.bits.OFF01 = a_IspOB;
    pOB->rgboff.bits.S01 = 1; // negative
    pOB->rgboff.bits.OFF00 = a_IspOB;
    pOB->rgboff.bits.S00 = 1; // negative

    AAA_CCTOP_LOG("[OB] = %d\n", a_IspOB);
*/
    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAEGetIspRAWGain(
    MUINT32 *a_pIspRawGain,
    MUINT32 *a_pOutLen
)
{
    MINT32 err;

    MY_LOG("[ACDK_CCT_OP_AE_GET_ISP_RAW_GAIN]\n");

    ISP_MGR_OBC_T::getInstance(m_eSensorDev).getIspAEGain(a_pIspRawGain);
    
    *a_pOutLen = sizeof(MUINT32);

    MY_LOG("[RAW Gain] = %d\n", *a_pIspRawGain);

    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAESetIspRAWGain(
    MUINT32 a_IspRAWGain
)
{
    MY_LOG("[ACDK_CCT_OP_AE_SET_ISP_RAW_GAIN]\n");
    MY_LOG("ISP RAW Gain = %d\n", a_IspRAWGain);

    IspTuningMgr::getInstance().setISPAEGain((ESensorDev_T)m_eSensorDev, MFALSE, a_IspRAWGain>>1);

    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAESetSensorExpTime(
    MUINT32 a_ExpTime
)
{
    MINT32 err;

    MY_LOG("[ACDK_CCT_OP_AE_SET_SENSOR_EXP_TIME]\n");
    MY_LOG("Exposure Time = %d\n", a_ExpTime);


    err = AAASensorMgr::getInstance().setSensorExpTime((ESensorDev_T)m_eSensorDev, a_ExpTime);
    if (FAILED(err)) {
        MY_ERR("[CCTOPAESetSensorExpTime] AAASensorMgr::getInstance().setSensorExpTime fail\n");
        return err;
    }

    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAESetSensorExpLine(
    MUINT32 a_ExpLine
) const
{
    MINT32 err;

    MY_LOG("[ACDK_CCT_OP_AE_SET_SENSOR_EXP_LINE]\n");
    MY_LOG("Exposure Line = %d\n", a_ExpLine);

    // Set exposure line
    err = AAASensorMgr::getInstance().setSensorExpLine((ESensorDev_T)m_eSensorDev, a_ExpLine);
    if (FAILED(err)) {
        MY_ERR("[CCTOPAESetSensorExpLine] AAASensorMgr::getInstance().setSensorExpLine fail\n");
        return err;
    }

    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAESetSensorGain(
    MUINT32 a_SensorGain
) const
{
    MINT32 err;

    MY_LOG("[ACDK_CCT_OP_AE_SET_SENSOR_GAIN]\n");
    MY_LOG("Sensor Gain = %d\n", a_SensorGain);

    err = AAASensorMgr::getInstance().setSensorGain((ESensorDev_T)m_eSensorDev, a_SensorGain);

    if (FAILED(err)) {
        MY_ERR("[CCTOPAESetSensorGain] AAASensorMgr::getInstance().setSensorGain fail\n");
        return err;
    }

    return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAESetCaptureMode(
    MUINT32 a_CaptureMode
)
{
    MINT32 err;
    MUINT32 u4AFEGain = 0, u4IspGain = 1024, u4BinningRatio = 1, u4SensitivityRatio = 1024;
    strAEInput rAEInput;
    strAEOutput rAEOutput;
    MBOOL bStrobeOn;
    
    MY_LOG("[ACDK_CCT_OP_AE_CAPTURE_MODE]\n");

    // Update capture exposure time/gain/flare
    if(a_CaptureMode == 0) {   // preview mode
        u4SensitivityRatio = m_p3ANVRAM->rAENVRAM.rDevicesInfo.u4Cap2PreRatio;
    } else if(a_CaptureMode == 2) {
        u4SensitivityRatio = 1024 *m_p3ANVRAM->rAENVRAM.rDevicesInfo.u4Cap2PreRatio  / m_p3ANVRAM->rAENVRAM.rDevicesInfo.u4Video2PreRatio;
    }

    if(u4SensitivityRatio <= 300) {
        u4BinningRatio = 4;
    } else if(u4SensitivityRatio <= 450) {
        u4BinningRatio = 3;
    } else if(u4SensitivityRatio <= 768) {
        u4BinningRatio = 2;
    } else {
        u4BinningRatio = 1;
    }
    
    bStrobeOn = FlashMgr::getInstance().isFlashOnCapture(m_eSensorDev);

    MY_LOG("Capture mode = %d Binning:%d bStrobeOn:%d\n", a_CaptureMode, u4BinningRatio, bStrobeOn);

    if(a_CaptureMode == 0) {      // preview mode or video mode
        if(bStrobeOn == TRUE) {
            mCaptureMode.u4Eposuretime = mCaptureMode.u4Eposuretime/u4BinningRatio;
            if(m_bIsAutoFlare == MTRUE) {   // The flare is auto
                if(m_rAEInitInput.rAENVRAM.rCCTConfig.bEnableStrobeThres == MFALSE) {
                    mCaptureMode.i2FlareOffset = m_rAEInitInput.rAENVRAM.rCCTConfig.u4StrobeFlareOffset;
                    mCaptureMode.i2FlareGain = (MINT16)(FLARE_SCALE_UNIT * FLARE_OFFSET_DOMAIN / (FLARE_OFFSET_DOMAIN  - m_rAEInitInput.rAENVRAM.rCCTConfig.u4StrobeFlareOffset));
                }
            }
            MY_LOG("u4Exposure time:%d Strobe thres :%d offset:%d\n", mCaptureMode.u4Eposuretime, m_rAEInitInput.rAENVRAM.rCCTConfig.bEnableVideoThres, m_rAEInitInput.rAENVRAM.rCCTConfig.u4StrobeFlareOffset);
        } else {
            rAEInput.eAeState = AE_STATE_AELOCK;
            if(m_pIAeAlgo != NULL) {
                m_pIAeAlgo->handleAE(&rAEInput, &rAEOutput);
                copyAEInfo2mgr(&m_rAEOutput.rPreviewMode, &rAEOutput);
                if(m_bIsAutoFlare == MFALSE) {   // The flare is not auto
                    m_rAEOutput.rPreviewMode.i2FlareOffset = g_u4PreviewFlareOffset;
                    m_rAEOutput.rPreviewMode.i2FlareGain = (MINT16)(FLARE_SCALE_UNIT * FLARE_OFFSET_DOMAIN / (FLARE_OFFSET_DOMAIN  - g_u4PreviewFlareOffset));
                }
            } else {
                MY_LOG("The AE algo class is NULL (15)\n");
            }            
            updateCaptureParams(m_rAEOutput.rPreviewMode);
            m_pIAeAlgo->SetCaptureFlareValue(m_rAEOutput.rPreviewMode.i2FlareOffset);
        }
   } else if(a_CaptureMode == 1) {   // capture mode
        if(bStrobeOn == FALSE) {
            updateCaptureParams(m_rAEOutput.rCaptureMode[0]);
        } else {
            if(m_bIsAutoFlare == MTRUE) {   // The flare is auto
                if(m_rAEInitInput.rAENVRAM.rCCTConfig.bEnableStrobeThres == MFALSE) {
                    mCaptureMode.i2FlareOffset = m_rAEInitInput.rAENVRAM.rCCTConfig.u4StrobeFlareOffset;
                    mCaptureMode.i2FlareGain = (MINT16)(FLARE_SCALE_UNIT * FLARE_OFFSET_DOMAIN / (FLARE_OFFSET_DOMAIN  - m_rAEInitInput.rAENVRAM.rCCTConfig.u4StrobeFlareOffset));
                }
            }
            MY_LOG("u4Exposure time:%d Strobe thres :%d offset:%d\n", mCaptureMode.u4Eposuretime, m_rAEInitInput.rAENVRAM.rCCTConfig.bEnableVideoThres, m_rAEInitInput.rAENVRAM.rCCTConfig.u4StrobeFlareOffset);
        }
   } else if(a_CaptureMode == 2) {
        if(bStrobeOn == TRUE) {
            mCaptureMode.u4Eposuretime = mCaptureMode.u4Eposuretime/u4BinningRatio;
            if(m_bIsAutoFlare == MTRUE) {   // The flare is auto
                if(m_rAEInitInput.rAENVRAM.rCCTConfig.bEnableStrobeThres == MFALSE) {
                    mCaptureMode.i2FlareOffset = m_rAEInitInput.rAENVRAM.rCCTConfig.u4StrobeFlareOffset;
                    mCaptureMode.i2FlareGain = (MINT16)(FLARE_SCALE_UNIT * FLARE_OFFSET_DOMAIN / (FLARE_OFFSET_DOMAIN  - m_rAEInitInput.rAENVRAM.rCCTConfig.u4StrobeFlareOffset));
                }
            }
            MY_LOG("u4Exposure time:%d Strobe thres :%d offset:%d\n", mCaptureMode.u4Eposuretime, m_rAEInitInput.rAENVRAM.rCCTConfig.bEnableVideoThres, m_rAEInitInput.rAENVRAM.rCCTConfig.u4StrobeFlareOffset);
        } else {
            if(m_bIsAutoFlare == MTRUE) {   // The flare is auto
                if(m_rAEInitInput.rAENVRAM.rCCTConfig.bEnableVideoThres) {
                    if(m_rAEInitInput.rAENVRAM.rCCTConfig.bEnableCaptureThres) {
                        m_rAEOutput.rCaptureMode[0].i2FlareOffset = m_rAEOutput.rPreviewMode.i2FlareOffset*m_rAEInitInput.rAENVRAM.rCCTConfig.u4VideoFlareThres / m_rAEInitInput.rAENVRAM.rCCTConfig.u4CaptureFlareThres;
                    } else {
                        m_rAEOutput.rCaptureMode[0].i2FlareOffset = 16*m_rAEInitInput.rAENVRAM.rCCTConfig.u4VideoFlareThres;
                    }
                    if(m_rAEOutput.rCaptureMode[0].i2FlareOffset > (MINT16) m_rAEInitInput.rAENVRAM.rCCTConfig.u4VideoMaxFlareThres) {
                        m_rAEOutput.rCaptureMode[0].i2FlareOffset = (MINT16) m_rAEInitInput.rAENVRAM.rCCTConfig.u4VideoMaxFlareThres;
                    } else if(m_rAEOutput.rCaptureMode[0].i2FlareOffset < (MINT16) m_rAEInitInput.rAENVRAM.rCCTConfig.u4VideoMinFlareThres) {
                        m_rAEOutput.rCaptureMode[0].i2FlareOffset = (MINT16) m_rAEInitInput.rAENVRAM.rCCTConfig.u4VideoMinFlareThres;
                    }
                    m_rAEOutput.rCaptureMode[0].i2FlareGain = (MINT16)(FLARE_SCALE_UNIT * FLARE_OFFSET_DOMAIN / (FLARE_OFFSET_DOMAIN  - m_rAEOutput.rPreviewMode.i2FlareOffset));
                } else {
                    m_rAEOutput.rCaptureMode[0].i2FlareOffset = m_rAEInitInput.rAENVRAM.rCCTConfig.u4VideoFlareOffset;
                    m_rAEOutput.rCaptureMode[0].i2FlareGain = (MINT16)(FLARE_SCALE_UNIT * FLARE_OFFSET_DOMAIN / (FLARE_OFFSET_DOMAIN  - m_rAEInitInput.rAENVRAM.rCCTConfig.u4VideoFlareOffset));
                }
            }
            updateCaptureParams(m_rAEOutput.rCaptureMode[0]);
            m_pIAeAlgo->SetCaptureFlareValue(m_rAEOutput.rCaptureMode[0].i2FlareOffset);
            mCaptureMode.u4Eposuretime = mCaptureMode.u4Eposuretime/u4BinningRatio;
        }
   } else {
        MY_ERR("[ACDK_CCT_OP_AE_CAPTURE_MODE] Err capture mode:%d\n", a_CaptureMode);
   }

   return S_AE_OK;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
AeMgr::CCTOPAEGetFlareOffset(
    MUINT32 a_FlareThres,
    MUINT32 *a_pAEFlareOffset,
    MUINT32 *a_pOutLen
)
{
    MY_LOG("[ACDK_CCT_OP_DEV_AE_GET_FLARE_CALIBRATION]\n");

    if(m_pIAeAlgo != NULL) {
        *a_pAEFlareOffset = m_pIAeAlgo->CalculateFlareOffset(a_FlareThres);
    } else {
        MY_LOG("The AE algo class is NULL (15)\n");
        *a_pAEFlareOffset = 0;
    }

    *a_pOutLen = sizeof(MUINT32);

    MY_LOG("[Flare Calibration] Offset : %d Threshold:%d\n", *a_pAEFlareOffset, a_FlareThres);

    return S_AE_OK;

}

