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

#ifndef _TAGMAP_H_
#define _TAGMAP_H_

/******************************************************************************
 *
 ******************************************************************************/
#include <metadata/client/mtk_metadata_tag.h>
#include <system/camera_metadata.h>

/******************************************************************************
 *
 ******************************************************************************/


 #define ADD_ALL_MEMBERS \
     _IMP_TAGCONVERT_(    ANDROID_COLOR_CORRECTION_MODE  ,    MTK_COLOR_CORRECTION_MODE  )\
     _IMP_TAGCONVERT_(    ANDROID_COLOR_CORRECTION_TRANSFORM,    MTK_COLOR_CORRECTION_TRANSFORM)\
     _IMP_TAGCONVERT_(    ANDROID_COLOR_CORRECTION_GAINS ,    MTK_COLOR_CORRECTION_GAINS )\
     _IMP_TAGCONVERT_(    ANDROID_COLOR_CORRECTION_ABERRATION_MODE,    MTK_COLOR_CORRECTION_ABERRATION_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_COLOR_CORRECTION_AVAILABLE_ABERRATION_MODES,    MTK_COLOR_CORRECTION_AVAILABLE_ABERRATION_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_ANTIBANDING_MODE ,    MTK_CONTROL_AE_ANTIBANDING_MODE )\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_EXPOSURE_COMPENSATION,    MTK_CONTROL_AE_EXPOSURE_COMPENSATION)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_LOCK,    MTK_CONTROL_AE_LOCK)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_MODE,    MTK_CONTROL_AE_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_REGIONS,    MTK_CONTROL_AE_REGIONS)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_TARGET_FPS_RANGE,    MTK_CONTROL_AE_TARGET_FPS_RANGE)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_PRECAPTURE_TRIGGER,    MTK_CONTROL_AE_PRECAPTURE_TRIGGER)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AF_MODE,    MTK_CONTROL_AF_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AF_REGIONS,    MTK_CONTROL_AF_REGIONS)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AF_TRIGGER,    MTK_CONTROL_AF_TRIGGER)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AWB_LOCK,    MTK_CONTROL_AWB_LOCK)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AWB_MODE,    MTK_CONTROL_AWB_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AWB_REGIONS,    MTK_CONTROL_AWB_REGIONS)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_CAPTURE_INTENT,    MTK_CONTROL_CAPTURE_INTENT)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_EFFECT_MODE,    MTK_CONTROL_EFFECT_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_MODE,    MTK_CONTROL_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_SCENE_MODE,    MTK_CONTROL_SCENE_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_VIDEO_STABILIZATION_MODE,    MTK_CONTROL_VIDEO_STABILIZATION_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_AVAILABLE_ANTIBANDING_MODES,    MTK_CONTROL_AE_AVAILABLE_ANTIBANDING_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_AVAILABLE_MODES,    MTK_CONTROL_AE_AVAILABLE_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES,    MTK_CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_COMPENSATION_RANGE,    MTK_CONTROL_AE_COMPENSATION_RANGE)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_COMPENSATION_STEP,    MTK_CONTROL_AE_COMPENSATION_STEP)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AF_AVAILABLE_MODES,    MTK_CONTROL_AF_AVAILABLE_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AVAILABLE_EFFECTS,    MTK_CONTROL_AVAILABLE_EFFECTS)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AVAILABLE_SCENE_MODES,    MTK_CONTROL_AVAILABLE_SCENE_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES,    MTK_CONTROL_AVAILABLE_VIDEO_STABILIZATION_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AWB_AVAILABLE_MODES,    MTK_CONTROL_AWB_AVAILABLE_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_MAX_REGIONS,    MTK_CONTROL_MAX_REGIONS)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_SCENE_MODE_OVERRIDES,    MTK_CONTROL_SCENE_MODE_OVERRIDES)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_PRECAPTURE_ID,    MTK_CONTROL_AE_PRECAPTURE_ID)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AE_STATE,    MTK_CONTROL_AE_STATE)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AF_STATE,    MTK_CONTROL_AF_STATE)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AF_TRIGGER_ID,    MTK_CONTROL_AF_TRIGGER_ID)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AWB_STATE,    MTK_CONTROL_AWB_STATE)\
     _IMP_TAGCONVERT_(    ANDROID_CONTROL_AVAILABLE_HIGH_SPEED_VIDEO_CONFIGURATIONS,    MTK_CONTROL_AVAILABLE_HIGH_SPEED_VIDEO_CONFIGURATIONS)\
     _IMP_TAGCONVERT_(    ANDROID_DEMOSAIC_MODE          ,    MTK_DEMOSAIC_MODE          )\
     _IMP_TAGCONVERT_(    ANDROID_EDGE_MODE              ,    MTK_EDGE_MODE              )\
     _IMP_TAGCONVERT_(    ANDROID_EDGE_STRENGTH,    MTK_EDGE_STRENGTH)\
     _IMP_TAGCONVERT_(    ANDROID_EDGE_AVAILABLE_EDGE_MODES,    MTK_EDGE_AVAILABLE_EDGE_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_FLASH_FIRING_POWER     ,    MTK_FLASH_FIRING_POWER     )\
     _IMP_TAGCONVERT_(    ANDROID_FLASH_FIRING_TIME,    MTK_FLASH_FIRING_TIME)\
     _IMP_TAGCONVERT_(    ANDROID_FLASH_MODE,    MTK_FLASH_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_FLASH_COLOR_TEMPERATURE,    MTK_FLASH_COLOR_TEMPERATURE)\
     _IMP_TAGCONVERT_(    ANDROID_FLASH_MAX_ENERGY,    MTK_FLASH_MAX_ENERGY)\
     _IMP_TAGCONVERT_(    ANDROID_FLASH_STATE,    MTK_FLASH_STATE)\
     _IMP_TAGCONVERT_(    ANDROID_FLASH_INFO_AVAILABLE   ,    MTK_FLASH_INFO_AVAILABLE   )\
     _IMP_TAGCONVERT_(    ANDROID_FLASH_INFO_CHARGE_DURATION,    MTK_FLASH_INFO_CHARGE_DURATION)\
     _IMP_TAGCONVERT_(    ANDROID_HOT_PIXEL_MODE         ,    MTK_HOT_PIXEL_MODE         )\
     _IMP_TAGCONVERT_(    ANDROID_HOT_PIXEL_AVAILABLE_HOT_PIXEL_MODES,    MTK_HOT_PIXEL_AVAILABLE_HOT_PIXEL_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_JPEG_GPS_COORDINATES   ,    MTK_JPEG_GPS_COORDINATES   )\
     _IMP_TAGCONVERT_(    ANDROID_JPEG_GPS_PROCESSING_METHOD,    MTK_JPEG_GPS_PROCESSING_METHOD)\
     _IMP_TAGCONVERT_(    ANDROID_JPEG_GPS_TIMESTAMP,    MTK_JPEG_GPS_TIMESTAMP)\
     _IMP_TAGCONVERT_(    ANDROID_JPEG_ORIENTATION,    MTK_JPEG_ORIENTATION)\
     _IMP_TAGCONVERT_(    ANDROID_JPEG_QUALITY,    MTK_JPEG_QUALITY)\
     _IMP_TAGCONVERT_(    ANDROID_JPEG_THUMBNAIL_QUALITY,    MTK_JPEG_THUMBNAIL_QUALITY)\
     _IMP_TAGCONVERT_(    ANDROID_JPEG_THUMBNAIL_SIZE,    MTK_JPEG_THUMBNAIL_SIZE)\
     _IMP_TAGCONVERT_(    ANDROID_JPEG_AVAILABLE_THUMBNAIL_SIZES,    MTK_JPEG_AVAILABLE_THUMBNAIL_SIZES)\
     _IMP_TAGCONVERT_(    ANDROID_JPEG_MAX_SIZE,    MTK_JPEG_MAX_SIZE)\
     _IMP_TAGCONVERT_(    ANDROID_JPEG_SIZE,    MTK_JPEG_SIZE)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_APERTURE          ,    MTK_LENS_APERTURE          )\
     _IMP_TAGCONVERT_(    ANDROID_LENS_FILTER_DENSITY,    MTK_LENS_FILTER_DENSITY)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_FOCAL_LENGTH,    MTK_LENS_FOCAL_LENGTH)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_FOCUS_DISTANCE,    MTK_LENS_FOCUS_DISTANCE)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_OPTICAL_STABILIZATION_MODE,    MTK_LENS_OPTICAL_STABILIZATION_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_FACING,    MTK_SENSOR_INFO_FACING)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_FOCUS_RANGE,    MTK_LENS_FOCUS_RANGE)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_STATE,    MTK_LENS_STATE)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_INFO_AVAILABLE_APERTURES,    MTK_LENS_INFO_AVAILABLE_APERTURES)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_INFO_AVAILABLE_FILTER_DENSITIES,    MTK_LENS_INFO_AVAILABLE_FILTER_DENSITIES)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_INFO_AVAILABLE_FOCAL_LENGTHS,    MTK_LENS_INFO_AVAILABLE_FOCAL_LENGTHS)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_INFO_AVAILABLE_OPTICAL_STABILIZATION,    MTK_LENS_INFO_AVAILABLE_OPTICAL_STABILIZATION)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_INFO_HYPERFOCAL_DISTANCE,    MTK_LENS_INFO_HYPERFOCAL_DISTANCE)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_INFO_MINIMUM_FOCUS_DISTANCE,    MTK_LENS_INFO_MINIMUM_FOCUS_DISTANCE)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_INFO_SHADING_MAP_SIZE,    MTK_LENS_INFO_SHADING_MAP_SIZE)\
     _IMP_TAGCONVERT_(    ANDROID_LENS_INFO_FOCUS_DISTANCE_CALIBRATION,    MTK_LENS_INFO_FOCUS_DISTANCE_CALIBRATION)\
     _IMP_TAGCONVERT_(    ANDROID_NOISE_REDUCTION_MODE   ,    MTK_NOISE_REDUCTION_MODE   )\
     _IMP_TAGCONVERT_(    ANDROID_NOISE_REDUCTION_STRENGTH,    MTK_NOISE_REDUCTION_STRENGTH)\
     _IMP_TAGCONVERT_(    ANDROID_NOISE_REDUCTION_AVAILABLE_NOISE_REDUCTION_MODES,    MTK_NOISE_REDUCTION_AVAILABLE_NOISE_REDUCTION_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_QUIRKS_METERING_CROP_REGION,    MTK_QUIRKS_METERING_CROP_REGION)\
     _IMP_TAGCONVERT_(    ANDROID_QUIRKS_TRIGGER_AF_WITH_AUTO,    MTK_QUIRKS_TRIGGER_AF_WITH_AUTO)\
     _IMP_TAGCONVERT_(    ANDROID_QUIRKS_USE_ZSL_FORMAT,    MTK_QUIRKS_USE_ZSL_FORMAT)\
     _IMP_TAGCONVERT_(    ANDROID_QUIRKS_USE_PARTIAL_RESULT,    MTK_QUIRKS_USE_PARTIAL_RESULT)\
     _IMP_TAGCONVERT_(    ANDROID_QUIRKS_PARTIAL_RESULT,    MTK_QUIRKS_PARTIAL_RESULT)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_FRAME_COUNT    ,    MTK_REQUEST_FRAME_COUNT    )\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_ID,    MTK_REQUEST_ID)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_INPUT_STREAMS,    MTK_REQUEST_INPUT_STREAMS)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_METADATA_MODE,    MTK_REQUEST_METADATA_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_OUTPUT_STREAMS,    MTK_REQUEST_OUTPUT_STREAMS)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_TYPE,    MTK_REQUEST_TYPE)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_MAX_NUM_OUTPUT_STREAMS,    MTK_REQUEST_MAX_NUM_OUTPUT_STREAMS)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_MAX_NUM_REPROCESS_STREAMS,    MTK_REQUEST_MAX_NUM_REPROCESS_STREAMS)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_MAX_NUM_INPUT_STREAMS,    MTK_REQUEST_MAX_NUM_INPUT_STREAMS)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_PIPELINE_DEPTH,    MTK_REQUEST_PIPELINE_DEPTH)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_PIPELINE_MAX_DEPTH,    MTK_REQUEST_PIPELINE_MAX_DEPTH)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_PARTIAL_RESULT_COUNT,    MTK_REQUEST_PARTIAL_RESULT_COUNT)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_AVAILABLE_CAPABILITIES,    MTK_REQUEST_AVAILABLE_CAPABILITIES)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_AVAILABLE_REQUEST_KEYS,    MTK_REQUEST_AVAILABLE_REQUEST_KEYS)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_AVAILABLE_RESULT_KEYS,    MTK_REQUEST_AVAILABLE_RESULT_KEYS)\
     _IMP_TAGCONVERT_(    ANDROID_REQUEST_AVAILABLE_CHARACTERISTICS_KEYS,    MTK_REQUEST_AVAILABLE_CHARACTERISTICS_KEYS)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_CROP_REGION     ,    MTK_SCALER_CROP_REGION     )\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_AVAILABLE_FORMATS,    MTK_SCALER_AVAILABLE_FORMATS)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_AVAILABLE_JPEG_MIN_DURATIONS,    MTK_SCALER_AVAILABLE_JPEG_MIN_DURATIONS)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_AVAILABLE_JPEG_SIZES,    MTK_SCALER_AVAILABLE_JPEG_SIZES)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_AVAILABLE_MAX_DIGITAL_ZOOM,    MTK_SCALER_AVAILABLE_MAX_DIGITAL_ZOOM)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_AVAILABLE_PROCESSED_MIN_DURATIONS,    MTK_SCALER_AVAILABLE_PROCESSED_MIN_DURATIONS)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_AVAILABLE_PROCESSED_SIZES,    MTK_SCALER_AVAILABLE_PROCESSED_SIZES)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_AVAILABLE_RAW_MIN_DURATIONS,    MTK_SCALER_AVAILABLE_RAW_MIN_DURATIONS)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_AVAILABLE_RAW_SIZES,    MTK_SCALER_AVAILABLE_RAW_SIZES)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_AVAILABLE_INPUT_OUTPUT_FORMATS_MAP,    MTK_SCALER_AVAILABLE_INPUT_OUTPUT_FORMATS_MAP)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_AVAILABLE_STREAM_CONFIGURATIONS,    MTK_SCALER_AVAILABLE_STREAM_CONFIGURATIONS)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_AVAILABLE_MIN_FRAME_DURATIONS,    MTK_SCALER_AVAILABLE_MIN_FRAME_DURATIONS)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_AVAILABLE_STALL_DURATIONS,    MTK_SCALER_AVAILABLE_STALL_DURATIONS)\
     _IMP_TAGCONVERT_(    ANDROID_SCALER_CROPPING_TYPE,    MTK_SCALER_CROPPING_TYPE)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_EXPOSURE_TIME   ,    MTK_SENSOR_EXPOSURE_TIME   )\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_FRAME_DURATION,    MTK_SENSOR_FRAME_DURATION)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_SENSITIVITY,    MTK_SENSOR_SENSITIVITY)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_REFERENCE_ILLUMINANT1,    MTK_SENSOR_REFERENCE_ILLUMINANT1)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_REFERENCE_ILLUMINANT2,    MTK_SENSOR_REFERENCE_ILLUMINANT2)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_CALIBRATION_TRANSFORM1,    MTK_SENSOR_CALIBRATION_TRANSFORM1)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_CALIBRATION_TRANSFORM2,    MTK_SENSOR_CALIBRATION_TRANSFORM2)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_COLOR_TRANSFORM1,    MTK_SENSOR_COLOR_TRANSFORM1)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_COLOR_TRANSFORM2,    MTK_SENSOR_COLOR_TRANSFORM2)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_FORWARD_MATRIX1,    MTK_SENSOR_FORWARD_MATRIX1)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_FORWARD_MATRIX2,    MTK_SENSOR_FORWARD_MATRIX2)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_BASE_GAIN_FACTOR,    MTK_SENSOR_BASE_GAIN_FACTOR)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_BLACK_LEVEL_PATTERN,    MTK_SENSOR_BLACK_LEVEL_PATTERN)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_MAX_ANALOG_SENSITIVITY,    MTK_SENSOR_MAX_ANALOG_SENSITIVITY)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_ORIENTATION,    MTK_SENSOR_INFO_ORIENTATION)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_PROFILE_HUE_SAT_MAP_DIMENSIONS,    MTK_SENSOR_PROFILE_HUE_SAT_MAP_DIMENSIONS)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_TIMESTAMP,    MTK_SENSOR_TIMESTAMP)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_TEMPERATURE ,    MTK_SENSOR_TEMPERATURE )\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_NEUTRAL_COLOR_POINT ,    MTK_SENSOR_NEUTRAL_COLOR_POINT )\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_NOISE_PROFILE ,    MTK_SENSOR_NOISE_PROFILE )\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_PROFILE_HUE_SAT_MAP ,    MTK_SENSOR_PROFILE_HUE_SAT_MAP )\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_PROFILE_TONE_CURVE,    MTK_SENSOR_PROFILE_TONE_CURVE )\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_GREEN_SPLIT ,    MTK_SENSOR_GREEN_SPLIT)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_TEST_PATTERN_DATA ,     MTK_SENSOR_TEST_PATTERN_DATA)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_TEST_PATTERN_MODE ,    MTK_SENSOR_TEST_PATTERN_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_AVAILABLE_TEST_PATTERN_MODES ,    MTK_SENSOR_AVAILABLE_TEST_PATTERN_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_ROLLING_SHUTTER_SKEW ,    MTK_SENSOR_ROLLING_SHUTTER_SKEW)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_INFO_ACTIVE_ARRAY_SIZE,    MTK_SENSOR_INFO_ACTIVE_ARRAY_REGION)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_INFO_SENSITIVITY_RANGE ,    MTK_SENSOR_INFO_SENSITIVITY_RANGE )\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT,    MTK_SENSOR_INFO_COLOR_FILTER_ARRANGEMENT)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_INFO_EXPOSURE_TIME_RANGE,    MTK_SENSOR_INFO_EXPOSURE_TIME_RANGE)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_INFO_MAX_FRAME_DURATION,    MTK_SENSOR_INFO_MAX_FRAME_DURATION)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_INFO_PHYSICAL_SIZE,    MTK_SENSOR_INFO_PHYSICAL_SIZE)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_INFO_PIXEL_ARRAY_SIZE,    MTK_SENSOR_INFO_PIXEL_ARRAY_SIZE)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_INFO_WHITE_LEVEL,    MTK_SENSOR_INFO_WHITE_LEVEL)\
     _IMP_TAGCONVERT_(    ANDROID_SENSOR_INFO_TIMESTAMP_SOURCE,    MTK_SENSOR_INFO_TIMESTAMP_SOURCE)\
     _IMP_TAGCONVERT_(    ANDROID_SHADING_MODE           ,    MTK_SHADING_MODE           )\
     _IMP_TAGCONVERT_(    ANDROID_SHADING_STRENGTH,    MTK_SHADING_STRENGTH)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_FACE_DETECT_MODE,    MTK_STATISTICS_FACE_DETECT_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_HISTOGRAM_MODE,    MTK_STATISTICS_HISTOGRAM_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_SHARPNESS_MAP_MODE,    MTK_STATISTICS_SHARPNESS_MAP_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_HOT_PIXEL_MAP_MODE,    MTK_STATISTICS_HOT_PIXEL_MAP_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_FACE_IDS,    MTK_STATISTICS_FACE_IDS)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_FACE_LANDMARKS,    MTK_STATISTICS_FACE_LANDMARKS)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_FACE_RECTANGLES,    MTK_STATISTICS_FACE_RECTANGLES)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_FACE_SCORES,    MTK_STATISTICS_FACE_SCORES)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_HISTOGRAM,    MTK_STATISTICS_HISTOGRAM)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_SHARPNESS_MAP,    MTK_STATISTICS_SHARPNESS_MAP)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_LENS_SHADING_CORRECTION_MAP,    MTK_STATISTICS_LENS_SHADING_CORRECTION_MAP)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_LENS_SHADING_MAP ,    MTK_STATISTICS_LENS_SHADING_MAP )\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_PREDICTED_COLOR_GAINS ,    MTK_STATISTICS_PREDICTED_COLOR_GAINS )\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_PREDICTED_COLOR_TRANSFORM ,    MTK_STATISTICS_PREDICTED_COLOR_TRANSFORM )\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_SCENE_FLICKER ,    MTK_STATISTICS_SCENE_FLICKER )\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_HOT_PIXEL_MAP ,    MTK_STATISTICS_HOT_PIXEL_MAP )\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_LENS_SHADING_MAP_MODE ,    MTK_STATISTICS_LENS_SHADING_MAP_MODE )\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_INFO_AVAILABLE_FACE_DETECT_MODES,    MTK_STATISTICS_INFO_AVAILABLE_FACE_DETECT_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_INFO_HISTOGRAM_BUCKET_COUNT,    MTK_STATISTICS_INFO_HISTOGRAM_BUCKET_COUNT)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_INFO_MAX_FACE_COUNT,    MTK_STATISTICS_INFO_MAX_FACE_COUNT)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_INFO_MAX_HISTOGRAM_COUNT,    MTK_STATISTICS_INFO_MAX_HISTOGRAM_COUNT)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_INFO_MAX_SHARPNESS_MAP_VALUE,    MTK_STATISTICS_INFO_MAX_SHARPNESS_MAP_VALUE)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_INFO_SHARPNESS_MAP_SIZE,    MTK_STATISTICS_INFO_SHARPNESS_MAP_SIZE)\
     _IMP_TAGCONVERT_(    ANDROID_STATISTICS_INFO_AVAILABLE_HOT_PIXEL_MAP_MODES    ,    MTK_STATISTICS_INFO_AVAILABLE_HOT_PIXEL_MAP_MODES     )\
     _IMP_TAGCONVERT_(    ANDROID_TONEMAP_CURVE_BLUE     ,    MTK_TONEMAP_CURVE_BLUE     )\
     _IMP_TAGCONVERT_(    ANDROID_TONEMAP_CURVE_GREEN,    MTK_TONEMAP_CURVE_GREEN)\
     _IMP_TAGCONVERT_(    ANDROID_TONEMAP_CURVE_RED,    MTK_TONEMAP_CURVE_RED)\
     _IMP_TAGCONVERT_(    ANDROID_TONEMAP_MODE,    MTK_TONEMAP_MODE)\
     _IMP_TAGCONVERT_(    ANDROID_TONEMAP_MAX_CURVE_POINTS,    MTK_TONEMAP_MAX_CURVE_POINTS)\
     _IMP_TAGCONVERT_(    ANDROID_TONEMAP_AVAILABLE_TONE_MAP_MODES,    MTK_TONEMAP_AVAILABLE_TONE_MAP_MODES)\
     _IMP_TAGCONVERT_(    ANDROID_LED_TRANSMIT           ,    MTK_LED_TRANSMIT           )\
     _IMP_TAGCONVERT_(    ANDROID_LED_AVAILABLE_LEDS,    MTK_LED_AVAILABLE_LEDS)\
     _IMP_TAGCONVERT_(    ANDROID_INFO_SUPPORTED_HARDWARE_LEVEL,    MTK_INFO_SUPPORTED_HARDWARE_LEVEL)\
     _IMP_TAGCONVERT_(    ANDROID_BLACK_LEVEL_LOCK ,    MTK_BLACK_LEVEL_LOCK )\
     _IMP_TAGCONVERT_(    ANDROID_SYNC_FRAME_NUMBER,    MTK_SYNC_FRAME_NUMBER )\
     _IMP_TAGCONVERT_(    ANDROID_SYNC_MAX_LATENCY,    MTK_SYNC_MAX_LATENCY )\

#endif  //_TAGMAP_H_


