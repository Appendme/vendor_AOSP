/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
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
package com.mediatek.settingsprovider.plugin;

import android.content.Context;
import android.provider.Settings;
import android.util.Log;

import com.android.ims.ImsConfig;

import com.mediatek.common.PluginImpl;
import com.mediatek.providers.settings.ext.DefaultDatabaseHelperExt;

/**
 * Helps customize certain values in SettingsProvider.
 */

@PluginImpl(interfaceName = "com.mediatek.providers.settings.ext.IDatabaseHelperExt")
public class Op16DatabaseHelperExt extends DefaultDatabaseHelperExt {

    private static final String TAG = "Op16DatabaseHelperExt";
    private Context mContext;
    /**
     * @param context Context
     * constructor
     */
    public Op16DatabaseHelperExt(Context context) {
         super(context);
         mContext = context;
    }

    /**
     * @param context Context
     * @param name String
     * @param defaultValue String
     * @return the value
     * Used in settings provider.
     * From orange requirement, loading the differen default value
     * get the boolean type
     */
    public String getResInteger(Context context, String name, String defaultValue) {
        String res;
        if (Settings.Global.WFC_IMS_ENABLED.equals(name)) {
            res = Integer.toString(ImsConfig.FeatureValueConstants.OFF);
        } else if (Settings.Global.WFC_IMS_MODE.equals(name)) {
            res = Integer.toString(ImsConfig.WfcModeFeatureValueConstants.WIFI_PREFERRED);
        } else {
            res = defaultValue;
        }
        Log.d(TAG, "get name = " + name + " boolean value = " + res);
        return res;
    }

}
