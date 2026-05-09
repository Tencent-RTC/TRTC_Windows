// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan

using System;
using System.Runtime.InteropServices;
#if !(UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL)
using System.Windows;
#endif

namespace trtc {
#if UNITY_IOS
  public class IosExtensionLauncher {
    [DllImport("__Internal")]
    public static extern void TRTCUnityExtensionLauncher();
  }
#endif

  public class TRTCCloudNative : TRTCBaseNative {
    // 1.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr trtc_cloud_get_instance(IntPtr context);

    // 1.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_destroy_instance(IntPtr context);

    // 2.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_enter_room(IntPtr context,
                                                    TRTCParamsInner param,
                                                    TRTCAppScene scene);

    // 2.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_exit_room(IntPtr instance);

    // 2.3
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_switch_role(IntPtr instance, TRTCRoleType role);

    // 2.5
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_switch_room(IntPtr instance, TRTCSwitchRoomConfig config);

    // 2.6
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_connect_other_room(IntPtr instance, [
      MarshalAs(UnmanagedType.LPStr)
    ] string jsonparam);

    // 2.7
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_disconnect_other_room(IntPtr instance);

    // 2.8
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_default_stream_recv_mode(IntPtr instance,
                                                                      bool autoRecvAudio,
                                                                      bool autoRecvVideo);

    // 2.9
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr trtc_cloud_create_sub_cloud(IntPtr instance);

    // 2.10
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_destroy_sub_cloud(IntPtr instance, IntPtr subInstance);

    // 3.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_start_publishing(IntPtr instance,
                                                          string streamId,
                                                          TRTCVideoStreamType config);

    // 3.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_stop_publishing(IntPtr instance);

    // 3.5
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_mix_transcoding_config(IntPtr instance, IntPtr config);

    // 3.6
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_start_publish_media_stream(IntPtr instance,
                                                                    ref PublishTarget target,
                                                                    ref StreamEncoderParam param,
                                                                    ref StreamMixingConfig config);
    // 3.7
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_update_publish_media_stream(IntPtr instance,
                                                                    string taskId,
                                                                    ref PublishTarget target,
                                                                    ref StreamEncoderParam param,
                                                                    ref StreamMixingConfig config);

    // 3.8
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_stop_publish_media_stream(IntPtr instance, string taskId);

    // 4.1 4.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_start_local_preview(IntPtr instance,
                                                             bool frontCamera,
                                                             IntPtr view);
    // 4.4
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_stop_local_preview(IntPtr instance);

    // 4.5
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_mute_local_video(IntPtr instance,
                                                          TRTCVideoStreamType streamType,
                                                          bool mute);

    // 4.7
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_start_remote_view(
        IntPtr instance,
        [MarshalAs(UnmanagedType.LPStr)] string userId,
        TRTCVideoStreamType streamType,
        IntPtr view);

    // 4.9
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_stop_remote_view(
        IntPtr instance,
        [MarshalAs(UnmanagedType.LPStr)] string userId,
        TRTCVideoStreamType streamType);

    // 4.10
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_stop_all_remote_view(IntPtr instance);

    // 4.11
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_mute_remote_video_stream(IntPtr instance,
        [MarshalAs(UnmanagedType.LPStr)] string userId,
        TRTCVideoStreamType streamType,
        bool mute);

    // 4.12
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_mute_all_remote_video_streams(IntPtr instance, bool mute);

    // 4.13
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_video_encoder_param(IntPtr instance,
                                                                 TRTCVideoEncParam param);

    // 4.14
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_network_qos_param(IntPtr instance,
                                                               TRTCNetworkQosParam param);

    // 4.15
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_local_render_params(IntPtr instance,
                                                                 TRTCVideoRotation rotation,
                                                                 TRTCVideoFillMode fillMode,
                                                                 TRTCVideoMirrorType mirrorType);

    // 4.16
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_remote_render_params(
        IntPtr instance,
        [MarshalAs(UnmanagedType.LPStr)] string userId,
        TRTCVideoStreamType streamType,
        TRTCRenderParams param);

    // 4.17
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_video_encoder_rotation(IntPtr instance,
                                                                    TRTCVideoRotation rotation);

    // 4.18
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_video_encoder_mirror(IntPtr instance, bool mirror);

    // 4.20
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_enable_small_video_stream(IntPtr instance,
                                                                   bool enable,
                                                                   TRTCVideoEncParam param);

    // 4.21
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_remote_video_stream_type(
        IntPtr instance,
        [MarshalAs(UnmanagedType.LPStr)] string userId,
        TRTCVideoStreamType type);

    // 4.22
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_snapshot_video(IntPtr instance,
        IntPtr userId,
        TRTCVideoStreamType streamType, TRTCSnapshotSourceType sourceType);

    // 5.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_start_local_audio(IntPtr instance,
                                                           TRTCAudioQuality quality);

    // 5.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_stop_local_audio(IntPtr instance);

    // 5.3
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_mute_local_audio(IntPtr instance, bool mute);

    // 5.4
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_mute_remote_audio(
        IntPtr instance,
        [MarshalAs(UnmanagedType.LPStr)] string userId,
        bool mute);

    // 5.5
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_mute_all_remote_audio(IntPtr instance, bool mute);

    // 5.7
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_remote_audio_volume(
        IntPtr instance,
        [MarshalAs(UnmanagedType.LPStr)] string userId,
        int volume);

    // 5.8
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_audio_capture_volume(IntPtr instance, int volume);

    // 5.9
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_get_audio_capture_volume(IntPtr instance);

    // 5.10
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_audio_playout_volume(IntPtr instance, int volume);

    // 5.11
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_get_audio_playout_volume(IntPtr instance);

    // 5.12
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_enable_audio_volume_evaluation(
        IntPtr instance,
        bool enable,
        TRTCAudioVolumeEvaluateParams param);

    // 5.15
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_start_local_recording(IntPtr instance,
                                                               TRTCLocalRecordingParams param);

    // 5.16
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_stop_local_recording(IntPtr instance);

    // 5.17
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_gravity_sensor_adaptive_mode(IntPtr instance,
                                                               TRTCGravitySensorAdaptiveMode mode);

    // 6.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr trtc_cloud_get_device_manager(IntPtr instance);

    // 7.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_beauty_style(IntPtr instance,
                                                          TRTCBeautyStyle style,
                                                          uint beauty,
                                                          uint white,
                                                          uint ruddiness);

    // 7.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_water_mark(
        IntPtr instance,
        TRTCVideoStreamType streamType,
        [MarshalAs(UnmanagedType.LPStr)] string srcData,
        TRTCWaterMarkSrcType srcType,
        uint nWidth,
        uint nHeight,
        float xOffset,
        float yOffset,
        float fWidthRatio,
        bool isVisibleOnLocalPreview = false);

    // 8.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr trtc_cloud_get_audio_effect_manager(IntPtr instance);

    // 8.2  UNITY_STANDALONE_OSX || UNITY_STANDALONE_WIN
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_start_system_audio_loopback(IntPtr instance, [
      MarshalAs(UnmanagedType.LPStr)
    ] string deviceName);

    // 8.3  UNITY_STANDALONE_OSX || UNITY_STANDALONE_WIN
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_stop_system_audio_loopback(IntPtr instance);

    // 9.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_start_screen_capture(IntPtr instance,
                                                              IntPtr view,
                                                              TRTCVideoStreamType type,
                                                              ref TRTCVideoEncParam param);

    // 9.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_stop_screen_capture(IntPtr instance);

    // 9.3
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_pause_screen_capture(IntPtr instance);

    // 9.4
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_resume_screen_capture(IntPtr instance);

    // TARGET_PLATFORM_DESKTOP
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_get_screen_capture_source_list(IntPtr instance,
                                                                       TRTCSize thumbnail,
                                                                       TRTCSize icon,
                                                                       ref IntPtr sourceList,
                                                                       ref int count);

    // 9.5 TARGET_PLATFORM_DESKTOP
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_get_screen_capture_sources_info(IntPtr sourceList,
                                                                        int index,
                                                                        ref ScreenCaptureSourceInfo
                                                                            sourceInfoList);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_release_screen_capture_sources_list(IntPtr sourceList);

    // 9.6
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_select_screen_capture_target(
        IntPtr instance,
        ScreenCaptureSourceInfo source,
        RECT capture_rect,
        ScreenCaptureProperty property);

    // 9.7
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_sub_stream_encoder_param(
        IntPtr instance,
        TRTCVideoResolution videoResolution,
        TRTCVideoResolutionMode resMode,
        uint videoFPS,
        uint videoBitrate,
        uint minVideoBitrate,
        bool enableAdjustRes);

    // 9.8
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_sub_stream_mix_volume(IntPtr instance, uint volume);

    // 9.9
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_add_excluded_share_window(IntPtr instance, IntPtr windowID);

    // 9.10
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_remove_excluded_share_window(IntPtr instance, IntPtr windowID);

    // 9.11
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_remove_all_excluded_share_windows(IntPtr instance);

    // 9.12
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_add_included_share_window(IntPtr instance, IntPtr windowID);

    // 9.13
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_remove_included_share_window(IntPtr instance, IntPtr windowID);

    // 9.14
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_remove_all_included_share_windows(IntPtr instance);

    // 10.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_enable_custom_video_capture(IntPtr instance, TRTCVideoStreamType streamType, bool enable);

    // 10.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_send_custom_video_data(IntPtr instance,
                                                                TRTCVideoStreamType streamType,
                                                                VideoFrame frame);

    // 10.3
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_enable_custom_audio_capture(IntPtr instance, bool enable);

    // 10.4
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_send_custom_audio_data(IntPtr instance, AudioFrame frame);

    // 10.5
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_enable_mix_external_audio_frame(IntPtr instance,
                                                                         bool enablePublish,
                                                                         bool enablePlayout);

    // 10.9.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_enable_local_video_custom_process(
        IntPtr instance,
        bool enable,
        TRTCVideoPixelFormat pixelFormat,
        TRTCVideoBufferType bufferType);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr trtc_cloud_create_video_frame_callback(
        IntPtr instance,
        TRTCVideoFrameCallbackNative.OnGLContextCreatedHandler onGLContextCreated,
        TRTCVideoFrameCallbackNative.OnProcessVideoFrameHandler onProcessVideoFrame,
        TRTCVideoFrameCallbackNative.OnGLContextDestroyHandler onGLContextDestroy);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_reset_video_frame_callback(IntPtr callback);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_destroy_video_frame_callback(IntPtr callback);

    // 10.9.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_local_video_custom_process_callback(IntPtr instance,
                                                                                 IntPtr callback);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr trtc_cloud_create_video_render_callback(
        IntPtr instance,
        TRTCVideoRenderCallbackNative.OnRenderVideoFrameHandler onVideoRender);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_reset_video_render_callback(IntPtr callback);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_destroy_video_render_callback(IntPtr callback);

    // 10.10
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_set_local_video_render_callback(
        IntPtr instance,
        TRTCVideoPixelFormat pixelFormat,
        TRTCVideoBufferType bufferType,
        IntPtr callback);

    // 10.11
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_set_remote_video_render_callback(
        IntPtr instance,
        [MarshalAs(UnmanagedType.LPStr)] string userId,
        TRTCVideoPixelFormat pixelFormat,
        TRTCVideoBufferType bufferType,
        IntPtr callback);

    // 10.12
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_set_audio_frame_callback(IntPtr instance, IntPtr callback);

    // 10.13
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_set_captured_audio_frame_callback_format(IntPtr instance,
                                                                           TRTCAudioFrameCallbackFormat format);

    // 10.14
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_set_local_processed_audio_frame_callback_format(IntPtr instance,
                                                                                  TRTCAudioFrameCallbackFormat format);

    // 10.15
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_set_mixed_play_audio_frame_callback_format(IntPtr instance,
                                                                             TRTCAudioFrameCallbackFormat format);


    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr trtc_cloud_create_audio_frame_callback(
        IntPtr instance,
        TRTCAudioFrameCallbackNative.OnCapturedAudioFrameHandler onCapturedAudioFrame,
        TRTCAudioFrameCallbackNative.OnLocalProcessedAudioFrameHandler onLocalProcessedAudioFrame,
        TRTCAudioFrameCallbackNative.OnPlayAudioFrameHandler onPlayAudioFrame,
        TRTCAudioFrameCallbackNative.OnMixedPlayAudioFrameHandler onMixedPlayAudioFrame,
        TRTCAudioFrameCallbackNative.OnMixedAllAudioFrameHandler onMixedAllAudioFrame);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_reset_audio_frame_callback(IntPtr callback);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_destroy_audio_frame_callback(IntPtr callback);

    // 11.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_send_sustom_cmd_msg(IntPtr instance,
                                                            int cmdId,
                                                            byte[] data,
                                                            int dataSize,
                                                            bool reliable,
                                                            bool ordered);

    // 11.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int trtc_cloud_send_sei_msg(IntPtr instance,
                                                     [MarshalAs(UnmanagedType.LPArray,
                                                                SizeParamIndex = 2)] byte[] data,
                                                     int dataSize,
                                                     int repeatCount);

    // 12.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_start_speed_test(IntPtr instance,
                                                          TRTCSpeedTestParams param);

    // 12.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_stop_speed_test(IntPtr instance);

    // 13.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr trtc_cloud_get_sdk_version(IntPtr instance);

    // 13.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_log_param(IntPtr instance, TRTCLog level);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr trtc_cloud_create_log_callback(
        IntPtr instance,
        TRTCLogCallbackNative.OnLogHandler onLog);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_reset_log_callback(IntPtr callback);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_destroy_log_callback(IntPtr callback);

    // 13.6
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_set_log_callback(IntPtr instance, IntPtr callback);

    // 13.9
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_call_experimental_api(IntPtr instance, [
      MarshalAs(UnmanagedType.LPStr)
    ] string jsonStr);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr trtc_cloud_copy_native_memery(IntPtr dest, IntPtr src, int count);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void trtc_cloud_write_log(TRTCLogWriteLevel log_write_level,
                                                   string log_file_line,
                                                   string log_tag,
                                                   string log_message);

#if UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
    [DllImport("TXFFmpeg", CallingConvention = CallingConvention.Cdecl)]
    public static extern void load_TXFFmpeg();

    [DllImport("TXSoundTouch", CallingConvention = CallingConvention.Cdecl)]
    public static extern void load_TXSoundTouch();
#endif
  }
}