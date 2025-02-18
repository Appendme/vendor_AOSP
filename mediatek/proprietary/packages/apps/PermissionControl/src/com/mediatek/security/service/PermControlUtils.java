package com.mediatek.security.service;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.drawable.Drawable;
import android.provider.Settings;

import android.os.SystemProperties;
import android.net.ConnectivityManager;
import android.telephony.TelephonyManager;
import android.util.Log;
import com.mediatek.common.mom.IMobileManager;
import com.mediatek.common.mom.Permission;
import com.mediatek.common.mom.PermissionRecord;
import com.mediatek.common.mom.SubPermissions;

import com.mediatek.security.R;
import com.mediatek.security.datamanager.DatabaseManager;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;



public class PermControlUtils {
    private static final String TAG = "PmUtils";
    private static Map<String, Integer> sPermControlMap;
    private static Set<String> sAvaiFeatureMap;

    public static final String PACKAGE_NAME = "exta_package_name";
    public static final String PERMISSION_NAME = "extra_permission_name";
    public static final String PERMISSION_FLAG = "extra_permission_flag";
    public static final String PERMISSION_CONTROL_STATE = "permission_control_state";
    public static final String PERMISSION_CONTROL_ATTACH = "permission_control_attached";
    public static final String PERMISSION_SWITCH_OFF_DLG_STATE = "permission_switch_off_dlg_state";
    public static final String EXTRA_STATE = "state";

    public static final int MAX_WATI_TIME = 20000; //in mill-seconds
    private static final int NOTIFY_ID = 1200;

    public static final String PERM_UI_ACTION = "com.mediatek.security.PERMISSION_CONTROL";
    public static final String PERM_CONTROL_DATA_UPDATE = "com.mediatek.security.action.DATA_UPDATE";
    public static final String START_SERVICE_ACTION = "com.mediatek.security.ACTION_START_PERMISSION";

    public static final String PKG_NAME = "package_name";

    /*
     * When add/reduce the permission remember to modify
     * PERM_CODE []
     * PERM_NAME []
     * PERM_ICON []
     * sAvaiFeatureMap
     * R.array.permission_label
     * R.array.confirm_msg_body
     *
     */
    // when adding one of these:
    //  - increment _NUM_OP
    public static final int PERM_NONE = -1;
    public static final int PERM_MAKE_CALL = 0; //FEATURE_TELEPHONY
    public static final int PERM_MAKE_CONF_CALL = 1; //FEATURE_TELEPHONY
    public static final int PERM_SEND_SMS = 2; //FEATURE_TELEPHONY
    public static final int PERM_SEND_MMS = 3; //FEATURE_TELEPHONY
    public static final int PERM_RECORD_VOICE = 4; //FEATURE_MICROPHONE
    public static final int PERM_READ_SMS = 5; //FEATURE_TELEPHONY
    public static final int PERM_READ_MMS = 6; //FEATURE_TELEPHONY
    public static final int PERM_READ_CONTACT = 7;
    public static final int PERM_READ_CALL_LOG = 8; //FEATURE_TELEPHONY
    public static final int PERM_ACCESS_LOCATION = 9;
    public static final int PERM_OPEN_CAMERA = 10; //FEATURE_CAMERA
    public static final int PERM_CHANGE_NETWORK_STATE_ON = 11; //FEATURE_TELEPHONY
    public static final int PERM_WIFI_STATE_ON = 12; //FEATURE_WIFI
    public static final int PERM_BT_STATE_ON = 13; //FEATURE_BLUETOOTH
    public static final int PERM_HOTKNOT_STATE_ON = 14; //FEATURE_HOTKNOT
    // L4 request
    public static final int PERM_MODIFY_SMS = 15; //FEATURE_MODIFY_OR_DELETE_SMS
    public static final int PERM_MODIFY_MMS = 16; //FEATURE_MODIFY_OR_DELETE_MMS
    public static final int PERM_MODIFY_CONTACTS = 17; //FEATURE_MODIFY_OR_DELETE_CONTACTS
    public static final int PERM_MODIFY_CALL_LOG = 18; //FEATURE_MODIFY_OR_DELETE_CALL_LOG
    // L5 request
    public static final int PERM_SEND_EMAIL = 19; //FEATURE_SEND_EMAIL
    // OP01 RCS request
    public static final int PERM_RCS_SET_PROFILE_INFO = 20;
    public static final int PERM_RCS_SET_GROUP_CHAIRMEN = 21;
    public static final int PERM_RCS_MODIFY_GROUP_SUBJECT = 22;
    public static final int PERM_RCS_ACCEPT_GROUP_INVITE = 23;
    public static final int PERM_RCS_REMOVE_PARTICIPANTS = 24;
    public static final int PERM_RCS_MODIFY_SELF_NICKNAME = 25;
    public static final int PERM_RCS_QUIT_CONVERSATION = 26;
    public static final int PERM_RCS_ABORT_CONVERSATION = 27;
    public static final int PERM_RCS_REPORT_PA = 28;
    public static final int PERM_RCS_FOLLOW_PA = 29;
    public static final int PERM_RCS_UNFOLLOW_PA = 30;

    /** @hide */
    public static final int NUM_OP = 31;

    /**
     * This maps each operation to the operation that serves as the
     * switch to determine whether it is allowed.
     */
    private static final int[] PERM_CODE = new int[] {
        PERM_MAKE_CALL,
        PERM_MAKE_CONF_CALL,
        PERM_SEND_SMS,
        PERM_SEND_MMS,
        PERM_SEND_EMAIL,
        PERM_RECORD_VOICE,
        PERM_READ_SMS,
        PERM_MODIFY_SMS,
        PERM_READ_MMS,
        PERM_MODIFY_MMS,
        PERM_READ_CONTACT,
        PERM_MODIFY_CONTACTS,
        PERM_READ_CALL_LOG,
        PERM_MODIFY_CALL_LOG,
        PERM_ACCESS_LOCATION,
        PERM_OPEN_CAMERA,
        PERM_CHANGE_NETWORK_STATE_ON,
        PERM_WIFI_STATE_ON,
        PERM_BT_STATE_ON,
        PERM_HOTKNOT_STATE_ON,
        PERM_RCS_SET_PROFILE_INFO,
        PERM_RCS_SET_GROUP_CHAIRMEN,
        PERM_RCS_MODIFY_GROUP_SUBJECT,
        PERM_RCS_ACCEPT_GROUP_INVITE,
        PERM_RCS_REMOVE_PARTICIPANTS,
        PERM_RCS_MODIFY_SELF_NICKNAME,
        PERM_RCS_QUIT_CONVERSATION,
        PERM_RCS_ABORT_CONVERSATION,
        PERM_RCS_REPORT_PA,
        PERM_RCS_FOLLOW_PA,
        PERM_RCS_UNFOLLOW_PA
    };

    /**
     * This optionally maps a permission to an operation.  If there
     * is no permission associated with an operation, it is null.
     */
    private static final String[] PERM_NAME = new String[] {
        SubPermissions.MAKE_CALL,
        SubPermissions.MAKE_CONFERENCE_CALL,
        SubPermissions.SEND_SMS,
        SubPermissions.SEND_MMS,
        SubPermissions.SEND_EMAIL,
        SubPermissions.RECORD_MIC,
        SubPermissions.QUERY_SMS,
        SubPermissions.MODIFY_SMS,
        SubPermissions.QUERY_MMS,
        SubPermissions.MODIFY_MMS,
        SubPermissions.QUERY_CONTACTS,
        SubPermissions.MODIFY_CONTACTS,
        SubPermissions.QUERY_CALL_LOG,
        SubPermissions.MODIFY_CALL_LOG,
        SubPermissions.ACCESS_LOCATION,
        SubPermissions.OPEN_CAMERA,
        SubPermissions.CHANGE_NETWORK_STATE_ON,
        SubPermissions.CHANGE_WIFI_STATE_ON,
        SubPermissions.CHANGE_BT_STATE_ON,
        SubPermissions.ACCESS_HOTKNOT,
        SubPermissions.RCS_SET_PROFILE_INFO,
        SubPermissions.RCS_SET_GROUP_CHAIRMEN,
        SubPermissions.RCS_MODIFY_GROUP_SUBJECT,
        SubPermissions.RCS_ACCEPT_GROUP_INVITE,
        SubPermissions.RCS_REMOVE_PARTICIPANTS,
        SubPermissions.RCS_MODIFY_SELF_NICKNAME,
        SubPermissions.RCS_QUIT_CONVERSATION,
        SubPermissions.RCS_ABORT_CONVERSATION,
        SubPermissions.RCS_REPORT_PA,
        SubPermissions.RCS_FOLLOW_PA,
        SubPermissions.RCS_UNFOLLOW_PA
    };

    private static final int[] PERM_ICON = {
        R.drawable.perm_group_phone_calls,
        R.drawable.perm_group_multi_party_call,
        R.drawable.perm_group_messages,
        R.drawable.perm_group_sent_mms,
        R.drawable.perm_group_sent_email,
        R.drawable.perm_group_start_sound_recording,
        R.drawable.perm_group_read_message,
        R.drawable.perm_group_modify_messages,
        R.drawable.perm_group_read_mms,
        R.drawable.perm_group_modify_mms,
        R.drawable.perm_group_read_contacts,
        R.drawable.perm_group_modify_contacts,
        R.drawable.perm_group_read_calllog,
        R.drawable.perm_group_modify_calllog,
        R.drawable.perm_group_location,
        R.drawable.perm_group_camera,
        R.drawable.perm_group_turn_on_data_connection,
        R.drawable.perm_group_network,
        R.drawable.perm_group_bluetooth,
        R.drawable.perm_group_turn_on_hotknot,
        R.drawable.perm_group_rcs,
        R.drawable.perm_group_rcs,
        R.drawable.perm_group_rcs,
        R.drawable.perm_group_rcs,
        R.drawable.perm_group_rcs,
        R.drawable.perm_group_rcs,
        R.drawable.perm_group_rcs,
        R.drawable.perm_group_rcs,
        R.drawable.perm_group_rcs,
        R.drawable.perm_group_rcs,
        R.drawable.perm_group_rcs
    };

    public static void initUtil(Context context) {
        if (sPermControlMap == null) {
            sPermControlMap = new HashMap<String, Integer>();
            initFeatureMap(context);
            int i = 0;
            for (String key : PERM_NAME) {
                if (sAvaiFeatureMap.contains(key)) {
                    sPermControlMap.put(key, PERM_CODE[i]);
                }
                i++;
            }
            Log.d("@M_" + TAG, "sPermControlMap = " + sPermControlMap);
        }
    }

    public static boolean isSupportFeature(String pemrName) {
        if (sAvaiFeatureMap != null) {
            return sAvaiFeatureMap.contains(pemrName);
        } else {
            return true;
        }
    }

    private static void initFeatureMap(Context context) {
        if (sAvaiFeatureMap == null) {
            sAvaiFeatureMap = new HashSet<String>();
        }
        // Not related to feature always add in
        sAvaiFeatureMap.add(SubPermissions.SEND_MMS);
        sAvaiFeatureMap.add(SubPermissions.QUERY_CONTACTS);
        sAvaiFeatureMap.add(SubPermissions.CHANGE_NETWORK_STATE_ON);
        PackageManager pkgMgr = context.getPackageManager();
        // Telephony feature
        if (pkgMgr.hasSystemFeature(PackageManager.FEATURE_TELEPHONY)) {
            sAvaiFeatureMap.add(SubPermissions.MAKE_CALL);
            sAvaiFeatureMap.add(SubPermissions.MAKE_CONFERENCE_CALL);
            sAvaiFeatureMap.add(SubPermissions.SEND_SMS);
            sAvaiFeatureMap.add(SubPermissions.QUERY_SMS);
            sAvaiFeatureMap.add(SubPermissions.QUERY_MMS);
            sAvaiFeatureMap.add(SubPermissions.QUERY_CALL_LOG);
            sAvaiFeatureMap.add(SubPermissions.MODIFY_SMS);
            sAvaiFeatureMap.add(SubPermissions.MODIFY_MMS);
        }
        // Microphone feature
        if (pkgMgr.hasSystemFeature(PackageManager.FEATURE_MICROPHONE)) {
            sAvaiFeatureMap.add(SubPermissions.RECORD_MIC);
        }

        // Camera feature
        if (pkgMgr.hasSystemFeature(PackageManager.FEATURE_CAMERA) &&
            pkgMgr.hasSystemFeature(PackageManager.FEATURE_CAMERA_AUTOFOCUS)) {
            sAvaiFeatureMap.add(SubPermissions.OPEN_CAMERA);
        }

        // Position feature
        if (pkgMgr.hasSystemFeature(PackageManager.FEATURE_LOCATION) &&
            pkgMgr.hasSystemFeature(PackageManager.FEATURE_LOCATION_GPS)) {
            sAvaiFeatureMap.add(SubPermissions.ACCESS_LOCATION);
        }

        // WIFI feature
        if (pkgMgr.hasSystemFeature(PackageManager.FEATURE_WIFI)) {
            sAvaiFeatureMap.add(SubPermissions.CHANGE_WIFI_STATE_ON);
        }

        // BT feature
        if (pkgMgr.hasSystemFeature(PackageManager.FEATURE_BLUETOOTH)) {
            sAvaiFeatureMap.add(SubPermissions.CHANGE_BT_STATE_ON);
        }

        // always add HotKnot ,but check it in UI display part
        sAvaiFeatureMap.add(SubPermissions.ACCESS_HOTKNOT);

        sAvaiFeatureMap.add(SubPermissions.MODIFY_CONTACTS);
        sAvaiFeatureMap.add(SubPermissions.MODIFY_CALL_LOG);

        // add for email
        sAvaiFeatureMap.add(SubPermissions.SEND_EMAIL);

        // OP01 RCS feature
        if (("1".equals(SystemProperties.get("ro.mtk_op01_rcs")))) {
            sAvaiFeatureMap.add(SubPermissions.RCS_SET_PROFILE_INFO);
            sAvaiFeatureMap.add(SubPermissions.RCS_SET_GROUP_CHAIRMEN);
            sAvaiFeatureMap.add(SubPermissions.RCS_MODIFY_GROUP_SUBJECT);
            sAvaiFeatureMap.add(SubPermissions.RCS_ACCEPT_GROUP_INVITE);
            sAvaiFeatureMap.add(SubPermissions.RCS_REMOVE_PARTICIPANTS);
            sAvaiFeatureMap.add(SubPermissions.RCS_MODIFY_SELF_NICKNAME);
            sAvaiFeatureMap.add(SubPermissions.RCS_QUIT_CONVERSATION);
            sAvaiFeatureMap.add(SubPermissions.RCS_ABORT_CONVERSATION);
            sAvaiFeatureMap.add(SubPermissions.RCS_REPORT_PA);
            sAvaiFeatureMap.add(SubPermissions.RCS_FOLLOW_PA);
            sAvaiFeatureMap.add(SubPermissions.RCS_UNFOLLOW_PA);
        }

        // for tablet project feature configuration
        boolean isTablet = ("tablet".equals(SystemProperties.get("ro.build.characteristics")));
        if (isTablet) {
            ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
            TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
            boolean isWifiOnly = false;
            boolean isVoiceCapable = false;
            boolean isSmsCapable = false;

            if (null != cm) {
                isWifiOnly = !cm.isNetworkSupported(ConnectivityManager.TYPE_MOBILE);
            }
            if (null != tm) {
                isVoiceCapable = tm.isVoiceCapable();
                isSmsCapable = tm.isSmsCapable();
            }

            if (isWifiOnly) {
                // WIFI_ONLY supported
                // only has turn on wifi , remove all phone related (except contacts)
                Log.d("@M_" + TAG, "tablet WIFI_ONLY supported");
                sAvaiFeatureMap.remove(SubPermissions.MAKE_CALL);
                sAvaiFeatureMap.remove(SubPermissions.MAKE_CONFERENCE_CALL);
                sAvaiFeatureMap.remove(SubPermissions.SEND_MMS);
                sAvaiFeatureMap.remove(SubPermissions.SEND_SMS);
                sAvaiFeatureMap.remove(SubPermissions.QUERY_MMS);
                sAvaiFeatureMap.remove(SubPermissions.QUERY_SMS);
                // sAvaiFeatureMap.remove(SubPermissions.QUERY_CONTACTS); wifi only tablet has contacts
                sAvaiFeatureMap.remove(SubPermissions.QUERY_CALL_LOG);
                sAvaiFeatureMap.remove(SubPermissions.CHANGE_NETWORK_STATE_ON);
                sAvaiFeatureMap.remove(SubPermissions.MODIFY_SMS);
                sAvaiFeatureMap.remove(SubPermissions.MODIFY_MMS);
                sAvaiFeatureMap.remove(SubPermissions.MODIFY_CALL_LOG);
            } else if (!isVoiceCapable) {
                if (isSmsCapable) {
                    // 3GDATA_SMS supported
                    // remove phone related except message controller
                    Log.d("@M_" + TAG, "tablet 3GDATA_SMS supported");
                    sAvaiFeatureMap.remove(SubPermissions.MAKE_CALL);
                    sAvaiFeatureMap.remove(SubPermissions.QUERY_CALL_LOG);
                    sAvaiFeatureMap.remove(SubPermissions.MAKE_CONFERENCE_CALL);
                    sAvaiFeatureMap.remove(SubPermissions.MODIFY_CALL_LOG);


                    sAvaiFeatureMap.add(SubPermissions.SEND_SMS);
                    sAvaiFeatureMap.add(SubPermissions.SEND_MMS);
                    sAvaiFeatureMap.add(SubPermissions.QUERY_SMS);
                    sAvaiFeatureMap.add(SubPermissions.QUERY_MMS);
                    sAvaiFeatureMap.add(SubPermissions.MODIFY_SMS);
                    sAvaiFeatureMap.add(SubPermissions.MODIFY_MMS);
                } else {
                    // 3GDATA_ONLY supported, same as WIFI_ONLY
                    // only network setting left
                    Log.d("@M_" + TAG, "tablet 3GDATA_ONLY supported");
                    sAvaiFeatureMap.remove(SubPermissions.MAKE_CALL);
                    sAvaiFeatureMap.remove(SubPermissions.MAKE_CONFERENCE_CALL);
                    sAvaiFeatureMap.remove(SubPermissions.SEND_MMS);
                    sAvaiFeatureMap.remove(SubPermissions.SEND_SMS);
                    sAvaiFeatureMap.remove(SubPermissions.QUERY_MMS);
                    sAvaiFeatureMap.remove(SubPermissions.QUERY_SMS);
                    // sAvaiFeatureMap.remove(SubPermissions.QUERY_CONTACTS); 3g data only tablet has contacts
                    sAvaiFeatureMap.remove(SubPermissions.QUERY_CALL_LOG);
                    sAvaiFeatureMap.remove(SubPermissions.MODIFY_SMS);
                    sAvaiFeatureMap.remove(SubPermissions.MODIFY_MMS);
                    sAvaiFeatureMap.remove(SubPermissions.MODIFY_CALL_LOG);
                }
            } else {
                // 3GDATA_FULL supported
                Log.d("@M_" + TAG, "tablet 3GDATA_FULL supported");
            }
        } else {
            // Phone 3GDATA_FULL supported
            Log.d("@M_" + TAG, "phone 3GDATA_FULL supported");
        }
        Log.d("@M_" + TAG, "sAvaiFeatureMap = " + sAvaiFeatureMap);
    }

    /**
     * Search the permission name is in the control list
     * @param permName input permission name
     * @return true for control
     */
    public static boolean isPermNeedControl(String permName) {
        return sPermControlMap.containsKey(permName);
    }

    /**
     * Get the label of permission
     * @param permName permission name
     * @param permLabelArray the array of permission name
     * @return the label of permission
     */
    public static String getPermissionLabel(String permName, String[] permLabelArray) {
        if (sPermControlMap == null) {
            return null;
        }
        Integer code = sPermControlMap.get(permName);

        int i = getCodeIndex(code);
        if (i != PERM_NONE) {
            return permLabelArray[i];
        }
        return null;
    }

    /**
     * Get the icon of permission
     * @param permName permission name
     * @return icon of the permisssion
     */
    public static int getPermissionIcon(String permName) {
        if (sPermControlMap == null) {
            return PERM_NONE;
        }
        Integer code = sPermControlMap.get(permName);

        int i = getCodeIndex(code);
        if (i != PERM_NONE) {
            return PERM_ICON[i];
        }
        return PERM_NONE;
    }

    /**
     * Get the permission index in permission array such as PERM_MAKE_CALL = 0
     * @param permName permission name
     * @return the index of permission
     */
    public static int getPermissionIndex(String permName) {
        Integer code = sPermControlMap.get(permName);
        return code;
    }

    /**
     * By using the code value to get permission name
     * @param code the permission code
     * @return permission name
     */
    public static String getPermissionName(int code) {
        int i = getCodeIndex(code);
        if (i != PERM_NONE) {
            return PERM_NAME[i];
        }
        return null;
    }

    public static int getCodeIndex(int code) {
        int index = PERM_NONE;
        // find the permission code's index  in the array of PERM_CODE
        for (int i = 0 ; i < PERM_CODE.length ; i++) {
            if (PERM_CODE[i] == code) {
                index = i;
                break;
            }
        }
        return index;
    }
    /**
     * The controlled permission name array
     * @return the array
     */
    public static String[] getControlPermArray() {
        return PERM_NAME;
    }

    /**
     * Permission control in some case need to be disabled, to get the state by calling this function
     * @param context Context
     * @return true for enable
     */
    public static boolean isInHouseEnabled(Context context) {
        int state = Settings.System.getInt(context.getContentResolver(),
                            PERMISSION_CONTROL_ATTACH,
                            context.getResources().getInteger(R.integer.default_attach_state));
        return state > 0;
    }

    public static void setInHouseEnabled(Context context, boolean isEnable) {
        Settings.System.putInt(context.getContentResolver(),
                PermControlUtils.PERMISSION_CONTROL_ATTACH, isEnable ? 1 : 0);
    }

    /**
     * Get application name by passing the  package name
     * @param context Context
     * @param pkgName package name
     * @return the application name
     */
    public static String getApplicationName(Context context, String pkgName) {
        if (pkgName == null) {
            return null;
        }
        String appName = null;
        try {
            PackageManager pkgManager = context.getPackageManager();
            ApplicationInfo info = pkgManager.getApplicationInfo(pkgName, 0);
            appName = pkgManager.getApplicationLabel(info).toString();
        } catch (NameNotFoundException e) {
            e.printStackTrace();
        }
        return appName;
    }

    /**
     * Get whether the pkg is under installed, false not installed.
     * @param context
     * @param pkgName
     * @return
     */
    public static boolean isPkgInstalled(Context context, String pkgName) {
        boolean isPkgInstalled = true;
        try {
            PackageManager pkgManager = context.getPackageManager();
            ApplicationInfo info = pkgManager.getApplicationInfo(pkgName, 0);
        } catch (NameNotFoundException e) {
            e.printStackTrace();
            Log.e("@M_" + TAG, "Package is not installed");
            isPkgInstalled = false;
        }
        return isPkgInstalled;
    }


    /**
     * Get application icon by passing the  package name
     * @param context Context
     * @param pkgName package name
     * @return the application icon
     */
    public static Drawable getApplicationIcon(Context context, String pkgName) {
        Drawable appIcon = null;
        try {
            PackageManager pkgManager = context.getPackageManager();
            ApplicationInfo info = pkgManager.getApplicationInfo(pkgName, 0);
            appIcon = pkgManager.getApplicationIcon(info);
        } catch (NameNotFoundException ex) {
            Log.w("@M_" + TAG, "get icon is null");
        }
        return appIcon;
    }

    /**
     * Modify the permission status of the package
     * @param permRecord PermissionRecord data type
     * @param context Context
     */
    public static void changePermission(PermissionRecord permRecord, Context context) {
        IMobileManager moMService = (IMobileManager) context.getSystemService(Context.MOBILE_SERVICE);
        try {
            moMService.setPermissionRecord(permRecord);
        } catch (SecurityException ex) {
            Log.w(TAG, "moMService.setPermissionRecord(permRecord); exception");
        }
        DatabaseManager.modify(permRecord);
    }

    /**
     * Switch on/off permission control monitor, only on the permission control will work
     * @param state true for turn on monitor
     * @param context Context
     */
    public static void enablePermissionControl(boolean state, Context context) {
        Log.d("@M_" + TAG, "enablePermissionControl state = " + state);
        Intent intent = new Intent(context, PermControlService.class);
        intent.putExtra(EXTRA_STATE, state);
        Settings.System.putInt(context.getContentResolver(),
                                PERMISSION_CONTROL_STATE, state ? 1 : 0);
        if (state) {
            intent.setAction(START_SERVICE_ACTION);
            context.startService(intent);
        } else {
            context.stopService(intent);
        }
    }

    /**
     * Get PermissionRecord List by passing packge name and corresponding Permission List,
     * because not every permission need to control, so only the control permission package
     * return list
     *
     * @param pkgName package name
     * @param permList Permission List of the package
     * @return the List of PermissionRecord of the package, if permList is null then return null
     */
    public static List<PermissionRecord> getPermRecordListByPkg(String pkgName, List<Permission> permList) {
        Log.d("@M_" + TAG, "getPermRecordListByPkg() pkgName = " + pkgName);
        if (permList == null) {
            Log.e("@M_" + TAG, "permList null");
            return null;
        }
        List<PermissionRecord> permRecordList = null;
        for (Permission permission : permList) {
            List<Permission> subPermList = permission.mSubPermissions;
            //If there is no sub permission and parent perm under monitor add into list
            if (subPermList == null) {
                if (sPermControlMap.containsKey(permission.mPermissionName)) {
                    PermissionRecord record = new PermissionRecord(pkgName, permission.mPermissionName,
                                                            IMobileManager.PERMISSION_STATUS_CHECK);
                    if (permRecordList == null) {
                        permRecordList = new ArrayList<PermissionRecord>();
                    }
                    permRecordList.add(record);
                }
            } else {
                List<PermissionRecord> subPermRecordList = getSubPermRecordList(pkgName, subPermList);
                if (subPermRecordList != null) {
                    if (permRecordList == null) {
                        permRecordList = new ArrayList<PermissionRecord>();
                    }
                    permRecordList.addAll(subPermRecordList);
                }
            }
        }
        return permRecordList;
    }

    private static List<PermissionRecord> getSubPermRecordList(String pkgName, List<Permission> subPermList) {
        List<PermissionRecord> subPermRecordList = null;
        for (Permission subPerm : subPermList) {
            if (sPermControlMap.containsKey(subPerm.mPermissionName)) {
                if (subPermRecordList == null) {
                    subPermRecordList = new ArrayList<PermissionRecord>();
                }
                PermissionRecord record = new PermissionRecord(pkgName, subPerm.mPermissionName,
                                                                IMobileManager.PERMISSION_STATUS_CHECK);
                subPermRecordList.add(record);
            }
        }
        return subPermRecordList;
    }

    /**
     * show a toast with deny hint msg body
     * @param context the context
     * @param pkgName the package name
     * @param permissionName the related permissin name
     */
    public static void showDenyToast(Context context, String pkgName, String permissionName) {
        String label = getApplicationName(context, pkgName);
        Log.d("@M_" + TAG, "showDenyToast() pkgName = " + pkgName + " label = " + label);
        if (label != null) {
            String msg = context.getString(R.string.toast_deny_msg_body, label, getMessageBody(context, permissionName));
            CustomizedToast.toast(context, msg);
        }
    }

    /**
     * Get the permission name related dialog message body
     * @param context Context
     * @param permName Permission name
     * @return String of the message body
     */
    public static String getMessageBody(Context context, String permName) {
        String msgArray[] = context.getResources().getStringArray(R.array.confirm_msg_body);
        int permIndex = getPermissionIndex(permName);
        int i = getCodeIndex(permIndex);
        String msg = msgArray[i];
        return msg;
    }

    public static boolean isPermControlOn(Context context) {
        return Settings.System.getInt(context.getContentResolver(),
                PermControlUtils.PERMISSION_CONTROL_STATE,
                context.getResources().getInteger(R.integer.default_enable_state)) > 0;
    }

    public static void showHintNotify(Context context) {
        int state = Settings.System.getInt(context.getContentResolver(),
                PermControlUtils.PERMISSION_CONTROL_STATE,
                context.getResources().getInteger(R.integer.default_enable_state));
        Log.d("@M_" + TAG, "showHintNotify state = " + state);
        if (state == context.getResources().getInteger(R.integer.default_enable_state)) {
            NotificationManager notificationManager =
                    (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
            String titleStr = context.getString(R.string.notification_control_state_title);
            String summaryStr = context.getString(R.string.notification_control_state_summary);
            Notification notification = new Notification();
            notification.icon = R.drawable.ic_settings_security;
            notification.when = 0;
            notification.flags = Notification.FLAG_AUTO_CANCEL;
            notification.tickerText = titleStr;
            notification.defaults = 0; // please be quiet
            notification.sound = null;
            notification.vibrate = null;
            notification.priority = Notification.PRIORITY_DEFAULT;
            Intent intent = new Intent();
            intent.setAction(PermControlUtils.PERM_UI_ACTION);
            PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, 0);
            notification.setLatestEventInfo(context, titleStr, summaryStr, pendingIntent);
            notificationManager.notify(NOTIFY_ID, notification);
        }
    }

    public static void cancelNotification(Context context) {
        NotificationManager notifyManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
        notifyManager.cancel(NOTIFY_ID);
    }
}
