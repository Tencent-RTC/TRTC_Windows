// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
using UnityEngine;
#else
using System.Windows;
#endif

namespace trtc {
  public class TRTCCloudImplement : ITRTCCloud {
    // 单例
    private static TRTCCloudImplement _sInstance;
    // 实例指针
    private IntPtr _nativeObj;
    // 共享的主实例指
    private IntPtr _shareNativeObj;
    private TRTCWrapperCallback _wrapperCallback;
#if UNITY_ANDROID && !UNITY_EDITOR
    private static AndroidJavaObject _handlerThread;
#endif

    private readonly TXDeviceManagerImplement _deviceManager;
    private readonly TXAudioEffectManagerImplement _audioEffectManager;
    private TRTCLog _logParam;
    private static List<TRTCCloudImplement> _subCloudList = new List<TRTCCloudImplement>();
    private const int _sourceNameLen = 512;
    private TRTCCloudImplement() : this(false) { }

    private TRTCCloudImplement(bool subCloud) {
#if UNITY_ANDROID && !UNITY_EDITOR
      var trtcCloudCls = new AndroidJavaClass("com.tencent.trtc.TRTCCloud");
      var javaSDKVersion = trtcCloudCls.CallStatic<string>("getSDKVersion");
      TRTCLogger.Info("TRTCCloud SDKVersion: " + javaSDKVersion);

      var androidJavaClass = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
      var currentActivity = androidJavaClass.GetStatic<AndroidJavaObject>("currentActivity");
      var objPtr = currentActivity.GetRawObject();
      _shareNativeObj = TRTCCloudNative.trtc_cloud_get_instance(objPtr);
      
      if(subCloud == false) {
        if (_handlerThread == null) {
          _handlerThread = new AndroidJavaObject("android.os.HandlerThread", "TRTCEventDispatcher");
          _handlerThread.Call("start");
        }
        AndroidJavaObject trtcCloud = trtcCloudCls.CallStatic<AndroidJavaObject>("sharedInstance", currentActivity);
        AndroidJavaObject looper = _handlerThread.Call<AndroidJavaObject>("getLooper");
        AndroidJavaObject handlerObj = new AndroidJavaObject("android.os.Handler", looper);
        trtcCloud.Call("setListenerHandler", handlerObj);
        trtcCloudCls.CallStatic("destroySharedInstance");
      }
#elif UNITY_OPENHARMONY && !UNITY_EDITOR
      OpenHarmonyJSClass openHarmonyJSClass = new OpenHarmonyJSClass("LiteavBase");
      openHarmonyJSClass.CallStatic<Array>("initialize");
      
      _shareNativeObj = TRTCCloudNative.trtc_cloud_get_instance(IntPtr.Zero);
#else
      _shareNativeObj = TRTCCloudNative.trtc_cloud_get_instance(IntPtr.Zero);
#endif
      _nativeObj =
          subCloud ? TRTCCloudNative.trtc_cloud_create_sub_cloud(_shareNativeObj) : _shareNativeObj;
      CloudManager.AddCloudImplement(_nativeObj, this);

      _wrapperCallback = new TRTCWrapperCallback(_nativeObj);

      IntPtr deviceManagerIntPtr = TRTCCloudNative.trtc_cloud_get_device_manager(_nativeObj);
      _deviceManager = new TXDeviceManagerImplement(deviceManagerIntPtr);

      IntPtr audioEffectManagerPtr =
          TRTCCloudNative.trtc_cloud_get_audio_effect_manager(_nativeObj);
      _audioEffectManager = new TXAudioEffectManagerImplement(audioEffectManagerPtr);

      _logParam = new TRTCLog();
    }

    ~TRTCCloudImplement() {
      TRTCLogger.Info(" ~TRTCCloudImplement");
      Destroy();
    }

#if UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
    private static void LoadTrtcPlugin() {
      try {
        TRTCCloudNative.load_TXFFmpeg();
      }
      catch (Exception e) {
      }

      try {
        TRTCCloudNative.load_TXSoundTouch();
      }
      catch (Exception e) {
      }
    }
#endif

    private void Destroy() {
      TRTCLogger.Info();
      if (_nativeObj == IntPtr.Zero) {
        TRTCLogger.Info("_nativeObj is IntPtr.Zero");
        return;
      }

      if (_nativeObj == _shareNativeObj && !CloudManager.IsEmpty()) {
        TRTCLogger.Warning("Watch! Destroy main TRTC, but exist sub clouds still not destroyed!");
      }

      CloudManager.RemoveCloudImplement(_nativeObj);

      if (_wrapperCallback != null) {
        _wrapperCallback.OnDestroy();
        _wrapperCallback = null;
      }

      if (_nativeObj != IntPtr.Zero && _nativeObj != _shareNativeObj) {
        TRTCCloudNative.trtc_cloud_destroy_sub_cloud(_shareNativeObj, _nativeObj);
      }
      else {
        TRTCCloudNative.trtc_cloud_destroy_instance(_nativeObj);
      }
      _shareNativeObj = IntPtr.Zero;
      _nativeObj = IntPtr.Zero;

      _deviceManager?.DestroyNativeObj();
      _audioEffectManager?.DestroyNativeObj();
    }

    public TRTCWrapperCallback GetCallback() {
      return _wrapperCallback;
    }

    // 1.1
    public static ITRTCCloud getShareInstance() {
      if (null == _sInstance) {
#if UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
        LoadTrtcPlugin();
#endif
        _sInstance = new TRTCCloudImplement();
        TRTCLogger.Info("unity plugin version: " + ITRTCCloud.PLUGIN_VERSION);
      }
      return _sInstance;
    }

    // 1.2
    public static void destroyShareInstance() {
      TRTCLogger.Info("destroyShareInstance start");
      foreach (TRTCCloudImplement subCloud in _subCloudList) {
        subCloud.Destroy();
      }
      _subCloudList.Clear();
      if (_sInstance != null) {
        _sInstance.Destroy();
        _sInstance = null;
      }

      TRTCLogger.Info("destroyShareInstance end");
    }

    public static ITRTCCloud queryTRTCShareInstance() {
      return _sInstance;
    }

    // 1.3
    public override void addCallback(ITRTCCloudCallback callback) {
      TRTCLogger.Info();
      _wrapperCallback?.AddAppCloudCallback(callback);
    }

    // 1.4
    public override void removeCallback(ITRTCCloudCallback callback) {
      TRTCLogger.Info();
      _wrapperCallback?.RemoveAppCloudCallback(callback);
    }

    // 2.1
    public override void enterRoom(ref TRTCParams param, TRTCAppScene scene) {
      if (param.role != TRTCRoleType.TRTCRoleAnchor && param.role != TRTCRoleType.TRTCRoleAudience) {
        param.role = TRTCRoleType.TRTCRoleAudience;
      }

      var stringBuilder = new StringBuilder();
      stringBuilder.Append("{");
      stringBuilder.Append("  \"api\": \"setFramework\",");
      stringBuilder.Append("  \"params\": {");
      stringBuilder.AppendFormat(
          "\"framework\": 8");  // trtc::FrameworkType::FRAMEWORK_TYPE_TRTCCLOUD_UNITY3D
      stringBuilder.Append("  }");
      stringBuilder.Append("}");
      callExperimentalAPI(stringBuilder.ToString());

      TRTCParamsInner innerParam = TRTCTypeConverter.ConvertTRTCParamsToInner(param);
      TRTCCloudNative.trtc_cloud_enter_room(_nativeObj, innerParam, scene);
      TRTCTypeConverter.FreeTRTCParamsInner(innerParam);
    }

    private void exitRoomInternal(bool stopLocalVideo) {
      if (stopLocalVideo) {
        TRTCVideoRenderViewManager.getInstance().removeAllVideoRenderView();
      }
      else {
        TRTCVideoRenderViewManager.getInstance().removeAllRemoteVideoRenderView();
        TRTCVideoRenderViewManager.getInstance().removeVideoRenderView("",
          TRTCVideoStreamType.TRTCVideoStreamTypeSub);
      }

      TRTCCloudNative.trtc_cloud_exit_room(_nativeObj);
    }

    // 2.2
    public override void exitRoom() {
      exitRoomInternal(true);
    }

    public override void exitRoom(bool stopLocalVideo, bool stopLocalAudio) {
      var stringBuilder = new StringBuilder();
      stringBuilder.Append("{");
      stringBuilder.Append("  \"api\": \"keepCapturingAfterExiting\",");
      stringBuilder.Append("  \"params\": {");
      stringBuilder.AppendFormat("\"keepVideoCapturing\":{0:0},", stopLocalVideo ? 0 : 1);
      stringBuilder.AppendFormat("\"keepAudioCapturing\": {0:0} ", stopLocalAudio ? 0 : 1);
      stringBuilder.Append("  }");
      stringBuilder.Append("}");
      callExperimentalAPI(stringBuilder.ToString());

      exitRoomInternal(stopLocalVideo);
    }

    // 2.3
    public override void switchRole(TRTCRoleType role) {
      TRTCCloudNative.trtc_cloud_switch_role(_nativeObj, role);
    }

    // 2.5
    public override void switchRoom(TRTCSwitchRoomConfig config) {
      TRTCCloudNative.trtc_cloud_switch_room(_nativeObj, config);
    }

    // 2.6
    public override void connectOtherRoom(string jsonParams) {
      TRTCCloudNative.trtc_cloud_connect_other_room(_nativeObj, jsonParams);
    }

    // 2.7
    public override void disconnectOtherRoom() {
      TRTCCloudNative.trtc_cloud_disconnect_other_room(_nativeObj);
    }

    // 2.8
    public override void setDefaultStreamRecvMode(bool autoRecvAudio, bool autoRecvVideo) {
      TRTCCloudNative.trtc_cloud_set_default_stream_recv_mode(_nativeObj, autoRecvAudio,
                                                              autoRecvVideo);
    }

    public override ITRTCCloud createSubCloud() {
#if UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
      LoadTrtcPlugin();
#endif
      TRTCCloudImplement subCloud = new TRTCCloudImplement(true);

      if (subCloud != null && !_subCloudList.Contains(subCloud)) {
        _subCloudList.Add(subCloud);
      }

      return subCloud;
    }

    public override void destroySubCloud(ITRTCCloud subCloud) {
      TRTCLogger.Info("destroySubCloud");
      if (subCloud == null) {
        return;
      }
      var cloudImpl = (TRTCCloudImplement)subCloud;
      cloudImpl.Destroy();

      if (_subCloudList.Contains(cloudImpl))
        _subCloudList.Remove(cloudImpl);
    }

    // 3.1
    public override void startPublishing(string streamId, TRTCVideoStreamType streamType) {
      TRTCCloudNative.trtc_cloud_start_publishing(_nativeObj, streamId, streamType);
    }

    // 3.2
    public override void stopPublishing() {
      TRTCCloudNative.trtc_cloud_stop_publishing(_nativeObj);
    }

    // 3.5
    public override void setMixTranscodingConfig(TRTCTranscodingConfig? config) {
      if (config == null) {
        TRTCCloudNative.trtc_cloud_set_mix_transcoding_config(_nativeObj, IntPtr.Zero);
      }
      else {
        TranscodingConfig inner_transcodingConfig = new TranscodingConfig();
        var tmpConfig = (TRTCTranscodingConfig)config;
        inner_transcodingConfig = TRTCTypeConverter.ConvertTRTCTranscodingConfig(tmpConfig);

        if (tmpConfig.mixUsersArraySize > 0) {
          inner_transcodingConfig.mixUsersArray = Marshal.AllocHGlobal(
              tmpConfig.mixUsersArray.Length * Marshal.SizeOf(typeof(TRTCMixUser)));
          for (var i = 0; i < tmpConfig.mixUsersArraySize; i++) {
            Marshal.StructureToPtr(
                tmpConfig.mixUsersArray[i],
                inner_transcodingConfig.mixUsersArray + (i * Marshal.SizeOf(typeof(TRTCMixUser))),
                false);
          }
        }

        var ptr = Marshal.AllocHGlobal(Marshal.SizeOf(inner_transcodingConfig));
        Marshal.StructureToPtr(inner_transcodingConfig, ptr, false);
        TRTCCloudNative.trtc_cloud_set_mix_transcoding_config(_nativeObj, ptr);

        Marshal.FreeHGlobal(ptr);
        if (inner_transcodingConfig.mixUsersArray != IntPtr.Zero) {
          Marshal.FreeHGlobal(inner_transcodingConfig.mixUsersArray);
        }
      }
    }

    // 3.6
    public override void startPublishMediaStream(ref TRTCPublishTarget target,
                                                 ref TRTCStreamEncoderParam param,
                                                 ref TRTCStreamMixingConfig config) {
      PublishTarget inner_target = new PublishTarget();
      inner_target.cdnUrlList = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(TRTCPublishCdnUrl)) *
                                                     (int)target.cdnUrlListSize);
      for (int i = 0; i < target.cdnUrlListSize; i++) {
        Marshal.StructureToPtr(
            target.cdnUrlList[i],
            inner_target.cdnUrlList + Marshal.SizeOf(typeof(TRTCPublishCdnUrl)) * i, false);
      }
      inner_target.cdnUrlListSize = target.cdnUrlListSize;
      inner_target.mode = target.mode;
      inner_target.mixStreamIdentity =
          Marshal.AllocHGlobal(Marshal.SizeOf(target.mixStreamIdentity));
      Marshal.StructureToPtr(target.mixStreamIdentity, inner_target.mixStreamIdentity, false);

      StreamEncoderParam inner_param = new StreamEncoderParam();
      inner_param = TRTCTypeConverter.ConvertTRTCStreamEncoderParam(param);

      StreamMixingConfig inner_config = new StreamMixingConfig();
      inner_config = TRTCTypeConverter.ConvertTRTCStreamMixingConfig(config);

      TRTCCloudNative.trtc_cloud_start_publish_media_stream(_nativeObj, ref inner_target,
                                                            ref inner_param, ref inner_config);

      Marshal.FreeHGlobal(inner_target.mixStreamIdentity);
      Marshal.FreeHGlobal(inner_target.cdnUrlList);
      TRTCTypeConverter.ReleaseStreamMixingConfig(inner_config);
    }

    // 3.7
    public override void updatePublishMediaStream(string taskId,
                                                  ref TRTCPublishTarget target,
                                                  ref TRTCStreamEncoderParam param,
                                                  ref TRTCStreamMixingConfig config) {
      PublishTarget inner_target = new PublishTarget();
      inner_target.cdnUrlList = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(TRTCPublishCdnUrl)) *
                                                     (int)target.cdnUrlListSize);
      for (int i = 0; i < target.cdnUrlListSize; i++) {
        Marshal.StructureToPtr(
            target.cdnUrlList[i],
            inner_target.cdnUrlList + Marshal.SizeOf(typeof(TRTCPublishCdnUrl)) * i, false);
      }
      inner_target.cdnUrlListSize = target.cdnUrlListSize;
      inner_target.mode = target.mode;
      inner_target.mixStreamIdentity =
          Marshal.AllocHGlobal(Marshal.SizeOf(target.mixStreamIdentity));
      Marshal.StructureToPtr(target.mixStreamIdentity, inner_target.mixStreamIdentity, false);

      StreamEncoderParam inner_param = new StreamEncoderParam();
      inner_param = TRTCTypeConverter.ConvertTRTCStreamEncoderParam(param);

      StreamMixingConfig inner_config = new StreamMixingConfig();
      inner_config = TRTCTypeConverter.ConvertTRTCStreamMixingConfig(config);

      TRTCCloudNative.trtc_cloud_update_publish_media_stream(_nativeObj, taskId, ref inner_target,
                                                             ref inner_param, ref inner_config);

      Marshal.FreeHGlobal(inner_target.mixStreamIdentity);
      Marshal.FreeHGlobal(inner_target.cdnUrlList);
      TRTCTypeConverter.ReleaseStreamMixingConfig(inner_config);
    }

    // 3.8
    public override void stopPublishMediaStream(string taskId) {
      TRTCCloudNative.trtc_cloud_stop_publish_media_stream(_nativeObj, taskId);
    }

    // 4.1 + 4.2
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
    public override void startLocalPreview(bool frontCamera, GameObject view) {
      TRTCCloudNative.trtc_cloud_start_local_preview(_nativeObj, frontCamera, IntPtr.Zero);
      if (view) {
        TRTCVideoRenderViewManager.getInstance().addVideoRenderView(view, "",
          TRTCVideoStreamType.TRTCVideoStreamTypeBig);
      }
    }
#else
    public override void startLocalPreview(bool frontCamera, System.Windows.Controls.Image view) {
      TRTCCloudNative.trtc_cloud_start_local_preview(_nativeObj, frontCamera, IntPtr.Zero);
      if (view != null) {
        TRTCVideoRenderViewManager.getInstance().addVideoRenderView(view, "",
            TRTCVideoStreamType.TRTCVideoStreamTypeBig);
      }
    }
    
    public override void startLocalPreview(bool frontCamera, IntPtr hwnd) {
      TRTCCloudNative.trtc_cloud_start_local_preview(_nativeObj, frontCamera, hwnd);
    }
#endif

    // 4.4
    public override void stopLocalPreview() {
      TRTCCloudNative.trtc_cloud_stop_local_preview(_nativeObj);
      TRTCVideoRenderViewManager.getInstance().removeVideoRenderView("",
         TRTCVideoStreamType.TRTCVideoStreamTypeBig);
    }

    // 4.5
    public override void muteLocalVideo(TRTCVideoStreamType streamType, bool mute) {
      TRTCCloudNative.trtc_cloud_mute_local_video(_nativeObj, streamType, mute);
    }

    // 4.7
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
    public override void startRemoteView(string userId,
                                         TRTCVideoStreamType streamType,
                                         GameObject view) {
      TRTCCloudNative.trtc_cloud_start_remote_view(_nativeObj, userId, streamType, IntPtr.Zero);
      if (view != null) {
        TRTCVideoRenderViewManager.getInstance().addVideoRenderView(view, userId, streamType);
      }
    }
#else
    public override void startRemoteView(string userId,
                                 TRTCVideoStreamType streamType,
                                 System.Windows.Controls.Image view) {
      TRTCCloudNative.trtc_cloud_start_remote_view(_nativeObj, userId, streamType, IntPtr.Zero);
      if (view != null) {
        TRTCVideoRenderViewManager.getInstance().addVideoRenderView(view, userId,
            streamType);
      }
    }
    public override void startRemoteView(string userId,
                                 TRTCVideoStreamType streamType,
                                 IntPtr hwnd) {
      TRTCCloudNative.trtc_cloud_start_remote_view(_nativeObj, userId, streamType, hwnd);
    }
#endif
    // 4.9
    public override void stopRemoteView(string userId, TRTCVideoStreamType streamType) {
      TRTCCloudNative.trtc_cloud_stop_remote_view(_nativeObj, userId, streamType);
      TRTCVideoRenderViewManager.getInstance().removeVideoRenderView(userId, streamType);
    }

    // 4.10
    public override void stopAllRemoteView() {
      TRTCCloudNative.trtc_cloud_stop_all_remote_view(_nativeObj);
      TRTCVideoRenderViewManager.getInstance().removeAllRemoteVideoRenderView();
    }

    // 4.11
    public override void muteRemoteVideoStream(string userId, TRTCVideoStreamType streamType, bool mute) {
      TRTCCloudNative.trtc_cloud_mute_remote_video_stream(_nativeObj, userId, streamType, mute);
    }

    // 4.12
    public override void muteAllRemoteVideoStreams(bool mute) {
      TRTCCloudNative.trtc_cloud_mute_all_remote_video_streams(_nativeObj, mute);
    }


    // 4.13
    public override void setVideoEncoderParam(ref TRTCVideoEncParam param) {
      TRTCCloudNative.trtc_cloud_set_video_encoder_param(_nativeObj, param);
    }

    // 4.14
    public override void setNetworkQosParam(ref TRTCNetworkQosParam param) {
      TRTCCloudNative.trtc_cloud_set_network_qos_param(_nativeObj, param);
    }

    // 4.15
    public override void setLocalRenderParams(TRTCRenderParams renderParams) {
      TRTCVideoRenderViewManager.getInstance().setVideoRenderParams("",
        TRTCVideoStreamType.TRTCVideoStreamTypeBig, renderParams);
    }

    // 4.16
    public override void setRemoteRenderParams(string userId,
                                               TRTCVideoStreamType streamType,
                                               ref TRTCRenderParams renderParams) {
      TRTCVideoRenderViewManager.getInstance().setVideoRenderParams(userId, streamType, renderParams);
    }

    // 4.17
    public override void setVideoEncoderRotation(TRTCVideoRotation rotation) {
      TRTCCloudNative.trtc_cloud_set_video_encoder_rotation(_nativeObj, rotation);
    }

    // 4.18
    public override void setVideoEncoderMirror(bool mirror) {
      TRTCCloudNative.trtc_cloud_set_video_encoder_mirror(_nativeObj, mirror);
    }

    // 4.20
    public override void enableSmallVideoStream(bool enable,
                                                ref TRTCVideoEncParam smallVideoParam) {
      TRTCCloudNative.trtc_cloud_enable_small_video_stream(_nativeObj, enable, smallVideoParam);
    }

    // 4.21
    public override void setRemoteVideoStreamType(string userId, TRTCVideoStreamType type) {
      TRTCCloudNative.trtc_cloud_set_remote_video_stream_type(_nativeObj, userId, type);
    }

    // 4.22
    public override void snapshotVideo(string userId, TRTCVideoStreamType streamType, TRTCSnapshotSourceType sourceType) {
      using (var userIdMarshaler = new Utf8StringMarshaler(userId)) {
        TRTCCloudNative.trtc_cloud_snapshot_video(_nativeObj, userIdMarshaler.GetUtf8IntPtr(), streamType, sourceType);
      }
    }

    // 4.25
    public override void setGravitySensorAdaptiveMode(TRTCGravitySensorAdaptiveMode mode) {
      TRTCCloudNative.trtc_cloud_set_gravity_sensor_adaptive_mode(_nativeObj, mode);
    }

    // 5.1
    public override void startLocalAudio(TRTCAudioQuality quality) {
      TRTCCloudNative.trtc_cloud_start_local_audio(_nativeObj, quality);
    }

    // 5.2
    public override void stopLocalAudio() {
      TRTCCloudNative.trtc_cloud_stop_local_audio(_nativeObj);
    }

    // 5.3
    public override void muteLocalAudio(bool mute) {
      TRTCCloudNative.trtc_cloud_mute_local_audio(_nativeObj, mute);
    }

    // 5.4
    public override void muteRemoteAudio(string userId, bool mute) {
      TRTCCloudNative.trtc_cloud_mute_remote_audio(_nativeObj, userId, mute);
    }

    // 5.5
    public override void muteAllRemoteAudio(bool mute) {
      TRTCCloudNative.trtc_cloud_mute_all_remote_audio(_nativeObj, mute);
    }

    // 5.7
    public override void setRemoteAudioVolume(string userId, int volume) {
      TRTCCloudNative.trtc_cloud_set_remote_audio_volume(_nativeObj, userId, volume);
    }

    // 5.8
    public override void setAudioCaptureVolume(int volume) {
      TRTCCloudNative.trtc_cloud_set_audio_capture_volume(_nativeObj, volume);
    }

    // 5.9
    public override int getAudioCaptureVolume() {
      return TRTCCloudNative.trtc_cloud_get_audio_capture_volume(_nativeObj);
    }

    // 5.10
    public override void setAudioPlayoutVolume(int volume) {
      TRTCCloudNative.trtc_cloud_set_audio_playout_volume(_nativeObj, volume);
    }

    // 5.11
    public override int getAudioPlayoutVolume() {
      return TRTCCloudNative.trtc_cloud_get_audio_playout_volume(_nativeObj);
    }

    // 5.12
    public override void enableAudioVolumeEvaluation(bool enable,
                                                     TRTCAudioVolumeEvaluateParams evaluateParams) {
      if (enable) {
        TRTCCloudNative.trtc_cloud_enable_audio_volume_evaluation(_nativeObj, true, evaluateParams);
      }
      else {
        evaluateParams.interval = 0;
        evaluateParams.enableVadDetection = false;
        evaluateParams.enableSpectrumCalculation = false;
        TRTCCloudNative.trtc_cloud_enable_audio_volume_evaluation(_nativeObj, false,
                                                                  evaluateParams);
      }
    }

    // 5.15
    public override void startLocalRecording(ref TRTCLocalRecordingParams localRecordingParams) {
      TRTCCloudNative.trtc_cloud_start_local_recording(_nativeObj, localRecordingParams);
    }

    // 5.16
    public override void stopLocalRecording() {
      TRTCCloudNative.trtc_cloud_stop_local_recording(_nativeObj);
    }

    public override ITXDeviceManager getDeviceManager() {
      return _deviceManager;
    }

    // 7.1
    public override void setBeautyStyle(TRTCBeautyStyle style,
                                        uint beauty,
                                        uint white,
                                        uint ruddiness) {
      TRTCCloudNative.trtc_cloud_set_beauty_style(_nativeObj, style, beauty, white, ruddiness);
    }

    // 7.2
    public override void setWaterMark(TRTCVideoStreamType streamType,
                                      string srcData,
                                      TRTCWaterMarkSrcType srcType,
                                      uint nWidth,
                                      uint nHeight,
                                      float xOffset,
                                      float yOffset,
                                      float fWidthRatio,
                                      bool isVisibleOnLocalPreview = false) {
      TRTCCloudNative.trtc_cloud_set_water_mark(_nativeObj, streamType, srcData, srcType, nWidth,
                                                nHeight, xOffset, yOffset, fWidthRatio,
                                                isVisibleOnLocalPreview);
    }

    public override ITXAudioEffectManager getAudioEffectManager() {
      return _audioEffectManager;
    }

    // 8.2
    public override void startSystemAudioLoopback(string deviceName) {
      TRTCCloudNative.trtc_cloud_start_system_audio_loopback(_nativeObj, deviceName);
    }

    // 8.3
    public override void stopSystemAudioLoopback() {
      TRTCCloudNative.trtc_cloud_stop_system_audio_loopback(_nativeObj);
    }

    // 9.1
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
    public override void startScreenCapture(GameObject view, TRTCVideoStreamType type, ref TRTCVideoEncParam param) {
      TRTCCloudNative.trtc_cloud_start_screen_capture(_nativeObj, IntPtr.Zero, type, ref param);
      if (view != null) {
        TRTCVideoRenderViewManager.getInstance().addVideoRenderView(view, "",
          TRTCVideoStreamType.TRTCVideoStreamTypeSub);
      }
    }
#else
    public override void startScreenCapture(System.Windows.Controls.Image view, TRTCVideoStreamType type, ref TRTCVideoEncParam param) {
      TRTCCloudNative.trtc_cloud_start_screen_capture(_nativeObj, IntPtr.Zero, type, ref param);
      if (view != null) {
        TRTCVideoRenderViewManager.getInstance().addVideoRenderView(view, "",
            TRTCVideoStreamType.TRTCVideoStreamTypeSub);
      }
    }
    public override void startScreenCapture(IntPtr hwnd, TRTCVideoStreamType type, ref TRTCVideoEncParam param) {
      TRTCCloudNative.trtc_cloud_start_screen_capture(_nativeObj, hwnd, type, ref param);
    }
#endif

    // 9.2
    public override void stopScreenCapture() {
      TRTCCloudNative.trtc_cloud_stop_screen_capture(_nativeObj);
      TRTCVideoRenderViewManager.getInstance().removeVideoRenderView("",
        TRTCVideoStreamType.TRTCVideoStreamTypeSub);
    }

    // 9.3
    public override void pauseScreenCapture() {
      TRTCCloudNative.trtc_cloud_pause_screen_capture(_nativeObj);
    }

    // 9.4
    public override void resumeScreenCapture() {
      TRTCCloudNative.trtc_cloud_resume_screen_capture(_nativeObj);
    }

    public override TRTCScreenCaptureSourceInfo[] getScreenCaptureSources(SIZE thumbnailSize, SIZE iconSize) {
      if (thumbnailSize.width <= 0 || thumbnailSize.height <= 0 ||
        iconSize.width <= 0 || iconSize.height <= 0) {
        return null;
      }
      TRTCSize thumbnailTrtcSize = new TRTCSize();
      TRTCSize iconTrtcSize = new TRTCSize();
      thumbnailTrtcSize.width = thumbnailSize.width;
      thumbnailTrtcSize.height = thumbnailSize.height;
      iconTrtcSize.width = iconSize.width;
      iconTrtcSize.height = iconSize.height;
      var sourceInfoLists = Array.Empty<TRTCScreenCaptureSourceInfo>();
      int count = 0;
      IntPtr sourceListPtr = IntPtr.Zero;
      var ret = TRTCCloudNative.trtc_cloud_get_screen_capture_source_list(
          _nativeObj, thumbnailTrtcSize, iconTrtcSize, ref sourceListPtr, ref count);
      if (ret != 0 || count == 0) {
        return sourceInfoLists;
      }

      sourceInfoLists = new TRTCScreenCaptureSourceInfo[count];
      for (var i = 0; i < count; i++) {
        ScreenCaptureSourceInfo captureSourceInfo = new ScreenCaptureSourceInfo();
        captureSourceInfo.sourceName = Marshal.AllocHGlobal(_sourceNameLen);

        captureSourceInfo.thumbBGRA.buffer =
            Marshal.AllocHGlobal((int)thumbnailTrtcSize.width * (int)thumbnailTrtcSize.height * 4);
        captureSourceInfo.iconBGRA.buffer =
            Marshal.AllocHGlobal((int)iconTrtcSize.width * (int)iconTrtcSize.height * 4);

        if (0 == TRTCCloudNative.trtc_cloud_get_screen_capture_sources_info(sourceListPtr, i,
                                                                         ref captureSourceInfo)) {
          sourceInfoLists[i].sourceId = captureSourceInfo.sourceId;
          sourceInfoLists[i].type = captureSourceInfo.type;
          sourceInfoLists[i].sourceName = TRTCTypeConverter.NativeUtf8PtrToString(captureSourceInfo.sourceName);
          sourceInfoLists[i].isMainScreen = captureSourceInfo.isMainScreen;
          sourceInfoLists[i].thumbBGRA.buffer = new byte[captureSourceInfo.thumbBGRA.length];
          Marshal.Copy(captureSourceInfo.thumbBGRA.buffer, sourceInfoLists[i].thumbBGRA.buffer, 0,
                       captureSourceInfo.thumbBGRA.length);
          sourceInfoLists[i].thumbBGRA.width = captureSourceInfo.thumbBGRA.width;
          sourceInfoLists[i].thumbBGRA.height = captureSourceInfo.thumbBGRA.height;
          sourceInfoLists[i].thumbBGRA.length = captureSourceInfo.thumbBGRA.length;
        }
        Marshal.FreeHGlobal(captureSourceInfo.thumbBGRA.buffer);
        Marshal.FreeHGlobal(captureSourceInfo.iconBGRA.buffer);
        Marshal.FreeHGlobal(captureSourceInfo.sourceName);
      }

      TRTCCloudNative.trtc_cloud_release_screen_capture_sources_list(sourceListPtr);
      return sourceInfoLists;
    }

    // 9.5
    public override TRTCScreenCaptureSourceInfo[] getScreenCaptureSources(int thumbnailWidth,
                                                                          int thumbnailHeight) {
      if (thumbnailWidth <= 0 || thumbnailWidth <= 0) {
        return null;
      }
      var sourceInfoLists = Array.Empty<TRTCScreenCaptureSourceInfo>();
      TRTCSize thumbnail = new TRTCSize();
      TRTCSize icon = new TRTCSize();
      thumbnail.width = thumbnailWidth;
      thumbnail.height = thumbnailHeight;
      icon.width = 20;
      icon.height = 20;
      int count = 0;
      IntPtr sourceListPtr = IntPtr.Zero;
      var ret = TRTCCloudNative.trtc_cloud_get_screen_capture_source_list(
          _nativeObj, thumbnail, icon, ref sourceListPtr, ref count);
      if (count == 0) {
        return sourceInfoLists;
      }

      sourceInfoLists = new TRTCScreenCaptureSourceInfo[count];
      for (var i = 0; i < count; i++) {
        ScreenCaptureSourceInfo captureSourceInfo = new ScreenCaptureSourceInfo();
        captureSourceInfo.sourceName = Marshal.AllocHGlobal(_sourceNameLen);
        captureSourceInfo.thumbBGRA.buffer =
            Marshal.AllocHGlobal(thumbnailWidth * thumbnailHeight * 4);
        captureSourceInfo.iconBGRA.buffer =
            Marshal.AllocHGlobal((int)icon.width * (int)icon.height * 4);

        ret = TRTCCloudNative.trtc_cloud_get_screen_capture_sources_info(sourceListPtr, i,
                                                                         ref captureSourceInfo);
        sourceInfoLists[i].sourceId = captureSourceInfo.sourceId;
        sourceInfoLists[i].type = captureSourceInfo.type;
        sourceInfoLists[i].sourceName = TRTCTypeConverter.NativeUtf8PtrToString(captureSourceInfo.sourceName);
        sourceInfoLists[i].isMainScreen = captureSourceInfo.isMainScreen;
        sourceInfoLists[i].thumbBGRA.buffer = new byte[captureSourceInfo.thumbBGRA.length];
        Marshal.Copy(captureSourceInfo.thumbBGRA.buffer, sourceInfoLists[i].thumbBGRA.buffer, 0,
                     captureSourceInfo.thumbBGRA.length);
        sourceInfoLists[i].thumbBGRA.width = captureSourceInfo.thumbBGRA.width;
        sourceInfoLists[i].thumbBGRA.height = captureSourceInfo.thumbBGRA.height;
        sourceInfoLists[i].thumbBGRA.length = captureSourceInfo.thumbBGRA.length;

        Marshal.FreeHGlobal(captureSourceInfo.thumbBGRA.buffer);
        Marshal.FreeHGlobal(captureSourceInfo.iconBGRA.buffer);
        Marshal.FreeHGlobal(captureSourceInfo.sourceName);
      }

      TRTCCloudNative.trtc_cloud_release_screen_capture_sources_list(sourceListPtr);
      return sourceInfoLists;
    }

    // 9.6
    public override void selectScreenCaptureTarget(TRTCScreenCaptureSourceInfo source,
                                                   Rect captureRect,
                                                   TRTCScreenCaptureProperty property) {
      ScreenCaptureSourceInfo inner_source = new ScreenCaptureSourceInfo();
      inner_source.type = source.type;
      inner_source.sourceId = source.sourceId;
      inner_source.sourceName = TRTCTypeConverter.StringToNativeUtf8Ptr(source.sourceName);
      inner_source.isMinimizeWindow = source.isMinimizeWindow;
      inner_source.isMainScreen = source.isMainScreen;
      inner_source.x = source.x;
      inner_source.y = source.y;
      inner_source.width = source.width;
      inner_source.height = source.height;

      RECT inner_capture_rect = new RECT();
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
      inner_capture_rect.left = (int)captureRect.xMin;
      inner_capture_rect.top = (int)captureRect.yMin;
      inner_capture_rect.right = (int)captureRect.xMax;
      inner_capture_rect.bottom = (int)captureRect.yMax;
#else
      inner_capture_rect.left = (int)captureRect.Left;
      inner_capture_rect.top = (int)captureRect.Top;
      inner_capture_rect.right = (int)captureRect.Right;
      inner_capture_rect.bottom = (int)captureRect.Bottom;
#endif

      // TRTCScreenCaptureProperty
      ScreenCaptureProperty inner_property = new ScreenCaptureProperty();
      inner_property.enableCaptureMouse = property.enableCaptureMouse;
      inner_property.enableHighLight = property.enableHighLight;
      inner_property.enableHighPerformance = property.enableHighPerformance;
      inner_property.highLightColor = property.highLightColor;
      inner_property.highLightWidth = property.highLightWidth;
      inner_property.enableCaptureChildWindow = property.enableCaptureChildWindow;

      TRTCCloudNative.trtc_cloud_select_screen_capture_target(_nativeObj, inner_source,
                                                              inner_capture_rect, inner_property);
      TRTCTypeConverter.SafeFreeHGlobal(ref inner_source.sourceName);
    }

    // 9.7
    public override void setSubStreamEncoderParam(ref TRTCVideoEncParam param) {
      TRTCCloudNative.trtc_cloud_set_sub_stream_encoder_param(
          _nativeObj, param.videoResolution, param.resMode, param.videoFps, param.videoBitrate,
          param.minVideoBitrate, param.enableAdjustRes);
    }

    // 9.8
    public override void setSubStreamMixVolume(uint volume)
    {
      TRTCCloudNative.trtc_cloud_set_sub_stream_mix_volume(_nativeObj, volume);
    }

    // 9.9
    public override void addExcludedShareWindow(IntPtr windowID)
    {
      TRTCCloudNative.trtc_cloud_add_excluded_share_window(_nativeObj, windowID);
    }

    // 9.10
    public override void removeExcludedShareWindow(IntPtr windowID)
    {
      TRTCCloudNative.trtc_cloud_remove_excluded_share_window(_nativeObj, windowID);
    }

    // 9.11
    public override void removeAllExcludedShareWindows()
    {
      TRTCCloudNative.trtc_cloud_remove_all_excluded_share_windows(_nativeObj);
    }

    // 9.12
    public override void addIncludedShareWindow(IntPtr windowID)
    {
      TRTCCloudNative.trtc_cloud_add_included_share_window(_nativeObj, windowID);
    }

    // 9.13
    public override void removeIncludedShareWindow(IntPtr windowID)
    {
      TRTCCloudNative.trtc_cloud_remove_included_share_window(_nativeObj, windowID);
    }

    // 9.14
    public override void removeAllIncludedShareWindows()
    {
      TRTCCloudNative.trtc_cloud_remove_all_included_share_windows(_nativeObj);
    }

    // 10.1
    public override void enableCustomVideoCapture(TRTCVideoStreamType streamType, bool enable) {
      TRTCCloudNative.trtc_cloud_enable_custom_video_capture(_nativeObj, streamType, enable);
    }

    // 10.2
    public override void sendCustomVideoData(TRTCVideoStreamType streamType, TRTCVideoFrame frame) {
      VideoFrame inner_param = new VideoFrame();
      inner_param.videoFormat = frame.videoFormat;
      inner_param.bufferType = frame.bufferType;
      // inner_param.texture = frame.texture;
      inner_param.data = frame.data;

      inner_param.length = frame.length;
      inner_param.width = frame.width;
      inner_param.height = frame.height;
      inner_param.timestamp = frame.timestamp;
      inner_param.rotation = frame.rotation;
      TRTCCloudNative.trtc_cloud_send_custom_video_data(_nativeObj, streamType, inner_param);
      if (inner_param.data != null) {
        Marshal.FreeHGlobal(inner_param.data);
      }
    }

    // 10.3
    public override void enableCustomAudioCapture(bool enable) {
      TRTCCloudNative.trtc_cloud_enable_custom_audio_capture(_nativeObj, enable);
    }

    // 10.4
    public override void sendCustomAudioData(TRTCAudioFrame frame) {
      AudioFrame inner_param = new AudioFrame();
      inner_param.audioFormat = frame.audioFormat;
      if (frame.length > 0) {
        inner_param.data = Marshal.AllocHGlobal((int)frame.length);
        Marshal.Copy(frame.data, 0, inner_param.data, (int)frame.length);
      }

      inner_param.length = frame.length;
      inner_param.sampleRate = frame.sampleRate;
      inner_param.channel = frame.channel;
      inner_param.timestamp = frame.timestamp;
      if (frame.extraDatalength > 0) {
        inner_param.extraData = Marshal.AllocHGlobal((int)frame.extraDatalength);
        Marshal.Copy(frame.extraData, 0, inner_param.extraData, (int)frame.extraDatalength);
      }
      inner_param.extraDatalength = frame.extraDatalength;
      TRTCCloudNative.trtc_cloud_send_custom_audio_data(_nativeObj, inner_param);

      if (inner_param.data != null) {
        Marshal.FreeHGlobal(inner_param.data);
      }
      if (inner_param.extraData != null) {
        Marshal.FreeHGlobal(inner_param.extraData);
      }
    }

    // 10.5
    public override void enableMixExternalAudioFrame(bool enablePublish, bool enablePlayout) {
      TRTCCloudNative.trtc_cloud_enable_mix_external_audio_frame(_nativeObj, enablePublish,
                                                                 enablePlayout);
    }

    // 10.9.1
    public override int enableLocalVideoCustomProcess(bool enable,
                                                      TRTCVideoPixelFormat pixelFormat,
                                                      TRTCVideoBufferType bufferType) {
      return TRTCCloudNative.trtc_cloud_enable_local_video_custom_process(_nativeObj, enable,
                                                                          pixelFormat, bufferType);
    }

    // 10.9.2
    public override void setLocalVideoCustomProcessCallback(ITRTCVideoFrameCallback callback) {
      _wrapperCallback.SetAppVideoFrameCallback(callback);
      TRTCCloudNative.trtc_cloud_set_local_video_custom_process_callback(
          _nativeObj, _wrapperCallback.GetNativeVideoFrameCallback());
    }

    // 10.10
    public override int setLocalVideoRenderCallback(TRTCVideoStreamType streamType,
                                                    TRTCVideoPixelFormat pixelFormat,
                                                    TRTCVideoBufferType bufferType,
                                                    ITRTCVideoRenderCallback callback) {
      var key = new RenderKey("", streamType);
      _wrapperCallback.SetAppVideoRenderCallback(key, callback);
      return TRTCCloudNative.trtc_cloud_set_local_video_render_callback(
          _nativeObj, pixelFormat, bufferType, _wrapperCallback.GetNativeVideoRenderCallback(key));
    }

    public override int setLocalVideoRenderCallback(TRTCVideoPixelFormat pixelFormat,
                                                   TRTCVideoBufferType bufferType,
                                                   ITRTCVideoRenderCallback callback) {
      var key = new RenderKey("");
      _wrapperCallback.SetAppVideoRenderCallback(key, callback);
      return TRTCCloudNative.trtc_cloud_set_local_video_render_callback(
          _nativeObj, pixelFormat, bufferType, _wrapperCallback.GetNativeVideoRenderCallback(key));
    }

    // 10.11
    public override int setRemoteVideoRenderCallback(string userId,
                                                     TRTCVideoStreamType streamType,
                                                     TRTCVideoPixelFormat pixelFormat,
                                                     TRTCVideoBufferType bufferType,
                                                     ITRTCVideoRenderCallback callback) {
      var key = new RenderKey(userId, streamType);
      _wrapperCallback.SetAppVideoRenderCallback(key, callback);
      return TRTCCloudNative.trtc_cloud_set_remote_video_render_callback(
          _nativeObj, userId, pixelFormat, bufferType,
          _wrapperCallback.GetNativeVideoRenderCallback(key));
    }

    public override int setRemoteVideoRenderCallback(string userId,
                                                    TRTCVideoPixelFormat pixelFormat,
                                                    TRTCVideoBufferType bufferType,
                                                    ITRTCVideoRenderCallback callback) {
      var key = new RenderKey(userId);
      _wrapperCallback.SetAppVideoRenderCallback(key, callback);
      return TRTCCloudNative.trtc_cloud_set_remote_video_render_callback(
          _nativeObj, userId, pixelFormat, bufferType,
          _wrapperCallback.GetNativeVideoRenderCallback(key));
    }

    // 10.12
    public override int setAudioFrameCallback(ITRTCAudioFrameCallback callback) {
      _wrapperCallback.SetAppAudioFrameCallback(callback);
      return TRTCCloudNative.trtc_cloud_set_audio_frame_callback(
          _nativeObj, _wrapperCallback.GetNativeAudioFrameCallback());
    }

    // 10.13
    public override int setCapturedAudioFrameCallbackFormat(TRTCAudioFrameCallbackFormat format) {
      return TRTCCloudNative.trtc_cloud_set_captured_audio_frame_callback_format(_nativeObj, format);
    }

    // 10.14
    public override int setLocalProcessedAudioFrameCallbackFormat(TRTCAudioFrameCallbackFormat format) {
      return TRTCCloudNative.trtc_cloud_set_local_processed_audio_frame_callback_format(_nativeObj, format);
    }

    // 10.15
    public override int setMixedPlayAudioFrameCallbackFormat(TRTCAudioFrameCallbackFormat format) {
      return TRTCCloudNative.trtc_cloud_set_mixed_play_audio_frame_callback_format(_nativeObj, format);
    }

    // 11.1
    public override bool sendCustomCmdMsg(int cmdId,
                                          byte[] data,
                                          int dataSize,
                                          bool reliable,
                                          bool ordered) {
      if (data == null || dataSize == 0) {
        return false;
      }

      int ret = TRTCCloudNative.trtc_cloud_send_sustom_cmd_msg(_nativeObj, cmdId, data, dataSize,
                                                               reliable, ordered);
      return ret != 0;
    }

    // 11.2
    public override bool sendSEIMsg(byte[] data, int dataSize, int repeatCount) {
      if (data == null || dataSize == 0) {
        return false;
      }

      int ret = TRTCCloudNative.trtc_cloud_send_sei_msg(_nativeObj, data, dataSize, repeatCount);
      return ret != 0;
    }

    // 12.1
    public override void startSpeedTest(TRTCSpeedTestParams testParams) {
      TRTCCloudNative.trtc_cloud_start_speed_test(_nativeObj, testParams);
    }

    // 12.2
    public override void stopSpeedTest() {
      TRTCCloudNative.trtc_cloud_stop_speed_test(_nativeObj);
    }

    // 13.0
    public override string getScriptVersion() {
      return PLUGIN_VERSION;
    }

    // 13.1
    public override string getSDKVersion() {
      return Marshal.PtrToStringAnsi(TRTCCloudNative.trtc_cloud_get_sdk_version(_nativeObj));
    }

    // 13.2
    public override void setLogLevel(TRTCLogLevel level) {
      _logParam.level = level;
      TRTCCloudNative.trtc_cloud_set_log_param(_nativeObj, _logParam);
    }

    // 13.3
    public override void setConsoleEnabled(bool enabled) {
      _logParam.consoleEnabled = enabled;
      TRTCCloudNative.trtc_cloud_set_log_param(_nativeObj, _logParam);
    }

    // 13.4
    public override void setLogCompressEnabled(bool enabled) {
      _logParam.compressEnabled = enabled;
      TRTCCloudNative.trtc_cloud_set_log_param(_nativeObj, _logParam);
    }

    // 13.5
    public override void setLogDirPath(string path) {
      _logParam.path = path;
      TRTCCloudNative.trtc_cloud_set_log_param(_nativeObj, _logParam);
    }

    // 13.6
    public override void setLogCallback(ITRTCLogCallback callback) {
      _wrapperCallback.SetAppLogCallback(callback);
      TRTCCloudNative.trtc_cloud_set_log_callback(_nativeObj,
                                                  _wrapperCallback.GetNativeLogCallback());
    }

    // 13.9
    public override void callExperimentalAPI(string jsonStr) {
      TRTCCloudNative.trtc_cloud_call_experimental_api(_nativeObj, jsonStr);
    }

    [Obsolete]
    public override void muteLocalVideo(bool mute) {
      muteLocalVideo(TRTCVideoStreamType.TRTCVideoStreamTypeBig, mute);
    }

    [Obsolete]
    public override void enableAudioVolumeEvaluation(uint interval) {
      var evaluateParams = new TRTCAudioVolumeEvaluateParams { interval = interval };
      enableAudioVolumeEvaluation(true, evaluateParams);
    }

    [Obsolete]
    public override void startSpeedTest(int sdkAppId, string userId, string userSig) {
      var testParams =
          new TRTCSpeedTestParams { sdkAppId = sdkAppId, userId = userId, userSig = userSig };
      startSpeedTest(testParams);
    }

    [Obsolete]
    public override void sendCustomVideoData(TRTCVideoFrame frame) {
      sendCustomVideoData(TRTCVideoStreamType.TRTCVideoStreamTypeBig, frame);
    }
  }
}