// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan

using System;

namespace trtc {
  public class TXDeviceManagerImplement : ITXDeviceManager {
    private IntPtr _nativeObj;
    public TXDeviceManagerImplement(IntPtr nativeObj) { _nativeObj = nativeObj; }

    public void DestroyNativeObj() { _nativeObj = IntPtr.Zero; }

    // 1.1
    public override bool isFrontCamera() {
      return TXDeviceManagerNative.tx_device_manager_is_front_camera(_nativeObj);
    }

    // 1.2
    public override int switchCamera(bool frontCamera) {
      return TXDeviceManagerNative.tx_device_manager_switch_camera(_nativeObj, frontCamera);
    }

    // 1.3
    public override double getCameraZoomMaxRatio() {
      return (double)TXDeviceManagerNative.tx_device_manager_get_camera_zoom_max_ratio(_nativeObj);
    }

    // 1.4
    public override int setCameraZoomRatio(double zoomRatio) {
      return TXDeviceManagerNative.tx_device_manager_set_camera_zoom_ratio(_nativeObj,
                                                                           (float)zoomRatio);
    }

    // 1.5
    public override bool isAutoFocusEnabled() {
      return TXDeviceManagerNative.tx_device_manager_is_audio_focus_enabled(_nativeObj);
    }

    // 1.6
    public override int enableCameraAutoFocus(bool enabled) {
      return TXDeviceManagerNative.tx_device_manager_enable_camera_auto_focus(_nativeObj, enabled);
    }

    // 1.7
    public override int setCameraFocusPosition(int x, int y) {
      return TXDeviceManagerNative.tx_device_manager_set_camera_focus_position(_nativeObj, x, y);
    }

    // 1.8
    public override int enableCameraTorch(bool enabled) {
      return TXDeviceManagerNative.tx_device_manager_enable_camera_torch(_nativeObj, enabled);
    }

    public override int setAudioRoute(TXAudioRoute route) {
      return TXDeviceManagerNative.tx_device_manager_set_audio_route(_nativeObj, route);
    }

    // 2.1
    public override TXDeviceInfo[] getDevicesList(TXMediaDeviceType type) {
      int count = TXDeviceManagerNative.tx_device_manager_get_device_count(_nativeObj, type);
      if (count <= 0) {
        return new TXDeviceInfo[0];
      }

      TXDeviceInfo[] deviceInfos = new TXDeviceInfo[count];
      DeviceInfo inner_deviceInfo = new DeviceInfo();
      for (int i = 0; i < count; i++) {
        try {
          TRTCTypeConverter.AllocateDeviceInfoMemory(ref inner_deviceInfo);
          TXDeviceManagerNative.tx_device_manager_get_device_info(_nativeObj, type, i,
                                                                  ref inner_deviceInfo);
          deviceInfos[i] = TRTCTypeConverter.ConvertToTXDeviceInfo(ref inner_deviceInfo);
        }
        finally {
          TRTCTypeConverter.FreeDeviceInfoMemory(ref inner_deviceInfo);
        }
      }

      return deviceInfos;
    }

    // 2.2
    public override int setCurrentDevice(TXMediaDeviceType type, String deviceId) {
      return TXDeviceManagerNative.tx_device_manager_set_current_device(_nativeObj, type, deviceId);
    }

    // 2.3
    public override TXDeviceInfo getCurrentDevice(TXMediaDeviceType type) {
      DeviceInfo inner_deviceInfo = new DeviceInfo();
      try {
        TRTCTypeConverter.AllocateDeviceInfoMemory(ref inner_deviceInfo);
        TXDeviceManagerNative.tx_device_manager_get_current_device(_nativeObj, type,
                                                                   ref inner_deviceInfo);
        return TRTCTypeConverter.ConvertToTXDeviceInfo(ref inner_deviceInfo);
      }
      finally {
        TRTCTypeConverter.FreeDeviceInfoMemory(ref inner_deviceInfo);
      }
    }

    public override int startMicDeviceTest(uint interval) {
      return TXDeviceManagerNative.tx_device_manager_start_mic_device_test(_nativeObj, interval);
    }

    public override int startMicDeviceTest(uint interval, bool playback) {
      return TXDeviceManagerNative.tx_device_manager_start_mic_device_test_and_playback(_nativeObj, interval, playback);
    }

    public override int stopMicDeviceTest() {
      return TXDeviceManagerNative.tx_device_manager_stop_mic_device_test(_nativeObj);
    }

    public override int startSpeakerDeviceTest(string filePath) {
      return TXDeviceManagerNative.tx_device_manager_start_speaker_device_test(_nativeObj, filePath);
    }

    public override int stopSpeakerDeviceTest() {
      return TXDeviceManagerNative.tx_device_manager_stop_speaker_device_test(_nativeObj);
    }
    // 2.4
    public override int setSystemVolumeType(TXSystemVolumeType type) {
#if UNITY_IPHONE || UNITY_ANDROID || UNITY_OPENHARMONY
      return TXDeviceManagerNative.tx_device_manager_set_system_volume_type(_nativeObj, type);
#endif
      return -1;
    }
  }
}