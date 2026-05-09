using System;
using System.Runtime.InteropServices;

namespace trtc {
  public class TRTCCloudCallbackNative : TRTCBaseNative {
    // 1
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnErrorHandler(IntPtr instance,
                                        TXLiteAVError errCode,
                                        string errMsg,
                                        IntPtr extraInfo,
                                        IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnWarningHandler(IntPtr instance,
                                          TXLiteAVWarning warningCode,
                                          string warningMsg,
                                          IntPtr extraInfo,
                                          IntPtr userData);
    // 2
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnEnterRoomHandler(IntPtr instance, int result, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnExitRoomHandler(IntPtr instance, int reason, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnSwitchRoleHandler(IntPtr instance,
                                             TXLiteAVError errCode,
                                             string errMsg,
                                             IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnSwitchRoomHandler(IntPtr instance,
                                             TXLiteAVError errCode,
                                             string errMsg,
                                             IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnConnectOtherRoomHandler(IntPtr instance,
                                                   string userId,
                                                   TXLiteAVError errCode,
                                                   string errMsg,
                                                   IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnDisconnectOtherRoomHandler(IntPtr instance,
                                                      TXLiteAVError errCode,
                                                      string errMsg,
                                                      IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnUpdateOtherRoomForwardModeHandler(IntPtr instance,
                                                             TXLiteAVError errCode,
                                                             string errMsg,
                                                             IntPtr userData);

    // 3
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRemoteUserEnterRoomHandler(IntPtr instance,
                                                      string userID,
                                                      IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRemoteUserLeaveRoomHandler(IntPtr instance,
                                                      string userID,
                                                      int reason,
                                                      IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnUserVideoAvailableHandler(IntPtr instance,
                                                     string userID,
                                                     [MarshalAs(UnmanagedType.U1)] bool available,
                                                     IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnUserSubStreamAvailableHandler(IntPtr instance,
                                                         string userID,
                                                         [MarshalAs(UnmanagedType.U1)] bool available,
                                                         IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnUserAudioAvailableHandler(IntPtr instance,
                                                     string userID,
                                                     [MarshalAs(UnmanagedType.U1)] bool available,
                                                     IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnFirstVideoFrameHandler(IntPtr instance,
                                                  string userID,
                                                  TRTCVideoStreamType streamType,
                                                  int width,
                                                  int height,
                                                  IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnFirstAudioFrameHandler(IntPtr instance, string userID, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnSendFirstLocalVideoFrameHandler(IntPtr instance,
                                                           TRTCVideoStreamType streamType,
                                                           IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnSendFirstLocalAudioFrameHandler(IntPtr instance, IntPtr userData);

    // 4
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnNetworkQualityHandler(IntPtr instance,
                                                 string strLocalQuality,
                                                 string strRemoteQuality,
                                                 IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnStatisticsHandler(IntPtr instance,
                                             string strStatistics,
                                             IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnSpeedTestResultHandler(IntPtr instance,
                                                  string strTestResult,
                                                  IntPtr userData);

    // 5
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnConnectionLostHandler(IntPtr instance, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnTryToReconnectHandler(IntPtr instance, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnConnectionRecoveryHandler(IntPtr instance, IntPtr userData);

    // 6
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnCameraDidReadyHandler(IntPtr instance, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnMicDidReadyHandler(IntPtr instance, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnAudioRouteChangedHandler(IntPtr instance,
                                                    TRTCAudioRoute newRoute,
                                                    TRTCAudioRoute oldRoute,
                                                    IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnUserVoiceVolumeHandler(IntPtr instance,
                                                  string userVolumes,
                                                  uint totalVolume,
                                                  IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnDeviceChangeHandler(IntPtr instance,
                                               string deviceID,
                                               TRTCDeviceType type,
                                               TRTCDeviceState state,
                                               IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnAudioDeviceCaptureVolumeChangedHandler(IntPtr instance,
                                                                  uint volume,
                                                                  [MarshalAs(UnmanagedType.U1)] bool muted,
                                                                  IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnAudioDevicePlayoutVolumeChangedHandler(IntPtr instance,
                                                                  uint volume,
                                                                  [MarshalAs(UnmanagedType.U1)] bool muted,
                                                                  IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnTestMicVolumeHandler(IntPtr instance, uint volume, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnTestSpeakerVolumeHandler(IntPtr instance, uint volume, IntPtr userData);

    // 7
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRecvCustomCmdMsgHandler(IntPtr instance,
                                                   IntPtr userID,
                                                   int cmdID,
                                                   int seq,
                                                   IntPtr message,
                                                   int messageSize,
                                                   IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnMissCustomCmdMsgHandler(IntPtr instance,
                                                   IntPtr userID,
                                                   int cmdID,
                                                   int errCode,
                                                   int missed,
                                                   IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRecvSEIMsgHandler(IntPtr instance,
                                             IntPtr userID,
                                             IntPtr message,
                                             uint messageSize,
                                             IntPtr userData);

    // 8.1
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnStartPublishingHandler(IntPtr instance,
                                                  int err,
                                                  string errMsg,
                                                  IntPtr userData);
    // 8.2
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnStopPublishingHandler(IntPtr instance,
                                                 int err,
                                                 string errMsg,
                                                 IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnSetMixTranscodingConfigHandler(IntPtr instance,
                                                          int err,
                                                          string errMsg,
                                                          IntPtr userData);

    // 8.6
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnStartPublishMediaStreamHandler(IntPtr instance,
                                                          string taskID,
                                                          int code,
                                                          string message,
                                                          string extraInfo,
                                                          IntPtr userData);

    // 8.7
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnUpdatePublishMediaStreamHandler(IntPtr instance,
                                                           string taskID,
                                                           int code,
                                                           string message,
                                                           string extraInfo,
                                                           IntPtr userData);

    // 8.8
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnStopPublishMediaStreamHandler(IntPtr instance,
                                                         string taskID,
                                                         int code,
                                                         string message,
                                                         string extraInfo,
                                                         IntPtr userData);

    // 8.9
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnCdnStreamStateChangedHandler(IntPtr instance,
                                                        string cdnURL,
                                                        int status,
                                                        int code,
                                                        string message,
                                                        string extraInfo,
                                                        IntPtr userData);

    // 9
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnScreenCaptureStartedHandler(IntPtr instance, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnScreenCapturePausedHandler(IntPtr instance, int reason, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnScreenCaptureResumedHandler(IntPtr instance,
                                                       int reason,
                                                       IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnScreenCaptureStopedHandler(IntPtr instance, int reason, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnSnapshotCompleteHandler(IntPtr instance,
                                                   IntPtr userId,
                                                   TRTCVideoStreamType type,
                                                   IntPtr data,
                                                   uint length,
                                                   uint width,
                                                   uint height,
                                                   TRTCVideoPixelFormat format,
                                                   IntPtr userData);

    // main_handler
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_error_handler(IntPtr instance,
                                                              OnErrorHandler ErrorHandler,
                                                              IntPtr userDataPtr);
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_warning_handler(IntPtr instance,
                                                                OnWarningHandler WarningHandler,
                                                                IntPtr userDataPtr);

    // room_handler
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_enter_room_handler(IntPtr instance,
                                                                   OnEnterRoomHandler onEnterRoom,
                                                                   IntPtr userDataPtr);
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_exit_room_handler(IntPtr instance,
                                                                  OnExitRoomHandler onExitRoom,
                                                                  IntPtr userDataPtr);
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_switch_role_handler(
        IntPtr instance,
        OnSwitchRoleHandler onSwitchRole,
        IntPtr userDataPtr);
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_switch_room_handler(
        IntPtr instance,
        OnSwitchRoomHandler onSwitchRoom,
        IntPtr userDataPtr);
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_connect_other_room_handler(
        IntPtr instance,
        OnConnectOtherRoomHandler onConnectOtherRoom,
        IntPtr userDataPtr);
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_disconnect_other_room_handler(
        IntPtr instance,
        OnDisconnectOtherRoomHandler onDisconnectOtherRoom,
        IntPtr userDataPtr);
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_update_other_room_forward_mode_handler(
        IntPtr instance,
        OnUpdateOtherRoomForwardModeHandler onUpdateOtherRoomForwardMode,
        IntPtr userDataPtr);

    // user_handler
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_remote_user_enter_room_handler(
        IntPtr instance,
        OnRemoteUserEnterRoomHandler onRemoteUserEnterRoom,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_remote_user_leave_room_handler(
        IntPtr instance,
        OnRemoteUserLeaveRoomHandler onRemoteUserLeaveRoom,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_user_video_available_handler(
        IntPtr instance,
        OnUserVideoAvailableHandler onUserVideoAvailable,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_user_sub_stream_available_handler(
        IntPtr instance,
        OnUserSubStreamAvailableHandler onUserSubStreamAvailable,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_user_audio_available_handler(
        IntPtr instance,
        OnUserAudioAvailableHandler onUserAudioAvailable,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_first_video_frame_handler(
        IntPtr instance,
        OnFirstVideoFrameHandler onFirstVideoFrame,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_first_audio_frame_handler(
        IntPtr instance,
        OnFirstAudioFrameHandler onFirstAudioFrame,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_send_first_local_video_frame_handler(
        IntPtr instance,
        OnSendFirstLocalVideoFrameHandler onSendFirstLocalVideoFrame,
        IntPtr userDataPtr);


    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_send_first_local_audio_frame_handler(
        IntPtr instance,
        OnSendFirstLocalAudioFrameHandler onSendFirstLocalAudioFrame,
        IntPtr userDataPtr);


    // net_stat_handler
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_network_quality_handler(
        IntPtr instance,
        OnNetworkQualityHandler onNetworkQuality,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_statistics_handler(IntPtr instance,
                                                                   OnStatisticsHandler onStatistics,
                                                                   IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_speed_test_result_handler(
        IntPtr instance,
        OnSpeedTestResultHandler onSpeedTestResult,
        IntPtr userDataPtr);

    // connect_handler
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_connection_lost_handler(
        IntPtr instance,
        OnConnectionLostHandler onConnectionLost,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_try_to_reconnect_handler(
        IntPtr instance,
        OnTryToReconnectHandler onTryToReconnect,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_connection_recovery_handler(
        IntPtr instance,
        OnConnectionRecoveryHandler onConnectionRecovery,
        IntPtr userDataPtr);

    // hardware_handler
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_camera_did_ready_handler(
        IntPtr instance,
        OnCameraDidReadyHandler onCameraDidReady,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_mic_did_ready_handler(
        IntPtr instance,
        OnMicDidReadyHandler onMicDidReady,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_audio_route_changed_handler(
        IntPtr instance,
        OnAudioRouteChangedHandler onAudioRouteChanged,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_user_voice_volume_handler(
        IntPtr instance,
        OnUserVoiceVolumeHandler onUserVoiceVolume,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_device_change_handler(
        IntPtr instance,
        OnDeviceChangeHandler onDeviceChange,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_test_mic_volume_handler(
        IntPtr instance,
        OnTestMicVolumeHandler onTestMicVolume,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_test_speaker_volume_handler(
        IntPtr instance,
        OnTestSpeakerVolumeHandler onTestSpeakerVolume,
        IntPtr userDataPtr);

    // custom_msg_handler
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_recv_custom_cmd_msg_handler(
        IntPtr instance,
        OnRecvCustomCmdMsgHandler onRecvCustromCmdMsg,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_miss_custom_cmd_msg_handler(
        IntPtr instance,
        OnMissCustomCmdMsgHandler onMissCustromCmdMsg,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_recv_sei_msg_handler(
        IntPtr instance,
        OnRecvSEIMsgHandler onRecvSeiMsg,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_start_publishing_handler(
        IntPtr instance,
        OnStartPublishingHandler onStartPublishing,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_stop_publishing_handler(
        IntPtr instance,
        OnStopPublishingHandler onStopPublishing,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_set_mix_transcoding_config_handler(
        IntPtr instance,
        OnSetMixTranscodingConfigHandler onSetMixTranscodingConfig,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_start_publish_media_stream_handler(
        IntPtr instance,
        OnStartPublishMediaStreamHandler onStartPublishMediaStream,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_update_publish_media_stream_handler(
        IntPtr instance,
        OnUpdatePublishMediaStreamHandler onUpdatePublishMediaStream,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_stop_publish_media_stream_handler(
        IntPtr instance,
        OnStopPublishMediaStreamHandler onStopPublishMediaStream,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_cdn_stream_state_changed_handler(
        IntPtr instance,
        OnCdnStreamStateChangedHandler onCdnStreamStateChanged,
        IntPtr userDataPtr);

    // screen_share_handler
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_screen_capture_started_handler(
        IntPtr instance,
        OnScreenCaptureStartedHandler onScreenCaptureStarted,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_screen_capture_paused_handler(
        IntPtr instance,
        OnScreenCapturePausedHandler onScreenCapturePaused,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_screen_capture_resumed_handler(
        IntPtr instance,
        OnScreenCaptureResumedHandler onScreenCaptureResumed,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_screen_capture_stoped_handler(
        IntPtr instance,
        OnScreenCaptureStopedHandler onScreenCaptureStoped,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_on_snapshot_complete_handler(
        IntPtr instance,
        OnSnapshotCompleteHandler onSnapshotCompleted,
        IntPtr userDataPtr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr trtc_cloud_create_cloud_callback(IntPtr instance);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_reset_all_handler(IntPtr instance);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_destroy_cloud_callback(IntPtr callback);
  }

  public class TRTCVideoRenderCallbackNative {
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRenderVideoFrameHandler(IntPtr instance,
                                                   string userID,
                                                   TRTCVideoStreamType streamType,
                                                   ref VideoFrame videoFrame);
  }

  public class TRTCVideoFrameCallbackNative {
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnGLContextCreatedHandler(IntPtr instance);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int OnProcessVideoFrameHandler(IntPtr instance,
                                                   ref VideoFrame src_frame,
                                                   ref VideoFrame dst_frame);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnGLContextDestroyHandler(IntPtr instance);
  }

  public class TRTCAudioFrameCallbackNative {
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnCapturedAudioFrameHandler(IntPtr instance, ref AudioFrame audioFrame);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnLocalProcessedAudioFrameHandler(IntPtr instance, ref AudioFrame frame);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnPlayAudioFrameHandler(IntPtr instance,
                                                 ref AudioFrame frame,
                                                 string userId);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnMixedPlayAudioFrameHandler(IntPtr instance, ref AudioFrame frame);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnMixedAllAudioFrameHandler(IntPtr instance, ref AudioFrame frame);
  }

  public class TRTCLogCallbackNative {
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnLogHandler(IntPtr instance,
                                      string log,
                                      TRTCLogLevel level,
                                      string module);
  }
}