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

package com.mediatek.mediatekdm.test.server;

import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.mediatek.mediatekdm.test.server.MockServerService.CMCCTestRequest;

public class ServerBinderBuilder {
    private static final String TAG = "MDMTest/ServerBinderBuilder";

    public static IMockServer.Stub buildServerBinder(Intent intent, Context context) {
        String action = intent.getAction();
        Log.d(TAG, "buildServerBinder: " + action);
        IMockServer.Stub result = null;
        if (action.equals(CMCCTestRequest.PAIRING_INFO_RETRIEVAL)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.DM_ACCOUNT_RETRIEVAL)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.PARAMETERS_RETRIEVAL)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.PARAMETERS_RETRIEVAL_RETRY)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.PARAMETERS_PROVISION)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.PARAMETERS_PROVISION_RETRY)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.FIRMWARE_UPDATE)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.FIRMWARE_QUERY)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.SOFTWARE_INSTALL)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.SOFTWARE_LIST_RETRIEVAL)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.LOCK_PHONE)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.UNLOCK_PHONE)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.WIPE_PHONE)) {
            result = buildTemplateDriverBinder(intent, context);
        } else if (action.equals(CMCCTestRequest.PROBE_PHONE)) {
            result = buildTemplateDriverBinder(intent, context);
        }
        return result;
    }

    private static IMockServer.Stub buildTemplateDriverBinder(Intent intent, Context context) {
        int port = intent.getIntExtra(MockServerService.KEY_PORT, MockServerService.DEFAULT_PORT);
        TemplateDrivenBinder result = new TemplateDrivenBinder(port, context);
        result.put(TemplateDrivenBinder.KEY_TYPE, intent.getAction());
        return result;
    }
}
