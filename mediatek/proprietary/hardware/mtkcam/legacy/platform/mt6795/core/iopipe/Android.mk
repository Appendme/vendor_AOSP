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

#-----------------------------------------------------------
sinclude $(TOP)/$(MTK_MTKCAM_PLATFORM)/mtkcam.mk

#-----------------------------------------------------------
MTKCAM_USE_LEGACY_IMAGEIO := '1'
MTKCAM_USE_LEGACY_SIMAGER := '1'
MTKCAM_USE_LEGACY_JPEGHW := '1'
#---
# FIXME, ResDrv in CamDrv 
#---
MTKCAM_USE_LEGACY_CAMDRV := '1'
ifeq ($(BUILD_MTK_LDVT),true)
    MTKCAM_USE_LEGACY_SIMAGER := '0'
    MTKCAM_USE_LEGACY_JPEGHW := '0'
endif
#-----------------------------------------------------------
LOCAL_SRC_FILES += $(call all-c-cpp-files-under, .)

#-----------------------------------------------------------
LOCAL_C_INCLUDES += $(MTKCAM_C_INCLUDES)
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/gralloc_extra/include
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/mtkcam/ext/include

LOCAL_C_INCLUDES += $(TOP)/$(MTKCAM_C_INCLUDES)/..
LOCAL_C_INCLUDES += $(TOP)/$(MTK_MTKCAM_PLATFORM)/include
#

#-----------------------------------------------------------
LOCAL_CFLAGS += $(MTKCAM_CFLAGS)
#
LOCAL_CFLAGS += -DMTKCAM_USE_LEGACY_IMAGEIO="$(MTKCAM_USE_LEGACY_IMAGEIO)"
LOCAL_CFLAGS += -DMTKCAM_USE_LEGACY_JPEGHW="$(MTKCAM_USE_LEGACY_JPEGHW)"

#-----------------------------------------------------------
LOCAL_WHOLE_STATIC_LIBRARIES += libcam.iopipe.postproc
LOCAL_WHOLE_STATIC_LIBRARIES += libcam.iopipe.camio
ifeq "'1'" "$(strip $(MTKCAM_USE_LEGACY_SIMAGER))"
    LOCAL_WHOLE_STATIC_LIBRARIES += libcam.iopipe.simager
endif

ifeq ($(BUILD_MTK_LDVT),true)
#    LOCAL_SHARED_LIBRARIES += libaed
#    LOCAL_WHOLE_STATIC_LIBRARIES += libcam_utils.common
    #LOCAL_WHOLE_STATIC_LIBRARIES += libcam_utils.common.metadata
endif
#
LOCAL_STATIC_LIBRARIES := 

#-----------------------------------------------------------
LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libutils
LOCAL_SHARED_LIBRARIES += libcutils
LOCAL_SHARED_LIBRARIES += libcam_hwutils
#
LOCAL_SHARED_LIBRARIES += libimageio_plat_drv
#
#ifneq ($(BUILD_MTK_LDVT),true)
    LOCAL_SHARED_LIBRARIES += libcam_utils
    LOCAL_SHARED_LIBRARIES += libcam.metadata
#endif
LOCAL_SHARED_LIBRARIES += libcam.halsensor
#
ifeq "'1'" "$(strip $(MTKCAM_USE_LEGACY_IMAGEIO))"
    LOCAL_SHARED_LIBRARIES += libimageio    
#    LOCAL_SHARED_LIBRARIES += libcam.campipe 
endif
#


# for res mgr ??
ifeq "'1'" "$(strip $(MTKCAM_USE_LEGACY_CAMDRV))"
    LOCAL_SHARED_LIBRARIES += libcamdrv
endif
#

# SImager
ifeq "'1'" "$(strip $(MTKCAM_USE_LEGACY_SIMAGER))"
    LOCAL_SHARED_LIBRARIES += libdpframework
endif
#

#
ifeq "'1'" "$(strip $(MTKCAM_USE_LEGACY_JPEGHW))"
    LOCAL_SHARED_LIBRARIES += libJpgEncPipe
endif
#

#-----------------------------------------------------------
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libcam.iopipe

#-----------------------------------------------------------
include $(BUILD_SHARED_LIBRARY)


################################################################################
#
################################################################################
include $(CLEAR_VARS)
include $(call all-makefiles-under,$(LOCAL_PATH))

