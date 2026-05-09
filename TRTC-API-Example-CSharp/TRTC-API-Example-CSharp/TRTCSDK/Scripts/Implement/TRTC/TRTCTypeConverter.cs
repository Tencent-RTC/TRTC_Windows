// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan

using System;
using System.Runtime.InteropServices;

namespace trtc {
  [StructLayout(LayoutKind.Sequential)]
  public struct TRTCInnerMixUser {
    public string userId;

    public string roomId;

    public RECT rect;

    public int zOrder;

    public TRTCVideoStreamType streamType;

    public byte pureAudio;

    public TRTCMixInputType inputType;

    public UInt32 renderMode;

    public UInt32 soundLevel;

    public string image;
  }

#if !(UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL)
  [AttributeUsage(AttributeTargets.All)]
  internal class MonoPInvokeCallbackAttribute : Attribute {
    public MonoPInvokeCallbackAttribute(Type delegateType) {
    }
  }
#endif

  public class TRTCTypeConverter {
    private const int _deviceInfoLen = 1024;
    public static TRTCInnerMixUser ConvertTRTCMixUser(TRTCMixUser user) {
      TRTCInnerMixUser innerMixUser = new TRTCInnerMixUser();
      innerMixUser.userId = user.userId;
      innerMixUser.roomId = user.roomId;
      innerMixUser.rect = user.rect;
      innerMixUser.zOrder = user.zOrder;
      innerMixUser.streamType = user.streamType;
      innerMixUser.pureAudio = (byte)(user.pureAudio ? 1 : 0);
      innerMixUser.inputType = user.inputType;
      innerMixUser.renderMode = user.renderMode;
      innerMixUser.soundLevel = user.soundLevel;
      innerMixUser.image = user.image;
      return innerMixUser;
    }

    public static StreamEncoderParam ConvertTRTCStreamEncoderParam(
        TRTCStreamEncoderParam encodeParam) {
      StreamEncoderParam innerEncodeParam = new StreamEncoderParam();
      innerEncodeParam.videoEncodedWidth = encodeParam.videoEncodedWidth;
      innerEncodeParam.videoEncodedHeight = encodeParam.videoEncodedHeight;
      innerEncodeParam.videoEncodedFps = encodeParam.videoEncodedFps;
      innerEncodeParam.videoEncodedGop = encodeParam.videoEncodedGop;
      innerEncodeParam.audioEncodedKbps = encodeParam.audioEncodedKbps;
      innerEncodeParam.audioEncodedSampleRate = encodeParam.audioEncodedSampleRate;
      innerEncodeParam.audioEncodedChannelNum = encodeParam.audioEncodedChannelNum;
      innerEncodeParam.audioEncodedKbps = encodeParam.audioEncodedKbps;
      innerEncodeParam.audioEncodedCodecType = encodeParam.audioEncodedCodecType;
      innerEncodeParam.videoEncodedCodecType = encodeParam.videoEncodedCodecType;
      innerEncodeParam.videoSeiParams = encodeParam.videoSeiParams;
      return innerEncodeParam;
    }

    public static StreamMixingConfig ConvertTRTCStreamMixingConfig(TRTCStreamMixingConfig config) {
      StreamMixingConfig innerMixingConfig = new StreamMixingConfig();
      innerMixingConfig.backgroundColor = config.backgroundColor;
      innerMixingConfig.backgroundImage = config.backgroundImage;
      innerMixingConfig.videoLayoutListSize = 0;
      innerMixingConfig.audioMixUserListSize = 0;
      innerMixingConfig.watermarkListSize = 0;

      if (config.videoLayoutList != null && config.videoLayoutList.Length > 0) {
        VideoLayout[] videoLayoutList = new VideoLayout[config.videoLayoutList.Length];
        for (int i = 0; i < config.videoLayoutList.Length; i++) {
          videoLayoutList[i].rect = config.videoLayoutList[i].rect;
          videoLayoutList[i].zOrder = config.videoLayoutList[i].zOrder;
          videoLayoutList[i].fillMode = config.videoLayoutList[i].fillMode;
          videoLayoutList[i].backgroundColor = config.videoLayoutList[i].backgroundColor;
          videoLayoutList[i].placeHolderImage = config.videoLayoutList[i].placeHolderImage;
          videoLayoutList[i].fixedVideoStreamType = config.videoLayoutList[i].fixedVideoStreamType;
          videoLayoutList[i].fixedVideoUser = Marshal.AllocHGlobal(Marshal.SizeOf(config.videoLayoutList[i].fixedVideoUser));
          Marshal.StructureToPtr(config.videoLayoutList[i].fixedVideoUser, videoLayoutList[i].fixedVideoUser, false);
        }

        innerMixingConfig.videoLayoutListSize = (UInt32)config.videoLayoutList.Length;
        innerMixingConfig.videoLayoutList = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(VideoLayout)) * (int)innerMixingConfig.videoLayoutListSize);
        for (int i = 0; i < innerMixingConfig.videoLayoutListSize; i++) {
          IntPtr ptr = new IntPtr(innerMixingConfig.videoLayoutList.ToInt64() + i * Marshal.SizeOf(typeof(VideoLayout)));
          Marshal.StructureToPtr(videoLayoutList[i], ptr, false);
        }
      }
      else {
        innerMixingConfig.videoLayoutList = IntPtr.Zero;
      }

      if (config.audioMixUserList != null && config.audioMixUserList.Length > 0) {
        innerMixingConfig.audioMixUserListSize = (UInt32)config.audioMixUserList.Length;
        innerMixingConfig.audioMixUserList = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(TRTCMixUser)) * (int)innerMixingConfig.audioMixUserListSize);
        for (int i = 0; i < innerMixingConfig.audioMixUserListSize; i++) {
          IntPtr ptr = new IntPtr(innerMixingConfig.audioMixUserList.ToInt64() + i * Marshal.SizeOf(typeof(TRTCMixUser)));
          Marshal.StructureToPtr(config.audioMixUserList[i], ptr, false);
        }
      }
      else {
        innerMixingConfig.audioMixUserList = IntPtr.Zero;
      }

      if (config.watermarkList != null && config.watermarkList.Length > 0) {
        innerMixingConfig.watermarkListSize = (UInt32)config.watermarkList.Length;
        innerMixingConfig.watermarkList = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(TRTCWaterMark)) * (int)innerMixingConfig.watermarkListSize);
        for (int i = 0; i < innerMixingConfig.watermarkListSize; i++) {
          IntPtr ptr = new IntPtr(innerMixingConfig.watermarkList.ToInt64() + i * Marshal.SizeOf(typeof(TRTCWaterMark)));
          Marshal.StructureToPtr(config.watermarkList[i], ptr, false);
        }
      }
      else {
        innerMixingConfig.watermarkList = IntPtr.Zero;
      }

      return innerMixingConfig;
    }

    public static void ReleaseStreamMixingConfig(StreamMixingConfig config) {
      for (int i = 0; i < config.videoLayoutListSize; i++) {
        IntPtr ptr = new IntPtr(config.videoLayoutList.ToInt64() + i * Marshal.SizeOf(typeof(VideoLayout)));
        VideoLayout layout = (VideoLayout)Marshal.PtrToStructure(ptr, typeof(VideoLayout));
        Marshal.FreeHGlobal(layout.fixedVideoUser);
        Marshal.DestroyStructure(ptr, typeof(VideoLayout));
      }
      Marshal.FreeHGlobal(config.videoLayoutList);

      for (int i = 0; i < config.audioMixUserListSize; i++) {
        IntPtr ptr = new IntPtr(config.audioMixUserList.ToInt64() + i * Marshal.SizeOf(typeof(TRTCMixUser)));
        Marshal.DestroyStructure(ptr, typeof(TRTCMixUser));
      }
      Marshal.FreeHGlobal(config.audioMixUserList);

      for (int i = 0; i < config.watermarkListSize; i++) {
        IntPtr ptr = new IntPtr(config.watermarkList.ToInt64() + i * Marshal.SizeOf(typeof(TRTCWaterMark)));
        Marshal.DestroyStructure(ptr, typeof(TRTCWaterMark));
      }
      Marshal.FreeHGlobal(config.watermarkList);
    }

    public static TranscodingConfig ConvertTRTCTranscodingConfig(TRTCTranscodingConfig config) {
      TranscodingConfig innerTranscodingConfig = new TranscodingConfig();
      innerTranscodingConfig.mode = config.mode;
      innerTranscodingConfig.appId = config.appId;
      innerTranscodingConfig.bizId = config.bizId;
      innerTranscodingConfig.videoWidth = config.videoWidth;
      innerTranscodingConfig.videoHeight = config.videoHeight;
      innerTranscodingConfig.videoBitrate = config.videoBitrate;
      innerTranscodingConfig.videoFramerate = config.videoFramerate;
      innerTranscodingConfig.videoGOP = config.videoGOP;
      innerTranscodingConfig.backgroundColor = config.backgroundColor;
      innerTranscodingConfig.backgroundImage = config.backgroundImage;
      innerTranscodingConfig.audioSampleRate = config.audioSampleRate;
      innerTranscodingConfig.audioBitrate = config.audioBitrate;
      innerTranscodingConfig.audioChannels = config.audioChannels;
      innerTranscodingConfig.audioCodec = config.audioCodec;
      innerTranscodingConfig.mixUsersArraySize = config.mixUsersArraySize;
      innerTranscodingConfig.streamId = config.streamId;
      innerTranscodingConfig.videoSeiParams = config.videoSeiParams;

      return innerTranscodingConfig;
    }

    public static IntPtr StringToNativeUtf8Ptr(string inputStr) {
      if (inputStr == null) return IntPtr.Zero;
      byte[] bytes = System.Text.Encoding.UTF8.GetBytes(inputStr);
      IntPtr memPtr = Marshal.AllocHGlobal(bytes.Length + 1);
      Marshal.Copy(bytes, 0, memPtr, bytes.Length);
      Marshal.WriteByte(memPtr, bytes.Length, 0);
      return memPtr;
    }

    public static string NativeUtf8PtrToString(IntPtr ptr) {
      if (ptr == IntPtr.Zero) return null;
      int len = 0;
      while (Marshal.ReadByte(ptr, len) != 0) ++len;
      byte[] buffer = new byte[len];
      Marshal.Copy(ptr, buffer, 0, len);
      return System.Text.Encoding.UTF8.GetString(buffer);
    }

    public static TRTCParamsInner ConvertTRTCParamsToInner(TRTCParams param) {
      TRTCParamsInner innerParam = new TRTCParamsInner();
      innerParam.sdkAppId = param.sdkAppId;
      innerParam.userId = StringToNativeUtf8Ptr(param.userId);
      innerParam.userSig = StringToNativeUtf8Ptr(param.userSig);
      innerParam.roomId = param.roomId;
      innerParam.strRoomId = StringToNativeUtf8Ptr(param.strRoomId);
      innerParam.role = param.role;
      innerParam.streamId = StringToNativeUtf8Ptr(param.streamId);
      innerParam.userDefineRecordId = StringToNativeUtf8Ptr(param.userDefineRecordId);
      innerParam.privateMapKey = StringToNativeUtf8Ptr(param.privateMapKey);
      innerParam.businessInfo = StringToNativeUtf8Ptr(param.businessInfo);
      return innerParam;
    }

    public static void SafeFreeHGlobal(ref IntPtr ptr) {
      if (ptr != IntPtr.Zero) {
        Marshal.FreeHGlobal(ptr);
        ptr = IntPtr.Zero;
      }
    }

    public static void FreeTRTCParamsInner(TRTCParamsInner innerParam) {
      SafeFreeHGlobal(ref innerParam.userId);
      SafeFreeHGlobal(ref innerParam.userSig);
      SafeFreeHGlobal(ref innerParam.strRoomId);
      SafeFreeHGlobal(ref innerParam.streamId);
      SafeFreeHGlobal(ref innerParam.userDefineRecordId);
      SafeFreeHGlobal(ref innerParam.privateMapKey);
      SafeFreeHGlobal(ref innerParam.businessInfo);
    }

    public static void AllocateDeviceInfoMemory(ref DeviceInfo deviceInfo) {
      deviceInfo.devicePID = Marshal.AllocHGlobal(_deviceInfoLen);
      deviceInfo.deviceName = Marshal.AllocHGlobal(_deviceInfoLen);
      deviceInfo.deviceProperties = Marshal.AllocHGlobal(_deviceInfoLen);
      deviceInfo.devicePIDLen = _deviceInfoLen;
      deviceInfo.deviceNameLen = _deviceInfoLen;
      deviceInfo.devicePropertiesLen = _deviceInfoLen;
    }

    public static void FreeDeviceInfoMemory(ref DeviceInfo deviceInfo) {
      SafeFreeHGlobal(ref deviceInfo.devicePID);
      SafeFreeHGlobal(ref deviceInfo.deviceName);
      SafeFreeHGlobal(ref deviceInfo.deviceProperties);
    }

    public static TXDeviceInfo ConvertToTXDeviceInfo(ref DeviceInfo innerDeviceInfo) {
      TXDeviceInfo deviceInfo = new TXDeviceInfo();
      deviceInfo.devicePID = NativeUtf8PtrToString(innerDeviceInfo.devicePID);
      deviceInfo.deviceName = NativeUtf8PtrToString(innerDeviceInfo.deviceName);
      deviceInfo.deviceProperties = NativeUtf8PtrToString(innerDeviceInfo.deviceProperties);
      return deviceInfo;
    }

    public static AudioMusicParamInner ConvertAudioMusicParamToInner(AudioMusicParam param) {
      AudioMusicParamInner innerParam = new AudioMusicParamInner();
      innerParam.id = param.id;
      innerParam.path = StringToNativeUtf8Ptr(param.path);
      innerParam.loopCount = param.loopCount;
      innerParam.publish = param.publish;
      innerParam.isShortFile = param.isShortFile;
      innerParam.startTimeMS = param.startTimeMS;
      innerParam.endTimeMS = param.endTimeMS;
      return innerParam;
    }

    public static void FreeAudioMusicParamInner(AudioMusicParamInner innerParam) {
      SafeFreeHGlobal(ref innerParam.path);
    }
  }

  public class Utf8StringMarshaler : IDisposable {
    private string _stringValue;
    private IntPtr _intPtrValue;
    private bool _isStringSource;
    private bool _disposed = false;

    public Utf8StringMarshaler(string str) {
      _stringValue = str;
      _isStringSource = true;
      _intPtrValue = IntPtr.Zero;
    }

    public Utf8StringMarshaler(IntPtr ptr) {
      _intPtrValue = ptr;
      _isStringSource = false;
      _stringValue = null;
    }

    public IntPtr GetUtf8IntPtr() {
      if (_isStringSource && _stringValue != null) {
        if (_intPtrValue == IntPtr.Zero) {
          _intPtrValue = MarshalStringToUtf8IntPtr(_stringValue);
        }
      }
      return _intPtrValue;
    }

    public string GetString() {
      if (!_isStringSource && _intPtrValue != IntPtr.Zero) {
        _stringValue = MarshalUtf8IntPtrToString(_intPtrValue);
      }
      return _stringValue;
    }

    public void Dispose() {
      if (!_disposed) {
        if (_isStringSource && _intPtrValue != IntPtr.Zero) {
          MarshalFreeUtf8Ptr(ref _intPtrValue);
        }
        _disposed = true;
        GC.SuppressFinalize(this);
      }
    }

    ~Utf8StringMarshaler() {
      if (!_disposed) {
        if (_isStringSource && _intPtrValue != IntPtr.Zero) {
          MarshalFreeUtf8Ptr(ref _intPtrValue);
        }
        _disposed = true;
      }
    }

    public static IntPtr MarshalStringToUtf8IntPtr(string inputStr) {
      if (inputStr == null) return IntPtr.Zero;
      byte[] bytes = System.Text.Encoding.UTF8.GetBytes(inputStr);
      IntPtr memPtr = Marshal.AllocHGlobal(bytes.Length + 1);
      Marshal.Copy(bytes, 0, memPtr, bytes.Length);
      Marshal.WriteByte(memPtr, bytes.Length, 0);
      return memPtr;
    }

    public static string MarshalUtf8IntPtrToString(IntPtr ptr) {
      if (ptr == IntPtr.Zero) return null;
      int len = 0;
      while (Marshal.ReadByte(ptr, len) != 0) ++len;
      byte[] buffer = new byte[len];
      Marshal.Copy(ptr, buffer, 0, len);
      return System.Text.Encoding.UTF8.GetString(buffer);
    }

    public static void MarshalFreeUtf8Ptr(ref IntPtr ptr) {
      if (ptr != IntPtr.Zero) {
        Marshal.FreeHGlobal(ptr);
        ptr = IntPtr.Zero;
      }
    }
  }
}
