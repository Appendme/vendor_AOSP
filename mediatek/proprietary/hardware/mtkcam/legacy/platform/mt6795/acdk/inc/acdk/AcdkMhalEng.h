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

///////////////////////////////////////////////////////////////////////////////
// No Warranty
// Except as may be otherwise agreed to in writing, no warranties of any
// kind, whether express or implied, are given by MTK with respect to any MTK
// Deliverables or any use thereof, and MTK Deliverables are provided on an
// "AS IS" basis.  MTK hereby expressly disclaims all such warranties,
// including any implied warranties of merchantability, non-infringement and
// fitness for a particular purpose and any warranties arising out of course
// of performance, course of dealing or usage of trade.  Parties further
// acknowledge that Company may, either presently and/or in the future,
// instruct MTK to assist it in the development and the implementation, in
// accordance with Company's designs, of certain softwares relating to
// Company's product(s) (the "Services").  Except as may be otherwise agreed
// to in writing, no warranties of any kind, whether express or implied, are
// given by MTK with respect to the Services provided, and the Services are
// provided on an "AS IS" basis.  Company further acknowledges that the
// Services may contain errors, that testing is important and Company is
// solely responsible for fully testing the Services and/or derivatives
// thereof before they are used, sublicensed or distributed.  Should there be
// any third party action brought against MTK, arising out of or relating to
// the Services, Company agree to fully indemnify and hold MTK harmless.
// If the parties mutually agree to enter into or continue a business
// relationship or other arrangement, the terms and conditions set forth
// hereunder shall remain effective and, unless explicitly stated otherwise,
// shall prevail in the event of a conflict in the terms in any agreements
// entered into between the parties.
////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2008, MediaTek Inc.
// All rights reserved.
//
// Unauthorized use, practice, perform, copy, distribution, reproduction,
// or disclosure of this information in whole or in part is prohibited.
////////////////////////////////////////////////////////////////////////////////

//! \file  AcdkMhalEng.h

#ifndef _ACDKMHALENG_H_
#define _ACDKMHALENG_H_

#if 0//ndef USING_MTK_LDVT   // Not using LDVT.
#define ACDK_CAMERA_3A
#endif

#ifdef ACDK_CAMERA_3A
#include <mtkcam/featureio/IHal3A.h>
using namespace NS3A;
#endif

namespace NSAcdkMhal
{
    /**
         *@class AcdkMhalEng
         *@brief This class is the implementation of AcdkMhalBase and also communicates with 3A
       */
    //class AcdkMhalEng : public AcdkMhalBase,
    //                        public I3ACallBack
    class AcdkMhalEng     : public AcdkMhalBase//, public I3ACallBack
    {
        public :

            /**
                       *@brief AcdkMhalEng constructor
                     */
            AcdkMhalEng();

            /**
                       *@brief AcdkMhalEng destructor
                     */
            ~AcdkMhalEng() {};

            /**
                       *@brief Destory AcdkMhalEng Object
                     */
            virtual void destroyInstance();

            /**
                       *@brief Set current sate to newState
                       *
                       *@param[in] newState : new state
                     */
            virtual MVOID acdkMhalSetState(acdkMhalState_e newState);

            /**
                       *@brief Get current sate of AcdkMhalBase
                     */
            virtual acdkMhalState_e acdkMhalGetState();

            /**
                       *@brief Indicates whether is ready for capture or not
                       *@return
                       *-MFALSE indicates not ready, MTRUE indicates ready
                     */
            virtual MBOOL acdkMhalReadyForCap();

            /**
                       *@brief Initialize function
                       *@note Must call this function right after createInstance and before other functions
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MINT32 acdkMhalInit();

            /**
                       *@brief Uninitialize function
                       *@note Must call this function before destroyInstance
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MINT32 acdkMhalUninit();

            /**
                       *@brief Handle callback
                       *
                       *@param[in] a_type : callback type
                       *@param[in] a_addr1 : return data address
                       *@param[in] a_addr2 : return data address
                       *@param[in] a_dataSize : return data size
                     */
            virtual MVOID  acdkMhalCBHandle(MUINT32 a_type, MUINT8* a_addr1, MUINT8* a_addr2 = 0, MUINT32 const a_dataSize = 0);

            /**
                       *@brief Start preview
                       *@note Config and control ISP to start preview
                       *
                       *@param[in] a_pBuffIn : pointer to acdkMhalPrvParam_t data
                       *
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MINT32 acdkMhalPreviewStart(MVOID *a_pBuffIn);

            /**
                       *@brief Stop preview
                       *@note Config and control ISP to stop preview
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MINT32 acdkMhalPreviewStop();

            /**
                       *@brief Start capture
                       *
                       *@param[in] a_pBuffIn
                       *
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MINT32 acdkMhalCaptureStart(MVOID *a_pBuffIn);

            /**
                       *@brief Stop capture
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MINT32 acdkMhalCaptureStop();

            /**
                       *@brief Execute preview process
                       *@note Here is a preview loop
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MINT32 acdkMhalPreviewProc();

            /**
                       *@brief Change state to preCapture state and do related opertion
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MINT32 acdkMhalPreCapture();

            /**
                       *@brief  Execute capture process
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MINT32 acdkMhalCaptureProc();

            /**
                       *@brief  Get shutter time in us
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MUINT32 acdkMhalGetShutTime();

            /**
                       *@brief  Set shutter time
                       *
                       *@param[in] a_time : specific shutter time in us
                       *
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MVOID acdkMhalSetShutTime(MUINT32 a_time);
            /**
                       *@brief  Get AF information sucuess or fail
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MUINT32 acdkMhalGetAFInfo();
        private :

            /**
                       *@brief  Set 3A Parameter
                       *
                       *@param[in] devID : sensor device
                       *
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual MINT32 acdkMhal3ASetParam(MINT32 devID, MUINT8 IsFactory);

            /**
                       *@brief  Set 3A doNotifyCb
                       *
                       *@param[in] _msgType : callback message type
                       *@param[in] _ext1 : callback info
                       *@param[in] _ext2 : callback info
                       *@param[in] _ext3 : callback info
                       *
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual void doNotifyCb(int32_t _msgType,
                                        int32_t _ext1,
                                        int32_t _ext2,
                                        int32_t _ext3);

            /**
                       *@brief  Set 3A doNotifyCb
                       *
                       *@param[in] _msgType : callback message type
                       *@param[in] _data : callback data
                       *@param[in] _size : callback data size
                       *
                       *@return
                       *-0 indicates success, otherwise indicates fail
                     */
            virtual void doDataCb(int32_t _msgType,
                                      void *_data,
                                      uint32_t _size);

            /******************************************************************************/

            mutable Mutex mLock;

            MBOOL mFocusDone;
            MBOOL mFocusSucceed;

            MBOOL mReadyForCap;
            MUINT32 mCaptureType;
            MUINT32 mu4SensorDelay;
            MUINT32 mu4PreWidth;
            MUINT32 mu4PreHeight;

            acdkMhalPrvParam_t mAcdkMhalPrvParam;
            acdkMhalCapParam_t mAcdkMhalCapParam;
            acdkMhalState_e mAcdkMhalState;
#ifdef ACDK_CAMERA_3A
            //3A object
            IHal3A *m_p3AHal;
            CaptureParam_T mCap3AParam;
            MUINT32        mCurMagicNum;
            MUINT32        sensorScenario;
#endif
    };
};

#endif //end AcdkMhalEng.h

