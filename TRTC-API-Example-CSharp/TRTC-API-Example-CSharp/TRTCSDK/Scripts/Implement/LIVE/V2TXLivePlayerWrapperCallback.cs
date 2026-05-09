// Copyright (c) 2023 Tencent. All rights reserved.
// Author: makbaktan

using System;
using System.Runtime.InteropServices;
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
using AOT;
#endif

namespace liteav {
#if !(UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL)
  [AttributeUsage(AttributeTargets.All)]
  internal class MonoPInvokeCallbackAttribute : Attribute {
    public MonoPInvokeCallbackAttribute(Type delegateType) {
    }
  }
#endif
  public class V2TXLivePlayerWrapperCallback {
    private IntPtr _nativeObj = IntPtr.Zero;
    private LivePlayerCallback _livePlayerCallback = null;

    ~V2TXLivePlayerWrapperCallback() { }

    public V2TXLivePlayerWrapperCallback(IntPtr nativeObj) {
      _nativeObj = nativeObj;

      V2TXLivePlayerCallbackNative.v2tx_live_player_set_on_error_handler(
          _nativeObj, LivePlayerCallback.OnErrorHandler);

      V2TXLivePlayerCallbackNative.v2tx_live_player_set_on_warning_handler(
          _nativeObj, LivePlayerCallback.OnWarningHandler);

      V2TXLivePlayerCallbackNative.v2tx_live_player_set_on_video_resolution_changed_handler(
          _nativeObj, LivePlayerCallback.OnVideoResolutionChangedHandler);

      V2TXLivePlayerCallbackNative.v2tx_live_player_set_on_connected_handler(
          _nativeObj, LivePlayerCallback.OnConnectedHandler);

      V2TXLivePlayerCallbackNative.v2tx_live_player_set_on_video_playing_handler(
          _nativeObj, LivePlayerCallback.OnVideoPlayingHandler);

      V2TXLivePlayerCallbackNative.v2tx_live_player_set_on_audio_playing_handler(
          _nativeObj, LivePlayerCallback.OnAudioPlayingHandler);

      V2TXLivePlayerCallbackNative.v2tx_live_player_set_on_render_video_frame_handler(
          _nativeObj, LivePlayerCallback.OnRenderVideoFrameHandler);
    }
  }
  public class LivePlayerCallback {
    public static readonly V2TXLivePlayerCallbackNative.OnErrorHandler OnErrorHandler = OnErrorHandlerRef;
    public static readonly V2TXLivePlayerCallbackNative.OnWarningHandler OnWarningHandler = OnWarningHandlerRef;
    public static readonly V2TXLivePlayerCallbackNative.OnVideoResolutionChangedHandler OnVideoResolutionChangedHandler = OnVideoResolutionChangedHandlerRef;
    public static readonly V2TXLivePlayerCallbackNative.OnConnectedHandler OnConnectedHandler = OnConnectedHandlerRef;
    public static readonly V2TXLivePlayerCallbackNative.OnAudioPlayingHandler OnAudioPlayingHandler = OnAudioPlayingHandlerRef;
    public static readonly V2TXLivePlayerCallbackNative.OnVideoPlayingHandler OnVideoPlayingHandler = OnVideoPlayingHandlerRef;
    public static readonly V2TXLivePlayerCallbackNative.OnRenderVideoFrameHandler OnRenderVideoFrameHandler = OnRenderVideoFrameHandlerRef;

    [MonoPInvokeCallback(typeof(V2TXLivePlayerCallbackNative.OnErrorHandler))]
    public static void OnErrorHandlerRef(IntPtr instance,
                                      V2TXLiveCode errCode,
                                      string errMsg,
                                      IntPtr extraInfo,
                                      IntPtr userData) {
      var livePlayerImpl = V2TXLivePlayerManager.GetLivePlayer(instance);
      if (livePlayerImpl == null || livePlayerImpl.GetCallback() == null) {
        return;
      }
      var callBack = livePlayerImpl.GetCallback();
      callBack.onError(livePlayerImpl, errCode, errMsg, extraInfo);
    }

    [MonoPInvokeCallback(typeof(V2TXLivePlayerCallbackNative.OnWarningHandler))]
    public static void OnWarningHandlerRef(IntPtr instance,
                                        V2TXLiveCode errCode,
                                        string errMsg,
                                        IntPtr extraInfo,
                                        IntPtr userData) {
      var livePlayerImpl = V2TXLivePlayerManager.GetLivePlayer(instance);
      if (livePlayerImpl == null || livePlayerImpl.GetCallback() == null) {
        return;
      }
      var callBack = livePlayerImpl.GetCallback();
      callBack.onWarning(livePlayerImpl, errCode, errMsg, extraInfo);
    }

    [MonoPInvokeCallback(typeof(V2TXLivePlayerCallbackNative.OnVideoResolutionChangedHandler))]
    public static void OnVideoResolutionChangedHandlerRef(IntPtr player,
                                                       int width,
                                                       int height,
                                                       IntPtr userData) {
      var livePlayerImpl = V2TXLivePlayerManager.GetLivePlayer(player);
      if (livePlayerImpl == null || livePlayerImpl.GetCallback() == null) {
        return;
      }
      var callBack = livePlayerImpl.GetCallback();
      callBack.onVideoResolutionChanged(livePlayerImpl, width, height);
    }

    [MonoPInvokeCallback(typeof(V2TXLivePlayerCallbackNative.OnConnectedHandler))]
    public static void OnConnectedHandlerRef(IntPtr player, IntPtr extraInfo, IntPtr userData) {
      var livePlayerImpl = V2TXLivePlayerManager.GetLivePlayer(player);
      if (livePlayerImpl == null || livePlayerImpl.GetCallback() == null) {
        return;
      }
      var callBack = livePlayerImpl.GetCallback();
      callBack.onConnected(livePlayerImpl, extraInfo);
    }

    [MonoPInvokeCallback(typeof(V2TXLivePlayerCallbackNative.OnAudioPlayingHandler))]
    public static void OnAudioPlayingHandlerRef(IntPtr player,
                                             int firstPlay,
                                             IntPtr extraInfo,
                                             IntPtr userData) {
      var livePlayerImpl = V2TXLivePlayerManager.GetLivePlayer(player);
      if (livePlayerImpl == null || livePlayerImpl.GetCallback() == null) {
        return;
      }
      var callBack = livePlayerImpl.GetCallback();
      callBack.onAudioPlaying(livePlayerImpl, firstPlay == 1, extraInfo);
    }

    [MonoPInvokeCallback(typeof(V2TXLivePlayerCallbackNative.OnVideoPlayingHandler))]
    public static void OnVideoPlayingHandlerRef(IntPtr player,
                                             int firstPlay,
                                             IntPtr extraInfo,
                                             IntPtr userData) {
      var livePlayerImpl = V2TXLivePlayerManager.GetLivePlayer(player);
      if (livePlayerImpl == null || livePlayerImpl.GetCallback() == null) {
        return;
      }
      var callBack = livePlayerImpl.GetCallback();
      callBack.onVideoPlaying(livePlayerImpl, firstPlay == 1, extraInfo);
    }

    [MonoPInvokeCallback(typeof(V2TXLivePlayerCallbackNative.OnRenderVideoFrameHandler))]
    public static void OnRenderVideoFrameHandlerRef(IntPtr player,
                                                 ref VideoFrame frame,
                                                 IntPtr userData) {
      V2TXLivePlayerImplement livePlayerImpl = V2TXLivePlayerManager.GetLivePlayer(player);

      var callBack = livePlayerImpl?.GetCallback();
      if (callBack == null ||
          frame.data == IntPtr.Zero || frame.length < 0) {
        return;
      }
      var appVideoFrame =
          new V2TXLiveVideoFrame {
            pixelFormat = frame.pixelFormat,
            bufferType = frame.bufferType,
            rotation = frame.rotation
          };
      appVideoFrame.data = new byte[frame.length];
      Marshal.Copy(frame.data, appVideoFrame.data, 0, (int)frame.length);
      appVideoFrame.length = frame.length;
      appVideoFrame.width = frame.width;
      appVideoFrame.height = frame.height;
      callBack.onRenderVideoFrame(livePlayerImpl, appVideoFrame);
    }
  }
}