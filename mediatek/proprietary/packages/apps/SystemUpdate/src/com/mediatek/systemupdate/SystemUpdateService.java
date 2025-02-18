package com.mediatek.systemupdate;

import android.app.AlertDialog;
import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.os.storage.StorageVolume;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Toast;

import com.mediatek.systemupdate.SystemUpdateService.SessionStateControlThread;

import java.util.List;

/**
 * This a service to listen boot complete etc.
 */
public class SystemUpdateService extends Service {

    private static final String TAG = "SystemUpdate/Service";

    static final int MSG_NETWORKERROR = 0;
    static final int MSG_RELOAD_ZIP_FILE = 2;
    static final int MSG_DLPKGCOMPLETE = 4;
    static final int MSG_DLPKGUPGRADE = 5;
    static final int MSG_DELTADELETED = 8;
    // static final int MSG_SDCARDCRASHORUNMOUNT = 9;
    static final int MSG_SDCARDUNKNOWNERROR = 10;
    static final int MSG_SDCARDINSUFFICENT = 11;
    static final int MSG_SDCARDPACKAGESDETECTED = 12;
    static final int MSG_UNKNOWERROR = 13;
    static final int MSG_OTA_PACKAGEERROR = 14;
    static final int MSG_OTA_NEEDFULLPACKAGE = 15;
    static final int MSG_OTA_USERDATAERROR = 16;
    static final int MSG_OTA_USERDATAINSUFFICENT = 17;
    static final int MSG_OTA_CLOSECLIENTUI = 18;
    static final int MSG_OTA_SDCARDINFUFFICENT = 19;
    static final int MSG_OTA_SDCARDERROR = 20;
    static final int MSG_UNZIP_ERROR = 21;
    static final int MSG_CKSUM_ERROR = 22;
    static final int MSG_UNZIP_LODING = 23;
    static final int MSG_LARGEPKG = 24;

    static final int MSG_SDCARDMOUNTED = 25;
    static final int MSG_SDCARDPACKAGEINVALIDATE = 26;
    static final int MSG_DL_STARTED = 27;
    static final int MSG_FILE_NOT_EXIST = 28;
    static final int MSG_NOTIFY_POWEROFF = 29;
    static final int MSG_NOTIFY_QUERY_DONE = 30;

    static final int MSG_CONFIRM_UPGRADE = 31;

    private static SessionStateControlThread sQueryNewVersionThread;
    private static SessionStateControlThread sDlPkgProgressThread;

    private ServiceBinder mBinder = new ServiceBinder();

    private HttpManager mHttpManager;
    private SdcardScanner mSdcardScanner;

    private boolean mForProtected = false;

    /**
     * binder.
     */
    public class ServiceBinder extends Binder {
        SystemUpdateService getService() {
            return SystemUpdateService.this;
        }
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d("@M_" + TAG, "Service onCreate");
        if (Util.getUpdateType() != Util.UPDATE_TYPES.SDCARD_UPDATE_ONLY) {
            mHttpManager = HttpManager.getInstance(getApplicationContext());
        }

        if (Util.getUpdateType() != Util.UPDATE_TYPES.OTA_UPDATE_ONLY) {
            mSdcardScanner = SdcardScanner.getInstance(getApplicationContext());
        }
    }

    @Override
    public void onDestroy() {
        Log.d("@M_" + TAG, "Service onDestroy");
        if (mForProtected) {
            mForProtected = false;
            stopForeground(true);
            Log.i("@M_" + TAG, "stopForeground");
        }

        super.onDestroy();
    }

    @Override
    public IBinder onBind(Intent arg0) {
        return mBinder;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        if (intent == null) {
            Log.e("@M_" + TAG, "[onStartCommand], intent = null");

            return START_NOT_STICKY;
        }

        String action = intent.getAction();
        Log.i("@M_" + TAG, "[onStartCommand], action = " + action);
        if (Util.Action.ACTION_AUTO_QUERY_NEWVERSION.equals(action)) {
            Log.i("@M_" + TAG, "[onStartCommand], new thread to query");
            queryPackages();
            stopSelf();
        } else if (Util.Action.ACTION_AUTO_DOWNLOAD.equals(action)) {
            /*
             * if (mHttpManager != null) { mHttpManager.notifyDlStarted(); }
             */
            startDlPkg();
            stopSelf();
        } else if (Util.Action.ACTION_MEDIA_MOUNTED.equals(action)) {

            DownloadInfo downloadInfo = DownloadInfo.getInstance(this.getApplicationContext());
            if (downloadInfo.getActivityID() >= 0) {
                Log.v("@M_" + TAG,
                        "[onStartCommand] mounted, in the downloading/installing process, ignore");
            } else if (isQuerying()) {
                Log.v("@M_" + TAG, "[onStartCommand] mounted, in the querying process");
                if (mSdcardScanner != null) {
                    StorageVolume sv = (StorageVolume) intent
                            .getParcelableExtra(StorageVolume.EXTRA_STORAGE_VOLUME);
                    String storagePath = sv.getPath();

                    Log.v("@M_" + TAG,
                            "[onStartCommand] mounted,Package scaning, "
                                    + "tell scanner the mounted path "
                                    + storagePath);
                    mSdcardScanner.onSdcardMounted(storagePath);
                }
            } else {

                Log.v("@M_" + TAG, "[onStartCommand] mounted, setIfNeedRefresh true");
                downloadInfo.setIfNeedRefresh(true);
                NotifyManager notification = new NotifyManager(this);
                notification.clearNotification(NotifyManager.NOTIFY_NEW_VERSION);

                Log.v("@M_" + TAG, "[onStartCommand] mounted, send info to activities");
                Intent i = new Intent(Util.Action.ACTION_MEDIA_MOUNT_UPDATEUI);
                sendBroadcast(i);
            }
            stopSelf();
        } else if (Util.Action.ACTION_LCA_PROTECT.equals(action)) {
//            startForeground(1, new Notification());
            mForProtected = true;
//            Log.i("@M_" + TAG, "startForeground");

        } else {
            // ACTION_MEDIA_UNMOUNTED/ACTION_MEDIA_BAD_REMOVAL/ACTION_MEDIA_NOFS
            StorageVolume sv = (StorageVolume) intent
                    .getParcelableExtra(StorageVolume.EXTRA_STORAGE_VOLUME);
            String storagePath = sv.getPath();
            Log.v("@M_" + TAG, storagePath + " crashed");

            DownloadInfo downloadInfo = DownloadInfo.getInstance(this.getApplicationContext());
            int order = downloadInfo.getActivityID();
            if (order >= 0) {
                PackageInfoReader reader = new PackageInfoReader(this,
                        Util.PathName.PKG_INFO_IN_DATA);
                UpdatePackageInfo info = reader.getInfo(order);
                String path = (info == null) ? Util.getAvailablePath(this) : info.path;
                String availablePath = Util.getAvailablePath(this);
                Log.v("@M_" + TAG,
                        "[onStartCommand] removed, Current downloading/installing Package path is "
                                + path);

                if ((path != null && path.contains(storagePath))
                        || (availablePath != null && availablePath.contains(storagePath))) {
                    Log.v("@M_" + TAG,
                            "[onStartCommand] removed, "
                                    + "downloading/installing SD card crash, requery packages");

                    AlertDialog dialog = new AlertDialog.Builder(this, AlertDialog.THEME_HOLO_DARK)
                            .setTitle(R.string.error_sdcard)
                            .setMessage(R.string.sdcard_crash_or_unmount_when_install)
                            .setPositiveButton(android.R.string.ok, null).create();
                    dialog.getWindow().setType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
                    dialog.show();

                    Log.v("@M_" + TAG, "[onStartCommand] removed,reset infos");

                    downloadInfo.resetDownloadInfo();

                    Log.v("@M_" + TAG, "[onStartCommand] removed, notify activities to finish self");
                    if (mHttpManager != null) {
                        mHttpManager.clearNotification(NotifyManager.NOTIFY_DOWNLOADING);
                        mHttpManager.clearNotification(NotifyManager.NOTIFY_DL_COMPLETED);
                        mHttpManager.clearNotification(NotifyManager.NOTIFY_NEW_VERSION);
                    }                    Intent i = new Intent(Util.Action.ACTION_MEDIA_UNMOUNT_UPDATEUI);
                    i.putExtra("storagePath", storagePath);
                    sendBroadcast(i);
                } else {
                    Log.v("@M_" + TAG, "[onStartCommand] removed,"
                            + "Not downloading/installing SD card crash, ignore.");
                }
            } else {
                if (Util.getUpdateType().equals(Util.UPDATE_TYPES.OTA_UPDATE_ONLY)) {
                    Log.v("@M_" + TAG,
                            "[onStartCommand] removed, OTA only ,"
                                    + "and NOT downloading/installing process, ignore");
                } else {

                    Log.v("@M_" + TAG,
                            "[onStartCommand] removed, "
                                    + "NOT downloading/installing process, requery");

                    if (isQuerying()) {
                        if (mSdcardScanner != null) {
                            Log.v("@M_" + TAG,
                                    "[onStartCommand] removed,Package scaning, "
                                            + "tell scanner the crashed path");
                            mSdcardScanner.onSdcardCrashed(storagePath);
                        }
                        Toast.makeText(this, R.string.sdcard_unmount, Toast.LENGTH_LONG).show();
                    } else {
                        Log.v("@M_" + TAG, "[onStartCommand] removed,"
                                + "Package scan done, setIfNeedRefresh true");
                        downloadInfo.setIfNeedRefresh(true);
                        NotifyManager notification = new NotifyManager(this);
                        notification.clearNotification(NotifyManager.NOTIFY_NEW_VERSION);

                        Log.v("@M_" + TAG, "[onStartCommand] removed, notify activities to finish self");
                        Intent i = new Intent(Util.Action.ACTION_MEDIA_UNMOUNT_UPDATEUI);
                        i.putExtra("storagePath", storagePath);
                        sendBroadcast(i);
                    }

                }
            }
            stopSelf();

        }

        return START_NOT_STICKY;
    }

    boolean isQuerying() {
        if (sQueryNewVersionThread != null && sQueryNewVersionThread.isAlive()) {
            Log.i("@M_" + TAG, "onQueryNewVersion back from interrupt, mQueryNewVersionThread="
                    + sQueryNewVersionThread);
            return true;
        }
        return false;
    }

    boolean isDownloading() {
        if (sDlPkgProgressThread != null && sDlPkgProgressThread.isAlive()) {
            Log.i("@M_" + TAG, "sDlPkgProgressThread back from interrupt, sDlPkgProgressThread="
                    + sDlPkgProgressThread);
            return true;
        }

        return false;
    }

    /**
     * query package.
     */
    public void queryPackages() {
        if (isQuerying()) {
            return;
        }

        boolean upgradeStarted = DownloadInfo.getInstance(this).getUpgradeStartedState();
        if (upgradeStarted) {
            Log.i("@M_" + TAG, "Have started upgrade,do not need query....... ");
            return;
        }

        Log.i("@M_" + TAG, "start query packages......");
        sQueryNewVersionThread = null;
        sQueryNewVersionThread = new SessionStateControlThread(DownloadInfo.STATE_QUERYNEWVERSION);

        sQueryNewVersionThread.start();
    }


    /**
     * if query not finished yet, would not get any info.
     * @return upgrade package info list.
     */
    public List<UpdatePackageInfo> loadPackages() {
        PackageInfoReader geter = new PackageInfoReader(this, Util.PathName.PKG_INFO_IN_DATA);
        return geter.getInfoList();
    }

    void cancelDlPkg() {

        stopSelf();
    }

    void startDlPkg() {

        if (isDownloading()) {
            mHttpManager.setDownloadState();
            return;
        }

        sDlPkgProgressThread = new SessionStateControlThread(DownloadInfo.STATE_DOWNLOADING);

        sDlPkgProgressThread.start();
    }

    boolean checkIsDownloading() {

        return sDlPkgProgressThread != null && sDlPkgProgressThread.isAlive();

    }

    void resetDescriptionInfo() {
        if (mHttpManager != null) {
            mHttpManager.resetDescriptionInfo();
        }
    }

    void setHandler(Handler handler) {
        if (mHttpManager != null) {
            mHttpManager.setMessageHandler(handler);
        }
        if (mSdcardScanner != null) {
            mSdcardScanner.setMessageHandler(handler);
        }
    }

    void resetHandler(Handler handler) {
        if (mHttpManager != null) {
            mHttpManager.resetMessageHandler(handler);
        }
        if (mSdcardScanner != null) {
            mSdcardScanner.resetMessageHandler(handler);
        }
    }

    /**
     *
     * Create a new thread.
     *
     */
    class SessionStateControlThread extends Thread {

        /**
         * Constructor function.
         *
         * @param statusType
         *            current state during the upgrade process
         * @see DownloadInfo.
         */
        public SessionStateControlThread(int statusType) {
            mStatus = statusType;
        }

        /**
         * Main executing function of this thread.
         */
        public void run() {

            Log.i("@M_" + TAG, "SessionStateControlThread, status = " + mStatus);
            Log.e("@M_" + TAG, "thread run " + Thread.currentThread().getName());

            switch (mStatus) {
            case DownloadInfo.STATE_QUERYNEWVERSION:
                // Util.deleteFile(Util.PathName.PKG_INFO_IN_DATA);

                DownloadInfo.getInstance(getApplicationContext()).resetDownloadInfo();

                if (mHttpManager != null) {
                    mHttpManager.queryNewVersion();
                }

                if (mSdcardScanner != null) {
                    mSdcardScanner.querySdcardPackage();
                }

                break;
            case DownloadInfo.STATE_DOWNLOADING:
                if (mHttpManager != null) {
                    mHttpManager.onDownloadImage();
                }
                break;
            case DownloadInfo.STATE_CANCELDOWNLOAD:
                cancelDlPkg();
                break;

            case DownloadInfo.STATE_PACKAGEERROR:
                resetDescriptionInfo();
                break;
            default:
                break;
            }

        }

        private int mStatus;
    }
}
