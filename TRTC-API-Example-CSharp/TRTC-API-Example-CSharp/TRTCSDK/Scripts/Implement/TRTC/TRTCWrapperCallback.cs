// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan

using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Linq;
using System.Runtime.InteropServices;
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
using AOT;
using UnityEngine;
#else
using System.Diagnostics;
#endif

namespace trtc {
  public struct RenderKey {
    public string UserId;
    public TRTCVideoStreamType StreamType;
    public bool UserDefaultStreamType;
    public RenderKey(string userId, TRTCVideoStreamType streamType) {
      UserId = userId;
      StreamType = streamType;
      UserDefaultStreamType = false;
    }
    public RenderKey(string userId) {
      UserId = userId;
      StreamType = TRTCVideoStreamType.TRTCVideoStreamTypeBig;
      UserDefaultStreamType = true;
    }
  }
  public class TRTCWrapperCallback {
    #region DataModel

    [Serializable]
    public struct QualityArrayInfo {
      public TRTCQualityInfo[] remoteQualityArray;
    }

    [Serializable]
    public struct VolumeArrayInfo {
      public TRTCVolumeInfo[] userVolumesArray;
    }

    #endregion

    #region CloudCallback

    private readonly List<ITRTCCloudCallback> _cloudCallbackList = new List<ITRTCCloudCallback>();
    private readonly object _callBackListLock = new object();

    public void AddAppCloudCallback(ITRTCCloudCallback callback) {
      lock (_callBackListLock) {
        _cloudCallbackList.Add(callback);
      }
    }

    public void RemoveAppCloudCallback(ITRTCCloudCallback callback) {
      lock (_callBackListLock) {
        _cloudCallbackList.Remove(callback);
      }
    }

    public void ClearAppCloudCallback() {
      lock (_callBackListLock) {
        _cloudCallbackList.Clear();
      }
    }

    public List<ITRTCCloudCallback> GetAppCloudCallback() {
      List<ITRTCCloudCallback> cloudCallbackList = null;
      lock (_callBackListLock) {
        cloudCallbackList = new List<ITRTCCloudCallback>(_cloudCallbackList);
      }
      return cloudCallbackList;
    }

    internal TRTCCallbackObj GetCallbackObj() {
      return _cloudCallbackObj;
    }

    #endregion

    #region VideoFrame

    private ITRTCVideoFrameCallback _videoFrameCallback;

    public void SetAppVideoFrameCallback(ITRTCVideoFrameCallback callback) {
      _videoFrameCallback = callback;
    }

    public void ClearAppVideoFrameCallback() {
      _videoFrameCallback = null;
    }

    public ITRTCVideoFrameCallback GetAppVideoFrameCallback() {
      return _videoFrameCallback;
    }

    public IntPtr GetNativeVideoFrameCallback() {
      return _videoFrameCallback == null ? IntPtr.Zero : _nativeVideoFrameCallback;
    }

    #endregion

    #region VideRender

    private readonly ConcurrentDictionary<RenderKey, ITRTCVideoRenderCallback> _videoRenderCallbackMap =
        new ConcurrentDictionary<RenderKey, ITRTCVideoRenderCallback>();

    public void SetAppVideoRenderCallback(RenderKey key, ITRTCVideoRenderCallback callback) {
      if (callback == null) {
        if (_videoRenderCallbackMap.ContainsKey(key)) {
          ITRTCVideoRenderCallback outCallback;
          _videoRenderCallbackMap.TryRemove(key, out outCallback);
        }
      }
      else {
        _videoRenderCallbackMap[key] = callback;
      }
    }

    public void ClearAppVideoRenderCallback() {
      _videoRenderCallbackMap.Clear();
    }

    public ITRTCVideoRenderCallback GetAppVideoRenderCallback(RenderKey key) {
      ITRTCVideoRenderCallback retCallback;
      _videoRenderCallbackMap.TryGetValue(key, out retCallback);
      return retCallback;
    }

    private bool HasOtherStreamRendering(string userID) {
      var keys = _videoRenderCallbackMap.Keys;
      for (var i = 0; i < _videoRenderCallbackMap.Count; i++) {
        var key = keys.ElementAt(i);
        if (key.UserId == userID) {
          return true;
        }
      }

      return false;
    }

    public IntPtr GetNativeVideoRenderCallback(RenderKey key) {
      if (HasOtherStreamRendering(key.UserId)) {
        return _nativeVideoRenderCallback;
      }

      return _videoRenderCallbackMap.ContainsKey(key) ? _nativeVideoRenderCallback : IntPtr.Zero;
    }

    #endregion

    #region AudioFrame

    private ITRTCAudioFrameCallback _audioFrameCallback;

    public void SetAppAudioFrameCallback(ITRTCAudioFrameCallback audioFrameCallback) {
      _audioFrameCallback = audioFrameCallback;
    }

    public void ClearAppAudioFrameCallback() {
      _audioFrameCallback = null;
    }

    public ITRTCAudioFrameCallback GetAppAudioFrameCallback() {
      return _audioFrameCallback;
    }

    public IntPtr GetNativeAudioFrameCallback() {
      return _audioFrameCallback == null ? IntPtr.Zero : _nativeAudioFrameCallback;
    }

    #endregion

    #region Log

    private ITRTCLogCallback _logCallback;

    public void SetAppLogCallback(ITRTCLogCallback logCallback) {
      _logCallback = logCallback;
    }

    public void ClearAppLogCallback() {
      _logCallback = null;
    }

    public ITRTCLogCallback GetAppLogCallback() {
      return _logCallback;
    }

    public IntPtr GetNativeLogCallback() {
      return _logCallback == null ? IntPtr.Zero : _nativeLogCallback;
    }

    #endregion

    private IntPtr _nativeObj;
    private TRTCCallbackObj _cloudCallbackObj;
    private IntPtr _nativeCloudCallback;
    private IntPtr _nativeVideoFrameCallback;
    private IntPtr _nativeVideoRenderCallback;
    private IntPtr _nativeAudioFrameCallback;
    private IntPtr _nativeLogCallback;

    public TRTCWrapperCallback(IntPtr nativeObj) {
      _nativeObj = nativeObj;
      TRTCLogger.Info("create trtc cloud callback", "TRTCWrapperCallback");
      _nativeCloudCallback = TRTCCloudCallbackNative.trtc_cloud_create_cloud_callback(_nativeObj);
      // main_handler
      TRTCCloudCallbackNative.trtc_cloud_set_on_error_handler(
          _nativeObj, CloudCallback._OnErrorHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_warning_handler(
          _nativeObj, CloudCallback._OnWarningHandlerRef, IntPtr.Zero);

      // room_handler
      TRTCCloudCallbackNative.trtc_cloud_set_on_enter_room_handler(
          _nativeObj, CloudCallback._OnEnterRoomHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_exit_room_handler(
          _nativeObj, CloudCallback._OnExitRoomHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_switch_role_handler(
          _nativeObj, CloudCallback._OnSwitchRoleHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_switch_room_handler(
          _nativeObj, CloudCallback._OnSwitchRoomHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_connect_other_room_handler(
          _nativeObj, CloudCallback._OnConnectOtherRoomHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_disconnect_other_room_handler(
          _nativeObj, CloudCallback._OnDisconnectOtherRoomHandlerRef, IntPtr.Zero);

      // user_handler
      TRTCCloudCallbackNative.trtc_cloud_set_on_remote_user_enter_room_handler(
          _nativeObj, CloudCallback._OnRemoteUserEnterRoomHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_remote_user_leave_room_handler(
          _nativeObj, CloudCallback._OnRemoteUserLeaveRoomHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_user_video_available_handler(
          _nativeObj, CloudCallback._OnUserVideoAvailableHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_user_sub_stream_available_handler(
          _nativeObj, CloudCallback._OnUserSubStreamAvailableHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_user_audio_available_handler(
          _nativeObj, CloudCallback._OnUserAudioAvailableHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_first_video_frame_handler(
          _nativeObj, CloudCallback._OnFirstVideoFrameHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_first_audio_frame_handler(
          _nativeObj, CloudCallback._OnFirstAudioFrameHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_send_first_local_video_frame_handler(
        _nativeObj, CloudCallback._OnSendFirstLocalVideoFrameHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_send_first_local_audio_frame_handler(
        _nativeObj, CloudCallback._OnSendFirstLocalAudioFrameHandlerRef, IntPtr.Zero);

      // net_stat_handler
      TRTCCloudCallbackNative.trtc_cloud_set_on_network_quality_handler(
          _nativeObj, CloudCallback._OnNetworkQualityHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_statistics_handler(
          _nativeObj, CloudCallback._OnStatisticsHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_speed_test_result_handler(
          _nativeObj, CloudCallback._OnSpeedTestResultHandlerRef, IntPtr.Zero);

      // connect_handler
      TRTCCloudCallbackNative.trtc_cloud_set_on_connection_lost_handler(
          _nativeObj, CloudCallback._OnConnectionLostHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_try_to_reconnect_handler(
          _nativeObj, CloudCallback._OnTryToReconnectHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_connection_recovery_handler(
          _nativeObj, CloudCallback._OnConnectionRecoveryHandlerRef, IntPtr.Zero);

      // hardware_handler
      TRTCCloudCallbackNative.trtc_cloud_set_on_camera_did_ready_handler(
          _nativeObj, CloudCallback._OnCameraDidReadyHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_mic_did_ready_handler(
          _nativeObj, CloudCallback._OnMicDidReadyHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_audio_route_changed_handler(
          _nativeObj, CloudCallback._OnAudioRouteChangedHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_user_voice_volume_handler(
          _nativeObj, CloudCallback._OnUserVoiceVolumeHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_device_change_handler(
          _nativeObj, CloudCallback._OnDeviceChangeHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_test_mic_volume_handler(
          _nativeObj, CloudCallback._OnTestMicVolumeHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_test_speaker_volume_handler(
          _nativeObj, CloudCallback._OnTestSpeakerVolumeHandlerRef, IntPtr.Zero);

      // custom_msg_handler
      TRTCCloudCallbackNative.trtc_cloud_set_on_recv_custom_cmd_msg_handler(
          _nativeObj, CloudCallback._OnRecvCustomCmdMsgHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_miss_custom_cmd_msg_handler(
          _nativeObj, CloudCallback._OnMissCustomCmdMsgHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_recv_sei_msg_handler(
          _nativeObj, CloudCallback._OnRecvSEIMsgHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_start_publishing_handler(
          _nativeObj, CloudCallback._OnStartPublishingHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_stop_publishing_handler(
          _nativeObj, CloudCallback._OnStopPublishingHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_set_mix_transcoding_config_handler(
          _nativeObj, CloudCallback._OnSetMixTranscodingConfigHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_start_publish_media_stream_handler(
          _nativeObj, CloudCallback._OnStartPublishMediaStreamHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_update_publish_media_stream_handler(
          _nativeObj, CloudCallback._OnUpdatePublishMediaStreamHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_stop_publish_media_stream_handler(
          _nativeObj, CloudCallback._OnStopPublishMediaStreamHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_cdn_stream_state_changed_handler(
          _nativeObj, CloudCallback._OnCdnStreamStateChangedHandlerRef, IntPtr.Zero);

      // screen_share_handler
      TRTCCloudCallbackNative.trtc_cloud_set_on_screen_capture_started_handler(
          _nativeObj, CloudCallback._OnScreenCaptureStartedHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_screen_capture_paused_handler(
          _nativeObj, CloudCallback._OnScreenCapturePausedHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_screen_capture_resumed_handler(
          _nativeObj, CloudCallback._OnScreenCaptureResumedHandlerRef, IntPtr.Zero);
      TRTCCloudCallbackNative.trtc_cloud_set_on_screen_capture_stoped_handler(
          _nativeObj, CloudCallback._OnScreenCaptureStopedHandlerRef, IntPtr.Zero);

      TRTCCloudCallbackNative.trtc_cloud_set_on_snapshot_complete_handler(
          _nativeObj, CloudCallback._OnSnapshotCompleteHandlerRef, IntPtr.Zero);

      _cloudCallbackObj =
          new TRTCCallbackObj("TRTCCloudNative_CallbackObj_" + _nativeObj.GetHashCode());

      _nativeVideoFrameCallback = TRTCCloudNative.trtc_cloud_create_video_frame_callback(
          _nativeObj, VideoFrameCallback._OnGLContextCreatedRef,
          VideoFrameCallback._OnProcessVideoFrameRef,
          VideoFrameCallback._OnGLContextDestroyRef);

      _nativeVideoRenderCallback = TRTCCloudNative.trtc_cloud_create_video_render_callback(
          _nativeObj, VideoRenderCallback._OnRenderVideoFrameRef);

      _nativeAudioFrameCallback = TRTCCloudNative.trtc_cloud_create_audio_frame_callback(
          _nativeObj, AudioFrameCallback._OnCapturedAudioFrameRef,
          AudioFrameCallback._OnLocalProcessedAudioFrameRef,
          AudioFrameCallback._OnPlayAudioFrameRef,
          AudioFrameCallback._OnMixedPlayAudioFrameRef,
          AudioFrameCallback._OnMixedAllAudioFrameRef);

      _nativeLogCallback =
          TRTCCloudNative.trtc_cloud_create_log_callback(_nativeObj, LogCallback._OnLogRef);
    }

    ~TRTCWrapperCallback() {
      TRTCLogger.Info("destory native callback", "~TRTCWrapperCallback");
      TRTCCloudCallbackNative.trtc_cloud_destroy_cloud_callback(_nativeCloudCallback);
      _nativeCloudCallback = IntPtr.Zero;

      TRTCCloudNative.trtc_cloud_destroy_video_frame_callback(_nativeVideoFrameCallback);
      _nativeVideoFrameCallback = IntPtr.Zero;

      TRTCCloudNative.trtc_cloud_destroy_video_render_callback(_nativeVideoRenderCallback);
      _nativeVideoRenderCallback = IntPtr.Zero;

      TRTCCloudNative.trtc_cloud_destroy_audio_frame_callback(_nativeAudioFrameCallback);
      _nativeAudioFrameCallback = IntPtr.Zero;

      TRTCCloudNative.trtc_cloud_destroy_log_callback(_nativeLogCallback);

      _nativeLogCallback = IntPtr.Zero;
      _nativeObj = IntPtr.Zero;
    }

    private void ResetTrtcNativeCallback() {
      TRTCLogger.Info("reset trtc native callback");
      TRTCCloudCallbackNative.trtc_cloud_reset_all_handler(_nativeObj);
      TRTCCloudNative.trtc_cloud_reset_video_frame_callback(_nativeVideoFrameCallback);
      TRTCCloudNative.trtc_cloud_reset_video_render_callback(_nativeVideoRenderCallback);
      TRTCCloudNative.trtc_cloud_reset_audio_frame_callback(_nativeAudioFrameCallback);
      TRTCCloudNative.trtc_cloud_reset_log_callback(_nativeLogCallback);
    }

    public void OnDestroy() {
      TRTCLogger.Info();
      ResetTrtcNativeCallback();
      ClearAppCloudCallback();
      ClearAppVideoFrameCallback();
      ClearAppVideoRenderCallback();
      ClearAppAudioFrameCallback();
      ClearAppLogCallback();
      TRTCCallbackObj.Destroy(_cloudCallbackObj);
      _cloudCallbackObj = null;
    }

  }

  static class CloudCallback {
    public static readonly TRTCCloudCallbackNative.OnErrorHandler _OnErrorHandlerRef = OnErrorHandler;
    public static readonly TRTCCloudCallbackNative.OnWarningHandler _OnWarningHandlerRef = OnWarningHandler;
    public static readonly TRTCCloudCallbackNative.OnEnterRoomHandler _OnEnterRoomHandlerRef = OnEnterRoomHandler;
    public static readonly TRTCCloudCallbackNative.OnExitRoomHandler _OnExitRoomHandlerRef = OnExitRoomHandler;
    public static readonly TRTCCloudCallbackNative.OnSwitchRoleHandler _OnSwitchRoleHandlerRef = OnSwitchRoleHandler;
    public static readonly TRTCCloudCallbackNative.OnSwitchRoomHandler _OnSwitchRoomHandlerRef = OnSwitchRoomHandler;
    public static readonly TRTCCloudCallbackNative.OnConnectOtherRoomHandler _OnConnectOtherRoomHandlerRef = OnConnectOtherRoomHandler;
    public static readonly TRTCCloudCallbackNative.OnDisconnectOtherRoomHandler _OnDisconnectOtherRoomHandlerRef = OnDisconnectOtherRoomHandler;
    public static readonly TRTCCloudCallbackNative.OnRemoteUserEnterRoomHandler _OnRemoteUserEnterRoomHandlerRef = OnRemoteUserEnterRoomHandler;
    public static readonly TRTCCloudCallbackNative.OnRemoteUserLeaveRoomHandler _OnRemoteUserLeaveRoomHandlerRef = OnRemoteUserLeaveRoomHandler;
    public static readonly TRTCCloudCallbackNative.OnUserVideoAvailableHandler _OnUserVideoAvailableHandlerRef = OnUserVideoAvailableHandler;
    public static readonly TRTCCloudCallbackNative.OnUserSubStreamAvailableHandler _OnUserSubStreamAvailableHandlerRef = OnUserSubStreamAvailableHandler;
    public static readonly TRTCCloudCallbackNative.OnUserAudioAvailableHandler _OnUserAudioAvailableHandlerRef = OnUserAudioAvailableHandler;
    public static readonly TRTCCloudCallbackNative.OnFirstVideoFrameHandler _OnFirstVideoFrameHandlerRef = OnFirstVideoFrameHandler;
    public static readonly TRTCCloudCallbackNative.OnFirstAudioFrameHandler _OnFirstAudioFrameHandlerRef = OnFirstAudioFrameHandler;
    public static readonly TRTCCloudCallbackNative.OnSendFirstLocalVideoFrameHandler _OnSendFirstLocalVideoFrameHandlerRef = OnSendFirstLocalVideoFrameHandler;
    public static readonly TRTCCloudCallbackNative.OnSendFirstLocalAudioFrameHandler _OnSendFirstLocalAudioFrameHandlerRef = OnSendFirstLocalAudioFrameHandler;
    public static readonly TRTCCloudCallbackNative.OnNetworkQualityHandler _OnNetworkQualityHandlerRef = OnNetworkQualityHandler;
    public static readonly TRTCCloudCallbackNative.OnStatisticsHandler _OnStatisticsHandlerRef = OnStatisticsHandler;
    public static readonly TRTCCloudCallbackNative.OnSpeedTestResultHandler _OnSpeedTestResultHandlerRef = OnSpeedTestResultHandler;
    public static readonly TRTCCloudCallbackNative.OnConnectionLostHandler _OnConnectionLostHandlerRef = OnConnectionLostHandler;
    public static readonly TRTCCloudCallbackNative.OnTryToReconnectHandler _OnTryToReconnectHandlerRef = OnTryToReconnectHandler;
    public static readonly TRTCCloudCallbackNative.OnConnectionRecoveryHandler _OnConnectionRecoveryHandlerRef = OnConnectionRecoveryHandler;
    public static readonly TRTCCloudCallbackNative.OnCameraDidReadyHandler _OnCameraDidReadyHandlerRef = OnCameraDidReadyHandler;
    public static readonly TRTCCloudCallbackNative.OnMicDidReadyHandler _OnMicDidReadyHandlerRef = OnMicDidReadyHandler;
    public static readonly TRTCCloudCallbackNative.OnAudioRouteChangedHandler _OnAudioRouteChangedHandlerRef = OnAudioRouteChangedHandler;
    public static readonly TRTCCloudCallbackNative.OnUserVoiceVolumeHandler _OnUserVoiceVolumeHandlerRef = OnUserVoiceVolumeHandler;
    public static readonly TRTCCloudCallbackNative.OnDeviceChangeHandler _OnDeviceChangeHandlerRef = OnDeviceChangeHandler;
    public static readonly TRTCCloudCallbackNative.OnAudioDeviceCaptureVolumeChangedHandler _OnAudioDeviceCaptureVolumeChangedHandlerRef = OnAudioDeviceCaptureVolumeChangedHandler;
    public static readonly TRTCCloudCallbackNative.OnAudioDevicePlayoutVolumeChangedHandler _OnAudioDevicePlayoutVolumeChangedHandlerRef = OnAudioDevicePlayoutVolumeChangedHandler;
    public static readonly TRTCCloudCallbackNative.OnTestMicVolumeHandler _OnTestMicVolumeHandlerRef = OnTestMicVolumeHandler;
    public static readonly TRTCCloudCallbackNative.OnTestSpeakerVolumeHandler _OnTestSpeakerVolumeHandlerRef = OnTestSpeakerVolumeHandler;
    public static readonly TRTCCloudCallbackNative.OnRecvCustomCmdMsgHandler _OnRecvCustomCmdMsgHandlerRef = OnRecvCustomCmdMsgHandler;
    public static readonly TRTCCloudCallbackNative.OnMissCustomCmdMsgHandler _OnMissCustomCmdMsgHandlerRef = OnMissCustomCmdMsgHandler;
    public static readonly TRTCCloudCallbackNative.OnRecvSEIMsgHandler _OnRecvSEIMsgHandlerRef = OnRecvSEIMsgHandler;
    public static readonly TRTCCloudCallbackNative.OnStartPublishingHandler _OnStartPublishingHandlerRef = OnStartPublishingHandler;
    public static readonly TRTCCloudCallbackNative.OnStopPublishingHandler _OnStopPublishingHandlerRef = OnStopPublishingHandler;
    public static readonly TRTCCloudCallbackNative.OnSetMixTranscodingConfigHandler _OnSetMixTranscodingConfigHandlerRef = OnSetMixTranscodingConfigHandler;
    public static readonly TRTCCloudCallbackNative.OnStartPublishMediaStreamHandler _OnStartPublishMediaStreamHandlerRef = OnStartPublishMediaStreamHandler;
    public static readonly TRTCCloudCallbackNative.OnUpdatePublishMediaStreamHandler _OnUpdatePublishMediaStreamHandlerRef = OnUpdatePublishMediaStreamHandler;
    public static readonly TRTCCloudCallbackNative.OnStopPublishMediaStreamHandler _OnStopPublishMediaStreamHandlerRef = OnStopPublishMediaStreamHandler;
    public static readonly TRTCCloudCallbackNative.OnCdnStreamStateChangedHandler _OnCdnStreamStateChangedHandlerRef = OnCdnStreamStateChangedHandler;
    public static readonly TRTCCloudCallbackNative.OnScreenCaptureStartedHandler _OnScreenCaptureStartedHandlerRef = OnScreenCaptureStartedHandler;
    public static readonly TRTCCloudCallbackNative.OnScreenCapturePausedHandler _OnScreenCapturePausedHandlerRef = OnScreenCapturePausedHandler;
    public static readonly TRTCCloudCallbackNative.OnScreenCaptureResumedHandler _OnScreenCaptureResumedHandlerRef = OnScreenCaptureResumedHandler;
    public static readonly TRTCCloudCallbackNative.OnScreenCaptureStopedHandler _OnScreenCaptureStopedHandlerRef = OnScreenCaptureStopedHandler;
    public static readonly TRTCCloudCallbackNative.OnSnapshotCompleteHandler _OnSnapshotCompleteHandlerRef = OnSnapshotCompleteHandler;

    private static List<ITRTCCloudCallback> QueryCallbacks(IntPtr instance,
                                                           ref TRTCCallbackObj callbackObj) {
      if (instance == IntPtr.Zero) {
        return new List<ITRTCCloudCallback>();
      }

      var cloudImpl = CloudManager.FindCloudImplement(instance);
      if (cloudImpl == null) {
        return new List<ITRTCCloudCallback>();
      }

      callbackObj = cloudImpl.GetCallback()?.GetCallbackObj();
      return cloudImpl.GetCallback()?.GetAppCloudCallback();
    }

    // 1
    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnErrorHandler))]
    public static void OnErrorHandler(IntPtr instance,
                                      TXLiteAVError errCode,
                                      string errMsg,
                                      IntPtr extraInfo,
                                      IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onError(errCode, errMsg, extraInfo);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnWarningHandler))]
    public static void OnWarningHandler(IntPtr instance,
                                        TXLiteAVWarning warningCode,
                                        string warningMsg,
                                        IntPtr extraInfo,
                                        IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onWarning(warningCode, warningMsg, extraInfo);
        }
      }, true);
    }

    // 2
    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnEnterRoomHandler))]
    public static void OnEnterRoomHandler(IntPtr instance, int result, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onEnterRoom(result);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnExitRoomHandler))]
    public static void OnExitRoomHandler(IntPtr instance, int reason, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onExitRoom(reason);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnSwitchRoleHandler))]
    public static void OnSwitchRoleHandler(IntPtr instance,
                                           TXLiteAVError errCode,
                                           string errMsg,
                                           IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onSwitchRole(errCode, errMsg);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnSwitchRoomHandler))]
    public static void OnSwitchRoomHandler(IntPtr instance,
                                           TXLiteAVError errCode,
                                           string errMsg,
                                           IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onSwitchRoom(errCode, errMsg);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnConnectOtherRoomHandler))]
    public static void OnConnectOtherRoomHandler(IntPtr instance,
                                                 string userId,
                                                 TXLiteAVError errCode,
                                                 string errMsg,
                                                 IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onConnectOtherRoom(userId, errCode, errMsg);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnDisconnectOtherRoomHandler))]
    public static void OnDisconnectOtherRoomHandler(IntPtr instance,
                                                    TXLiteAVError errCode,
                                                    string errMsg,
                                                    IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (ITRTCCloudCallback callback in callbacks) {
          callback.onDisconnectOtherRoom(errCode, errMsg);
        }
      });
    }

    // 3
    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnRemoteUserEnterRoomHandler))]
    public static void OnRemoteUserEnterRoomHandler(IntPtr instance,
                                                    string userId,
                                                    IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onRemoteUserEnterRoom(userId);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnRemoteUserLeaveRoomHandler))]
    public static void OnRemoteUserLeaveRoomHandler(IntPtr instance,
                                                    string userId,
                                                    int reason,
                                                    IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onRemoteUserLeaveRoom(userId, reason);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnUserVideoAvailableHandler))]
    public static void OnUserVideoAvailableHandler(IntPtr instance,
                                                   string userId,
                                                   bool available,
                                                   IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onUserVideoAvailable(userId, available);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnUserSubStreamAvailableHandler))]
    public static void OnUserSubStreamAvailableHandler(IntPtr instance,
                                                       string userId,
                                                       bool available,
                                                       IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onUserSubStreamAvailable(userId, available);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnUserAudioAvailableHandler))]
    public static void OnUserAudioAvailableHandler(IntPtr instance,
                                                   string userId,
                                                   bool available,
                                                   IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onUserAudioAvailable(userId, available);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnFirstVideoFrameHandler))]
    public static void OnFirstVideoFrameHandler(IntPtr instance,
                                                string userId,
                                                TRTCVideoStreamType streamType,
                                                int width,
                                                int height,
                                                IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onFirstVideoFrame(userId, (TRTCVideoStreamType)streamType, width, height);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnFirstAudioFrameHandler))]
    public static void OnFirstAudioFrameHandler(IntPtr instance, string userId, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onFirstAudioFrame(userId);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnSendFirstLocalVideoFrameHandler))]
    public static void OnSendFirstLocalVideoFrameHandler(IntPtr instance,
                                                         TRTCVideoStreamType streamType,
                                                         IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onSendFirstLocalVideoFrame((TRTCVideoStreamType)streamType);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnSendFirstLocalAudioFrameHandler))]
    public static void OnSendFirstLocalAudioFrameHandler(IntPtr instance, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onSendFirstLocalAudioFrame();
        }
      });
    }

    // 4

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnNetworkQualityHandler))]
    public static void OnNetworkQualityHandler(IntPtr instance,
                                               string strLocalQuality,
                                               string strRemoteQuality,
                                               IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        TRTCQualityInfo localQuality;
        TRTCWrapperCallback.QualityArrayInfo remoteQuality;
        try {
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
          localQuality = JsonUtility.FromJson<TRTCQualityInfo>(strLocalQuality);
          remoteQuality = JsonUtility.FromJson<TRTCWrapperCallback.QualityArrayInfo>(strRemoteQuality);
#else
          localQuality = Newtonsoft.Json.JsonConvert.DeserializeObject<TRTCQualityInfo>(strLocalQuality);
          remoteQuality = Newtonsoft.Json.JsonConvert.DeserializeObject<TRTCWrapperCallback.QualityArrayInfo>(strRemoteQuality);
#endif
        }
        catch (System.Exception exception) {
          TRTCLogger.Error("Exception caught while OnNetworkQualityHandler: " + exception);
          return;
        }

        foreach (var callback in callbacks) {
          uint remoteQualityArrayLen = 0;
          if (remoteQuality.remoteQualityArray != null) {
            remoteQualityArrayLen = (uint)remoteQuality.remoteQualityArray.Length;
          }

          callback.onNetworkQuality(localQuality, remoteQuality.remoteQualityArray,
                                    remoteQualityArrayLen);
        }
      }, true);
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnStatisticsHandler))]
    public static void OnStatisticsHandler(IntPtr instance, string strStatistics, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        TRTCStatistics statistics;
        try {
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
          statistics = JsonUtility.FromJson<TRTCStatistics>(strStatistics);
#else
          statistics = Newtonsoft.Json.JsonConvert.DeserializeObject<TRTCStatistics>(strStatistics);
#endif
        }
        catch (System.Exception exception) {
          TRTCLogger.Error("Exception caught while OnStatisticsHandler: " + exception);
          return;
        }
        foreach (var callback in callbacks) {
          callback.onStatistics(statistics);
        }
      }, true);
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnSpeedTestResultHandler))]
    public static void OnSpeedTestResultHandler(IntPtr instance,
                                                string strTestResult,
                                                IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        TRTCSpeedTestResult testResult;
        try {
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
          testResult = JsonUtility.FromJson<TRTCSpeedTestResult>(strTestResult);
#else
          testResult = Newtonsoft.Json.JsonConvert.DeserializeObject<TRTCSpeedTestResult>(strTestResult);
#endif
        }
        catch (System.Exception exception) {
          TRTCLogger.Error("Exception caught while OnSpeedTestResultHandler: " + exception);
          return;
        }
        foreach (var callback in callbacks) {
          callback.onSpeedTestResult(testResult);
        }
      });
    }

    // 5
    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnConnectionLostHandler))]
    public static void OnConnectionLostHandler(IntPtr instance, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onConnectionLost();
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnTryToReconnectHandler))]
    public static void OnTryToReconnectHandler(IntPtr instance, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onTryToReconnect();
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnConnectionRecoveryHandler))]
    public static void OnConnectionRecoveryHandler(IntPtr instance, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onConnectionRecovery();
        }
      });
    }

    // 6
    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnCameraDidReadyHandler))]
    public static void OnCameraDidReadyHandler(IntPtr instance, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onCameraDidReady();
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnMicDidReadyHandler))]
    public static void OnMicDidReadyHandler(IntPtr instance, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onMicDidReady();
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnAudioRouteChangedHandler))]
    public static void OnAudioRouteChangedHandler(IntPtr instance,
                                                  TRTCAudioRoute newRoute,
                                                  TRTCAudioRoute oldRoute,
                                                  IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onAudioRouteChanged(newRoute, oldRoute);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnUserVoiceVolumeHandler))]
    public static void OnUserVoiceVolumeHandler(IntPtr instance,
                                                string strUserVolumes,
                                                uint totalVolume,
                                                IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        TRTCWrapperCallback.VolumeArrayInfo userVolumeArrayInfo;
        try {
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
          userVolumeArrayInfo = JsonUtility.FromJson<TRTCWrapperCallback.VolumeArrayInfo>(strUserVolumes);
#else
          userVolumeArrayInfo = Newtonsoft.Json.JsonConvert.DeserializeObject<TRTCWrapperCallback.VolumeArrayInfo>(strUserVolumes);
#endif
        }
        catch (System.Exception exception) {
          TRTCLogger.Error("Exception caught while OnUserVoiceVolumeHandler: " + exception);
          return;
        }

        var userVolumeInfo = userVolumeArrayInfo.userVolumesArray;
        var userVolumesCount = userVolumeInfo?.Length ?? 0;
        foreach (var callback in callbacks) {
          callback.onUserVoiceVolume(userVolumeInfo, (uint)userVolumesCount, totalVolume);
        }
      }, true);
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnDeviceChangeHandler))]
    public static void OnDeviceChangeHandler(IntPtr instance,
                                             string deviceId,
                                             TRTCDeviceType type,
                                             TRTCDeviceState state,
                                             IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onDeviceChange(deviceId, type, state);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnAudioDeviceCaptureVolumeChangedHandler))]
    public static void OnAudioDeviceCaptureVolumeChangedHandler(IntPtr instance,
                                                                uint volume,
                                                                bool muted,
                                                                IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onAudioDeviceCaptureVolumeChanged((int)volume, muted);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnAudioDevicePlayoutVolumeChangedHandler))]
    public static void OnAudioDevicePlayoutVolumeChangedHandler(IntPtr instance,
                                                                uint volume,
                                                                bool muted,
                                                                IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onAudioDevicePlayoutVolumeChanged((int)volume, muted);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnTestMicVolumeHandler))]
    public static void OnTestMicVolumeHandler(IntPtr instance, uint volume, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onTestMicVolume((int)volume);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnTestSpeakerVolumeHandler))]
    public static void OnTestSpeakerVolumeHandler(IntPtr instance, uint volume, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onTestSpeakerVolume((int)volume);
        }
      });
    }

    // 7
    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnRecvCustomCmdMsgHandler))]
    public static void OnRecvCustomCmdMsgHandler(IntPtr instance,
                                                 IntPtr userId,
                                                 int cmdID,
                                                 int seq,
                                                 IntPtr message,
                                                 int messageSize,
                                                 IntPtr userData) {
      if (message == IntPtr.Zero || messageSize <= 0) {
        TRTCLogger.Error("OnRecvCustomCmdMsgHandler: messageSize: " + messageSize);
        return;
      }
      string userIdStr = TRTCTypeConverter.NativeUtf8PtrToString(userId);
      var data = new byte[messageSize];
      Marshal.Copy(message, data, 0, messageSize);
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onRecvCustomCmdMsg(userIdStr, cmdID, seq, data, messageSize);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnMissCustomCmdMsgHandler))]
    public static void OnMissCustomCmdMsgHandler(IntPtr instance,
                                                 IntPtr userId,
                                                 int cmdID,
                                                 int errCode,
                                                 int missed,
                                                 IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      string userIdStr = TRTCTypeConverter.NativeUtf8PtrToString(userId);
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onMissCustomCmdMsg(userIdStr, cmdID, errCode, missed);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnRecvSEIMsgHandler))]
    public static void OnRecvSEIMsgHandler(IntPtr instance,
                                           IntPtr userId,
                                           IntPtr message,
                                           UInt32 messageSize,
                                           IntPtr userData) {
      if (message == IntPtr.Zero || messageSize <= 0) {
        TRTCLogger.Error("OnRecvSEIMsgHandler: messageSize:" + messageSize);
        return;
      }
      string userIdStr = TRTCTypeConverter.NativeUtf8PtrToString(userId);
      var data = new byte[messageSize];
      Marshal.Copy(message, data, 0, (int)messageSize);
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onRecvSEIMsg(userIdStr, data, messageSize);
        }
      });
    }

    // 8
    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnStartPublishingHandler))]
    public static void OnStartPublishingHandler(IntPtr instance,
                                                int err,
                                                string errMsg,
                                                IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onStartPublishing(err, errMsg);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnStopPublishingHandler))]
    public static void OnStopPublishingHandler(IntPtr instance,
                                               int err,
                                               string errMsg,
                                               IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onStopPublishing(err, errMsg);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnSetMixTranscodingConfigHandler))]
    public static void OnSetMixTranscodingConfigHandler(IntPtr instance,
                                                        int err,
                                                        string errMsg,
                                                        IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onSetMixTranscodingConfig(err, errMsg);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnStartPublishMediaStreamHandler))]
    public static void OnStartPublishMediaStreamHandler(IntPtr instance,
                                                        string taskID,
                                                        int code,
                                                        string message,
                                                        string extraInfo,
                                                        IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (ITRTCCloudCallback callback in callbacks) {
          callback.onStartPublishMediaStream(taskID, code, message, extraInfo);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnUpdatePublishMediaStreamHandler))]
    public static void OnUpdatePublishMediaStreamHandler(IntPtr instance,
                                                         string taskID,
                                                         int code,
                                                         string message,
                                                         string extraInfo,
                                                         IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (ITRTCCloudCallback callback in callbacks) {
          callback.onUpdatePublishMediaStream(taskID, code, message, extraInfo);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnStopPublishMediaStreamHandler))]
    public static void OnStopPublishMediaStreamHandler(IntPtr instance,
                                                       string taskID,
                                                       int code,
                                                       string message,
                                                       string extraInfo,
                                                       IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (ITRTCCloudCallback callback in callbacks) {
          callback.onStopPublishMediaStream(taskID, code, message, extraInfo);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnCdnStreamStateChangedHandler))]
    public static void OnCdnStreamStateChangedHandler(IntPtr instance,
                                                      string cdnURL,
                                                      int status,
                                                      int code,
                                                      string message,
                                                      string extraInfo,
                                                      IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (ITRTCCloudCallback callback in callbacks) {
          callback.onCdnStreamStateChanged(cdnURL, status, code, message, extraInfo);
        }
      });
    }

    // 9
    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnScreenCaptureStartedHandler))]
    public static void OnScreenCaptureStartedHandler(IntPtr instance, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onScreenCaptureStarted();
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnScreenCapturePausedHandler))]
    public static void OnScreenCapturePausedHandler(IntPtr instance, int reason, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onScreenCapturePaused(reason);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnScreenCaptureResumedHandler))]
    public static void OnScreenCaptureResumedHandler(IntPtr instance, int reason, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onScreenCaptureResumed(reason);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnScreenCaptureStopedHandler))]
    public static void OnScreenCaptureStopedHandler(IntPtr instance, int reason, IntPtr userData) {
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onScreenCaptureStoped(reason);
        }
      });
    }

    [MonoPInvokeCallback(typeof(TRTCCloudCallbackNative.OnSnapshotCompleteHandler))]
    public static void OnSnapshotCompleteHandler(IntPtr instance,
                                                    IntPtr userId,
                                                    TRTCVideoStreamType type,
                                                    IntPtr data,
                                                    uint length,
                                                    uint width,
                                                    uint height,
                                                    TRTCVideoPixelFormat format,
                                                    IntPtr userData) {
      byte[] dataByte = null;
      if (data == IntPtr.Zero || length <= 0) {
        TRTCLogger.Warning("OnSnapshotCompleteHandler: length:" + length);
        length = 0;
      }
      else {
        dataByte = new byte[length];
        try {
          Marshal.Copy(data, dataByte, 0, (int)length);
        }
        catch (AccessViolationException ex) {
          TRTCLogger.Error("OnSnapshotCompleteHandler: Marshal.Copy access violation - " + ex.Message);
          dataByte = null;
          length = 0;
        }
        catch (Exception ex) {
          TRTCLogger.Error("OnSnapshotCompleteHandler: Marshal.Copy error - " + ex.Message);
          dataByte = null;
          length = 0;
        }
      }
      TRTCCallbackObj callbackObj = null;
      var callbacks = QueryCallbacks(instance, ref callbackObj);
      var userIdStr = TRTCTypeConverter.NativeUtf8PtrToString(userId);
      callbackObj?.GetActionQueue().Enqueue(() => {
        foreach (var callback in callbacks) {
          callback.onSnapshotComplete(userIdStr, type, dataByte, length, width, height, format);
        }
      });
    }
  }


  public static class VideoFrameCallback {
    public static readonly TRTCVideoFrameCallbackNative.OnGLContextCreatedHandler _OnGLContextCreatedRef = OnGLContextCreatedHandler;
    public static readonly TRTCVideoFrameCallbackNative.OnProcessVideoFrameHandler _OnProcessVideoFrameRef = OnProcessVideoFrameHandler;
    public static readonly TRTCVideoFrameCallbackNative.OnGLContextDestroyHandler _OnGLContextDestroyRef = OnGLContextDestroyHandler;
    //  todo 老版本未用到，暂不实现
    [MonoPInvokeCallback(typeof(TRTCVideoFrameCallbackNative.OnGLContextCreatedHandler))]
    public static void OnGLContextCreatedHandler(IntPtr instance) { }

    [MonoPInvokeCallback(typeof(TRTCVideoFrameCallbackNative.OnProcessVideoFrameHandler))]
    public static int OnProcessVideoFrameHandler(IntPtr instance,
                                                 ref VideoFrame src_frame,
                                                 ref VideoFrame dst_frame) {
      return 0;
    }

    [MonoPInvokeCallback(typeof(TRTCVideoFrameCallbackNative.OnGLContextDestroyHandler))]
    public static void OnGLContextDestroyHandler(IntPtr instance) { }
  }

  public static class VideoRenderCallback {
    public static readonly TRTCVideoRenderCallbackNative.OnRenderVideoFrameHandler _OnRenderVideoFrameRef = OnRenderVideoFrameHandler;
    private static ITRTCVideoRenderCallback QueryCallbacks(IntPtr instance, RenderKey key) {
      if (instance == IntPtr.Zero) {
        return null;
      }

      var cloudImpl = CloudManager.FindCloudImplement(instance);
      return cloudImpl?.GetCallback()?.GetAppVideoRenderCallback(key);
    }

    [MonoPInvokeCallback(typeof(TRTCVideoRenderCallbackNative.OnRenderVideoFrameHandler))]
    public static void OnRenderVideoFrameHandler(IntPtr instance,
                                                 string userID,
                                                 TRTCVideoStreamType streamType,
                                                 ref VideoFrame frame) {
      if (frame.data == IntPtr.Zero || frame.length <= 0) {
        TRTCLogger.Error("OnRenderVideoFrameHandler: frame.length:" + frame.length);
        return;
      }

      var videoFrame =
          new TRTCVideoFrame { videoFormat = frame.videoFormat, bufferType = frame.bufferType };
      videoFrame.data = frame.data;

      videoFrame.length = frame.length;
      videoFrame.width = frame.width;
      videoFrame.height = frame.height;
      videoFrame.timestamp = frame.timestamp;
      // videoFrame.textureId = textureId;
      videoFrame.rotation = frame.rotation;

      var key = new RenderKey(userID, streamType);
      var callback = QueryCallbacks(instance, key);
      if (callback != null) {
        callback.onRenderVideoFrame(userID, streamType, videoFrame);
      }

      var keyWithUserID = new RenderKey(userID);
      var newCallback = QueryCallbacks(instance, keyWithUserID);
      if (newCallback != null) {
        newCallback.onRenderVideoFrame(userID, streamType, videoFrame);
      }
    }
  }

  public static class AudioFrameCallback {
    public static readonly TRTCAudioFrameCallbackNative.OnCapturedAudioFrameHandler _OnCapturedAudioFrameRef = OnCapturedAudioFrameHandler;
    public static readonly TRTCAudioFrameCallbackNative.OnLocalProcessedAudioFrameHandler _OnLocalProcessedAudioFrameRef = OnLocalProcessedAudioFrameHandler;
    public static readonly TRTCAudioFrameCallbackNative.OnPlayAudioFrameHandler _OnPlayAudioFrameRef = OnPlayAudioFrameHandler;
    public static readonly TRTCAudioFrameCallbackNative.OnMixedPlayAudioFrameHandler _OnMixedPlayAudioFrameRef = OnMixedPlayAudioFrameHandler;
    public static readonly TRTCAudioFrameCallbackNative.OnMixedAllAudioFrameHandler _OnMixedAllAudioFrameRef = OnMixedAllAudioFrameHandler;
    private static ITRTCAudioFrameCallback QueryCallbacks(IntPtr instance) {
      if (instance == IntPtr.Zero) {
        return null;
      }

      var cloudImpl = CloudManager.FindCloudImplement(instance);
      return cloudImpl?.GetCallback()?.GetAppAudioFrameCallback();
    }

    [MonoPInvokeCallback(typeof(TRTCAudioFrameCallbackNative.OnCapturedAudioFrameHandler))]
    public static void OnCapturedAudioFrameHandler(IntPtr instance, ref AudioFrame audioFrame) {
      if (audioFrame.data == IntPtr.Zero || audioFrame.length <= 0) {
        TRTCLogger.Error("OnCapturedAudioFrameHandler: audioFrame.length:" + audioFrame.length);
        return;
      }

      var callback = QueryCallbacks(instance);
      if (callback == null) {
        return;
      }

      var frame = new TRTCAudioFrame {
        audioFormat = audioFrame.audioFormat,
        data = new byte[audioFrame.length]
      };
      Marshal.Copy(audioFrame.data, frame.data, 0, (int)audioFrame.length);
      frame.sampleRate = audioFrame.sampleRate;
      frame.channel = audioFrame.channel;
      frame.timestamp = audioFrame.timestamp;
      frame.length = audioFrame.length;

      callback.onCapturedRawAudioFrame(frame);

      Marshal.Copy(frame.data, 0, audioFrame.data, (int)audioFrame.length);
      frame.data = null;
    }

    [MonoPInvokeCallback(typeof(TRTCAudioFrameCallbackNative.OnLocalProcessedAudioFrameHandler))]
    public static void OnLocalProcessedAudioFrameHandler(IntPtr instance,
                                                         ref AudioFrame audioFrame) {
      if (audioFrame.data == IntPtr.Zero || audioFrame.length <= 0) {
        TRTCLogger.Error("OnLocalProcessedAudioFrameHandler: audioFrame.length:" + audioFrame.length);
        return;
      }
      var callback = QueryCallbacks(instance);
      if (callback == null) {
        return;
      }

      TRTCAudioFrame frame = new TRTCAudioFrame();
      frame.audioFormat = audioFrame.audioFormat;
      frame.data = new byte[audioFrame.length];
      Marshal.Copy(audioFrame.data, frame.data, 0, (int)audioFrame.length);
      frame.sampleRate = audioFrame.sampleRate;
      frame.channel = audioFrame.channel;
      frame.timestamp = audioFrame.timestamp;
      frame.length = audioFrame.length;

      callback.onLocalProcessedAudioFrame(frame);

      Marshal.Copy(frame.data, 0, audioFrame.data, (int)audioFrame.length);
      frame.data = null;
    }

    [MonoPInvokeCallback(typeof(TRTCAudioFrameCallbackNative.OnPlayAudioFrameHandler))]
    public static void OnPlayAudioFrameHandler(IntPtr instance,
                                               ref AudioFrame audioFrame,
                                               string userId) {
      if (audioFrame.data == IntPtr.Zero || audioFrame.length <= 0) {
        TRTCLogger.Error("OnLocalProcessedAudioFrameHandler: audioFrame.length:" + audioFrame.length);
        return;
      }

      var callback = QueryCallbacks(instance);
      if (callback == null) {
        return;
      }

      var frame = new TRTCAudioFrame {
        audioFormat = audioFrame.audioFormat,
        data = new byte[audioFrame.length]
      };
      Marshal.Copy(audioFrame.data, frame.data, 0, (int)audioFrame.length);
      frame.sampleRate = audioFrame.sampleRate;
      frame.channel = audioFrame.channel;
      frame.timestamp = audioFrame.timestamp;
      frame.length = audioFrame.length;

      callback.onPlayAudioFrame(frame, userId);

      Marshal.Copy(frame.data, 0, audioFrame.data, (int)audioFrame.length);
      frame.data = null;
    }

    [MonoPInvokeCallback(typeof(TRTCAudioFrameCallbackNative.OnMixedPlayAudioFrameHandler))]
    public static void OnMixedPlayAudioFrameHandler(IntPtr instance, ref AudioFrame audioFrame) {
      if (audioFrame.data == IntPtr.Zero || audioFrame.length <= 0) {
        TRTCLogger.Error("OnMixedPlayAudioFrameHandler: audioFrame.length:" + audioFrame.length);
        return;
      }

      var callback = QueryCallbacks(instance);
      if (callback == null) {
        return;
      }

      var frame = new TRTCAudioFrame {
        audioFormat = audioFrame.audioFormat,
        data = new byte[audioFrame.length]
      };
      Marshal.Copy(audioFrame.data, frame.data, 0, (int)audioFrame.length);
      frame.sampleRate = audioFrame.sampleRate;
      frame.channel = audioFrame.channel;
      frame.timestamp = audioFrame.timestamp;
      frame.length = audioFrame.length;

      callback.onMixedPlayAudioFrame(frame);

      Marshal.Copy(frame.data, 0, audioFrame.data, (int)audioFrame.length);
      frame.data = null;
    }

    [MonoPInvokeCallback(typeof(TRTCAudioFrameCallbackNative.OnMixedAllAudioFrameHandler))]
    public static void OnMixedAllAudioFrameHandler(IntPtr instance, ref AudioFrame audioFrame) { }
  }

  public static class LogCallback {
    public static readonly TRTCLogCallbackNative.OnLogHandler _OnLogRef = OnLogHandler;
    private static ITRTCLogCallback QueryCallbacks(IntPtr instance) {
      if (instance == IntPtr.Zero) {
        return null;
      }

      var cloudImpl = CloudManager.FindCloudImplement(instance);
      return cloudImpl?.GetCallback()?.GetAppLogCallback();
    }

    [MonoPInvokeCallback(typeof(TRTCLogCallbackNative.OnLogHandler))]
    public static void OnLogHandler(IntPtr instance,
                                    string log,
                                    TRTCLogLevel level,
                                    string module) {
      var callback = QueryCallbacks(instance);
      if (callback != null) {
        callback.onLog(log, level, module);
      }
    }
  }
}