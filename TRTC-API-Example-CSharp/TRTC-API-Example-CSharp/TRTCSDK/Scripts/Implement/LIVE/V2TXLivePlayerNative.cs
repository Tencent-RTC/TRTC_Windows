// Copyright (c) 2023 Tencent. All rights reserved.
// Author: makbaktan

using System;
using System.Runtime.InteropServices;
namespace liteav {
  public abstract class V2TXLivePlayerNative : V2TXLiveBaseNative {
    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr create_v2tx_live_player();

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void release_v2tx_live_player(IntPtr instance);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_start_play(IntPtr instance, String url);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_stop_play(IntPtr instance);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_is_playing(IntPtr instance);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_pause_audio(IntPtr instance);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_resume_audio(IntPtr instance);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_pause_video(IntPtr instance);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_resume_video(IntPtr instance);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_switch_stream(IntPtr instance, String url);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_set_render_view(IntPtr instance, IntPtr view);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_set_playout_volume(IntPtr instance, int volume);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_set_render_rotation(IntPtr instance,
                                                                  V2TXLiveRotation rotation);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_enable_receive_sei_message(IntPtr instance,
                                                                         bool enable,
                                                                         int payloadType);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_enable_volume_evaluation(IntPtr instance,
                                                                       int intervalMs);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_show_debug_view(IntPtr instance, int is_show);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_snapshot(IntPtr instance);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int v2tx_live_player_enable_observer_video_frame(
        IntPtr instance,
        bool enable,
        V2TXLivePixelFormat pixel_format,
        V2TXLiveBufferType buffer_type);

#if UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
    [DllImport("TXFFmpeg", CallingConvention = CallingConvention.Cdecl)]
    public static extern void load_TXFFmpeg();

    [DllImport("TXSoundTouch", CallingConvention = CallingConvention.Cdecl)]
    public static extern void load_TXSoundTouch();
#endif
  }
}