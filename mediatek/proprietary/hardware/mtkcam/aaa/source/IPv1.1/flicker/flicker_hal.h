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

#ifndef _FLICKER_HAL_H_
#define _FLICKER_HAL_H_

#include <CamIO/INormalPipe.h>
#include "flicker_hal_base.h"
//#include "af_param.h"
//#include "imem_buffer.h"
using namespace android;
using namespace NSIoPipe;
using namespace NSCamIOPipe;


using namespace android;
/*
typedef struct _flicker_threshold_t {
    MHAL_INT16 i2ThresC0;
    MHAL_INT16 i2ThresC1;
    MHAL_INT16 i2ThresA0;
    MHAL_INT16 i2ThresA1;
    MHAL_INT16 i2ThresF0;
    MHAL_INT16 i2ThresF1;
    MHAL_INT16 i2ThresF2;
} flicker_threshold_t;
*/
namespace NS3Av3
{
typedef struct FLKThreSetting_S
{
	MUINT32 u4FlickerPoss1; 		// impossible flicker
	MUINT32 u4FlickerPoss2; 		// maybe flicker exist
	MUINT32 u4FlickerFreq1; 		// flicker frequency detect
	MUINT32 u4FlickerFreq2; 		// flicker frequency detect
	MUINT32 u4ConfidenceLevel1;   // flicker confidence level
	MUINT32 u4ConfidenceLevel2;   // flicker confidence level
	MUINT32 u4ConfidenceLevel3;   // flicker confidence level
}FLKThreSetting_T;

  //move to ae_param.h
typedef struct FLKWinCFG_S
{
	MINT32 m_uImageW;
	MINT32 m_uImageH;
	MINT32 m_u4OffsetX;
	MINT32 m_u4OffsetY;
	MINT32 m_u4NumX;
	MINT32 m_u4NumY;
	MINT32 m_u4SizeX;
	MINT32 m_u4SizeY;
}FLKWinCFG_T;

/*******************************************************************************
*
********************************************************************************/




class FlickerHal
{
public:
    //static FlickerHalBase* getInstance();
    static FlickerHal* getInstance(int sensorDev);

    virtual int setSensorMode(int i4NewSensorMode);

    virtual int createBuf();
    virtual int releaseBuf();

    virtual int 	open(int sensorId, int tgInfo);
    virtual int 	close();
    virtual int update(FlickerInput* in, FlickerOutput* out);


    //virtual int pause();
	//virtual int resume();

	virtual int setFlickerMode(int mode);

	//virtual int cameraPreviewStart();
	//virtual int cameraPreviewEnd();
	//virtual int recordingStart();
	//virtual int recordingEnd();


private:
    virtual int enable(int en);
    FlickerHal(int sensorDev);
    virtual ~FlickerHal();

    virtual MINT32 setFlickerDrv(MBOOL flicker_en);
    virtual MINT32 setFlickerWinConfig(FLKWinCFG_T* ptFlkWinCfg);
    virtual MINT32 setFlickerDMAConfig(unsigned long flicker_DMA_address ,MINT32 DMASize );
    virtual MUINT32 GetFlicker_CurrentDMA();
	static MVOID* uninitThread(MVOID* arg);
	void setAlgPara(int sensorDev);
    virtual MINT32 init(MINT32 i4SensorDev);
    virtual MINT32 uninit();
    virtual MINT32  enableFlickerDetection(MBOOL bEnableFlicker);
    //virtual MINT32 analyzeFlickerFrequency(MINT32 i4LMVcntMINT32 *i4LMV_x, MINT32 *i4LMV_y, MINT64 *i4vAFstat);
    MINT32 analyzeFlickerFrequency(int exp, MINT32 *i4vAFstatisic);
    virtual MINT32 setWindowInfo(int* blkH, int* blkW);
    virtual MINT32 getFlickerStatus(MINT32 *a_flickerStatus);

private:
    virtual MINT32 createBufSub();
    virtual MVOID releaseBufSub();
    virtual int setFlickerModeSub(int mode);



private:
    int mTgInfo;
    int mIsInited;

    int mSensorMode;
    int mIsEnable;

    volatile int mUsers;
	pthread_t       mUninitThread;
    mutable Mutex m_lock;
    //SensorDrv *m_pSensorDrv;
    IspDrv *m_pIspDrv2;
    //isp_reg_t *m_pIspRegMap;


    MBOOL  m_bFlickerModeChange;
	MBOOL  m_bFlickerEnable;
    MBOOL m_bFlickerEnablebit;
    MUINT32 m_u4FlickerWidth;
    MUINT32 m_u4FlickerHeight;
    MUINT32 m_u4SensorPixelClkFreq;
    MINT32 m_u4FreqFrame;




    MUINT32 m_u4PixelsInLine;
    FLICKER_STATUS m_flkAlgStatus ;
    LMV_STATUS m_EIS_LMV_Flag;
    MINT32 *m_pVectorAddress1;
    MINT8 m_iDataBank;
    MINT32 *m_pVectorData1;
    MINT32 *m_pVectorData2;
    MINT32 m_u4FlickerFreq;
//    IMemDrv*  mpIMemDrv;
	IMEM_BUF_INFO flkbufInfo[2];
	FLKWinCFG_T strFlkWinCfg;
	MINT32 FLK_DMA_Size;

	MINT32 m_FlickerMotionErrCount;
	MINT32 m_FlickerMotionErrCount2;
    MBOOL  m_bPause;
    int m_flickerMode;


    MINT32 m_sensorDev;
    int m_sensorId;






private:
    MBOOL pipeSendCommand(MINT32 cmd, MUINTPTR arg1, MUINTPTR arg2, MUINTPTR arg3);

	MBOOL updateEISInfo();
	MBOOL updateAAAInfo();
	int Updated(FlickerInput* in, FlickerOutput* out);

	MBOOL getFlickerThresPara(NSCamCustom::eFlickerDetectSpeed idx ,FLKThreSetting_T *ptFlickerThreshold);





#if 1
protected:	////	Attributes.
		//
		MINT32			mi4DetectedResult;

		/*
		 *	AF Windows Status.
		 *
		 *	FIXME:
		 *		AF_WIN_NUM is defined in af_param.h
		 *		NUM should be queried from af_param.h.
		 */
		enum { eAFWinNum = 9/* AF_WIN_NUM*/ };
		//enum { eAFWinNum = 1 };
		MINT64 mai4AFWin[eAFWinNum];
		//
		//	EIS GMV X/Y
#if 1
MINT32			mai4GMV_X;
MINT32			mai4GMV_Y;

#else
		enum { eMaxGMVNum = 1 };
		MINT32			mai4GMV_X[eMaxGMVNum];
		MINT32			mai4GMV_Y[eMaxGMVNum];
#endif

#endif
};
};

#endif

