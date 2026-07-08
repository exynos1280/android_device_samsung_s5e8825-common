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

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.media.AudioDeviceInfo;
import android.media.AudioManager;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class ProximityHelperService extends Service {
    private static final String TAG = "ProximityHelper";
    private static final String TSP_CMD_PATH = "/sys/devices/virtual/sec/tsp/cmd";
    private static final int ENABLE_DELAY_MS = 500;
    private static final int DEBOUNCE_DELAY_MS = 1500;
    
    private AudioManager mAudioManager;
    private final Handler mHandler = new Handler(Looper.getMainLooper());

    private final Runnable mEnableRunnable = new Runnable() {
        @Override
        public void run() {
            Log.i(TAG, "Writing ear_detect_enable,3");
            writeTspCommand("ear_detect_enable,3");
        }
    };

    private final Runnable mDisableRunnable = new Runnable() {
        @Override
        public void run() {
            Log.i(TAG, "Debounce timer expired. Writing ear_detect_enable,1");
            writeTspCommand("ear_detect_enable,1");
        }
    };

    private final AudioManager.OnModeChangedListener mModeListener = new AudioManager.OnModeChangedListener() {
        @Override
        public void onModeChanged(int mode) {
            Log.d(TAG, "Audio mode changed to: " + mode);
            updateEarDetectState();
        }
    };

    private final AudioManager.OnCommunicationDeviceChangedListener mDeviceListener = 
            new AudioManager.OnCommunicationDeviceChangedListener() {
        @Override
        public void onCommunicationDeviceChanged(AudioDeviceInfo device) {
            Log.d(TAG, "Communication device changed to: " + (device != null ? device.getType() : "null"));
            updateEarDetectState();
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
            updateEarDetectState();
        }
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        mHandler.removeCallbacks(mEnableRunnable);
        mHandler.removeCallbacks(mDisableRunnable);
        if (mAudioManager != null) {
            mAudioManager.removeOnModeChangedListener(mModeListener);
            mAudioManager.removeOnCommunicationDeviceChangedListener(mDeviceListener);
        }
        writeTspCommand("ear_detect_enable,1");
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    private void updateEarDetectState() {
        if (mAudioManager == null) return;

        int mode = mAudioManager.getMode();
        AudioDeviceInfo commDevice = mAudioManager.getCommunicationDevice();
        boolean isEarpiece = commDevice != null && commDevice.getType() == AudioDeviceInfo.TYPE_BUILTIN_EARPIECE;

        boolean isCallOrVoip = mode == AudioManager.MODE_IN_CALL || mode == AudioManager.MODE_IN_COMMUNICATION;

        boolean shouldBeActive = isCallOrVoip && isEarpiece;

        Log.i(TAG, "updateEarDetectState: shouldBeActive=" + shouldBeActive 
                + " (isCallOrVoip=" + isCallOrVoip + ", isEarpiece=" + isEarpiece + ")");

        if (shouldBeActive) {
            mHandler.removeCallbacks(mDisableRunnable);
            // Delay writing 3 to let Sensors HAL write 1 first
            mHandler.removeCallbacks(mEnableRunnable);
            mHandler.postDelayed(mEnableRunnable, ENABLE_DELAY_MS);
        } else {
            mHandler.removeCallbacks(mEnableRunnable);
            // Always delay setting ED 1 to debounce intermediate routing events
            mHandler.removeCallbacks(mDisableRunnable);
            mHandler.postDelayed(mDisableRunnable, DEBOUNCE_DELAY_MS);
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
