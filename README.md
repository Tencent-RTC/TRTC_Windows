# TRTC SDK

_[简体中文](README-zh_CN.md) | English_
## Overview

Leveraging Tencent's many years of experience in network and audio/video technologies, Tencent Real-Time Communication (TRTC) offers solutions for group audio/video calls and low-latency interactive live streaming. With TRTC, you can quickly develop cost-effective, low-latency, and high-quality interactive audio/video services. [Learn more](https://cloud.tencent.com/document/product/647/16788)...

> We offer SDKs for web, Android, iOS, Windows, Flutter, WeChat Mini Program, and [other mainstream platforms](https://github.com/LiteAVSDK?q=TRTC_&type=all&sort=).



## Changelog
## Version 10.2 @ 2022.06.26

**New features:**

- All platforms: A new, more flexible and powerful mixed-stream retweet API has been introduced. See: startPublishMediaStream;
- All platforms: New 3D audio effect function, see: enable3DSpatialAudioEffect;
- All platforms: Add vocal detection function. When muteLoalAudio and setAudioCaptureVolume are 0, it will not affect the vocal detection result. See enableAudioVolumeEvaluation for details, Tips: It is convenient to prompt the user to open the microphone;
- All platforms: Add permission verification when switching roles. For details, see: switchRole(TRTCRoleType role, const char* privateMapKey).
- iOS & Mac: Customize C++ interface for preprocessing, support video processing in texture mode;

**Function optimization:**

- Android: Optimize ear return effect and reduce latency;
- Android: Optimize the audio capture link to solve the noise problem of some models;
- iOS: Optimize the uplink video processing link to save CPU and GPU usage;
- Windows & Mac: Optimize the encoding performance when sharing windows, the encoding width and height are no longer affected by the capture window size;
- Windows: Optimize performance, reduce memory fragmentation and performance overhead caused by its allocation;

Bug fixes:

- All platforms: Fix occasional uplink failure when switching network types;
- iOS: Fix the noise problem with locally recorded files on some iOS 14 systems;


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
```



## Note

- Currently, only the TRTC-API-Example-Qt and TRTC-API-Example-CSharp projects are provided. If you want to use the demo of other Windows platforms, see [TRTC SDK](https://github.com/tencentyun /TRTCSDK);



## Contact Us
- If you have questions, see [FAQs](https://cloud.tencent.com/document/product/647/43018).

- To learn about how the TRTC SDK can be used in different scenarios, see [Sample Code](https://intl.cloud.tencent.com/document/product/647/42963).

- For the complete API documentation, see [SDK API Documentation](http://doc.qcloudtrtc.com/md_introduction_trtc_Windows_cpp_%E6%A6%82%E8%A7%88.html).
- For aftersales technical support, please [contact us](https://intl.cloud.tencent.com/contact-us).
- To report bugs in our sample code, please create an issue.
