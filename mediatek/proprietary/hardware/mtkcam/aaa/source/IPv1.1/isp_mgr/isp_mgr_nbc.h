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
#ifndef _ISP_MGR_NBC_H_
#define _ISP_MGR_NBC_H_

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  NBC (ANR + CCR)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef class ISP_MGR_NBC : public ISP_MGR_BASE_T
{
    typedef ISP_MGR_NBC    MyType;
private:
    MBOOL m_bANR1_ENYEnable;
    MBOOL m_bANR1_ENCEnable;
    MBOOL m_bANR1ENCBackup;
    MBOOL m_bANR1ENYBackup;
    MBOOL m_bCCTANR1Enable;
    MUINT32 m_u4StartAddr; // for debug purpose: 0x0A20

    enum
    {
        //ERegInfo_DIP_X_ANR_TBL,
        ERegInfo_DIP_X_ANR_CON1,
        ERegInfo_DIP_X_ANR_CON2,
        ERegInfo_DIP_X_ANR_YAD1,
        ERegInfo_DIP_X_ANR_YAD2,
        ERegInfo_DIP_X_ANR_Y4LUT1,
        ERegInfo_DIP_X_ANR_Y4LUT2,
        ERegInfo_DIP_X_ANR_Y4LUT3,
        ERegInfo_DIP_X_ANR_C4LUT1,
        ERegInfo_DIP_X_ANR_C4LUT2,
        ERegInfo_DIP_X_ANR_C4LUT3,
        ERegInfo_DIP_X_ANR_A4LUT2,
        ERegInfo_DIP_X_ANR_A4LUT3,
        ERegInfo_DIP_X_ANR_L4LUT1,
        ERegInfo_DIP_X_ANR_L4LUT2,
        ERegInfo_DIP_X_ANR_L4LUT3,
        ERegInfo_DIP_X_ANR_PTY,
        ERegInfo_DIP_X_ANR_CAD,
        ERegInfo_DIP_X_ANR_PTC,
        ERegInfo_DIP_X_ANR_LCE,
        ERegInfo_DIP_X_ANR_MED1,
        ERegInfo_DIP_X_ANR_MED2,
        ERegInfo_DIP_X_ANR_MED3,
        ERegInfo_DIP_X_ANR_MED4,
        ERegInfo_DIP_X_ANR_HP1,
        ERegInfo_DIP_X_ANR_HP2,
        ERegInfo_DIP_X_ANR_HP3,
        ERegInfo_DIP_X_ANR_ACT1,
        ERegInfo_DIP_X_ANR_ACT2,
        ERegInfo_DIP_X_ANR_ACT3,
//        ERegInfo_DIP_X_ANR_ACTY,
        ERegInfo_DIP_X_ANR_ACTC,
        ERegInfo_DIP_X_ANR_RSV1,        
        ERegInfo_NUM
    };
    RegInfo_T m_rIspRegInfo[ERegInfo_NUM];

protected:
    ISP_MGR_NBC(ESensorDev_T const eSensorDev)
        : ISP_MGR_BASE_T(m_rIspRegInfo, ERegInfo_NUM, m_u4StartAddr, eSensorDev)
        , m_bANR1_ENYEnable(MTRUE)
        , m_bANR1_ENCEnable(MTRUE)
        , m_bANR1ENCBackup(MFALSE)
        , m_bANR1ENYBackup(MFALSE)
        , m_bCCTANR1Enable(MTRUE)
        , m_u4StartAddr(REG_ADDR_P2(DIP_X_ANR_CON1/*DIP_X_ANR_TBL*/))
    {
        // register info addr init
		//INIT_REG_INFO_ADDR_P2(DIP_X_ANR_TBL);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_CON1);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_CON2);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_YAD1);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_YAD2);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_Y4LUT1); 
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_Y4LUT2); 
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_Y4LUT3); 
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_C4LUT1); 
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_C4LUT2); 
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_C4LUT3); 
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_A4LUT2); 
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_A4LUT3); 
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_L4LUT1); 
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_L4LUT2); 
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_L4LUT3); 
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_PTY);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_CAD);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_PTC);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_LCE);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_MED1);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_MED2);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_MED3);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_MED4);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_HP1);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_HP2);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_HP3);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_ACT1);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_ACT2);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_ACT3);
#warning "FIXME"
//		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_ACTY);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_ACTC);	
		INIT_REG_INFO_ADDR_P2(DIP_X_ANR_RSV1);	
    }

    virtual ~ISP_MGR_NBC() {}

public:
    static MyType&  getInstance(ESensorDev_T const eSensorDev);

public: // Interfaces.

    template <class ISP_xxx_T>
    MyType& put(ISP_xxx_T const& rParam);

    template <class ISP_xxx_T>
    MyType& get(ISP_xxx_T & rParam);

    MBOOL
    isANR1_ENYEnable()
    {
        return m_bANR1_ENYEnable;
    }

    MBOOL
    isANR1_ENCEnable()
    {
        return m_bANR1_ENCEnable;
    }


    MBOOL
    isCCTANR1Enable()
    {
        return m_bCCTANR1Enable;
    }

    MVOID
    setANR1Enable(MBOOL bEnable)
    {
         setANR1_ENYEnable(bEnable);
         setANR1_ENCEnable(bEnable);
    }

    MVOID
    setANR1_ENYEnable(MBOOL bEnable)
    {
        m_bANR1_ENYEnable = bEnable;
    }

    MVOID
    setANR1_ENCEnable(MBOOL bEnable)
    {
        m_bANR1_ENCEnable = bEnable;
    }

    MVOID
    setCCTANR1Enable(MBOOL bEnable)
    {
         m_bCCTANR1Enable = bEnable;
    }

    MBOOL apply(EIspProfile_T eIspProfile, dip_x_reg_t* pReg);
} ISP_MGR_NBC_T;

template <ESensorDev_T const eSensorDev>
class ISP_MGR_NBC_DEV : public ISP_MGR_NBC_T
{
public:
    static
    ISP_MGR_NBC_T&
    getInstance()
    {
        static ISP_MGR_NBC_DEV<eSensorDev> singleton;
        return singleton;
    }
    virtual MVOID destroyInstance() {}

    ISP_MGR_NBC_DEV()
        : ISP_MGR_NBC_T(eSensorDev)
    {}

    virtual ~ISP_MGR_NBC_DEV() {}

};


#endif

