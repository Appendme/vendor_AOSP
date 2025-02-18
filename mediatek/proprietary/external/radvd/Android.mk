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


ifneq ($(MTK_EMULATOR_SUPPORT),true)

LOCAL_PATH := $(call my-dir)
etc_dir := $(TARGET_OUT)/etc/radvd

prebuilt_etc_files = radvd.conf

#
# radvd daemon
#
include $(CLEAR_VARS)
LOCAL_SRC_FILES := radvd.c device-common.c device-linux.c gram.c interface.c log.c \
	netlink.c privsep-linux.c process.c recv.c scanner.c send.c \
	socket.c timer.c util.c
# LOCAL_CFLAGS := $(cflags)
LOCAL_SHARED_LIBRARIES := libc libcutils
#bobule workaround to fix worng kernel header path, needing review
#LOCAL_C_INCLUDES := $(KERNEL_HEADERS)
#LOCAL_C_INCLUDES += $(TARGET_C_INCLUDES)
LOCAL_MODULE = radvd
include $(BUILD_EXECUTABLE)

#
# Configuration Files
#
$(foreach t, $(prebuilt_etc_files), \
  $(eval include $(CLEAR_VARS)) \
  $(eval LOCAL_SRC_FILES = $(t)) \
  $(eval LOCAL_MODULE = $(t)) \
  $(eval LOCAL_MODULE_PATH := $(etc_dir)) \
  $(eval LOCAL_MODULE_CLASS := ETC) \
  $(eval include $(BUILD_PREBUILT)) \
)  

endif
