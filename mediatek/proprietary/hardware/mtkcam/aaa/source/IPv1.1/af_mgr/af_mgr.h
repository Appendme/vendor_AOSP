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
/**
 * @file af_mgr.h
 * @brief AF manager, do focusing for raw sensor.
 */
#ifndef _AF_MGR_H_
#define _AF_MGR_H_
#include <isp_tuning.h>
#include <af_feature.h>
#include <iopipe/CamIO/INormalPipe.h>
#include <lib3a/af_algo_if.h>
#include <aaa_hal_if.h>
#include <pd_buf_common.h>
#include <laser_drv.h>
#include <vector>
#include <mcu_drv.h>
#include <drv/isp_drv.h>

class NvramDrvBase;
using namespace android;
using namespace NSIoPipe;
using namespace NSCamIOPipe;
using namespace std;

namespace NS3Av3
{

//--- pre-tuned scenechange parameters start ---
#define SENSOR_ACCE_SCALE       100
#define SENSOR_GYRO_SCALE       100

/* new part ; format chgT|chgN|stbT|stbN */
// thres unit: scale 100
const static MINT32 SCENE_GYRO_LEVEL[3]             = { 40051012,  40031010,  20031007};
const static MINT32 SCENE_GYRO_VID_LEVEL[3]         = { 40071015,  40051013,  20051010};// video mode cnt > normal mode cnt for stable
const static MINT32 SCENE_ACCE_LEVEL[3]             = { 80155012,  80125010,  60125007};
const static MINT32 SCENE_ACCE_VID_LEVEL[3]         = { 80155015,  80125013,  60125010};// video mode cnt > normal mode cnt for stable
// thres unit: % ; chgT and chgN from org part
const static MINT32 SCENE_AEBLOCK_CHG_THR_DIFF    = 10; // to add based on GS chgT
const static MINT32 SCENE_AEBLOCK_CHG_CNT_DIFF    = 5;  // to add based on GS chgN
const static MINT32 SCENE_GS_LEVEL[3]         = {      512,       510,       507};
const static MINT32 SCENE_GS_VID_LEVEL[3]       = {      515,       513,       510};// video mode cnt > normal mode cnt for stable
const static MINT32 SCENE_AEBLOCK_LEVEL[3]      = {     1012,      1010,      1007};
const static MINT32 SCENE_AEBLOCK_VID_LEVEL[3]    = {     1015,      1013,      1010};// video mode cnt > normal mode cnt for stable

/* org part */
const static MINT32 SCENE_FV_CHG_THR_LEVEL_DIFF[3]  = { 0,  0, -10};  // chgT lower to sensitive
const static MINT32 SCENE_FV_CHG_CNT_LEVEL_DIFF[3]  = { 3,  0,   0};  // chgN bigger to stable
const static MINT32 SCENE_FV_STB_THR_LEVEL_DIFF[3]  = { 0,  0,   0};  // stbT should fix
const static MINT32 SCENE_FV_STB_CNT_LEVEL_DIFF[3]  = { 5,  0,  -5};  // stbN bigger to stable
const static MINT32 SCENE_GS_CHG_THR_LEVEL_DIFF[3]  = { 0,  0,  -5};  // chgT lower to sensitive
const static MINT32 SCENE_GS_CHG_CNT_LEVEL_DIFF[3]  = { 3,  0,   0};  // chgN bigger to stable
//--- pre-tuned scenechange parameters end ---


typedef struct AF_FRM_INFO_t
{
    MINT32 i4FrmNum;  // magic number
    MINT32 i4RawFmt;  // raw format
    MINT32 i4LensPos; // lens position

  AF_FRM_INFO_t()
    {
    i4FrmNum  = 0;
      i4RawFmt  = 0;
      i4LensPos = 0;
  }
    AF_FRM_INFO_t( MINT32 FrmNum, MINT32 RawFmt, MINT32 LensPos)
    {
    i4FrmNum  = FrmNum;
      i4RawFmt  = RawFmt;
      i4LensPos = LensPos;
  }
} AF_FRM_INFO_T;


typedef struct AF_NON_VOLATILIZE_INFO_t
{
  MINT32 i4AFMode;
  LENS_INFO_T sLensPos;
  LIB3A_AF_MODE_T eLib3AAFMode;
  AF_NON_VOLATILIZE_INFO_t()
    {
    memset(&sLensPos, 0, sizeof(LENS_INFO_T));
    i4AFMode = AF_MODE_AFC;
    eLib3AAFMode = LIB3A_AF_MODE_AFC;
  }
} AF_NON_VOLATILIZE_INFO_T;


/**
 * @brief AF manager class
 */
class AfMgr
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Ctor/Dtor.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
private:    ////    Disallowed.
    //  Copy constructor is disallowed.
    AfMgr(AfMgr const&);
    //  Copy-assignment operator is disallowed.
    AfMgr& operator=(AfMgr const&);

public:  ////
    AfMgr(ESensorDev_T eSensorDev);
    ~AfMgr();
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Operations.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
    /**
     * @brief Get AF manager instance.
     */
    static AfMgr& getInstance(MINT32 const i4SensorDev);
    static AfMgr* s_pAfMgr; // FIXME: REMOVED LATTER

    MRESULT Hal3AInitState(MINT32 i4SensorIdx,MINT32 isInitMCU);

    MRESULT Hal3AUninitState(MINT32 isInitMCU);

    /**
     * @brief Initial AF manager functions.
     */
    MRESULT init(MINT32 i4SensorIdx,MINT32 isInitMCU);
    MBOOL sendAFNormalPipe(MINT32 cmd, MINTPTR arg1, MINTPTR arg2, MINTPTR arg3);

    /**
     * @brief Uninitial AF manager functions.
     */
    MRESULT uninit(MINT32 isInitMCU);

    /**
     * @brief get AF Ref Win when af.
     * @param [in] AF win.
     */
    MVOID getAFRefWin(CameraArea_T &rWinSize);

    /**
     * @brief set AE to AF Info when ae statistic result done.
     * @param [in] statistic result.
     */
    MVOID setAE2AFInfo(AE2AFInfo_T rAEInfo);

    /**
     * @brief Do AF task when af statistic result done.
     * @param [in] pAFStatBuf Buffer of statistic result.
     */
    MRESULT doAF(MVOID *pAFStatBuf);
    MVOID doPDBuffer(MVOID * buffer, MINT32 w,MINT32 h, MINT32 stride, MUINT32 i4FrmNum=0);
    MINT32 PDPureRawInterval();
    MRESULT doPDTask();

    /**
     * @brief Trigger do focusing.
     */
    MRESULT triggerAF();

    /**
     * @brief Set AF mode.
     * @param [in] a_eAFMode Set AF mode for single/continous/Fullscan/MF; Please refer AF_MODE_T in kd_camera_feature_enum.h
     */
    NS3A::EAfState_T getAFState();
    MINT32 getLensMoving();
    MRESULT setAFMode(MINT32 a_eAFMode);

    /**
     * @brief Set AF area
     * @param [in] a_sAFArea AF window information.
     */
    MRESULT setAFArea(CameraFocusArea_T a_sAFArea);

    /**
     * @brief Set camera mode
     * @param [in] a_eCamMode Camera modes; please refer EAppMode in CamDefs.h
     */
    MRESULT setCamMode(MINT32 a_eCamMode);

      /**
     * @brief Set camera zoom information.
     * @param [in] u4XOffset Zoom window X offset
     * @param [in] u4YOffset Zoom window Y offset
     * @param [in] u4Width  Zoom window Width;
     * @param [in] u4Height Zoom window Height;
     */
    MRESULT setZoomWinInfo(MUINT32 u4XOffset, MUINT32 u4YOffset, MUINT32 u4Width, MUINT32 u4Height);

    /**
     * @brief Set face detecion result information.
     * @param [in] a_sFaces Face detection result; Please refer MtkCameraFaceMetadata in faces.h
     */
    MRESULT setFDInfo(MVOID* a_sFaces);

    /**
     * @brief Set object tracking result information.
     * @param [in] a_sObtinfo object tracking result;
     */
    MRESULT setOTInfo(MVOID* a_sObtinfo);

    /**
     * @brief Get AF maximum area number
     * @return AF maximum area number
     */
    MINT32  getAFMaxAreaNum();

    /**
     * @brief Get maximum lens position value, it gets AF MCU internal position value.
     * @return AF maximum lens Position value, the macro boundary position value in MCU.
     */
    MINT32  getMaxLensPos();

    /**
     * @brief Get minimum. lens position value, it gets AF MCU internal position value.
     * @return AF minimum lens position value, the infinite boundary position value in MCU.
     */
    MINT32  getMinLensPos();

    /**
     * @brief Get AF best position value
     * @return Latest time AF searching result position value.
     */
    MINT32  getAFBestPos();

    /**
     * @brief Get AF current position value.
     * @return AF current position value.
     */
    MINT32  getAFPos();

    /**
     * @brief Check if AF is stable.
     * @return AF stable value; if 1, AF is stable.; if 0, AF is not stable.
     */
    MINT32  getAFStable();

    /**
     * @brief Get AF table Offset.
     * @return AF table offset value; It usually means the infinite position value.;Please refer i4Offset in lens_para_<MCU_Name>.cpp
    */
    MINT32  getAFTableOffset();

    /**
     * @brief Get AF table macro index.
     * @return AF table macro index; An index for AF table, it denotes the macro position of AF table.;Please refer i4MacroNum in lens_para_<MCU_Name>.cpp
     */
    MINT32  getAFTableMacroIdx();

    /**
     * @brief Get AF table index number.
     * @return AF table index number;It means that usable AF table length, and it's define to be 30.;Please refer AF_TABLE_NUM in camera_custom_nvram.h
     */
    MINT32  getAFTableIdxNum();

    /**
     * @brief Get AF Table.
     * @return AF table pointer;AF table is contains 30 entries. Please refer lens_para_<MCU_Name>.cpp
     */
    MVOID*  getAFTable();

    /**
     * @brief Get AF full statistic result. This result is generated for auto-flicker used, and it does not work while AF.
     * @return AF full statistic result; Please refer AF_FULL_STAT_T in af_param.h
     */
    AF_FULL_STAT_T getFLKStat();

    /**
     * @brief Enable AF function.
     * @param [in] a_i4En Enable switch. Value 1 means enable, 0 disable.
     */
    MRESULT enableAF(MINT32 a_i4En);

    /**
    * @brief Set manual focus position. When AF mode is MF, use it to set lens position.
     * @param [in] a_i4Pos Lens position. Usually value in 0~1023.
    */
    MRESULT setMFPos(MINT32 a_i4Pos);

    /**
     * @brief Set full scan step. When full scan, this value sets the step interval between each scanning. i.e.  Current position 100, next 103, thus step is 3.
     * @param [in] a_i4Step Steps for scan interval.
     */
    MRESULT setFullScanstep(MINT32 a_i4Step);

    /**
     * @brief Check if AF is finished
     * @return AF finish status;  Value 1 means finished, 0 not finished.
     */
    MINT32  isFocusFinish();

    /**
     * @brief Check if AF is focused success. This check can not be done before AF finished.
     * @return AF focused status;  Value 1 means success, 0 fail.
     */
    MINT32  isFocused();

    /**
     * @brief Check the AF mode of last focusing. This check can not be done before AF finished.
     * @return last focusing mode;  Value 1 means TAF, 0 CAF.
     */
    MINT32  isLastFocusModeTAF();

    /**
     * @brief Set auto-flicker window configure. Auto-flicker needs AF statistic result, and this function set the statistic window for it.
     */
    MVOID setFlkWinConfig();

    /**
     * @brief Get debug information. For internal debug information.
     * @param [in] rAFDebugInfo debug information data pointer.;Please refer AF_DEBUG_INFO_T in dbg_af_param.h
     */
    MRESULT getDebugInfo(AF_DEBUG_INFO_T &rAFDebugInfo);

    /**
     * @brief Set callback pointer to AF manger. When AF is doing or done, AF manger must report AF status. This pointer receives the reporting way.
     * @param [in] cb Callback function pointer.
     */
    MBOOL   setCallbacks(I3ACallBack* cb);

    /**
     * @brief Single AF Callback Notify. Reporting AF status when single AF done or AF timeout.
     */
    MRESULT SingleAF_CallbackNotify();

    /**
     * @brief Set best shot configure. When continous shot, camera get AF statistic result for each picture. This function configures the floating window for it.
     */
    MRESULT setBestShotConfig();

    /**
     * @brief Calculate best shot value. When continous shot, camera must do this function to calculate AF statistic result.
     * @param [in] pAFStatBuf Buffer of statistic result.
     */
    MRESULT calBestShotValue(MVOID *pAFStatBuf);

    /**
     * @brief Get best shot value. After calculating best shot AF statistic result, use it to get value.
     * @return Best shot AF statistic result.
     */
    MINT64  getBestShotValue();

    /**
     * @brief Set SGG Pre-gain. For AE control gain used.
     * @param [in] i4SGG_PGAIN SGG Pre-gain.
     */
    MVOID setSGGPGN(MINT32 i4SGG_PGAIN);

    /**
     * @brief Print AF ConfigLog, part 1. this print log only in engining version.
     */
    MVOID   printAFConfigLog0();

    /**
     * @brief Print AF ConfigLog, part 2. this print log only in engining version.
     */
    MVOID   printAFConfigLog1();

    /**
     * @brief Auto focus function, tell AF manger it is auto focus status now. This function doesn't trigger AF.
     */
    MVOID   autoFocus();

    /**
     * @brief Do callback for single AF trigger, to report AF status.
     */
    MVOID   DoCallback();

    /**
     * @brief Cancel auto focus, tell AF manger it is not auto focus status now. This function doesn't trigger AF.
     */
    MVOID   cancelAutoFocus();

    /**
     * @brief Timeout handle, when AF statistic is timeout or no response, this timeout function will be called and do something.
     */
    MVOID   TimeOutHandle();
    /**
     * @brief get the information if android is runing. If android not run, the service depends on it should
     */
  MVOID   setAndroidServiceState(MBOOL a_state);
    /**
     * @brief vsync singal
     */
  MVOID VsyncSignal();
    /**
     * @brief set sensor mode for PDAF virtual channel control.
     */
  MBOOL setSensorMode(MINT32 i4NewSensorMode, MUINT32 i4BINInfo_SzW, MUINT32 i4BINInfo_SzH);
    /**
     * @brief config Hybrid AF Algo mode.
     */
    MINT32 configHybridAFMode();
    /**
     * @brief Do Hybrid AF Proprocessing
     */
    MVOID DoAFPreprocessing();

    //Camera 3.0
    MVOID   setAperture(MFLOAT lens_aperture);
    MFLOAT  getAperture();
    MVOID   setFilterDensity(MFLOAT lens_filterDensity);
    MFLOAT  getFilterDensity();
    MVOID   setFocalLength(MFLOAT lens_focalLength);
    MFLOAT  getFocalLength();
    MVOID   setFocusDistance(MFLOAT lens_focusDistance);
    MFLOAT  getFocusDistance();
    MVOID   setOpticalStabilizationMode (MINT32 ois_OnOff);
    MINT32  getOpticalStabilizationMode();
    MVOID   getFocusRange(MFLOAT *vnear, MFLOAT *vfar);
    MINT32  getLensState();

  MBOOL   Stop();
    MBOOL   Start();
  MBOOL   CamPwrOnState();
    MBOOL   CamPwrOffState();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Private function
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
private:
    /**
     * @brief Set AF initial configure settings. This function apply AF initial settings to HW.
     * @param [in] a_sAFConfig Containing AF initial settings. Please refer AF_CONFIG_T in af_param.h.
     */
    MVOID setAFConfig(AF_CONFIG_T a_sAFConfig);

    /**
     * @brief Set AF window threshold. This function apply AF statistic filter settings to HW.
     * @param [in] a_sAFConfig Containing AF statistic filter settings. Please refer AF_CONFIG_T in af_param.h.
     */
    MVOID setAFWinTH(AF_CONFIG_T a_sAFConfig);

    /**
     * @brief Apply GMR settings to HW.
     * @param [in] a_sAFConfig Containing GMR settings. Please refer AF_CONFIG_T in af_param.h.
     */
    MVOID setGMR(AF_CONFIG_T a_sAFConfig);

    /**
     * @brief Apply AF window settings to HW. Final window setting will be check and apply to HW here.
     * @param [in] a_sAFArea AF window configure settings. Please refer AF_CONFIG_T in af_param.h.
     */
    MVOID setAFWinConfig(AF_AREA_T a_sAFArea);

    /**
     * @brief Apply zoom info settings to HW. Final window setting will be check and apply to HW here.
     * @param [in] None.
     */
    MVOID applyZoomInfo();

    /**
     * @brief This function deal with pAFStatBuf when doing continuous focus, and extract and calculate for usable statistic result.
     * @param [in] pAFStatBuf Buffer of statistic result.
     * @return AF statistic result. Please refer AF_STAT_T in af_param.h.
     */
    AF_STAT_T Trans4WintoOneStat(MVOID *pAFStatBuf);

    /**
     * @brief This function deal with pAFStatBuf when doing single focus, and extract and calculate for usable statistic result.
     * @param [in] pAFStatBuf Buffer of statistic result.
     * @return AF statistic result. Please refer AF_STAT_T in af_param.h.
     */
    AF_STAT_T TransAFtoOneStat(MVOID *pAFStatBuf);

    /**
     * @brief This function is used in AF manger internal, for check & limit variable boundary.
     * @param [in] a_i4Min Value minimum value.
     * @param [in] a_i4Vlu The variable for check and limit.
     * @param [in] a_i4Max Value maximum value.
     * @return limited result by a_i4Min & a_i4Max.
     */
    MINT32 Boundary(MINT32 a_i4Min, MINT32 a_i4Vlu, MINT32 a_i4Max);

    /**
     * @brief Update status of applying AF window settings to HW.
     * @param [in] a_sAFArea AF window configure settings. Please refer AF_CONFIG_T in af_param.h.
     */
    MVOID updateAFWinStatus(AREA_T &a_sAFArea);

    /**
     * @brief Return status of configuring HW.
     */
    MBOOL isAFWinReady();


    /**
     * @brief This function is used to get lens information in MCU driver.
     * @param [in] a_rLensInfo MCU lens information data pointer. Please refer LENS_INFO_T in af_param.h.
     */
    MRESULT getLensInfo(LENS_INFO_T &a_rLensInfo);

    /**
     * @brief This function is used for AF factory calibration. It executes:  1.Read sensor One Time Programmable(OTP) memory. 2. Calculate and apply the factory data to AF table.
     */
    MRESULT readOTP();

    /**
     * @brief get time stamp.
     */
    MINT64 getTimeStamp();

    /**
        * @brief configure hw register.
        */
    MRESULT cfgHWReg( AF_CONFIG_T &sAFHWCfg);


    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //    CCT feature
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
    MRESULT CCTMCUNameinit(MINT32 i4SensorIdx);
    MRESULT CCTMCUNameuninit();
    MINT32 CCTOPAFOpeartion();
    MINT32 CCTOPMFOpeartion(MINT32 a_i4MFpos);
    MINT32 CCTOPAFGetAFInfo(MVOID *a_pAFInfo, MUINT32 *a_pOutLen);
    MINT32 CCTOPAFGetBestPos(MINT32 *a_pAFBestPos, MUINT32 *a_pOutLen);
    MINT32 CCTOPAFCaliOperation(MVOID *a_pAFCaliData, MUINT32 *a_pOutLen);
    MINT32 CCTOPAFSetFocusRange(MVOID *a_pFocusRange);
    MINT32 CCTOPAFGetFocusRange(MVOID *a_pFocusRange, MUINT32 *a_pOutLen);
    MINT32 CCTOPAFGetNVRAMParam(MVOID *a_pAFNVRAM, MUINT32 *a_pOutLen);
    MINT32 CCTOPAFApplyNVRAMParam(MVOID *a_pAFNVRAM);
    MINT32 CCTOPAFSaveNVRAMParam();
    MINT32 CCTOPAFGetFV(MVOID *a_pAFPosIn, MVOID *a_pAFValueOut, MUINT32 *a_pOutLen);
    MINT32 CCTOPAFEnable();
    MINT32 CCTOPAFDisable();
    MINT32 CCTOPAFGetEnableInfo(MVOID *a_pEnableAF, MUINT32 *a_pOutLen);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Data member
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
private:
    mutable Mutex m_Lock;
    volatile int m_Users;
    volatile int m_CCTUsers;

    NS3A::IAfAlgo*    m_pIAfAlgo;
    MINT32      m_i4CurrSensorDev;
    MINT32      m_i4CurrSensorId;
    MINT32      m_i4CurrLensId;
    NVRAM_LENS_PARA_STRUCT  m_NVRAM_LENS;
    LIB3A_AF_MODE_T     m_eLIB3A_AFMode;
    IMetadata m_AFMeta;
    MBOOL m_getAFmeta;
    MINT32 m_i4EnableAF;
    MINT32 m_OisDisable;
    MINT32 m_bMotorDisable;
    MINT32 m_MotorManual;
    MINT32 m_i4MFPos;
    I3ACallBack*  m_pAFCallBack;
    MINT32 m_i4AFPreStatus;
    MINT32 m_bDebugEnable;
    MINT32 m_i4AutoFocus;
    MINT32 m_i4LastFocusModeTAF;
    MINT32 m_i4AutoFocuscb;
    AF_FULL_STAT_T m_sAFFullStat;
    MINT32 m_i4Factor;
    AREA_T m_sEZoom;
    MINT32 m_tcaf_mode;
    ESensorTG_T m_eSensorTG;
    MINT32      m_i4SensorIdx;
    MINT32 m_flkwin_synccnt;
    MINT32 m_Pix_Id;
    MUINT8 m_aeBlockV[25];
    MBOOL  m_PWrInit; /* MTRUE : search, MFALSE : monitor*/
    AREA_T gFDRect;
    AREA_T m_RawZoomInfo;
    MBOOL  m_NonInitState;
    MINT32 m_eCurrAFMode;
    MINT32 m_bDebugAFwinW;
    MINT32 m_bDebugAFwinH;
    MUINT32 m_FirstTimeAFON;
  AREA_T  m_FocsingAFArea;
    AF_NON_VOLATILIZE_INFO_T m_NonValInfo; /* auto initial once instance is created.*/
  MINT32 m_CaptureWidth;


    //Sensor information
  MINT32 m_i4SensorMode;
  MINT32 m_i4BINSzW;
  MINT32 m_i4BINSzH;
    MINT32 m_i4TGSzW;
    MINT32 m_i4TGSzH;

    // HybridAF IO
    AF_INPUT_T  m_sAFInput;
    AF_OUTPUT_T m_sAFOutput;
    AF_PARAM_T  m_sAFParam;

    // ROI Control
    CameraFocusArea_T m_CameraFocusArea;
    AF_AREA_T m_FDArea;
    AREA_T  m_HWAFArea;

    // Driver
    MCUDrv* m_pMcuDrv; /* VCM  driver*/
    LaserDrv* m_pLaserDrv; /* Laser driver*/
    INormalPipe* m_pPipe; /* Iopipe 2.0*/

    // g/gyro sensor listener handler and data
    MBOOL m_AndroidServiceState;
    MBOOL gAcceValid;
    MBOOL gGyroValid;
    MINT32 m_AEsetPGN;
    MINT32 m_prePosition;
    MINT32 m_af_state;

    // DAF
    AREA_T  m_LastAFArea;
    MUINT32 m_cur_frm_num;
    MUINT32 m_HybridAFMode;
    MUINT32 m_next_query_FrmNum;
    MUINT32 m_daf_distance;

    // PDAF
    MBOOL mAFMgrInited;
    MBOOL mPDVCTest;
    MINT32 m_PDAF_Sensor_Support_Mode;
  EPDBuf_Type_t m_PDBuf_Type;
    MVOID *m_pdaf_rawbuf;
    MINT32 m_pdaf_w;
    MINT32 m_pdaf_h;
    MINT32 m_pdaf_stride;
  MINT32 m_pdaf_raw_frmnum;
    SET_PD_BLOCK_INFO_T m_stPDsensorIF;
    MINT32 m_pdaf_cur_lens_pos;
    MINT32 m_pdaf_pre_lens_pos;
    AREA_T m_LastPDWin;
  MVOID  *m_AAOPDBuf;
    MINT32 m_PDOut_numRes;
  MINT64 m_MoveLensTimeStamp; /* ms*/
  //pre frame information
  vector< AF_FRM_INFO_T> m_vFrmInfo;
};

};  //  namespace NS3Av3
#endif // _AF_MGR_H_

