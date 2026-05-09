// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan

using System;
using System.Collections.Generic;
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
using AOT;
#endif

namespace trtc {
  public interface ITXCopyrightedMediaCallback : ITXMusicPreloadCallback {
    void onPreloadStart(string musicId, string bitrateDefinition);
    void onPreloadProgress(string musicId, string bitrateDefinition, float progress);
    void onPreloadComplete(string musicId, string bitrateDefinition, int errorCode, string msg);
  }

  public class TXCopyrightedMediaImplement : ITXCopyrightedMedia {
    #region callbaks
    private static readonly Dictionary<IntPtr, ITXMusicPreloadCallback> musicPreloadCallbackDic =
        new Dictionary<IntPtr, ITXMusicPreloadCallback>();
    private static readonly object callbackDicLock = new object();
    private static ITXMusicPreloadCallback getTXCopyrightedMediaCallback(IntPtr instance) {
      if (instance == IntPtr.Zero) {
        return null;
      }

      ITXMusicPreloadCallback musicPreloadCallback;
      lock (callbackDicLock) {
        musicPreloadCallbackDic.TryGetValue(instance, out musicPreloadCallback);
      }
      return musicPreloadCallback;
    }

    private static void addTXCopyrightedMediaCallback(IntPtr instance, ITXMusicPreloadCallback musicPreloadCallback) {
      if (instance == IntPtr.Zero) {
        return;
      }
      lock (callbackDicLock) {
        if (musicPreloadCallbackDic.ContainsKey(instance)) {
          return;
        }
        musicPreloadCallbackDic.Add(instance, musicPreloadCallback);
      }
    }

    private static void removeTXCopyrightedMediaCallback(IntPtr instance) {
      if (instance == IntPtr.Zero) {
        return;
      }
      lock (callbackDicLock) {
        if (musicPreloadCallbackDic.ContainsKey(instance)) {
          musicPreloadCallbackDic.Remove(instance);
        }
      }
    }

    [MonoPInvokeCallback(typeof(TXCopyrightedMediaNative.TXCopyrightedMediaOnPreloadStart))]
    public static void TXCopyrightedMediaOnPreloadStartHandler(IntPtr instance, string musicId,
                                                                    string bitrateDefinition) {
      getTXCopyrightedMediaCallback(instance)?.onPreloadStart(musicId, bitrateDefinition);
    }

    [MonoPInvokeCallback(typeof(TXCopyrightedMediaNative.TXCopyrightedMediaOnPreloadProgress))]
    public static void TXCopyrightedMediaOnPreloadProgressHandler(IntPtr instance,
                                                          string musicId, string bitrateDefinition, float progress) {
      getTXCopyrightedMediaCallback(instance)?.onPreloadProgress(musicId, bitrateDefinition, progress);
    }

    [MonoPInvokeCallback(typeof(TXCopyrightedMediaNative.TXCopyrightedMediaOnPreloadComplete))]
    public static void TXCopyrightedMediaOnPreloadCompleteHandler(IntPtr instance, string musicId, string bitrateDefinition, int errorCode, string msg) {
      getTXCopyrightedMediaCallback(instance)?.onPreloadComplete(musicId, bitrateDefinition, errorCode, msg);
    }

    #endregion

    private IntPtr _nativeObj;
    private IntPtr _nativeMediaPreloadCallback;
    public readonly TXCopyrightedMediaNative.TXCopyrightedMediaOnPreloadStart TXCopyrightedMediaOnPreloadStart = TXCopyrightedMediaOnPreloadStartHandler;
    public readonly TXCopyrightedMediaNative.TXCopyrightedMediaOnPreloadProgress TXCopyrightedMediaOnPreloadProgress = TXCopyrightedMediaOnPreloadProgressHandler;
    public readonly TXCopyrightedMediaNative.TXCopyrightedMediaOnPreloadComplete TXCopyrightedMediaOnPreloadComplete = TXCopyrightedMediaOnPreloadCompleteHandler;
    public TXCopyrightedMediaImplement() {
      TRTCLogger.Info("create_copyrighted_media");
      _nativeObj = TXCopyrightedMediaNative.create_copyrighted_media();
      _nativeMediaPreloadCallback = TXCopyrightedMediaNative.tx_copyrighted_media_create_music_preload_observer(_nativeObj,
                                                                                          TXCopyrightedMediaOnPreloadStart,
                                                                                          TXCopyrightedMediaOnPreloadProgress,
                                                                                          TXCopyrightedMediaOnPreloadComplete);
      TXCopyrightedMediaNative.tx_copyrighted_media_set_music_preload_observer(_nativeObj, _nativeMediaPreloadCallback);
    }

    public override void destroyCopyRightMedia() {
      TRTCLogger.Info("destroyCopyRightMedia");
      if (_nativeObj == IntPtr.Zero) {
        return;
      }
      _nativeMediaPreloadCallback = IntPtr.Zero;
      TXCopyrightedMediaNative.tx_copyrighted_media_set_music_preload_observer(_nativeObj, _nativeMediaPreloadCallback);
      TXCopyrightedMediaNative.tx_copyrighted_media_destroy_music_preload_observer(_nativeMediaPreloadCallback);
      TXCopyrightedMediaNative.destroy_copyrighted_media(_nativeObj);
      removeTXCopyrightedMediaCallback(_nativeObj);
      _nativeObj = IntPtr.Zero;
    }

    public override int setCopyrightedLicense(string key, string license_url) {
      return TXCopyrightedMediaNative.tx_copyrighted_media_set_copyrighted_license(_nativeObj, key, license_url);
    }

    public override bool genMusicURI(string musicId, int bgmType, string bitrateDefinition, IntPtr outData, int outDataSize) {
      return TXCopyrightedMediaNative.tx_copyrighted_media_gen_music_url(_nativeObj, musicId, bgmType, bitrateDefinition, outData, outDataSize) == 0 ? true : false;
    }

    public override void setMusicPreloadCallback(ITXMusicPreloadCallback callback) {
      if (callback == null) {
        TRTCLogger.Info("remove musicPreloadCallback");
        removeTXCopyrightedMediaCallback(_nativeObj);
      }
      else {
        TRTCLogger.Info("add musicPreloadCallback");
        addTXCopyrightedMediaCallback(_nativeObj, callback);
      }
    }

    public override int preloadMusic(string musicId, string bitrateDefinition, string playToken) {
      return TXCopyrightedMediaNative.tx_copyrighted_media_preload_music(_nativeObj, musicId, bitrateDefinition, playToken);
    }

    public override int cancelPreloadMusic(string musicId, string bitrateDefinition) {
      return TXCopyrightedMediaNative.tx_copyrighted_media_cancel_preload_music(_nativeObj, musicId, bitrateDefinition);
    }

    public override bool isMusicPreload(string musicId, string bitrateDefinition) {
      return TXCopyrightedMediaNative.tx_copyrighted_media_is_music_preload(_nativeObj, musicId, bitrateDefinition) == 1 ? true : false;
    }

    public override int clearMusicCache() {
      return TXCopyrightedMediaNative.tx_copyrighted_media_clear_music_cache(_nativeObj);
    }

    public override int setMusicCacheMaxCount(int maxCount) {
      return TXCopyrightedMediaNative.tx_copyrighted_media_set_music_cache_max_count(_nativeObj, maxCount);
    }
  }
}