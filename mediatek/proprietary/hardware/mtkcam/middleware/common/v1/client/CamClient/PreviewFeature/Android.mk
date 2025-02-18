# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.

################################################################################
#
################################################################################

LOCAL_PATH := $(call my-dir)

################################################################################
#
################################################################################
include $(CLEAR_VARS)

-include $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam/mtkcam.mk
#-----------------------------------------------------------
LOCAL_SRC_FILES += PreviewFeature.Thread.cpp
LOCAL_SRC_FILES += PreviewFeatureBase.cpp
LOCAL_SRC_FILES += PreviewFeatureBufMgr.cpp
LOCAL_SRC_FILES += PreviewFeatureClient.BufOps.cpp
#-----------------------------------------------------------
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/frameworks/av/include
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/external/mpo/mpoencoder
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/external/mpo
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/external/mpo/inc
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/include
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/include/mtkcam
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/include/mtkcam/v1/featureio
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam/middleware/common/v1/client/CamClient
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam/middleware/common/v1/client/CamClient/PreviewFeature
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam/middleware/common/v1/client/CamClient/PreviewFeature/MAV
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam/middleware/common/v1/client/CamClient/PreviewFeature/Panorama
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam/middleware/common/v1/client/CamClient/PreviewFeature/MotionTrack
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/gralloc_extra/include
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam/middleware/common/include
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam/ext/include
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam/middleware/common/include/v1/featureio

LOCAL_C_INCLUDES += $(TOP)/bionic
LOCAL_C_INCLUDES += $(TOP)/external/stlport/stlport
LOCAL_C_INCLUDES += $(TOP)/system/media/camera/include
LOCAL_C_INCLUDES += $(TOP)/system/core/libion/include

#ifeq (yes,$(MTK_CAM_MAV_SUPPORT))
LOCAL_WHOLE_STATIC_LIBRARIES += libcam.client.camclient.featurebase.commav
#endif
#ifeq (yes,$(MTK_CAM_AUTORAMA_SUPPORT))
LOCAL_WHOLE_STATIC_LIBRARIES += libcam.client.camclient.featurebase.companorama
#endif
ifeq (yes,$(MTK_MOTION_TRACK_SUPPORT))
LOCAL_WHOLE_STATIC_LIBRARIES += libcam.client.camclient.featurebase.commotiontrack
endif

#-----------------------------------------------------------
LOCAL_MODULE := libcam.client.camclient.previewfeature
#-----------------------------------------------------------

#
# Start of common part ------------------------------------
-include $(TOP)/$(MTK_PATH_PLATFORM)/hardware/mtkcam/mtkcam.mk
#-----------------------------------------------------------

LOCAL_CFLAGS += $(MTKCAM_CFLAGS)

#ifeq (yes,$(MTK_CAM_MAV_SUPPORT))  
	LOCAL_CFLAGS += -DMTK_CAM_MAV_SUPPORTED=1
#endif

#ifeq (yes,$(MTK_CAM_AUTORAMA_SUPPORT))
	LOCAL_CFLAGS += -DMTK_CAM_AUTORAMA_SUPPORTED=1 
#endif

ifeq (yes,$(MTK_MOTION_TRACK_SUPPORT))
	LOCAL_CFLAGS += -DMTK_MOTION_TRACK_SUPPORTED
endif

#-----------------------------------------------------------
LOCAL_C_INCLUDES += $(MTKCAM_C_INCLUDES)

# End of common part ---------------------------------------
#
include $(BUILD_STATIC_LIBRARY)



################################################################################
#
################################################################################
include $(CLEAR_VARS)
include $(call all-makefiles-under,$(LOCAL_PATH))

