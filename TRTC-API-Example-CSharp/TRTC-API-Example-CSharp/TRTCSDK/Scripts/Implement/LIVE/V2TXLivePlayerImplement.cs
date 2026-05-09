// Copyright (c) 2023 Tencent. All rights reserved.
// Author: makbaktan

using System;
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
using UnityEngine;
#endif

namespace liteav {

  public class V2TXLivePlayerImplement : V2TXLivePlayer {
    private IntPtr _nativeObj = IntPtr.Zero;
    private V2TXLivePlayerWrapperCallback _wrapperCallback = null;
    private V2TXLivePlayerObserver _callback = null;

    private readonly object _callBackLock = new object();
    public V2TXLivePlayerImplement() {
#if UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
      LoadTrtcPlugin();
#endif
      Init();
    }

    ~V2TXLivePlayerImplement() { Destroy(); }

    private void Init() {
      _nativeObj = V2TXLivePlayerNative.create_v2tx_live_player();

      V2TXLivePlayerManager.AddLivePlayer(_nativeObj, this);

      _wrapperCallback = new V2TXLivePlayerWrapperCallback(_nativeObj);
    }

    private void Destroy() {
      if (_nativeObj == IntPtr.Zero) {
        return;
      }
      V2TXLivePlayerManager.RemoveLivePlayer(_nativeObj);
      V2TXLivePlayerNative.release_v2tx_live_player(_nativeObj);
      _nativeObj = IntPtr.Zero;
      _wrapperCallback = null;
      _callback = null;
      GC.Collect();
    }
    public V2TXLivePlayerObserver GetCallback() {
      lock (_callBackLock) {
        return _callback;
      }
    }

    public override int isPlaying() {
      return V2TXLivePlayerNative.v2tx_live_player_is_playing(_nativeObj);
    }

    public override int pauseAudio() {
      return V2TXLivePlayerNative.v2tx_live_player_pause_audio(_nativeObj);
    }

    public override int pauseVideo() {
      return V2TXLivePlayerNative.v2tx_live_player_pause_video(_nativeObj);
    }

    public override int resumeAudio() {
      return V2TXLivePlayerNative.v2tx_live_player_resume_audio(_nativeObj);
    }

    public override int resumeVideo() {
      return V2TXLivePlayerNative.v2tx_live_player_resume_video(_nativeObj);
    }

    public override int setPlayoutVolume(int volume) {
      return V2TXLivePlayerNative.v2tx_live_player_set_playout_volume(_nativeObj, volume);
    }

    public override int setRenderRotation(V2TXLiveRotation rotation) {
      return V2TXLivePlayerNative.v2tx_live_player_set_render_rotation(_nativeObj, rotation);
    }

    public override void showDebugView(bool isShow) {
      V2TXLivePlayerNative.v2tx_live_player_show_debug_view(_nativeObj, isShow ? 1 : 0);
    }

    public override int startPlay(string url) {
      return V2TXLivePlayerNative.v2tx_live_player_start_play(_nativeObj, url);
    }

    public override int stopPlay() {
      return V2TXLivePlayerNative.v2tx_live_player_stop_play(_nativeObj);
    }

    public override void setCallback(V2TXLivePlayerObserver callback) {
      lock (_callBackLock) {
        _callback = callback;
      }
    }

    public override int enableObserveVideoFrame(bool enable,
                                                 V2TXLivePixelFormat pixelFormat,
                                                 V2TXLiveBufferType bufferType) {
      return V2TXLivePlayerNative.v2tx_live_player_enable_observer_video_frame(
          _nativeObj, enable, pixelFormat, bufferType);
    }
#if UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
    public static void LoadTrtcPlugin() {
      try {
        V2TXLivePlayerNative.load_TXFFmpeg();
      }
      catch (Exception e) {
      }

      try {
        V2TXLivePlayerNative.load_TXSoundTouch();
      }
      catch (Exception e) {
      }
    }

#endif
  }
}