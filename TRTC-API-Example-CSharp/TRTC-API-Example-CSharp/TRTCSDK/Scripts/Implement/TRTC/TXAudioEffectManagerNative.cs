// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan

using System;
using System.Runtime.InteropServices;

namespace trtc {
  public class TXAudioEffectManagerNative : TRTCBaseNative {
    // 1.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_enable_voice_ear_monitor(IntPtr instance,
                                                                               bool enable);

    // 1.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_voice_ear_monitor_volume(IntPtr instance,
                                                                                   int volume);

    // 1.3
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_voice_reverb_type(IntPtr instance,
                                                                            TXVoiceReverbType type);

    // 1.4
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_voice_changer_type(
        IntPtr instance,
        TXVoiceChangeType type);

    // 1.5
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_voice_capture_volume(IntPtr instance,
                                                                               int volume);

    // 1.6
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_voice_pitch(IntPtr instance,
                                                                      double pitch);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void TRTCUnityAEMMusicPlayOnStart(IntPtr instance, int musicId, int errCode);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void TRTCUnityAEMMusicPlayOnPlayProgress(IntPtr instance,
                                                             int musicId,
                                                             long curPtsMS,
                                                             long durationMS);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void TRTCUnityAEMMusicPlayOnComplete(IntPtr instance, int musicId, int errCode);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr tx_audio_effect_manager_create_music_play_observer(
        IntPtr instance,
        TRTCUnityAEMMusicPlayOnStart onStart,
        TRTCUnityAEMMusicPlayOnPlayProgress onPlayProgress,
        TRTCUnityAEMMusicPlayOnComplete onComplete);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_destroy_music_play_observer(IntPtr observer);

    // 2.0
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_music_observer(IntPtr instance,
                                                                         int musicId,
                                                                         IntPtr observer);

    // 2.1
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_start_play_music(IntPtr instance,
                                                                       AudioMusicParamInner musicParam);

    // 2.2
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_stop_play_music(IntPtr instance, int musicId);

    // 2.3
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_pause_play_music(IntPtr instance,
                                                                       int musicId);

    // 2.4
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_resume_play_music(IntPtr instance,
                                                                        int musicId);

    // 2.5
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_all_music_volume(IntPtr instance,
                                                                           int volume);

    // 2.6
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_music_publish_volume(IntPtr instance,
                                                                               int musicId,
                                                                               int volume);

    // 2.7
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_music_playout_volume(IntPtr instance,
                                                                               int musicId,
                                                                               int volume);

    // 2.8
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_music_pitch(IntPtr instance,
                                                                      int musicId,
                                                                      float pitch);

    // 2.9
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_music_speed_rate(IntPtr instance,
                                                                           int musicId,
                                                                           float speedRate);

    // 2.10
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern long tx_audio_effect_manager_get_current_pos_in_ms(IntPtr instance,
                                                                            int musicId);

    // 2.11
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern long tx_audio_effect_manager_get_music_duration_in_ms(IntPtr instance,
                                                                               IntPtr path);

    // 2.12
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_seek_music_to_pos_in_time(IntPtr instance,
                                                                                int musicId,
                                                                                int pts);

    // 2.13
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_music_scratch_speed_rate(
        IntPtr instance,
        int musicId,
        float scratchSpeedRate);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void TRTCUnityAEMMusicPreloadOnLoadProgress(IntPtr instance,
                                                                int musicId,
                                                                int progress);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void TRTCUnityAEMMusicPreloadOnLoadError(IntPtr instance,
                                                             int musicId,
                                                             int errorCode);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr tx_audio_effect_manager_create_music_preload_observer(
        IntPtr instance,
        TRTCUnityAEMMusicPreloadOnLoadProgress onLoadProgress,
        TRTCUnityAEMMusicPreloadOnLoadError onLoadError);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_destroy_music_preload_observer(
        IntPtr observer);

    // 2.14
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_preload_observer(IntPtr instance,
                                                                           IntPtr observer);

    // 2.15
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_preload_music(
        IntPtr instance,
        AudioMusicParamInner preloadMusicParam);

    // 2.16
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern long tx_audio_effect_manager_get_music_track_count(IntPtr instance,
                                                                            int musicId);

    // 2.17
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_audio_effect_manager_set_music_track(IntPtr instance,
                                                                      int musicId,
                                                                      int trackIndex);
  }
}