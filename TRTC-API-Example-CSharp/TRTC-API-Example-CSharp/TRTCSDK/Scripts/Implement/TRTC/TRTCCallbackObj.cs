// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
using UnityEngine;
#endif

namespace trtc {
  internal sealed class TRTCCallbackObj {
    private readonly string _gameObjName;
    private readonly TRTCActionQueue _actionQueue = new TRTCActionQueue();
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
    internal TRTCCallbackObj(string gameObjName) {
      _gameObjName = gameObjName;
      TryDestroy(_gameObjName);

      var callbackObj = new GameObject(_gameObjName) { hideFlags = HideFlags.HideInHierarchy };
      TRTCEventLooper eventLooper = callbackObj.AddComponent<TRTCEventLooper>();
      eventLooper.SetActionQueue(_actionQueue);
      Object.DontDestroyOnLoad(callbackObj);
    }
#else
    private TRTCEventLooper _eventLooper;
    internal TRTCCallbackObj(string gameObjName) {
      _gameObjName = gameObjName;
      TryDestroy(_gameObjName);
      _eventLooper = new TRTCEventLooper();
      _eventLooper.SetActionQueue(_actionQueue);
    }
#endif

    internal TRTCActionQueue GetActionQueue() { return _actionQueue; }

    private static void TryDestroy(string gameObjName) {
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
      try {
        var obj = GameObject.Find(gameObjName);
        if (ReferenceEquals(obj, null)) {
          return;
        }
        Object.Destroy(obj);
      }
      catch (System.Exception exception) {
        TRTCLogger.Error("TryDestroy Invoke " + exception);
      }
#endif
    }

    public static void Destroy(TRTCCallbackObj obj) {
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
      TryDestroy(obj?._gameObjName);
#else
      if (obj?._eventLooper != null) {
        obj._eventLooper.Dispose();
        obj._eventLooper = null;
      }
#endif
    }
  }
}