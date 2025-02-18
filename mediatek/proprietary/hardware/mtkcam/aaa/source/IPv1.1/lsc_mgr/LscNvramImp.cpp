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
#define LOG_TAG "lsc_nvram"
#ifndef ENABLE_MY_LOG
#define ENABLE_MY_LOG           (1)
#define GLOBAL_ENABLE_MY_LOG    (1)
#endif

#include "ILscNvram.h"
#include <LscUtil.h>
#include <mtkcam/common/include/IHalSensor.h>
#include <nvbuf_util.h>
#include "tsf_tuning_custom.h"
#include "cam_cal_drv.h"

using namespace NSIspTuning;

class LscNvramImp : public ILscNvram
{
public:
    static ILscNvram*                           getInstance(ESensorDev_T sensor);

    virtual NVRAM_CAMERA_ISP_PARAM_STRUCT*      getIspNvram() const {return m_pNvram_Isp;}
    virtual ISP_SHADING_STRUCT*                 getLscNvram() const {return m_prShadingLut;}
    virtual NVRAM_CAMERA_3A_STRUCT*             get3ANvram() const {return m_prNvram3A;}
    virtual const CAMERA_TSF_TBL_STRUCT*        getTsfNvram() const {return &m_rTsfCfgTbl;}
    virtual const CAM_CAL_SINGLE_LSC_STRUCT*    getOtpData() const {return &m_rOtp;}
    virtual MUINT32*                            getLut(ESensorMode_T eLscScn) const;
    virtual MUINT32*                            getLut(ESensorMode_T eLscScn, MUINT32 u4CtIdx) const;

    virtual E_LSC_OTP_T                         getOtpState() const {return m_eOtpState;}
//    virtual MUINT32                             getTotalLutSize(ESensorMode_T eLscScn) const;
//    virtual MUINT32                             getPerLutSize(ESensorMode_T eLscScn) const;
    virtual MBOOL                               check123InNvram() const;

//    virtual MBOOL                               readNvramTbl(MBOOL fgForce);
//    virtual MBOOL                               writeNvramTbl(void);

    virtual const ILscTbl*                      getGolden() const {return m_pGolden;};
    virtual const ILscTbl*                      getUnit() const {return m_pUnit;}

protected:
                                    LscNvramImp(ESensorDev_T sensor);
    virtual                         ~LscNvramImp();

    // read NVRAM
    virtual MVOID                   getNvramData(void);
    virtual MVOID                   getTsfCfgTbl(void);
    virtual E_LSC_OTP_T             importEEPromData();

    ESensorDev_T                    m_eSensorDev;
    MBOOL                           m_bIsEEPROMImported;
    E_LSC_OTP_T                     m_eOtpState;

    // NVRAM data
    NVRAM_CAMERA_ISP_PARAM_STRUCT*  m_pNvram_Isp;
    NVRAM_CAMERA_3A_STRUCT*         m_prNvram3A;
    ISP_SHADING_STRUCT*             m_prShadingLut;
    CAMERA_TSF_TBL_STRUCT           m_rTsfCfgTbl;

    // OTP
    CAM_CAL_SINGLE_LSC_STRUCT       m_rOtp;

    // OTP golden and unit in terms of ILscTbl
    ILscTbl*                        m_pGolden;
    ILscTbl*                        m_pUnit;
};

ILscNvram*
ILscNvram::
getInstance(ESensorDev_T sensor)
{
    return LscNvramImp::getInstance(sensor);
}

ILscNvram*
LscNvramImp::
getInstance(ESensorDev_T sensor)
{
    LSC_LOG_BEGIN("eSensorDev(0x%02x)", (MUINT32)sensor);

    switch (sensor)
    {
    default:
    case ESensorDev_Main:       //  Main Sensor
        static LscNvramImp singleton_main(ESensorDev_Main);
        LSC_LOG_END("ESensorDev_Main(%p)", &singleton_main);
        return &singleton_main;
    case ESensorDev_MainSecond: //  Main Second Sensor
        static LscNvramImp singleton_main2(ESensorDev_MainSecond);
        LSC_LOG_END("ESensorDev_MainSecond(%p)", &singleton_main2);
        return &singleton_main2;
    case ESensorDev_Sub:        //  Sub Sensor
        static LscNvramImp singleton_sub(ESensorDev_Sub);
        LSC_LOG_END("ESensorDev_Sub(%p)", &singleton_sub);
        return &singleton_sub;
    }
}

LscNvramImp::
LscNvramImp(ESensorDev_T sensor)
    : m_eSensorDev(sensor)
    , m_bIsEEPROMImported(MFALSE)
    , m_eOtpState(E_LSC_NO_OTP)
    , m_pNvram_Isp(NULL)
    , m_prNvram3A(NULL)
    , m_prShadingLut(NULL)
    , m_pGolden(NULL)
    , m_pUnit(NULL)
{
    LSC_LOG("Enter LscNvramImp");
    getNvramData();
    getTsfCfgTbl();
    m_eOtpState = importEEPromData();

    if (m_eOtpState == E_LSC_WITH_MTK_OTP)
    {
        ILscTbl::TBL_BAYER_T eBayerGain = ILscTbl::BAYER_B;
        const CAM_CAL_LSC_MTK_TYPE& rMtkLsc = m_rOtp.LscTable.MtkLcsData;
        MUINT32 u4XNum  = ((rMtkLsc.CapIspReg[1] >> 28) & 0x0000000F);
        MUINT32 u4YNum  = ((rMtkLsc.CapIspReg[1] >> 12) & 0x0000000F);
        MUINT32 u4BlkW  = ((rMtkLsc.CapIspReg[1] >> 16) & 0x00000FFF);
        MUINT32 u4BlkH  = ( rMtkLsc.CapIspReg[1]        & 0x00000FFF);
        MUINT32 u4LastW = ((rMtkLsc.CapIspReg[3] >> 16) & 0x00000FFF);
        MUINT32 u4LastH = ( rMtkLsc.CapIspReg[3]        & 0x00000FFF);
        switch (rMtkLsc.PixId)
        {
        case 0: eBayerGain = ILscTbl::BAYER_B;    break;
        case 1: eBayerGain = ILscTbl::BAYER_GB;   break;
        case 2: eBayerGain = ILscTbl::BAYER_GR;   break;
        case 3: eBayerGain = ILscTbl::BAYER_R;    break;
        }
#if 0
        // write unit gain table to NVRAM
        LSC_LOG("Write Unit Gain to NVRAM buffer");
        MUINT32 u4GainTblSize = (u4XNum + 2)*(u4YNum + 2)*4*2;  // in byte (x*y*4ch*2byte)
        m_prShadingLut->SensorGoldenCalTable.IspLSCReg[1] = rMtkLsc.CapIspReg[1];
        m_prShadingLut->SensorGoldenCalTable.IspLSCReg[3] = rMtkLsc.CapIspReg[3];
        m_prShadingLut->SensorGoldenCalTable.IspLSCReg[4] = rMtkLsc.CapIspReg[4];
        m_prShadingLut->SensorGoldenCalTable.TblSize = u4GainTblSize;
        ::memcpy((void*)m_prShadingLut->SensorGoldenCalTable.UnitGainTable,
                (void*)rMtkLsc.CapTable, u4GainTblSize);
#endif
        // init unit gain
        m_pUnit = new ILscTbl(ILscTbl::GAIN_FIXED);
        m_pUnit->setBayer(eBayerGain);
        m_pUnit->setConfig(ILscTbl::Config(ILscTbl::ConfigBlk(u4XNum, u4YNum, u4BlkW, u4BlkH, u4LastW, u4LastH)));
        m_pUnit->setData(rMtkLsc.CapTable, m_pUnit->getSize());

        // init golden gain
        u4XNum  = ((m_prShadingLut->SensorGoldenCalTable.IspLSCReg[1] >> 28) & 0x0000000F);
        u4YNum  = ((m_prShadingLut->SensorGoldenCalTable.IspLSCReg[1] >> 12) & 0x0000000F);
        u4BlkW  = ((m_prShadingLut->SensorGoldenCalTable.IspLSCReg[1] >> 16) & 0x00000FFF);
        u4BlkH  = ( m_prShadingLut->SensorGoldenCalTable.IspLSCReg[1]        & 0x00000FFF);
        u4LastW = ((m_prShadingLut->SensorGoldenCalTable.IspLSCReg[3] >> 16) & 0x00000FFF);
        u4LastH = ( m_prShadingLut->SensorGoldenCalTable.IspLSCReg[3]        & 0x00000FFF);
        eBayerGain = ILscTbl::BAYER_B;
        switch (m_prShadingLut->SensorGoldenCalTable.PixId)
        {
        case 0: eBayerGain = ILscTbl::BAYER_B;    break;
        case 1: eBayerGain = ILscTbl::BAYER_GB;   break;
        case 2: eBayerGain = ILscTbl::BAYER_GR;   break;
        case 3: eBayerGain = ILscTbl::BAYER_R;    break;
        }
        m_pGolden = new ILscTbl(ILscTbl::GAIN_FIXED);
        m_pGolden->setBayer(eBayerGain);
        m_pGolden->setConfig(ILscTbl::Config(ILscTbl::ConfigBlk(u4XNum, u4YNum, u4BlkW, u4BlkH, u4LastW, u4LastH)));
        m_pGolden->setData(m_prShadingLut->SensorGoldenCalTable.GainTable, m_pGolden->getSize());
    }
}

LscNvramImp::
~LscNvramImp()
{
    LSC_LOG("Delete LscNvramImp");

    if (m_pGolden)
        delete m_pGolden;
    if (m_pUnit)
        delete m_pUnit;
}

MVOID
LscNvramImp::
getNvramData(void)
{   
    LSC_LOG_BEGIN("m_eSensorDev(0x%02x)", (MINT32)m_eSensorDev);
    NVRAM_CAMERA_ISP_PARAM_STRUCT* pNvram_Isp = NULL;
    NVRAM_CAMERA_SHADING_STRUCT *pNvram_Shading = NULL;
    NVRAM_CAMERA_3A_STRUCT *pNvram3A = NULL;

    int err;
    err = NvBufUtil::getInstance().getBufAndRead(CAMERA_NVRAM_DATA_ISP, m_eSensorDev, (void*&)pNvram_Isp);
    if(err!=0)
    {
        LSC_ERR("Fail to getBufAndRead(CAMERA_NVRAM_DATA_ISP)!");
        goto lbExit;
    }
    
    err = NvBufUtil::getInstance().getBufAndRead(CAMERA_NVRAM_DATA_SHADING, m_eSensorDev, (void*&)pNvram_Shading);
    if(err!=0)
    {
        LSC_ERR("Fail to getBufAndRead(CAMERA_NVRAM_DATA_SHADING)!");
        goto lbExit;
    }

    err = NvBufUtil::getInstance().getBufAndRead(CAMERA_NVRAM_DATA_3A, m_eSensorDev, (void*&)pNvram3A);
    if(err!=0)
    {
        LSC_ERR("Fail to getBufAndRead(CAMERA_NVRAM_DATA_3A)!");
        goto lbExit;
    }

    m_pNvram_Isp = pNvram_Isp;
    LSC_LOG("m_pNvram_Isp(%p)", m_pNvram_Isp);

    m_prShadingLut = &pNvram_Shading->Shading;
#ifdef USING_MTK_LDVT
    m_prShadingLut->GridXNum = 17;
    m_prShadingLut->GridYNum = 17;
    m_prShadingLut->Width = 1600;
    m_prShadingLut->Height = 1200;
#endif
    LSC_LOG("m_prShadingLut(%p), Version(%d), SensorID(0x%08x), Grid(%dx%d)", m_prShadingLut,
        m_prShadingLut->Version, m_prShadingLut->SensorId,
        m_prShadingLut->GridXNum, m_prShadingLut->GridYNum);

    m_prNvram3A = pNvram3A;
    LSC_LOG("m_prNvram3A(%p)", m_prNvram3A);

lbExit:
    LSC_LOG_END();
}

MVOID
LscNvramImp::
getTsfCfgTbl(void)
{
    LSC_LOG_BEGIN("m_eSensorDev(0x%02x)", (MINT32)m_eSensorDev);

    CAMERA_TSF_TBL_STRUCT*      pDftTsf     = NULL;
    MBOOL           fgLoadOK                = MFALSE;
    MINT32          i4TsfEn                 = isEnableTSF(m_eSensorDev);
    MINT32          i4TsfCtIdx              = getTSFD65Idx();
    const MINT32*   pi4TsfAwbForceSetting   = getTSFAWBForceInput();
    MUINT32*        pu4TsfData              = (MUINT32*)getTSFTrainingData();
    MINT32*         pi4TsfPara              = (MINT32*)getTSFTuningData();
    
    int err;
    err = NvBufUtil::getInstance().getBufAndRead(CAMERA_DATA_TSF_TABLE, m_eSensorDev, (void*&)pDftTsf);
    if(err!=0)
    {
        LSC_ERR("Fail to getBufAndRead(CAMERA_DATA_TSF_TABLE)!");
    }
    else
    {
        fgLoadOK = MTRUE;
    }

    if (fgLoadOK)
    {
        ::memcpy(&m_rTsfCfgTbl, pDftTsf, sizeof(CAMERA_TSF_TBL_STRUCT));
        LSC_LOG("Load TSF table OK, TSF(%d), CtIdx(%d), data(%p), para(%p)",
            m_rTsfCfgTbl.TSF_CFG.isTsfEn, m_rTsfCfgTbl.TSF_CFG.tsfCtIdx,
            m_rTsfCfgTbl.TSF_DATA,
            m_rTsfCfgTbl.TSF_PARA);
    }
    else
    {
        m_rTsfCfgTbl.TSF_CFG.isTsfEn = i4TsfEn;
        m_rTsfCfgTbl.TSF_CFG.tsfCtIdx = i4TsfCtIdx;
        ::memcpy(m_rTsfCfgTbl.TSF_CFG.rAWBInput, pi4TsfAwbForceSetting, sizeof(MINT32)*8);
        ::memcpy(m_rTsfCfgTbl.TSF_PARA, pi4TsfPara, sizeof(MINT32)*1620);
        ::memcpy(m_rTsfCfgTbl.TSF_DATA, pu4TsfData, sizeof(MUINT32)*16000);
        LSC_ERR("Load TSF table Fail, use default: TSF(%d), CtIdx(%d), data(%p), para(%p)",
            m_rTsfCfgTbl.TSF_CFG.isTsfEn, m_rTsfCfgTbl.TSF_CFG.tsfCtIdx,
            m_rTsfCfgTbl.TSF_DATA,
            m_rTsfCfgTbl.TSF_PARA);
    }

    LSC_LOG_END();
}

ILscNvram::E_LSC_OTP_T
LscNvramImp::
importEEPromData()
{
    E_LSC_OTP_T eRet = E_LSC_NO_OTP;
    MUINT32 i;
    LSC_LOG_BEGIN();

    MINT32 i4SensorDevID;

    switch (m_eSensorDev)
    {
    case ESensorDev_Main:
        i4SensorDevID = NSCam::SENSOR_DEV_MAIN;
        break;
    case ESensorDev_Sub:
        i4SensorDevID = NSCam::SENSOR_DEV_SUB;
        break;
    case ESensorDev_MainSecond:
        i4SensorDevID = NSCam::SENSOR_DEV_MAIN_2;
        break;
    case ESensorDev_Main3D:
        i4SensorDevID = NSCam::SENSOR_DEV_MAIN_3D;
        break;
    default:
        i4SensorDevID = NSCam::SENSOR_DEV_NONE;
        break;
    }

    CAMERA_CAM_CAL_TYPE_ENUM eCamCalDataType = CAMERA_CAM_CAL_DATA_SHADING_TABLE;
    CAM_CAL_DATA_STRUCT* pCalData = new CAM_CAL_DATA_STRUCT;

    if (pCalData == NULL)
    {
        LSC_ERR("Fail to allocate buffer!");
        return E_LSC_OTP_ERROR;
    }
    
#ifndef LSC_DBG
    CamCalDrvBase* pCamCalDrvObj = CamCalDrvBase::createInstance();
    if (!pCamCalDrvObj) 
    {
        LSC_LOG("pCamCalDrvObj is NULL");
        delete pCalData;
        return E_LSC_NO_OTP;
    }

    MINT32 ret = pCamCalDrvObj->GetCamCalCalData(i4SensorDevID, eCamCalDataType, pCalData);
#else
    MINT32 ret = 0;
    ::memcpy(pCalData, &_rDbgCamCalData, sizeof(CAM_CAL_DATA_STRUCT));
#endif

    LSC_LOG("ret(0x%08x)", ret);
    if (ret & CamCalReturnErr[eCamCalDataType])
    {
        LSC_LOG("Error(%s)", CamCalErrString[eCamCalDataType]);
        m_bIsEEPROMImported = MTRUE;
        delete pCalData;
        return E_LSC_NO_OTP;
    }
    else
    {
        LSC_LOG("Get OK");
    }

    MUINT32 u4Rot = 0;
    CAM_CAL_DATA_VER_ENUM eDataType  = pCalData->DataVer;
    CAM_CAL_LSC_DATA*     pLscData   = NULL;    // union struct

    LSC_LOG("eDataType(%d)", eDataType);
    switch (eDataType) 
    {
    case CAM_CAL_SINGLE_EEPROM_DATA:
        LSC_LOG("CAM_CAL_SINGLE_EEPROM_DATA");
    case CAM_CAL_SINGLE_OTP_DATA:
        LSC_LOG("CAM_CAL_SINGLE_OTP_DATA");
        pLscData = &pCalData->SingleLsc.LscTable;
        u4Rot = pCalData->SingleLsc.TableRotation;
        break;
    case CAM_CAL_N3D_DATA:
        LSC_LOG("CAM_CAL_N3D_DATA");
        if (ESensorDev_Main == m_eSensorDev)
        {
            pLscData = &pCalData->N3DLsc.Data[0].LscTable;
            u4Rot = pCalData->N3DLsc.Data[0].TableRotation;
            LSC_LOG("CAM_CAL_N3D_DATA MAIN");
        }
        else
        {
            pLscData = &pCalData->N3DLsc.Data[1].LscTable;
            u4Rot = pCalData->N3DLsc.Data[1].TableRotation;
            LSC_LOG("CAM_CAL_N3D_DATA MAIN2");
        }
        break;
    default:
        LSC_ERR("Unknown eDataType(%d)", eDataType);
        m_bIsEEPROMImported = MTRUE;
        delete pCalData;
        return E_LSC_NO_OTP;
    }

    m_rOtp.TableRotation = u4Rot;
    ::memcpy(&m_rOtp.LscTable, pLscData, sizeof(CAM_CAL_LSC_DATA));
    LSC_LOG("u4Rot(%d), pLscData(%p)", u4Rot, pLscData);
    
    MUINT8 u1TblType = pLscData->MtkLcsData.MtkLscType;

    if (u1TblType & (1<<0))
    {
        // send table via sensor hal
        eRet = E_LSC_NO_OTP;
        //setSensorShading((MVOID*) &pLscData->SensorLcsData);
    }
    else if (u1TblType & (1<<1))
    {
        // do 1-to-3
        eRet = E_LSC_WITH_MTK_OTP;
        //m_fg1to3 = do123LutToSysram((MVOID*) &pLscData->MtkLcsData);
    }

    m_bIsEEPROMImported = MTRUE;

    delete pCalData;
    
    LSC_LOG_END();

    return eRet;
}

MUINT32*
LscNvramImp::
getLut(ESensorMode_T eLscScn) const
{
#if USING_BUILTIN_LSC
    if (eLscScn == LSC_SCENARIO_CAP)
        return def_coef_cap;
    else
        return def_coef;
#else
    #if 0
    switch (eLscScn)
    {
    case ESensorMode_Preview:
        return &m_prShadingLut->PrvTable[0][0];
    case ESensorMode_Capture:
        return &m_prShadingLut->CapTable[0][0];
    case ESensorMode_Video:
        return &m_prShadingLut->VdoTable[0][0];
    case ESensorMode_SlimVideo1:
        return &m_prShadingLut->Sv1Table[0][0];
    case ESensorMode_SlimVideo2:
        return &m_prShadingLut->Sv2Table[0][0];
    case ESensorMode_Custom1:
        return &m_prShadingLut->Cs1Table[0][0];    
    case ESensorMode_Custom2:
        return &m_prShadingLut->Cs2Table[0][0];    
    case ESensorMode_Custom3:
        return &m_prShadingLut->Cs3Table[0][0];
    case ESensorMode_Custom4:
        return &m_prShadingLut->Cs4Table[0][0];
    case ESensorMode_Custom5:
        return &m_prShadingLut->Cs5Table[0][0];
    default:
        LSC_ERR("Wrong eLscScn(%d)", eLscScn);
        break;
    }
    return NULL;
    #else
    return &m_prShadingLut->CapTable[0][0];
    #endif
#endif
}

MUINT32*
LscNvramImp::
getLut(ESensorMode_T eLscScn, MUINT32 u4CtIdx) const
{
#if USING_BUILTIN_LSC
    if (eLscScn == LSC_SCENARIO_CAP)
        return def_coef_cap;
    else
        return def_coef;
#else
    if (u4CtIdx < SHADING_SUPPORT_CT_NUM)
    {
        #if 0
        switch (eLscScn)
        {
        case ESensorMode_Preview:
            return &m_prShadingLut->PrvTable[u4CtIdx][0];
        case ESensorMode_Capture:
            return &m_prShadingLut->CapTable[u4CtIdx][0];
        case ESensorMode_Video:
            return &m_prShadingLut->VdoTable[u4CtIdx][0];
        case ESensorMode_SlimVideo1:
            return &m_prShadingLut->Sv1Table[u4CtIdx][0];
        case ESensorMode_SlimVideo2:
            return &m_prShadingLut->Sv2Table[u4CtIdx][0];
        case ESensorMode_Custom1:
            return &m_prShadingLut->Cs1Table[u4CtIdx][0];    
        case ESensorMode_Custom2:
            return &m_prShadingLut->Cs2Table[u4CtIdx][0];
        case ESensorMode_Custom3:
            return &m_prShadingLut->Cs3Table[u4CtIdx][0];
        case ESensorMode_Custom4:
            return &m_prShadingLut->Cs4Table[u4CtIdx][0];
        case ESensorMode_Custom5:
            return &m_prShadingLut->Cs5Table[u4CtIdx][0];
        default:
            LSC_ERR("Wrong eLscScn(%d)", eLscScn);
            break;
        }
        #else
        return &m_prShadingLut->CapTable[u4CtIdx][0];
        #endif
    }

    LSC_ERR("Wrong eLscScn(%d), CT(%d)", eLscScn, u4CtIdx);
    return NULL;
#endif
}

MBOOL
LscNvramImp::
check123InNvram() const
{
    MBOOL fgRet = 
        (m_pNvram_Isp->ISPComm.CommReg[CAL_INFO_IN_COMM_LOAD] == CAL_DATA_LOAD) ||
        (m_pNvram_Isp->ISPComm.CommReg[CAL_INFO_IN_COMM_LOAD] == (CAL_DATA_LOAD+1));
    return fgRet;
}

