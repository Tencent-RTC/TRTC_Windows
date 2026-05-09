// Copyright (c) 2025 Tencent. All rights reserved.
// Author: kleinjia

#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
using UnityEngine;
#else
using System;
using System.Windows.Threading;
#endif

namespace trtc {
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
  internal sealed class TRTCEventLooper : MonoBehaviour {
    private TRTCActionQueue _actionQueue;

    public void SetActionQueue(TRTCActionQueue actionQueue) {
      _actionQueue = actionQueue;
    }

    void Update() {
      _actionQueue?.Update();
    }

    void OnApplicationPause(bool pauseStatus) {
      _actionQueue?.OnApplicationPause(pauseStatus);
    }

    void OnDestroy() {
      _actionQueue?.Destroy();
      _actionQueue = null;
    }
  }
#else
  internal sealed class TRTCEventLooper : IDisposable {
    private TRTCActionQueue _actionQueue;
    private System.Timers.Timer _timer;

    public TRTCEventLooper() {
      _timer = new System.Timers.Timer(16); // 16ms interval
      _timer.Elapsed += (s, e) => _actionQueue?.Update();
      _timer.AutoReset = true;
      _timer.Start();
    }
    public void SetActionQueue(TRTCActionQueue actionQueue) {
      _actionQueue = actionQueue;
    }

    public void OnApplicationPause(bool pauseStatus) {
      _actionQueue?.OnApplicationPause(pauseStatus);
    }

    public void Dispose() {
      _timer?.Stop();
      _timer?.Dispose();
      _timer = null;
      _actionQueue?.Destroy();
      _actionQueue = null;
    }
  }
#endif
}