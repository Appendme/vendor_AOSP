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

#ifndef _MTK_HARDWARE_INCLUDE_MTKCAM_V1_LEGACYPIPELINE_IREQUESTCONTROLLER_H_
#define _MTK_HARDWARE_INCLUDE_MTKCAM_V1_LEGACYPIPELINE_IREQUESTCONTROLLER_H_
//
#include <utils/RefBase.h>
#include <utils/StrongPointer.h>
#include <utils/String8.h>
#include <utils/Vector.h>
//
#include <v1/IParamsManager.h>
#include <v1/camutils/CamInfo.h>
#include <LegacyPipeline/StreamBufferProvider.h>
#include <Scenario/IFlowControl.h>
#include "ILegacyPipeline.h"

using namespace NSCam::v1;

/******************************************************************************
 *
 ******************************************************************************/
//
namespace NSCam {
namespace v1 {

namespace NSLegacyPipeline {

class IRequestController
    : public virtual android::RefBase
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interface.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:

    static android::sp<IRequestController>      createInstance(
                                                    char                          const *name,
                                                    android::sp< CamMsgCbInfo >   const &rpCamMsgCbInfo,
                                                    android::sp< IParamsManager > const &rpParamsManager
                                                );

    virtual char const*                         getName() const                         = 0;

protected:

    virtual                                     ~IRequestController() {};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interface.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:  //// from adapter

    /**
     * Auto focus.
     *
     * @return
     *      0 indicates success; otherwise failure.
     *
     */
    virtual MERROR                              autoFocus()                             = 0;

    /**
     * Cancel auto focus.
     *
     * @return
     *      0 indicates success; otherwise failure.
     *
     */
    virtual MERROR                              cancelAutoFocus()                       = 0;

    /**
     * Start pre-capture.
     */
    virtual MERROR                              precapture()                            = 0;

    /**
     * Set parameters.
     *
     * @return
     *      0 indicates success; otherwise failure.
     *
     */
    virtual MERROR                              setParameters()                         = 0;

    /**
     * Send command.
     *
     * @param[in] cmd : command
     *
     * @return
     *      0 indicates success; otherwise failure.
     *
     */
    virtual MERROR                              sendCommand(
                                                    int32_t cmd,
                                                    int32_t arg1, int32_t arg2)         = 0;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interface.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:  //// for pipeline

    /**
     * Set template request.
     *
     * @param[in] type : template request type.
     *
     * @return
     *      0 indicates success; otherwise failure.
     *
     */
    virtual MERROR                              setRequestType( int type )              = 0;

    /**
     * Start sending request to pipeline.
     *
     * @param[in] cameraId : open camera ID.
     *
     * @param[in] pPipeline : pipeline that need to be started.
     *
     * @return
     *      0 indicates success; otherwise failure.
     *
     */
    virtual MERROR                              startPipeline(
                                                    MINT32                          aCameraId,
                                                    MINT32                          aStartRequestNumber,
                                                    MINT32                          aEndRequestNumber,
                                                    android::wp<ILegacyPipeline>    apPipeline,
                                                    android::wp<IRequestUpdater>    apRequestUpdater
                                                )                                       = 0;

    /**
     * Stop sending request to pipeline.
     *
     * @return
     *      0 indicates success; otherwise failure.
     *
     */
    virtual MERROR                              stopPipeline()                          = 0;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interface.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:  //// for feature

    /**
     * Submit user defined request.
     *
     * @param[in] setting: setting for pipeline frame.
     *
     * @param[in] resultBuffer: empty buffer pool to get result buffers.
     *
     */
    virtual MERROR                              submitRequest(
                                                    Vector< IMetadata* >                settings,
                                                    android::sp<StreamBufferProvider>& resultBuffer
                                                ) const                                 = 0;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interface.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:

    virtual MVOID                               dump()                                  = 0;

};

/******************************************************************************
*
******************************************************************************/
};  //namespace NSLegacyPipeline
};  //namespace v1
};  //namespace NSCam
#endif  //_MTK_HARDWARE_INCLUDE_MTKCAM_V1_LEGACYPIPELINE_IREQUESTCONTROLLER_H_

