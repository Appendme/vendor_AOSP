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
// AcdkCLITest.cpp  $Revision$
////////////////////////////////////////////////////////////////////////////////

//! \file  AcdkCLITest.cpp
//! \brief

#define LOG_TAG "FBSDKTest"

#include <stdio.h>
#include <stdlib.h>
//
#include <errno.h>
#include <fcntl.h>
#include <sdk/IFaceBeautySDKHal.h>
#include <utils/include/Format.h>

using namespace NSCam;
using namespace NSCam::Utils::Format;

static void usage()
{
    printf("Usage: fbsdktest width height filename\n");
}
/*******************************************************************************
*  Main Function
********************************************************************************/
int main(int argc, char** argv)
{
    int ret = 0;

    if( argc < 4 )
    {
        usage();
        return -1;
    }

#if 0
    printf("getchar()");
    getchar();
#endif

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int fd = ::open(argv[3], O_RDONLY);
    if (fd < 0) {
        printf("failed to open file [%s]: %s", argv[3], strerror(errno));
        return -1;
    }
    ::close(fd);

    IImageBufferAllocator* allocator = IImageBufferAllocator::getInstance();

    MUINT32 bufStridesInBytes[3] = {0};
    MUINT32 plane = queryPlaneCount(eImgFmt_I422);

    for (MUINT32 i = 0; i < plane; i++)
    {
        bufStridesInBytes[i] = NSCam::Utils::Format::queryPlaneWidthInPixels(eImgFmt_I422,i, width) * NSCam::Utils::Format::queryPlaneBitsPerPixel(eImgFmt_I422,i) / 8;
    }

    MINT32 bufBoundaryInBytes[3] = {0, 0, 0};
    //
    IImageBufferAllocator::ImgParam imgParam(
            eImgFmt_I422, MSize(width,height), bufStridesInBytes, bufBoundaryInBytes, plane
            );

    IImageBuffer* SrcImgBuffer = allocator->alloc_ion(LOG_TAG, imgParam);
    if  ( SrcImgBuffer == NULL )
    {
        printf("failed to allocat source image buffer");
        return -1;
    }
    SrcImgBuffer->loadFromFile(argv[3]);
    SrcImgBuffer->lockBuf( LOG_TAG, eBUFFER_USAGE_HW_CAMERA_READWRITE | eBUFFER_USAGE_SW_READ_OFTEN | eBUFFER_USAGE_SW_WRITE_OFTEN );

    IImageBuffer* DstImgBuffer = allocator->alloc_ion(LOG_TAG, imgParam);
    if  ( DstImgBuffer == NULL )
    {
        printf("failed to allocat dest image buffer");
        return -1;
    }
    DstImgBuffer->lockBuf( LOG_TAG, eBUFFER_USAGE_HW_CAMERA_READWRITE | eBUFFER_USAGE_SW_READ_OFTEN | eBUFFER_USAGE_SW_WRITE_OFTEN );

    IFaceBeautySDKHal* pFBSDK = IFaceBeautySDKHal::createInstance();
    if (pFBSDK == NULL)
    {
        printf("failed to create FaceBeautySDK instance");
        return -1;
    }
    pFBSDK->init(STILL_IMAGE_MODE);
    FACE_BEAUTY_SDK_HAL_PARAMS param;
    param.SmoothLevel = 9;
    param.SkinColor = 9;
    param.EnlargeEyeLevel = 9;
 param.SlimFaceLevel = 9;
    if (!pFBSDK->apply(SrcImgBuffer, DstImgBuffer, param))
    {
       printf("failed to apply effect");
       return -1;
    }

    DstImgBuffer->saveToFile("/sdcard/result.yuv");
    printf("Output result.yuv");

    pFBSDK->uninit();
    pFBSDK->destroyInstance();

    return 0;
}
