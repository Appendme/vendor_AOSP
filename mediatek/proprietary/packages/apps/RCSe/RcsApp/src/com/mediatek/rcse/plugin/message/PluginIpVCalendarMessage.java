/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2012. All rights reserved.
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
package com.mediatek.rcse.plugin.message;

import com.mediatek.rcse.api.Logger;
import com.mediatek.rcse.service.binder.FileStructForBinder;

/**
 * Provide calender message.
 */
public class PluginIpVCalendarMessage extends IpVCalendarMessage {
    /**
     * The Constant TAG.
     */
    private static final String TAG = "PluginIpVcardMessage";
    /**
     * The m progress.
     */
    private long mProgress = 0;
    /**
     * The m status.
     */
    private int mStatus = 0;
    /**
     * The m rcs status.
     */
    private int mRcsStatus = 0;
    /**
     * The m transfer tag.
     */
    private String mTransferTag;
    /**
     * The m file name.
     */
    private String mFileName;
    /**
     * The Constant SIZE_ONE.
     */
    private static final int SIZE_ONE = 1;

    /**
     * Instantiates a new plugin ip v calendar message.
     *
     * @param fileStruct the file struct
     * @param remote the remote
     */
    public PluginIpVCalendarMessage(FileStructForBinder fileStruct,
            String remote) {
        Logger.d(TAG, "PluginIpVCalendarMessage(), fileStruct = " + fileStruct
                + " remote = " + remote);
        setSimId((int) PluginUtils.DUMMY_SIM_ID);
        int size = (int) fileStruct.mFileSize / PluginUtils.SIZE_K;
        if (size == 0) {
            size = SIZE_ONE;
        }
        setSize(size);
        setPath(fileStruct.mFilePath);
        //setName(fileStruct.fileName);
        setType(IpMessageConsts.IpMessageType.CALENDAR);
        setFrom(remote);
        setTo(remote);
        mFileName = fileStruct.mFileName;
        mTransferTag = fileStruct.mFileTransferTag;
    }
    /**
     * Set file progress.
     *
     * @param progress the new progress
     */
    public void setProgress(long progress) {
        Logger.d(TAG, "setProgress(), progress = " + progress);
        mProgress = progress;
    }
    /**
     * get name.
     *
     * @return file name
     */
    public String getName() {
        Logger.d(TAG, "getName()");
        return mFileName;
    }
    /**
     * Get file progress.
     *
     * @return progress
     */
    public int getProgress() {
        Logger.d(TAG, "getProgress()");
        return (int) mProgress;
    }
    /**
     * Set file status.
     *
     * @param status the new status
     */
    public void setStatus(int status) {
        Logger.d(TAG, "SetStatus()");
        mStatus = status;
    }
    /**
     * Get file status.
     *
     * @return status
     */
    public int getStatus() {
        Logger.d(TAG, "getStatus()");
        return mStatus;
    }
    /**
     * Set rcse status.
     *
     * @param status the new rcs status
     */
    public void setRcsStatus(int status) {
        Logger.d(TAG, "setRcsStatus()");
        mRcsStatus = status;
    }
    /**
     * Get rcse status.
     *
     * @return status
     */
    public int getRcsStatus() {
        Logger.d(TAG, "getRcsStatus()");
        return mRcsStatus;
    }
    /**
     * Set file transfer tag.
     *
     * @param tag the new tag
     */
    public void setTag(String tag) {
        Logger.d(TAG, "setTag()");
        mTransferTag = tag;
    }
    /**
     * Get file transfer tag.
     *
     * @return tag
     */
    public String getTag() {
        Logger.d(TAG, "getTag()");
        return mTransferTag;
    }
}
