// Copyright (c) 2023 Tencent. All rights reserved.
// Author: makbaktan

using System.Runtime.InteropServices;

namespace liteav {
  public abstract class V2TXLivePremierNative : V2TXLiveBaseNative {
    [DllImport(V2TXLiveLibName, CallingConvention = CallingConvention.Cdecl)]
    public static extern void v2tx_live_premier_set_license(
        [MarshalAs(UnmanagedType.LPStr)] string url,
        [MarshalAs(UnmanagedType.LPStr)] string key);
  }
}
