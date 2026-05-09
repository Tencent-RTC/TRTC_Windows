// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan

using System;
using System.Runtime.InteropServices;

namespace trtc {
  public class TXCopyrightedMediaNative : TRTCBaseNative {
    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr create_copyrighted_media();

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void destroy_copyrighted_media(IntPtr copyrighted_media);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int tx_copyrighted_media_set_copyrighted_license(IntPtr instance, string key, string licenseUrl);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int tx_copyrighted_media_gen_music_url(IntPtr instance,
                                           string musicId,
                                           int bgmType,
                                           string bitrateDefinition,
                                           IntPtr outData,
                                           int outDataSize);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int tx_copyrighted_media_preload_music(IntPtr instance,
                                           string musicId,
                                           string bitrateDefinition,
                                           string playToken);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int tx_copyrighted_media_cancel_preload_music(IntPtr instance, string musicId, string bitrateDefinition);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int tx_copyrighted_media_is_music_preload(IntPtr instance, string musicId, string bitrateDefinition);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int tx_copyrighted_media_clear_music_cache(IntPtr instance);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern int tx_copyrighted_media_set_music_cache_max_count(IntPtr instance, int maxCount);


    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr tx_copyrighted_media_create_music_preload_observer(IntPtr instance,
                                                              TXCopyrightedMediaOnPreloadStart onPreloadStart,
                                                              TXCopyrightedMediaOnPreloadProgress onPreloadProgress,
                                                              TXCopyrightedMediaOnPreloadComplete onPreloadComplete);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_copyrighted_media_destroy_music_preload_observer(IntPtr observer);

    [DllImport(TRTCLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void tx_copyrighted_media_set_music_preload_observer(IntPtr instance, IntPtr observer);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void TXCopyrightedMediaOnPreloadStart(IntPtr instance, string musicId, string bitrateDefinition);


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void TXCopyrightedMediaOnPreloadProgress(IntPtr instance,
                                                             string musicId,
                                                             string bitrateDefinition,
                                                             float progress);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void TXCopyrightedMediaOnPreloadComplete(IntPtr instance,
                                                             string musicId,
                                                             string bitrateDefinition,
                                                             int errorCode,
                                                             string msg);
  }
}