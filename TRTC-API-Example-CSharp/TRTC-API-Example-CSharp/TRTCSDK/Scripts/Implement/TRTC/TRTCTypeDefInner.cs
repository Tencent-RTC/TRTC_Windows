// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan

using System;
using System.Runtime.InteropServices;

namespace trtc {
  //  5.1 
  [StructLayout(LayoutKind.Sequential)]
  public struct TRTCParamsInner {
    public UInt32 sdkAppId;
    public IntPtr userId;
    public IntPtr userSig;
    public UInt32 roomId;
    public IntPtr strRoomId;
    public TRTCRoleType role;
    public IntPtr streamId;
    public IntPtr userDefineRecordId;
    public IntPtr privateMapKey;
    public IntPtr businessInfo;
  }

  // 5.10
  [StructLayout(LayoutKind.Sequential)]
  public struct VideoFrame {
    public TRTCVideoPixelFormat videoFormat;
    public TRTCVideoBufferType bufferType;
    public IntPtr texture;
    public IntPtr data;
    public UInt32 length;
    public UInt32 width;
    public UInt32 height;
    public UInt64 timestamp;
    public TRTCVideoRotation rotation;
  }

  // 5.11
  [StructLayout(LayoutKind.Sequential)]
  public struct AudioFrame {
    public TRTCAudioFrameFormat audioFormat;
    public IntPtr data;
    public UInt32 length;
    public UInt32 sampleRate;
    public UInt32 channel;
    public UInt64 timestamp;
    public IntPtr extraData;
    public UInt32 extraDatalength;
  }

  // 5.13
  [StructLayout(LayoutKind.Sequential)]
  public struct TranscodingConfig {
    public TRTCTranscodingConfigMode mode;
    public UInt32 appId;
    public UInt32 bizId;
    public UInt32 videoWidth;
    public UInt32 videoHeight;
    public UInt32 videoBitrate;
    public UInt32 videoFramerate;
    public UInt32 videoGOP;
    public UInt32 backgroundColor;
    public string backgroundImage;
    public UInt32 audioSampleRate;
    public UInt32 audioBitrate;
    public UInt32 audioChannels;
    public UInt32 audioCodec;
    public IntPtr mixUsersArray;
    public UInt32 mixUsersArraySize;
    public string streamId;
    public string videoSeiParams;
  }

  // 5.20
  [StructLayout(LayoutKind.Sequential)]
  public struct ImageBuffer {
    public IntPtr buffer;
    public int length;
    public int width;
    public int height;
  }

  // 5.21
  [StructLayout(LayoutKind.Sequential)]
  public struct ScreenCaptureSourceInfo {
    public TRTCScreenCaptureSourceType type;
    public IntPtr sourceId;
    public IntPtr sourceName;
    public ImageBuffer thumbBGRA;
    public ImageBuffer iconBGRA;
    public bool isMinimizeWindow;
    public bool isMainScreen;
    public int x;
    public int y;
    public uint width;
    public uint height;
  }

  // 5.23
  [StructLayout(LayoutKind.Sequential)]
  public struct ScreenCaptureProperty {
    public bool enableCaptureMouse;
    public bool enableHighLight;
    public bool enableHighPerformance;
    public int highLightColor;
    public int highLightWidth;
    public bool enableCaptureChildWindow;
  }

  // 5.26
  [StructLayout(LayoutKind.Sequential)]
  public struct PublishCdnUrl {
    public string rtmpUrl;
    public bool isInternalLine;
  }

  // 5.27
  [StructLayout(LayoutKind.Sequential)]
  public struct PublishTarget {
    public TRTCPublishMode mode;
    public IntPtr cdnUrlList;
    public UInt32 cdnUrlListSize;
    public IntPtr mixStreamIdentity;
  }

  // 5.28
  [StructLayout(LayoutKind.Sequential)]
  public struct VideoLayout {
    public RECT rect;
    public int zOrder;
    public TRTCVideoFillMode fillMode;
    public UInt32 backgroundColor;
    public string placeHolderImage;
    public IntPtr fixedVideoUser;
    public TRTCVideoStreamType fixedVideoStreamType;
  }

  // 5.29
  [StructLayout(LayoutKind.Sequential)]
  public struct WaterMark {
    public string watermarkUrl;
    public RECT rect;
    public int zOrder;
  }

  // 5.30
  [StructLayout(LayoutKind.Sequential)]
  public struct StreamEncoderParam {
    public UInt32 videoEncodedWidth;
    public UInt32 videoEncodedHeight;
    public UInt32 videoEncodedFps;
    public UInt32 videoEncodedGop;
    public UInt32 videoEncodedKbps;
    public UInt32 audioEncodedSampleRate;
    public UInt32 audioEncodedChannelNum;
    public UInt32 audioEncodedKbps;
    public UInt32 audioEncodedCodecType;
    public UInt32 videoEncodedCodecType;
    public string videoSeiParams;
  }

  // 5.31
  [StructLayout(LayoutKind.Sequential)]
  public struct StreamMixingConfig {
    public UInt32 backgroundColor;
    public string backgroundImage;
    public IntPtr videoLayoutList;
    public UInt32 videoLayoutListSize;
    public IntPtr audioMixUserList;
    public UInt32 audioMixUserListSize;
    public IntPtr watermarkList;
    public UInt32 watermarkListSize;
  }

  [StructLayout(LayoutKind.Sequential)]
  public struct TRTCLog {
    public TRTCLogLevel level;
    public bool consoleEnabled;
    public bool compressEnabled;
    public String path;
  }

  [StructLayout(LayoutKind.Sequential)]
  public struct TRTCSize {
    public long width;
    public long height;
  }

  [StructLayout(LayoutKind.Sequential)]
  public struct DeviceInfo {
    public IntPtr devicePID;
    public IntPtr deviceName;
    public IntPtr deviceProperties;
    public UInt32 devicePIDLen;
    public UInt32 deviceNameLen;
    public UInt32 devicePropertiesLen;
  }

  [StructLayout(LayoutKind.Sequential)]
  public struct AudioMusicParamInner {
    public int id;
    public IntPtr path;
    public int loopCount;
    public bool publish;
    public bool isShortFile;
    public int startTimeMS;
    public int endTimeMS;
  }

  public enum TRTCLogWriteLevel {
    logInfo = 0,
    logWarning = 1,
    logError = 2,
    logFatal = 3,
  }
}