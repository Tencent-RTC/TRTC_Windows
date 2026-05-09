// Copyright (c) 2023 Tencent. All rights reserved.
// Author: makbaktan

namespace liteav {
  public class V2TXLiveBaseNative {
#if UNITY_ANDROID && !UNITY_EDITOR
    protected const string V2TXLiveLibName = "liteavsdk";
#elif UNITY_OPENHARMONY && !UNITY_EDITOR
    protected const string V2TXLiveLibName = "liteavsdk";
#elif (UNITY_IPHONE || UNITY_WEBGL) && !UNITY_EDITOR
    protected const string V2TXLiveLibName = "__Internal";
#elif UNITY_STANDALONE_OSX || UNITY_EDITOR_OSX
    protected const string V2TXLiveLibName = "TXLiteAVSDK_TRTC_Mac";
#else
    protected const string V2TXLiveLibName = "liteav";
#endif
  }
}
