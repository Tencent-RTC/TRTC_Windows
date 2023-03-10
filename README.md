# TRTC SDK

_[简体中文](README-zh_CN.md) | English_
## Overview

Leveraging Tencent's many years of experience in network and audio/video technologies, Tencent Real-Time Communication (TRTC) offers solutions for group audio/video calls and low-latency interactive live streaming. With TRTC, you can quickly develop cost-effective, low-latency, and high-quality interactive audio/video services. [Learn more](https://cloud.tencent.com/document/product/647/16788)...

> We offer SDKs for web, Android, iOS, Windows, Flutter, WeChat Mini Program, and [other mainstream platforms](https://github.com/LiteAVSDK?q=TRTC_&type=all&sort=).



## Changelog

### Version 11.0 @ 2023.03.08

**NEW FEATURES**
- Android: Interface changes, the return type of TXLiveBase.setLibraryPath is adjusted to bool, indicating whether the SDK dynamic library is loaded successfully.

**Function Optimization**
- All platforms: Improved the success rate of palying eonline BGM when network is weak.
- All platforms: Optimized the fluency of the first video frame in video call scene.
- Android: Optimized audio compatibility, reduce current noise and silent problems.

**BUG FIX**
- All platforms: Fixed the occasional crash problem when using the sendCustomCmdMsg function in the case of frequent check-in and check-out.
- All platforms: Fixed the problem of incorrectly calling back the onRemoteUserLeaveRoom, onUserVideoAvailable, and onUserAudioAvailable of the remote host when checking out locally.
- All platforms: Fixed the issue where noise may be heard when the remote anchor is muted.



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
