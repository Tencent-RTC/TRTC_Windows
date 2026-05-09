// Copyright (c) 2023 Tencent. All rights reserved.
// Author: makbaktan

using System;
namespace liteav {
  public struct VideoFrame {
    public V2TXLivePixelFormat pixelFormat;
    public V2TXLiveBufferType bufferType;
    public IntPtr data;
    public int length;
    public int width;
    public int height;
    public V2TXLiveRotation rotation;
  }
}
