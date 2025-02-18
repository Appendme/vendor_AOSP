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
 * MediaTek Inc. (C) 2015. All rights reserved.
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
package com.mediatek.camera.v2.setting;

import android.content.Context;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraMetadata;
import android.hardware.camera2.CaptureRequest.Key;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.media.CamcorderProfile;
import android.util.FloatMath;
import android.util.Log;
import android.util.Range;
import android.util.Rational;
import android.util.Size;
import android.view.SurfaceHolder;

import com.mediatek.camcorder.CamcorderProfileEx;
import com.mediatek.camera.v2.util.SettingKeys;
import com.mediatek.camera.v2.util.Utils;
import com.mediatek.camera.v2.vendortag.TagMetadata;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class SettingCharacteristics {
    private static final String TAG = "SettingCharacteristics";
    private Map<String, List<String>> mSupportedValuesMap = new HashMap<String, List<String>>();
    private List<Size> mSupportedPreviewSize;
    private CameraCharacteristics mCameraCharacteristics;
    private Context mContext;
    private String mCameraId;

    private static final String VIDEO_QUALITY_LOW = Integer.
            toString(CamcorderProfileEx.QUALITY_LOW);
    private static final String VIDEO_QUALITY_MEDIUM = Integer
            .toString(CamcorderProfileEx.QUALITY_MEDIUM);
    private static final String VIDEO_QUALITY_HIGH = Integer
            .toString(CamcorderProfileEx.QUALITY_HIGH);
    private static final String VIDEO_QUALITY_FINE = Integer
            .toString(CamcorderProfileEx.QUALITY_FINE);
    public static final String VIDEO_QUALITY_FINE_4K2K = Integer
            .toString(CamcorderProfileEx.QUALITY_FINE_4K2K);

    public SettingCharacteristics(CameraCharacteristics characteristics, String cameraId,
            Context context) {
        mCameraCharacteristics = characteristics;
        mCameraId = cameraId;
        mContext = context;
    }

    public List<String> getSupportedValues(String key) {
        int settingIndex = SettingKeys.getSettingId(key);
        List<String> supportedValues = null;
        int[] availableMode = null;
        switch(settingIndex) {
        case SettingKeys.ROW_SETTING_COLOR_EFFECT:
            availableMode = getValueFromKey(
                    CameraCharacteristics.CONTROL_AVAILABLE_EFFECTS);
            break;

        case SettingKeys.ROW_SETTING_WHITE_BALANCE:
            availableMode = getValueFromKey(
                    CameraCharacteristics.CONTROL_AWB_AVAILABLE_MODES);
            break;

        case SettingKeys.ROW_SETTING_ANTI_FLICKER:
            availableMode = getValueFromKey(
                    CameraCharacteristics.CONTROL_AE_AVAILABLE_ANTIBANDING_MODES);
            break;

        case SettingKeys.ROW_SETTING_GESTURE_SHOT:
            availableMode = getValueFromKey(TagMetadata.GESTURE_AVAILABLE_MODES);
            break;

        case SettingKeys.ROW_SETTING_SMILE_SHOT:
            availableMode = getValueFromKey(TagMetadata.SMILE_AVAILABLE_MODES);
            break;

        case SettingKeys.ROW_SETTING_ASD:
            availableMode = getValueFromKey(TagMetadata.ASD_AVAILABLE_MODES);
            break;

        case SettingKeys.ROW_SETTING_3DNR:
            availableMode = getValueFromKey(TagMetadata.NR3D_AVAILABLE_MODES);
            break;

        case SettingKeys.ROW_SETTING_ISO:
            Range<Integer> isoRange = getValueFromKey(
                    CameraCharacteristics.SENSOR_INFO_SENSITIVITY_RANGE);
            supportedValues = getSupportedIsoValues(isoRange);
            break;

        case SettingKeys.ROW_SETTING_EXPOSURE:
            Range<Integer> ecRange = getValueFromKey(
                    CameraCharacteristics.CONTROL_AE_COMPENSATION_RANGE);
            Rational ecStep = getValueFromKey(
                    CameraCharacteristics.CONTROL_AE_COMPENSATION_STEP);
            float exposureCompensationStep = (float) ecStep.getNumerator()
                    / ecStep.getDenominator();
            supportedValues = getSupportedExposureCompensation(ecRange,
                    exposureCompensationStep);
            break;

        case SettingKeys.ROW_SETTING_SCENCE_MODE:
            int[] scenes = getValueFromKey(
                    CameraCharacteristics.CONTROL_AVAILABLE_SCENE_MODES);
            supportedValues = getSupportedSceneMode(scenes);
            break;

        case SettingKeys.ROW_SETTING_PICTURE_RATIO:
            supportedValues = getSupportedPictureRatio();
            break;

        case SettingKeys.ROW_SETTING_PICTURE_SIZE:
            StreamConfigurationMap s = getValueFromKey(
                    CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
            supportedValues = getSupportedPictureSize(s, ImageFormat.JPEG);
            break;

        case SettingKeys.ROW_SETTING_CAMERA_FACE_DETECT:
            int faceCount = getValueFromKey(
                    CameraCharacteristics.STATISTICS_INFO_MAX_FACE_COUNT);
            Log.d(TAG, "faceCount:" + faceCount);
            if (faceCount > 0) {
                supportedValues = new ArrayList<String>(2);
                supportedValues.add("off");
                supportedValues.add("on");
            }
            break;

        case SettingKeys.ROW_SETTING_FLASH:
            Boolean flashInfo = getValueFromKey(
                    CameraCharacteristics.FLASH_INFO_AVAILABLE);
            if (flashInfo) {
                supportedValues = new ArrayList<String>(3);
                supportedValues.add("auto");
                supportedValues.add("on");
                supportedValues.add("off");
            }
            break;

        case SettingKeys.ROW_SETTING_VIDEO_QUALITY:
            supportedValues = getSupportedVideoQuality();
            break;

        case SettingKeys.ROW_SETTING_HDR:
            int[] sceneModes = getValueFromKey(
                    CameraCharacteristics.CONTROL_AVAILABLE_SCENE_MODES);
            if (sceneModes != null) {
                for (int scene : sceneModes) {
                    if (scene == CameraMetadata.CONTROL_SCENE_MODE_HDR) {
                        supportedValues = new ArrayList<String>(2);
                        supportedValues.add("off");
                        supportedValues.add("on");
                    }
                }
            }
            break;

        case SettingKeys.ROW_SETTING_DNG:
            if (isDngSupported()) {
                supportedValues = new ArrayList<String>(2);
                supportedValues.add("on");
                supportedValues.add("off");
            }
            break;

        case SettingKeys.ROW_SETTING_FACEBEAUTY_SMOOTH:
        case SettingKeys.ROW_SETTING_FACEBEAUTY_SKIN_COLOR:
            supportedValues = new ArrayList<String>(3);
            supportedValues.add("-4");
            supportedValues.add("0");
            supportedValues.add("4");
            break;

        case SettingKeys.ROW_SETTING_VIDEO_STABLE:
        case SettingKeys.ROW_SETTING_FACE_BEAUTY:
        case SettingKeys.ROW_SETTING_PHOTO_PIP:
            supportedValues = new ArrayList<String>(2);
            supportedValues.add("off");
            supportedValues.add("on");
            break;

        default:
            break;
        }

        if (availableMode != null) {
            String values[] = SettingConvertor.convertModeEnumToString(key, availableMode);
            supportedValues = new ArrayList<String>();
            for (String value : values) {
                supportedValues.add(value);
            }
        }
        Log.i(TAG, "camera:" + mCameraId + ", key:" + key + ", " +
                "supportedValues:" + supportedValues + "availableMode:" + availableMode);
        return supportedValues;
    }

    public List<Size> getSupportedPreviewSize() {
        if (mSupportedPreviewSize != null) {
            return mSupportedPreviewSize;
        }

        StreamConfigurationMap s = mCameraCharacteristics.get(
                CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
        Size[] sizes = s.getOutputSizes(SurfaceHolder.class);
        mSupportedPreviewSize = new ArrayList<Size>(sizes.length);
        for (Size size : sizes) {
            mSupportedPreviewSize.add(size);
        }

        return mSupportedPreviewSize;
    }

    private List<String> getSupportedVideoQuality() {
        ArrayList<String> supported = new ArrayList<String>();
        int cameraId = Integer.parseInt(mCameraId);
        if (CamcorderProfile.hasProfile(cameraId, CamcorderProfileEx.QUALITY_LOW)) {
            supported.add(VIDEO_QUALITY_LOW);
        }
        if (CamcorderProfile.hasProfile(cameraId, CamcorderProfileEx.QUALITY_MEDIUM)) {
            supported.add(VIDEO_QUALITY_MEDIUM);
        }
        if (CamcorderProfile.hasProfile(cameraId, CamcorderProfileEx.QUALITY_HIGH)) {
            supported.add(VIDEO_QUALITY_HIGH);
        }
        if (CamcorderProfile.hasProfile(cameraId, CamcorderProfileEx.QUALITY_FINE)) {
            supported.add(VIDEO_QUALITY_FINE);
        }
        if (CamcorderProfile.hasProfile(cameraId, CamcorderProfileEx.QUALITY_FINE_4K2K)) {
            supported.add(VIDEO_QUALITY_FINE_4K2K);
        }
        return supported;
    }

    private List<String> getSupportedPictureRatio() {
        List<String> pictureRatios = new ArrayList<String>();
        StreamConfigurationMap s = mCameraCharacteristics.get(
                CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
        Size[] sizes = s.getOutputSizes(SurfaceHolder.class);

        double fullRatio = Utils.findFullscreenRatio(mContext);
        if (mSupportedPreviewSize == null) {
            mSupportedPreviewSize = new ArrayList<Size>(sizes.length);
        }
        for (Size size : sizes) {
            mSupportedPreviewSize.add(size);
        }

        Size fullSize = Utils.getOptimalPreviewSize(mContext, mSupportedPreviewSize, fullRatio);

        // if fullSize is not null, it means that this chip support full ratio preview size.
        if (fullSize != null) {
            if (fullRatio != 1.3333) {
                pictureRatios.add(String.valueOf(fullRatio));
            }
        }

        pictureRatios.add("1.3333");

        return pictureRatios;
    }

    private List<String> getSupportedPictureSize(StreamConfigurationMap s, int format) {
        if (s == null) {
            return null;
        }
        Size[] sizes = s.getOutputSizes(format);
        List<String> supportedValues = new ArrayList<String>(sizes.length);
        for (Size size : sizes) {
            int width = size.getWidth();
            int height = size.getHeight();
            String sizeString = String.valueOf(width) + "x" + String.valueOf(height);
            supportedValues.add(sizeString);
        }
        return supportedValues;
    }

    private List<String> getSupportedIsoValues(Range<Integer> range) {
        List<String> supportedValues = new ArrayList<String>();
        if (range != null) {
            int minIso = range.getLower();
            int maxIso = range.getUpper();
            supportedValues.add("auto");
            int n = 0;
            for (int isoValue = minIso; isoValue < maxIso; n++) {
                isoValue = (int) (Math.pow(2, n)) * 100;
                supportedValues.add(String.valueOf(isoValue));
            }
            Log.i(TAG, "minIso:" + minIso + ", maxIso:" + maxIso);
        }
        return supportedValues;
    }

    private List<String> getSupportedExposureCompensation(Range<Integer> range, float step) {
        int minExposureCompensation = range.getLower();
        int maxExposureCompensation = range.getUpper();
        Log.d(TAG, "minExposureCompensation:" + minExposureCompensation + ", " +
                "maxExposureCompensation:" + maxExposureCompensation + ", " +
                "exposureCompensationStep:" + step);
        int maxValue = (int) FloatMath.floor(maxExposureCompensation * step);
        int minValue = (int) FloatMath.ceil(minExposureCompensation * step);
        ArrayList<String> supportedValues = new ArrayList<String>();
        Log.d(TAG, "maxValue:" + maxValue + ", minValue:" + minValue);
        for (int i = minValue; i <= maxValue; ++i) {
            supportedValues.add(String.valueOf(i));
        }
        Log.d(TAG, "supportedValues:" + supportedValues);
        return supportedValues;
    }

    private List<String> getSupportedSceneMode(int[] scenes) {
        // If no scene modes are supported by the camera device, only DISABLED list
        // in scenes. Otherwise DISABLED will not be listed.
        if (scenes == null
                || (scenes.length == 1
                    && scenes[0] == CameraMetadata.CONTROL_SCENE_MODE_DISABLED)) {
            return null;
        } else {
            List<String> supportedValues = new ArrayList<String>(scenes.length);
            String values[] = SettingConvertor.convertModeEnumToString(
                    SettingKeys.KEY_SCENE_MODE, scenes);
            for (String value : values) {
                supportedValues.add(value);
            }
            if (!supportedValues.contains("auto")) {
                supportedValues.add("auto");
            }
            return supportedValues;
        }
    }

    private List<Integer> getAvailableCapablities() {
        CameraCharacteristics.Key<int[]> key =
                CameraCharacteristics.REQUEST_AVAILABLE_CAPABILITIES;
        int[] availableCaps = getValueFromKey(key);
        if (availableCaps == null) {
            Log.i(TAG, "The camera " + mCameraId + " available capabilities is null");
            return new ArrayList<Integer>();
        }

        List<Integer> capList = new ArrayList<Integer>(availableCaps.length);
        String capString = "";
        for (int cap : availableCaps) {
            capList.add(cap);
            capString += (cap + ", ");
        }
        Log.i(TAG, "The camera " + mCameraId + " available capabilities are:" + capString);
        return capList;
    }

    private <T> T getValueFromKey(CameraCharacteristics.Key<T> key) {
        T value = null;
        try {
            value = mCameraCharacteristics.get(key);
            if (value == null) {
                Log.e(TAG, key.getName() + "was null");
            }
        } catch (IllegalArgumentException e) {
            Log.e(TAG, key.getName() + " was not supported by this device");
        }

        return value;
    }

    private boolean isDngSupported() {
        List<Integer> capList = getAvailableCapablities();
        if (!capList.contains(CameraCharacteristics.REQUEST_AVAILABLE_CAPABILITIES_RAW)) {
            Log.e(TAG, "RAW capablity do not support in camera " + mCameraId);
            return false;
        }

        StreamConfigurationMap config = mCameraCharacteristics.get(
                CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
        Size[] rawSizes = config.getOutputSizes(ImageFormat.RAW_SENSOR);
        if (rawSizes == null) {
            Log.e(TAG, "No capture sizes available for raw format");
            return false;
        }
        for (int i = 0; i < rawSizes.length; i++) {
            Log.d(TAG, "raw supported size:" + rawSizes[i]);
        }

        Rect activeArray = getValueFromKey(
                CameraCharacteristics.SENSOR_INFO_ACTIVE_ARRAY_SIZE);
        if (activeArray == null) {
            Log.e(TAG, "Active array is null");
            return false;
        } else {
            Log.d(TAG, "Active array is:" + activeArray);
            Size activeArraySize = new Size(activeArray.width(), activeArray.height());
            boolean contain = false;
            for (Size size : rawSizes) {
                if (size.getWidth() == activeArraySize.getWidth()
                        && size.getHeight() == activeArraySize.getHeight()) {
                    contain = true;
                    break;
                }
            }
            if (!contain) {
                Log.e(TAG, "Aavailable sizes for RAW format do not include active array size");
                return false;
            }
        }
        return true;
    }
}
