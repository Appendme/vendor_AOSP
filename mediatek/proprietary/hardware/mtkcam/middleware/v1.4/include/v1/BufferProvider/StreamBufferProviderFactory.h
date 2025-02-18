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

#ifndef _MTK_HARDWARE_MTKCAM_V1_BUFFERPROVIDER_STREAMBUFFERPROVIDERFACTORY_H_
#define _MTK_HARDWARE_MTKCAM_V1_BUFFERPROVIDER_STREAMBUFFERPROVIDERFACTORY_H_
//
#include <utils/RefBase.h>
#include <utils/Vector.h>

#include <v3/utils/streambuf/IStreamBufferProvider.h>
#include <LegacyPipeline/StreamBufferProvider.h>
#include <v1/camutils/IImgBufQueue.h>

using namespace android;
using namespace android::MtkCamUtils;
using namespace NSCam::v3;
#include <ImgBufProvidersManager.h>

/******************************************************************************
 *
 ******************************************************************************/
//
namespace NSCam {
namespace v1 {

class StreamBufferProviderFactory
    : public virtual RefBase
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interface.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
    static android::sp<StreamBufferProviderFactory>
                                       createInstance( char const* szCallerName );

    virtual                            ~StreamBufferProviderFactory() {};

    /**
     *
     * Set callback for buffer.
     *
     * @param[in] pCb : IImageCallback.
     *
     */
    virtual MERROR                      setBufferCallback(
                                            android::wp<IImageCallback> pCb
                                        )                                                       = 0;

    /**
     *
     * Set image stream info.
     *
     * @param[in] pStreamInfo : ImageStreamInfo.
     *
     */
    virtual MERROR                      setImageStreamInfo(
                                            android::sp<IImageStreamInfo> pStreamInfo
                                        )                                                       = 0;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interface. Buffer source.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
    /**
     *
     * Choose source for buffer pool.
     *    - CamClient
     *       - setCamClient()
     *    - allocate
     *       - setBufferPool()
     *    - other
     *       - setUsersBuffer()
     *
     */

    /**
     *
     * Set specific camClient as buffer source.
     *
     * @param[in] pSource : source CamClient.
     *
     */
    virtual MERROR                      setCamClient(
                                            //android::sp<IImgBufProvider> pSource
                                            sp<ImgBufProvidersManager> pSource,
                                            MINT32                     rMode
                                        )                                                       = 0;

    /**
     *
     * Pool allocate buffer as source.
     * Create consumer
     *
     * If the API called, factory output will contain consumer & producer
     *
     *
     */
    virtual MERROR                      needConsumer()                                          = 0;

    /**
     *
     * User provide buffer.
     *
     * @param[in] pSource : source CamClient.
     *
     */
    virtual MERROR                      setUsersBuffer(
                                            List< android::sp<IImageBuffer> > pSource
                                        )                                                       = 0;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interface.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
    /**
     *
     * Create StreamBufferProvider.
     * This API must be called after all required information is set.
     *
     * @param[out] pBuffer : multiple providers. consumer + producer
     *
     */
    virtual MERROR                      create(
                                            Vector< android::sp<StreamBufferProvider> >& pProviders
                                        )                                                       = 0;
};

/******************************************************************************
 *
 ******************************************************************************/
};  //namespace v1
};  //namespace NSCam
#endif  //_MTK_HARDWARE_MTKCAM_V1_BUFFERPROVIDER_STREAMBUFFERPROVIDERFACTORY_H_

