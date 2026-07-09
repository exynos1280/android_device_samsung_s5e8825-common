/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.samsung.proximityhelper;

import android.app.ActivityManager;
import android.app.IActivityManager;
import android.app.IProcessObserver;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.media.AudioDeviceInfo;
import android.media.AudioManager;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.PowerManager;
import android.os.RemoteException;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class ProximityHelperService extends Service {
    private static final String TAG = "ProximityHelper";
    private static final String TSP_CMD_PATH = "/sys/devices/virtual/sec/tsp/cmd";
    private static final String PROXIMITY_STATE_PATH = "/sys/class/sec/tsp/cmd_result";
    
    private static final int POLL_INTERVAL_MS = 500;

    private static final String[] SPECIAL_PACKAGES = {
        "org.telegram.messenger",
        "org.telegram.messenger.web",
        "nu.gpu.nagram",
        "org.thunderdog.challegram",
        "com.whatsapp",
        "com.discord",
        "com.android.dialer",
        "com.google.android.dialer",
        "com.samsung.android.dialer",
        "com.tencent.mm",
        "org.thoughtcrime.securesms",
        "com.viber.voip",
        "com.skype.raider",
        "jp.naver.line.android",
        "com.microsoft.teams",
        "us.zoom.videomeetings"
    };

    private AudioManager mAudioManager;
    private IActivityManager mAtm;
    private final Handler mHandler = new Handler(Looper.getMainLooper());

    private final Map<Integer, Integer> mForegroundPidToUid = new HashMap<>();
    private boolean mForegroundAppIsSpecial = false;
    private boolean mScreenOn = true;
    private boolean mPollingActive = false;

    private final Runnable mPollRunnable = new Runnable() {
        @Override
        public void run() {
            if (mPollingActive) {
                updateEarDetectState();
                mHandler.postDelayed(this, POLL_INTERVAL_MS);
            }
        }
    };

    private final AudioManager.OnModeChangedListener mModeListener = new AudioManager.OnModeChangedListener() {
        @Override
        public void onModeChanged(int mode) {
            Log.d(TAG, "Audio mode changed to: " + mode);
            mHandler.post(() -> {
                evaluatePollingState();
                updateEarDetectState();
            });
        }
    };

    private final AudioManager.OnCommunicationDeviceChangedListener mDeviceListener = 
            new AudioManager.OnCommunicationDeviceChangedListener() {
        @Override
        public void onCommunicationDeviceChanged(AudioDeviceInfo device) {
            Log.d(TAG, "Communication device changed to: " + (device != null ? device.getType() : "null"));
            mHandler.post(() -> {
                evaluatePollingState();
                updateEarDetectState();
            });
        }
    };

    private final IProcessObserver mProcessObserver = new IProcessObserver.Stub() {
        @Override
        public void onForegroundActivitiesChanged(int pid, int uid, boolean foregroundActivities) {
            synchronized (mForegroundPidToUid) {
                if (foregroundActivities) {
                    mForegroundPidToUid.put(pid, uid);
                } else {
                    mForegroundPidToUid.remove(pid);
                }
            }
            mHandler.post(() -> {
                updateForegroundAppState();
                updateEarDetectState();
            });
        }

        @Override
        public void onForegroundServicesChanged(int pid, int uid, int serviceTypes) {}

        @Override
        public void onProcessDied(int pid, int uid) {
            synchronized (mForegroundPidToUid) {
                mForegroundPidToUid.remove(pid);
            }
            mHandler.post(() -> {
                updateForegroundAppState();
                updateEarDetectState();
            });
        }

        @Override
        public void onProcessStarted(int pid, int processUid, int packageUid, String packageName, String processName) {}
    };

    private final BroadcastReceiver mScreenReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (Intent.ACTION_SCREEN_ON.equals(action)) {
                Log.d(TAG, "Screen turned ON.");
                mScreenOn = true;
            } else if (Intent.ACTION_SCREEN_OFF.equals(action)) {
                Log.d(TAG, "Screen turned OFF.");
                mScreenOn = false;
            }
            mHandler.post(() -> {
                evaluatePollingState();
                updateEarDetectState();
            });
        }
    };

    @Override
    public void onCreate() {
        super.onCreate();
        Log.i(TAG, "ProximityHelperService created.");

        mAudioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
        if (mAudioManager != null) {
            mAudioManager.addOnModeChangedListener(mHandler::post, mModeListener);
            mAudioManager.addOnCommunicationDeviceChangedListener(mHandler::post, mDeviceListener);
        }

        // Initialize screen state
        PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        mScreenOn = pm != null && pm.isInteractive();

        // Register screen state receiver
        IntentFilter filter = new IntentFilter();
        filter.addAction(Intent.ACTION_SCREEN_ON);
        filter.addAction(Intent.ACTION_SCREEN_OFF);
        registerReceiver(mScreenReceiver, filter);

        // Register process observer
        mAtm = ActivityManager.getService();
        if (mAtm != null) {
            try {
                mAtm.registerProcessObserver(mProcessObserver);
            } catch (RemoteException e) {
                Log.e(TAG, "Failed to register process observer", e);
            }
        }

        // Initialize foreground state and polling state
        updateForegroundAppState();
        updateEarDetectState();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        mPollingActive = false;
        mHandler.removeCallbacks(mPollRunnable);
        unregisterReceiver(mScreenReceiver);
        if (mAudioManager != null) {
            mAudioManager.removeOnModeChangedListener(mModeListener);
            mAudioManager.removeOnCommunicationDeviceChangedListener(mDeviceListener);
        }
        if (mAtm != null) {
            try {
                mAtm.unregisterProcessObserver(mProcessObserver);
            } catch (RemoteException e) {
                Log.e(TAG, "Failed to unregister process observer", e);
            }
        }
        writeTspCommand("ear_detect_enable,1");
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    private void updateForegroundAppState() {
        boolean isSpecial = false;
        synchronized (mForegroundPidToUid) {
            for (int uid : mForegroundPidToUid.values()) {
                if (isSpecialUid(uid)) {
                    isSpecial = true;
                    break;
                }
            }
        }
        if (mForegroundAppIsSpecial != isSpecial) {
            mForegroundAppIsSpecial = isSpecial;
            Log.d(TAG, "mForegroundAppIsSpecial changed to: " + mForegroundAppIsSpecial);
            evaluatePollingState();
        }
    }

    private void evaluatePollingState() {
        // We only poll the proximity sensor node if:
        // - A whitelisted app is in the foreground AND the screen is ON
        // - OR a call is active on the earpiece
        boolean shouldPoll = (mForegroundAppIsSpecial && mScreenOn) || isCallActiveOnEarpiece();
        if (shouldPoll && !mPollingActive) {
            mPollingActive = true;
            mHandler.post(mPollRunnable);
            Log.i(TAG, "Started proximity state polling loop.");
        } else if (!shouldPoll && mPollingActive) {
            mPollingActive = false;
            mHandler.removeCallbacks(mPollRunnable);
            Log.i(TAG, "Stopped proximity state polling loop.");
        }
    }

    private void updateEarDetectState() {
        if (mAudioManager == null) return;

        boolean isCallActiveOnEarpiece = isCallActiveOnEarpiece();

        // 2. Read last TSP command from cmd_result
        String cmdResult = readSysfs(PROXIMITY_STATE_PATH);
        boolean isProximityHALActive = cmdResult != null && cmdResult.contains("ear_detect_enable,1");

        // Active state should be triggered if:
        // - We are in an active call routed to the earpiece AND the HAL wrote 1 (or we want to ensure 3 is written)
        // - OR a whitelisted app is in the foreground AND the proximity sensor was activated by HAL (ear_detect_enable,1)
        boolean shouldBeActive = (isCallActiveOnEarpiece || mForegroundAppIsSpecial) && isProximityHALActive;

        if (shouldBeActive) {
            Log.i(TAG, "Proximity activation (ED 1) detected on TSP. Overriding to ED 3. Last cmd: " + cmdResult);
            writeTspCommand("ear_detect_enable,3");
        }
    }

    private boolean isCallActiveOnEarpiece() {
        int mode = mAudioManager.getMode();
        AudioDeviceInfo commDevice = mAudioManager.getCommunicationDevice();
        boolean isEarpiece = commDevice != null && commDevice.getType() == AudioDeviceInfo.TYPE_BUILTIN_EARPIECE;
        boolean isCallOrVoip = mode == AudioManager.MODE_IN_CALL || mode == AudioManager.MODE_IN_COMMUNICATION;
        return isCallOrVoip && isEarpiece;
    }

    private boolean isSpecialUid(int uid) {
        PackageManager pm = getPackageManager();
        if (pm == null) return false;
        String[] packages = pm.getPackagesForUid(uid);
        if (packages != null) {
            for (String pkg : packages) {
                if (isSpecialApp(pkg)) {
                    return true;
                }
            }
        }
        return false;
    }

    private boolean isSpecialApp(String packageName) {
        if (packageName == null) return false;
        for (String specialPkg : SPECIAL_PACKAGES) {
            if (packageName.equals(specialPkg)) {
                return true;
            }
        }
        if (packageName.contains("telephony") || packageName.contains("phone") || packageName.contains("dialer")) {
            return true;
        }
        return false;
    }

    private String readSysfs(String path) {
        File file = new File(path);
        if (!file.exists()) return null;
        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            return reader.readLine();
        } catch (IOException e) {
            return null;
        }
    }

    private void writeTspCommand(String cmd) {
        File file = new File(TSP_CMD_PATH);
        if (!file.exists()) {
            file = new File("/sys/class/sec/tsp/cmd");
        }
        if (!file.exists()) {
            Log.e(TAG, "TSP command node not found!");
            return;
        }
        try (FileOutputStream fos = new FileOutputStream(file)) {
            fos.write(cmd.getBytes());
            Log.i(TAG, "Successfully wrote to TSP: " + cmd);
        } catch (IOException e) {
            Log.e(TAG, "Failed to write TSP command: " + cmd, e);
        }
    }
}
