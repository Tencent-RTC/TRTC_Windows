# TRTC SDK

_English | [简体中文](README.md)_

## Overview

Leveraging Tencent's many years of experience in network and audio/video technologies, Tencent Real-Time Communication (TRTC) offers solutions for group audio/video calls and low-latency interactive live streaming. With TRTC, you can quickly develop cost-effective, low-latency, and high-quality interactive audio/video services. [Learn more](https://cloud.tencent.com/document/product/647/16788)...

> We offer SDKs for web, Android, iOS, Windows, Flutter, WeChat Mini Program, and [other mainstream platforms](https://github.com/LiteAVSDK?q=TRTC_&type=all&sort=).



## Changelog
## Version 10.1 @ 2022.06.06

**New features:**

- All platforms: Support smooth switching of roles, audio and video playback will not be interrupted briefly by role-switching.
- iOS: Support stereo audio capture.
- Android: Support capturing system playback audio on Android 10 and above (see: startSystemAudioLoopback for details).


**Function optimization:**

- All platforms: Optimize the echo cancellation capability in music scenes for more natural sound quality effect.
- All platforms: Optimize the sound quality and startup effect of switching roles + muteLocalAudio.
- All platforms: Optimize bandwidth prediction onSpeedTest callbacks.
- iOS:Optimize memory management to avoid memory buildup.
- Android: Optimize the delay of the ear return on some models of mobile phones.
- Windows: Optimize the performance of the video rendering link when video is downlink.
- Windows: Optimize the stereo capture logic to effectively avoid the problem of echo leakage.

**Bug fixes:**

- All platforms: Fix the reason exception issue of the check-out callback (onExitRoom)
- All platforms: Fix the black screen problem when sending upstream custom video with equal timestamp.
- All platforms: Fix the crash problem when muteLocalAudio first and then startLocalAudio audio
- All platforms: Fix the problem that turning on custom audio capture without manually setting 3A scene will open 3A.
- All platforms: Fix the occasional noise issue in custom audio rendering.
- iOS: Fixa memory leak when setting the log path (setLogDirPath) midway and the sandbox changes.
- iOS & Mac: Fix the crash of BGM-continuous-play scenes when the system audio service is abnormal.
- Android: Fix an occasional Bluetooth headset reconnection issue.
- Android: Fixthe occasional silence issue on some phones.
- Android: Fix the crash caused by repeatedly plugging and unpluggingheadphones on some models such as Redmi.
- Windows & iOS: Fix the problem of screenshot failure.
- Windows: Fix the crash when closing vod after turning on mirroring in VOD player.
- Windows: Fix the problem that multiple playback of pts may cause the pts to fall back if the pts are not using generatedCustomPts.
- Windows:  Fix the crash about muteLocalVideo api.


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
