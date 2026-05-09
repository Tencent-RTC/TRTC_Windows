// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan

using System;
using System.Collections.Concurrent;
namespace trtc {
  internal static class CloudManager {
    private static readonly ConcurrentDictionary<IntPtr, TRTCCloudImplement> CloudDic =
        new ConcurrentDictionary<IntPtr, TRTCCloudImplement>();

    public static bool IsEmpty() { return CloudDic.Count > 0; }

    public static void AddCloudImplement(IntPtr instance, TRTCCloudImplement cloudImplement) {
      if (instance == IntPtr.Zero) {
        return;
      }

      CloudDic.TryAdd(instance, cloudImplement);
    }

    public static void RemoveCloudImplement(IntPtr instance) {
      if (instance == IntPtr.Zero) {
        return;
      }
      TRTCCloudImplement cloudImplement;
      CloudDic.TryRemove(instance, out cloudImplement);
    }

    public static TRTCCloudImplement FindCloudImplement(IntPtr instance) {
      if (instance == IntPtr.Zero) {
        return null;
      }

      TRTCCloudImplement cloudImplement;
      CloudDic.TryGetValue(instance, out cloudImplement);
      return cloudImplement;
    }
  }
}