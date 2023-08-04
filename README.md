# TRTC SDK

_[简体中文](README-zh_CN.md) | English_
## Overview

Leveraging Tencent's many years of experience in network and audio/video technologies, Tencent Real-Time Communication (TRTC) offers solutions for group audio/video calls and low-latency interactive live streaming. With TRTC, you can quickly develop cost-effective, low-latency, and high-quality interactive audio/video services. [Learn more](https://cloud.tencent.com/document/product/647/16788)...

> We offer SDKs for web, Android, iOS, Windows, Flutter, WeChat Mini Program, and [other mainstream platforms](https://github.com/LiteAVSDK?q=TRTC_&type=all&sort=).



## Changelog

### Version 11.3 @ 2023.07.07

**New features**

- All Platforms: Added trapezoid correction for video (only supported by the Professional version) for manual correction of perspective distortion. See `setPerspectiveCorrectionPoints` for details.
- All Platforms: Added audio spectrum callback, which can be used for sound wave animation or volume spectrum display. See `enableAudioVolumeEvaluation` and `TRTCVolumeInfo` for details.
- All Platforms: Added a new reverb effect "Studio 2". See `TXVoiceReverbType` for details.
- All Platforms: Added SEI parameter settings for mixed stream, used for transport SEI infomation when publishing stream to CDN. See `TRTCTranscodingConfig` for details.
- Windows: Added real-time music scoring for Yinsuda Authorized Music, which can be used for real-time scoring of online singing. See `createSongScore` for details.
- iOS & Android: Added support for .ogg format music files in `startPlayMusic`.
- Flutter: Added `setSystemAudioLoopbackVolume`(iOS).


**Improvements**

- All platforms: Optimized adaptive digital gain algorithm to improve listening experience.
- All platforms: Optimized the loading speed of the first video frame after entering the room.
- All platforms: Optimized weak network resistance for single user streaming to improve smoothness under network delay and jitter.
- Android: Optimized audio capture and playback feature to avoid abnormal sound issues on some Android devices.
- Android: Optimized video sub-stream hardware encoding performance, improving quality of screen sharing.
- iOS: Optimized audio device restart strategy to reduce the occurrence of sound interruptions.
- iOS & Android: Removed on-demand related interfaces from `TXLivePlayer`. For on-demand video playback, please use `TXVodPlayer`.

**Bug fixes**
- Android: Fixed the issue where some locally recorded videos on Android 12 and above system versions cannot be played on Apple's Safari.


For the release notes of earlier versions, click [More](https://cloud.tencent.com/document/product/647/46907).


## Contents

```bash
├─ TRTC-API-Example-C++             // Demo for C++
│  ├─TRTC-API-Example-Qt            // Demo for Qt
│  ├─SDK                            // C++ SDK directory
│  │  ├─CPlusPlus
│  │  ├─README.md
├─ TRTC-API-Example-CSharp          // Demo for C#
│  ├─TRTC-Example-CSharp.sln        // Project solution
│  ├─TRTC-API-Example-CSharp        // Demo for C#
│  ├─ManageLiteAV                   // C# SDK's adapter project
│  │  ├─thirdparty                  // Third-party library directory
│  │  │  ├─liteav              
│  │  │  │  ├─CPlusPlus             // C++ SDK is accessed as a third-party library   
│  ├─SDK                            // C# SDK directory
│  │  ├─CSharp              
│  │  ├─README.md
├─ TRTC-API-Example-ActiveX          // Demo for ActiveX SDK 
│  ├─TRTC-API-Example-ActiveX        // Demo for ActiveX
│  ├─SDK                             // ActiveX SDK directory
│  │  ├─ActiveX              
│  │  ├─README.md
```



## Note

- Currently, only the TRTC-API-Example-Qt、TRTC-API-Example-CSharp and TRTC-API-Example-ActiveX projects are provided. If you want to use the demo of other Windows platforms, see [TRTC SDK](https://github.com/tencentyun /TRTCSDK);



## Contact Us
- If you have questions, see [FAQs](https://www.tencentcloud.com/document/product/647/36057?lang=en&pg=).

- To learn about how the TRTC SDK can be used in different scenarios, see [Sample Code](https://www.tencentcloud.com/document/product/647/42963).

- For the complete API documentation, see [SDK API Documentation](http://doc.qcloudtrtc.com/md_introduction_trtc_Windows_cpp_%E6%A6%82%E8%A7%88.html).
- For aftersales technical support, please [contact us](https://t.me/+EPk6TMZEZMM5OGY1).
- To report bugs in our sample code, please create an issue.
