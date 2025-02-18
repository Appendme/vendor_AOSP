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
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/

/**
* @file eis_hal_imp.h
*
* EIS Hal Implementation Header File
*
*/


#ifndef _EIS_HAL_IMP_H_
#define _EIS_HAL_IMP_H_

#include "Mutex.h"
#include "Condition.h"

#include <eis/eis_hal.h>
#include <common/include/IImageBuffer.h>
#include <Hal3/IHal3A.h>

using android::Mutex;
using android::Condition;


#define LIKELY(x)       __builtin_expect(!!(x), 1)
#define UNLIKELY(x)     __builtin_expect(!!(x), 0)

#define MAX_MEMORY_SIZE (30)

typedef enum EIS_HAL_STATE
{
    EIS_HAL_STATE_NONE   = 0x0000,
    EIS_HAL_STATE_ALIVE  = 0x0001,
    EIS_HAL_STATE_UNINIT = 0x0002
}EIS_HAL_STATE_ENUM;


#define TSRECORD_MAXSIZE    (108000)
#define EIS_ACCELEROMETER   (1)
#define EIS_GYROSCOPE          (2)
#define GYRO_DATA_PER_FRAME     (100)

typedef struct EIS_GyroRecord_t
{
    MINT32  id;
    MFLOAT x;
    MFLOAT y;
    MFLOAT z;
    MUINT64 ts;
}EIS_GyroRecord;


typedef struct GyroEISStatistics_t
{
    MINT32  eis_data[EIS_WIN_NUM*4];
    MUINT64 ts;
}GyroEISStatistics;

struct NVRAM_CAMERA_FEATURE_STRUCT_t;
struct NVRAM_CAMERA_FEATURE_GIS_STRUCT_t;

typedef android::sp<IImageBuffer> spIImageBuffer;

typedef queue<spIImageBuffer>  vecSPIImageBuffer;

/**
  *@class EisHalImp
  *@brief Implementation of EisHal class
*/
class EisHalImp : public EisHal
{

    template<const unsigned int> friend class EisHalObj;
private:
    EisHalImp(const EisHalImp&);
    EisHalImp &operator = (const EisHalImp&);
    /**
         *@brief EisHalImp constructor
       */
    EisHalImp(const MUINT32 &aSensorIdx);

    /**
         *@brief EisHalImp destructor
       */
    ~EisHalImp() {}

public:

    /**
         *@brief Create EisHal object
         *@param[in] aSensorIdx : sensor index
         *@return
         *-EisHal object
       */
    static EisHal *GetInstance(const MUINT32 &aSensorIdx);

    /**
         *@brief Destroy EisHal object
         *@param[in] userName : user name,i.e. who destroy EIS object
       */
    virtual MVOID DestroyInstance(char const *userName);

    /**
         *@brief Initialization function
         *@param[in] aSensorIdx : sensor index
         *@return
         *-EIS_RETURN_NO_ERROR indicates success, otherwise indicates fail
       */
    virtual MINT32 Init();

    /**
         *@brief Unitialization function
         *@return
         *-EIS_RETURN_NO_ERROR indicates success, otherwise indicates fail
       */
    virtual MINT32 Uninit();

    /**
         *@brief Configure EIS
         *@details Use this API after pass1/pass2 config and before pass1/pass2 start
         *@param[in] aEisPass : indicate pass1 or pass2
         *@param[in] aEisConfig : EIS config data
         *@return
         *-EIS_RETURN_NO_ERROR indicates success, otherwise indicates fail
       */
    virtual MINT32 ConfigEis(const EIS_PASS_ENUM &aEisPass,const EIS_HAL_CONFIG_DATA &aEisConfig);

    virtual MINT32 ConfigCalibration(const EIS_PASS_ENUM &aEisPass,const EIS_HAL_CONFIG_DATA &aEisConfig);

    /**
         *@brief Execute EIS
         *@param[in] aEisPass : indicate pass1 or pass2
         *@param[in] apEisConfig : EIS config data, mainly for pass2
         *@param[in] aTimeStamp : time stamp of pass1 image
         *@return
         *-EIS_RETURN_NO_ERROR indicates success, otherwise indicates fail
       */
   virtual MINT32 AbortP2Calibration();

    virtual MINT32 ForcedDoEisPass2();

    virtual MINT32 DoEis(const EIS_PASS_ENUM &aEisPass, QBufInfo const &pBufInfo);

    virtual MINT32 DoEis(const EIS_PASS_ENUM &aEisPass,EIS_HAL_CONFIG_DATA *apEisConfig = NULL,MINT64 aTimeStamp = -1);

    virtual MINT32 DoCalibration(const EIS_PASS_ENUM &aEisPass,EIS_HAL_CONFIG_DATA *apEisConfig = NULL,MINT64 aTimeStamp = -1,MBOOL bRecording = MFALSE);


    /**
         *@brief Get EIS Plus GPU infomation
         *@param[in] aGridX : pointer to array
         *@param[in] aGridY : pointer to array
       */
    virtual MVOID SetEisPlusGpuInfo(MINT32 * const aGridX, MINT32 * const aGridY);

    /**
         *@brief Set EIS Pass2 User
         *@param[in] userName : user name
       */
    virtual MVOID SetEisP2User(char const *userName);

    /**
         *@brief Get EIS algorithm result (CMV)
         *@param[out] a_CMV_X_Int : EIS algo result of X-direction integer part
         *@param[out] a_CMV_X_Flt  : EIS algo result of X-direction float part
         *@param[out] a_CMV_Y_Int : EIS algo result of Y-direction integer part
         *@param[out] a_CMV_Y_Flt  : EIS algo result of Y-direction float part
         *@param[out] a_TarWidth    : EIS width crop size
         *@param[out] a_TarHeight   : EIS height crop size
       */
    virtual MVOID GetEisResult(MUINT32 &a_CMV_X_Int,
                                   MUINT32 &a_CMV_X_Flt,
                                   MUINT32 &a_CMV_Y_Int,
                                   MUINT32 &a_CMV_Y_Flt,
                                   MUINT32 &a_TarWidth,
                                   MUINT32 &a_TarHeight,
                                   MUINT32 &a_MVtoCenterX,
                                   MUINT32 &a_MVtoCenterY,
                                   MUINT32 &a_isFromRRZ);

    /**
         *@brief Get EIS algorithm result (CMV)
         *@param[out] P_EIS_PLUS_RESULT_INFO_STRUCT : EISPlus result
       */
    virtual MVOID GetEisPlusResult(P_EIS_PLUS_RESULT_INFO_STRUCT apEisPlusResult);

    /**
         *@brief Get EIS GMV
         *@details The value is 256x
         *@param[out] aGMV_X : x-direction global motion vector between two frames
         *@param[out] aGMV_Y  : y-direction global motion vector between two frames
         *@param[out] aEisInW  : width of EIS input image (optional)
         *@param[out] aEisInH  : height of EIS input image (optional)
       */
    virtual MVOID GetEisGmv(MINT32 &aGMV_X,MINT32 &aGMV_Y,MUINT32 *confX=NULL,MUINT32 *confY=NULL);

    /**
         *@brief Get FEO register setting
         *@param[in,out] aFeoRegInfo : FEO_REG_INFO
       */
    virtual MVOID GetFeoRegInfo(FEO_REG_INFO *aFeoRegInfo);

    /**
         *@brief Get EIS Plus GPU infomation
         *@param[in,out] aEisPlusGpu : EIS_PLUS_GPU_INFO
       */
    virtual MVOID GetEisPlusGpuInfo(EIS_PLUS_GPU_INFO *aEisPlusGpu);

    /**
         *@brief Get EIS HW support or not
         *@param[in] aSensorIdx : sensor index
         *@return
         *-MTRUE indicates EIS HW is supported, MFALSE indicates not supported
       */
    virtual MBOOL GetEisSupportInfo(const MUINT32 &aSensorIdx);

    /**
         *@brief Get EIS plus crop ratio
         *@return
         *-EIS plus crop ratio
       */
    virtual MUINT32 GetEisPlusCropRatio();

    /**
         *@brief Get EIS pass1 result at pass2
         *@param[in] userName : user name
         *@param[in,out] apEisResult : pointer to EIS_P1_RESULT_INFO
         *@param[in] aTimeStamp : time stamp of current frame
         *@param[in] lastUser : last user or not
       */
    virtual MVOID GetEisInfoAtP2(char const *userName,EIS_P1_RESULT_INFO *apEisResult,const MINT64 &aTimeStamp,MBOOL lastUser);

    /**
         *@brief  Flush memory
         *@param[in] aDma : DMA port : EISO or FEO
          *@param[in] aFlush : flush type
       */
    virtual MVOID FlushMemory(const EIS_DMA_ENUM &aDma,const EIS_FLUSH_ENUM &aFlush);

    /**
         *@brief Command API
         *@param[in] EIS_CMD_ENUM : command enum
         *@param[in] arg1 : argument
         *@param[in] arg2 : argument
         *@param[in] arg3 : argument
       */
    virtual MVOID SendCommand(EIS_CMD_ENUM aCmd,MINT32 arg1, MINT32 arg2 = -1, MINT32 arg3 = -1);

    virtual NSCam::MSize QueryMinSize(MBOOL isEISOn, NSCam::MSize sensorSize, NSCam::MSize requestSize);

    virtual MINT32 GetBufEis(android::sp<IImageBuffer>& spBuf);

    virtual MINT32 NotifyEis(QBufInfo&  pBufInfo);

#if EIS_ALGO_READY

    /**
         *@brief Return EIS HW statistic result
         *@param[in,out] a_pEIS_Stat : EIS_STATISTIC_STRUCT
       */
    virtual MVOID GetEisStatistic(EIS_STATISTIC_STRUCT *a_pEIS_Stat);

#endif

/**
     *@brief  Get input width/height of EIS HW
   */
    MVOID GetEisInputSize(MUINT32 *aWidth, MUINT32 *aHeight);

/**
     *@brief  Return  EIS HW setting of EOS_OP_HORI
     *@return
     *-EOS_OP_HORI
   */
    MUINT32 GetEisDivH();

/**
     *@brief  Return  EIS HW setting of EIS_OP_VERT
     *@return
     *-EIS_OP_VERT
   */
    MUINT32 GetEisDivV();

private:

    /**
         *@brief Get sensor info
         *@return
         *-EIS_RETURN_NO_ERROR indicates success, otherwise indicates fail
       */
    MINT32 GetSensorInfo();

    /**
         *@brief Get FEO statistic
       */
    MVOID GetFeoStatistic();

#if EIS_ALGO_READY

    /**
         *@brief Get EIS customize info
         *@param[out] a_pDataOut : EIS_TUNING_PARA_STRUCT
       */
    MVOID GetEisCustomize(EIS_TUNING_PARA_STRUCT *a_pDataOut);

    /**
         *@brief Get EIS2.0 customize info
         *@param[out] a_pTuningData : EIS_PLUS_TUNING_PARA_STRUCT
       */
    MVOID GetEisPlusCustomize(EIS_PLUS_TUNING_PARA_STRUCT *a_pTuningData);

    /**
         *@brief Dump EIS HW statistic info
         *@param[in] aEisStat : EIS_STATISTIC_T
       */
    MVOID DumpStatistic(const EIS_STATISTIC_STRUCT &aEisStat);

#endif

    /**
         *@brief Create IMem buffer
         *@param[in,out] memSize : memory size, will align to L1 cache
         *@param[in] bufCnt : how many buffer
         *@param[in,out] bufInfo : IMem object
         *@return
         *-EIS_RETURN_NO_ERROR indicates success, otherwise indicates fail
       */
    MINT32 CreateMemBuf(MUINT32 memSize, android::sp<IImageBuffer>& spImageBuf);

    /**
         *@brief Create IMem buffer
         *@param[in,out] memSize : memory size, will align to L1 cache
         *@param[in] bufCnt : how many buffer
         *@param[in,out] bufInfo : IMem object
         *@return
         *-EIS_RETURN_NO_ERROR indicates success, otherwise indicates fail
       */
    MINT32 CreateMultiMemBuf(MUINT32 memSize, MUINT32 num, android::sp<IImageBuffer>& spMainImageBuf, android::sp<IImageBuffer> spImageBuf[MAX_MEMORY_SIZE]);

    /**
         *@brief Destroy IMem buffer
         *@param[in] bufCnt : how many buffer
         *@param[in,out] bufInfo : IMem object
         *@return
         *-EIS_RETURN_NO_ERROR indicates success, otherwise indicates fail
       */
    MINT32 DestroyMemBuf(android::sp<IImageBuffer>& spImageBuf);

    /**
         *@brief Destroy IMem buffer
         *@param[in] bufCnt : how many buffer
         *@param[in,out] bufInfo : IMem object
         *@return
         *-EIS_RETURN_NO_ERROR indicates success, otherwise indicates fail
       */
    MINT32 DestroyMultiMemBuf(MUINT32 num, android::sp<IImageBuffer>& spMainImageBuf, android::sp<IImageBuffer> spImageBuf[MAX_MEMORY_SIZE]);

    /**
         *@brief Prepare EIS pass1 result
         *@param[in] cmvX : EIS result
         *@param[in] cmvY : EIS result
         *@param[in] aGmvConfidX : gmvX confidence level
         *@param[in] aGmvConfidY : gmvY confidence level
         *@param[in] aTimeStamp : time stamp of pass1 image
       */
    MVOID PrepareEisResult(const MINT32 &cmvX,const MINT32 &cmvY,const MINT32 &aGmvConfidX,const MINT32 &aGmvConfidY,const MINT64 &aTimeStamp);

    /**
         *@brief Save EIS pass1 result for pass2
         *@param[in] aGmvConfidX : gmvX confidence level
         *@param[in] aGmvConfidY : gmvY confidence level
         *@param[in] aTimeStamp : time stamp of pass1 image
       */
    MVOID SaveEisResultForP2(const MINT32 &aGmvConfidX,const MINT32 &aGmvConfidY,const MINT64 &aTimeStamp);

    /***************************************************************************************/

    volatile MINT32 mUsers;
    mutable Mutex mLock;
    mutable Mutex mP1Lock;
    mutable Mutex mP2Lock;

    // EIS driver object
    EisDrv *m_pEisDrv;

#if EIS_ALGO_READY

    // EIS algo object
    MTKEis *m_pEisAlg;
    MTKEisPlus *m_pEisPlusAlg;
    EIS_SET_PROC_INFO_STRUCT mEisAlgoProcData;

    MTKGyro* m_pGisAlg;

#endif

    // IMEM
    //IMemDrv *m_pIMemDrv;
    android::sp<IImageBuffer> m_pEisDbgBuf;
    android::sp<IImageBuffer> m_pEisPlusWorkBuf;
    android::sp<IImageBuffer> m_pGisWorkBuf;

    // EIS member variable
    MUINT32 mEisInput_W;
    MUINT32 mEisInput_H;
    MUINT32 mP1Target_W;
    MUINT32 mP1Target_H;
    queue<EIS_P1_RESULT_INFO> mEisResultForP2;

    // EIS Plus Member Variable
    MUINT32 mSrzOutW;
    MUINT32 mSrzOutH;
    MUINT32 mGpuGridW;
    MUINT32 mGpuGridH;

    // EIS result
    MUINT32 mDoEisCount;    //Vent@20140427: Add for EIS GMV Sync Check.
    MUINT32 mCmvX_Int;
    MUINT32 mCmvX_Flt;
    MUINT32 mCmvY_Int;
    MUINT32 mCmvY_Flt;
    MUINT32 mMVtoCenterX;
    MUINT32 mMVtoCenterY;

    MINT32 mGMV_X;
    MINT32 mGMV_Y;
    EIS_GET_PLUS_INFO_STRUCT mEisLastData2EisPlus;
    queue<MUINT64> mEis2EisPlusGmvTS;
    queue<MFLOAT> mEis2EisPlusGmvX;
    queue<MFLOAT> mEis2EisPlusGmvY;
    queue<MINT32> mEis2EisPlusConfX;
    queue<MINT32> mEis2EisPlusConfY;

    mutable Mutex GisEisStatisticsQLock;
    queue<GyroEISStatistics> mGisEisStatisticsQ;
    // EISPlus result
    EIS_PLUS_RESULT_INFO_STRUCT mEisPlusResult;

    // member variable
    MUINT32 mFrameCnt;
    MUINT32 mEisPass1Only;
    MUINT32 mIsEisConfig;
    MUINT32 mIsEisPlusConfig;
    MUINT32 mMemAlignment;
    MUINT32 mEisPlusCropRatio;
    MUINT32 mEisP2UserCnt;
    MBOOL   mEisSupport;

    // FE
    FEO_STAT_DATA mFeoStatData;

    // sensor listener
    SensorListener *mpSensorListener;

    // sensor
    IHalSensorList *m_pHalSensorList;
    IHalSensor *m_pHalSensor;
    MUINT32 mSensorIdx;
    MUINT32 mSensorDev;
    SensorStaticInfo mSensorStaticInfo;
    SensorDynamicInfo mSensorDynamicInfo;
    MBOOL mGyroEnable;
    MBOOL mAccEnable;
    MUINT64 mTsForAlgoDebug;
    NVRAM_CAMERA_FEATURE_STRUCT_t* m_pNvram;
    MUINT32 mChangedInCalibration;
    MUINT32 mGisInputW;
    MUINT32 mGisInputH;
    MBOOL   mNVRAMRead;
    MUINT64 mSleepTime;
    MUINT64 mtRSTime;
    MBOOL   mSkipWaitGyro;
    MBOOL   mbLastCalibration;
    MUINT32 mSensorMode;
    MUINT32 mSensorPixelClock;
    MUINT32 mSensorLinePixel;
    MBOOL   mbEMSaveFlag;
    double    mRecordParameter[6];
    NVRAM_CAMERA_FEATURE_GIS_STRUCT_t* m_pNVRAM_defParameter;
    NS3Av3::IHal3A* m_pHal3A;

    android::sp<IImageBuffer> m_pEISOMainBuffer;
    android::sp<IImageBuffer> m_pEISOSliceBuffer[10];
    vecSPIImageBuffer           mEISOBufferList;
    MUINT32 mBufIndex;

#if EIS_WORK_AROUND

    MUINT32 mTgRrzRatio;

#endif

};

/**
  *@class EisHalObj
  *@brief singleton object for each EisHal which is seperated by sensor index
*/

template<const unsigned int aSensorIdx>
class EisHalObj : public EisHalImp
{
private:
    static EisHalObj<aSensorIdx>* spInstance;
    static Mutex    s_instMutex;

    EisHalObj() : EisHalImp(aSensorIdx) {}
    ~EisHalObj() {}
public:
    static EisHal *GetInstance()
    {
        if (0 == spInstance)
        {
            AutoMutex lock(s_instMutex);
            if (0 == spInstance)
            {
                spInstance = new EisHalObj();
                atexit(destroyInstance);
            }

        }
        return spInstance;
    }


    static void destroyInstance(void)
    {
        AutoMutex lock(s_instMutex);
        if (0 != spInstance)
        {
            delete spInstance;
            spInstance = 0;
        }
    }

};


#endif

