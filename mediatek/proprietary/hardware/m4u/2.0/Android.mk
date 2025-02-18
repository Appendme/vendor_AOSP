# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
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




ifneq (,$(filter $(strip $(TARGET_BOARD_PLATFORM)), mt6580 mt6795 mt6752 mt6735 mt6753 mt6735m mt6755 mt6797 mt8173))

LOCAL_PATH:= $(call my-dir)


include $(CLEAR_VARS)

LOCAL_SRC_FILES:= m4u_lib.cpp
LC_MTK_PLATFORM := $(shell echo $(MTK_PLATFORM) | tr A-Z a-z )

LOCAL_C_INCLUDES:= \
  $(TOP)/$(MTK_PATH_PLATFORM)/kernel/core/include/mach \
  $(TOP)/$(MTK_PATH_SOURCE)/hardware/m4u/$(LC_MTK_PLATFORM) \

#
ifeq ($(strip $(TARGET_BOARD_PLATFORM)),mt6735)
LOCAL_C_INCLUDES += $(TOP)/vendor/mediatek/proprietary/hardware/m4u/mt6735
LOCAL_EXPORT_C_INCLUDE_DIRS:= $(TOP)/vendor/mediatek/proprietary/hardware/m4u/mt6735
LOCAL_SRC_FILES += ../mt6735/D1/m4u_lib_port.cpp
endif

ifeq ($(strip $(TARGET_BOARD_PLATFORM)),mt6735m)
LOCAL_C_INCLUDES += $(TOP)/vendor/mediatek/proprietary/hardware/m4u/mt6735
LOCAL_EXPORT_C_INCLUDE_DIRS:= $(TOP)/vendor/mediatek/proprietary/hardware/m4u/mt6735
LOCAL_SRC_FILES += ../mt6735/D2/m4u_lib_port.cpp
endif

ifeq ($(strip $(TARGET_BOARD_PLATFORM)),mt6753)
LOCAL_C_INCLUDES += $(TOP)/vendor/mediatek/proprietary/hardware/m4u/mt6735
LOCAL_EXPORT_C_INCLUDE_DIRS:= $(TOP)/vendor/mediatek/proprietary/hardware/m4u/mt6735
LOCAL_SRC_FILES += ../mt6735/D3/m4u_lib_port.cpp
endif

LOCAL_SHARED_LIBRARIES := \
     libcutils \
     liblog \
 
 
LOCAL_MODULE := libm4u
LOCAL_MULTILIB := both

LOCAL_MODULE_TAGS := eng

#LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
# include $(call all-makefiles-under,$(LOCAL_PATH))

endif


