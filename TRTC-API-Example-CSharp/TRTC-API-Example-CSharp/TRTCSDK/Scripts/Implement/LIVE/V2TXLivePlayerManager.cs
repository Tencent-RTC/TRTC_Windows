// Copyright (c) 2023 Tencent. All rights reserved.
// Author: makbaktan

using System;
using System.Collections.Concurrent;

namespace liteav {
  internal static class V2TXLivePlayerManager {
    private static readonly ConcurrentDictionary<IntPtr, WeakReference> livePlayerDic =
        new ConcurrentDictionary<IntPtr, WeakReference>();

    public static bool IsEmpty() { return livePlayerDic.Count > 0; }

    public static void AddLivePlayer(IntPtr instance, V2TXLivePlayerImplement livePlayer) {
      if (instance == IntPtr.Zero) {
        return;
      }

      livePlayerDic.TryAdd(instance, new WeakReference(livePlayer));
    }

    public static void RemoveLivePlayer(IntPtr instance) {
      if (instance == IntPtr.Zero) {
        return;
      }
      WeakReference livePlayer;
      livePlayerDic.TryRemove(instance, out livePlayer);
    }

    public static V2TXLivePlayerImplement GetLivePlayer(IntPtr instance) {
      if (instance == IntPtr.Zero) {
        return null;
      }
      WeakReference livePlayer;
      if (livePlayerDic.TryGetValue(instance, out livePlayer)) {
        return livePlayer.Target as V2TXLivePlayerImplement;
      }
      return null;
    }
  }
}