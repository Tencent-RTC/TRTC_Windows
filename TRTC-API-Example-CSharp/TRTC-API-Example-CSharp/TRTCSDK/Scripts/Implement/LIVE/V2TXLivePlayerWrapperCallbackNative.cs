// Copyright (c) 2023 Tencent. All rights reserved.
// Author: makbaktan

using System;
using System.Runtime.InteropServices;
namespace liteav {
  public class V2TXLivePlayerCallbackNative : V2TXLiveBaseNative {
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnErrorHandler(IntPtr player,
                                        V2TXLiveCode code,
                                        string msg,
                                        IntPtr extraInfo,
                                        IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnWarningHandler(IntPtr player,
                                          V2TXLiveCode code,
                                          string msg,
                                          IntPtr extraInfo,
                                          IntPtr userData);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnVideoResolutionChangedHandler(IntPtr player,
                                                         int width,
                                                         int height,
                                                         IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnConnectedHandler(IntPtr player, IntPtr extraInfo, IntPtr userData);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnVideoPlayingHandler(IntPtr player,
                                               int firstPlay,
                                               IntPtr extraInfo,
                                               IntPtr userData);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnAudioPlayingHandler(IntPtr player,
                                               int firstPlay,
                                               IntPtr extraInfo,
                                               IntPtr userData);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnPlayoutVolumeUpdateHandler(IntPtr player, int volume, IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnSnapshotCompleteHandler(IntPtr player,
                                                   IntPtr image,
                                                   int length,
                                                   int width,
                                                   int height,
                                                   V2TXLivePixelFormat format,
                                                   IntPtr userData);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnReceiveSeiMessageHandler(IntPtr player,
                                                    int payloadType,
                                                    IntPtr data,
                                                    UInt32 dataSize,
                                                    IntPtr userData);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnStreamSwitchedHandler(IntPtr player,
                                                 string url,
                                                 V2TXLiveCode code,
                                                 IntPtr userData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void OnRenderVideoFrameHandler(IntPtr player,
                                                   ref VideoFrame audioFrame,
                                                   IntPtr userData);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void v2tx_live_player_set_on_error_handler(IntPtr observer,
                                                                    OnErrorHandler errorHandler);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void v2tx_live_player_set_on_warning_handler(
        IntPtr observer,
        OnWarningHandler warningHandler);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void v2tx_live_player_set_on_video_resolution_changed_handler(
        IntPtr observer,
        OnVideoResolutionChangedHandler resoulutionChangedHandler);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void v2tx_live_player_set_on_connected_handler(
        IntPtr observer,
        OnConnectedHandler connectedHandlercoconc);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void v2tx_live_player_set_on_video_playing_handler(
        IntPtr observer,
        OnVideoPlayingHandler videoPlayingHandler);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void v2tx_live_player_set_on_audio_playing_handler(
        IntPtr observer,
        OnAudioPlayingHandler audioPlayHandler);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void v2tx_live_player_set_on_playout_volume_update_handler(
        IntPtr observer,
        OnPlayoutVolumeUpdateHandler playoutVolumeUpdateHandler);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void v2tx_live_player_set_on_snapshot_complete_handler(
        IntPtr observer,
        OnSnapshotCompleteHandler snapshotCompeteHandler);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void v2tx_live_player_set_on_receive_sei_message_handler(
        IntPtr observer,
        OnReceiveSeiMessageHandler receiveSeiMessageHandler);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void v2tx_live_player_set_on_stream_switched_handler(
        IntPtr observer,
        OnStreamSwitchedHandler streamSwitchedHandler);

    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void v2tx_live_player_set_on_render_video_frame_handler(
        IntPtr observer,
        OnRenderVideoFrameHandler videoFrameHandler);
  }

}