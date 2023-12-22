# TRTC SDK

_[简体中文](README-zh_CN.md) | English_
## Overview

Leveraging Tencent's many years of experience in network and audio/video technologies, Tencent Real-Time Communication (TRTC) offers solutions for group audio/video calls and low-latency interactive live streaming. With TRTC, you can quickly develop cost-effective, low-latency, and high-quality interactive audio/video services. [Learn more](https://trtc.io/document)...

> We offer SDKs for web, Android, iOS, Windows, Flutter, WeChat Mini Program, and [other mainstream platforms](https://github.com/LiteAVSDK?q=TRTC_&type=all&sort=).



## Changelog
### Version 11.5 @ 2023.11.27

#### Improvements
- All platforms: Optimized the performance and stability of the video engine.

- All platforms: Optimized the stability of the audio engine.

- All platforms: Optimized the behavior strategy of some APIs, see the adjustment of interface behavior for details.

- All platforms: Optimized the strategy and performance of the background music module (BGM module), reducing the occurrence of BGM playback exceptions.

- Windows: Optimized HEVC hardware decoding compatibility with AMD and Nvidia graphics cards.

- Windows: Optimized the overall performance of screen sharing, improved screen capture frame rate and stability.

- Android: Optimized the playback effect of TRTC with VODPlayer.

- iOS & Mac: Optimized the performance of pre-processing and rendering using Metal.


#### Adjustment of Interface Behavior
- All platforms: When the video resolution is set to vertical 540P (expecting 540x960), the specific resolution for processing is adjusted from 544x960 to 536x960.

- All platforms: The callback interval of BGM progress callback `onPlayProgress` is adjusted from 200ms to 300ms.

- All platforms: The internal implementation of the BGM module is adjusted to a singleton, and the musicID needs to be globally unique in multiple instances. When developers use sub-instances to play BGM, please make sure that different instances use different musicIDs.

- All platforms: Local recording event status codes are adjusted to be returned asynchronously. The default return is 0 after the related interface is called, and the specific status code is obtained through the corresponding event callback.

- All platforms: Adjust the following status codes for the `onLocalRecordBegin` callback for starting recording events.

<table>
<tr>
<td rowspan="1" colSpan="1" >Event</td>

<td rowspan="1" colSpan="1" >Status code before v11.5	</td>

<td rowspan="1" colSpan="1" >Status code in v11.5</td>
</tr>

<tr>
<td rowspan="1" colSpan="1" >Recording has started, stop previous recording first</td>

<td rowspan="1" colSpan="1" >-1</td>

<td rowspan="1" colSpan="1" >-6</td>
</tr>

<tr>
<td rowspan="1" colSpan="1" >Directory has no write permission, please check directory permissions</td>

<td rowspan="1" colSpan="1" >-2</td>

<td rowspan="1" colSpan="1" >-8</td>
</tr>

<tr>
<td rowspan="1" colSpan="1" >Incorrect file extension (e.g. unsupported recording format)	</td>

<td rowspan="1" colSpan="1" >-3</td>

<td rowspan="1" colSpan="1" >-2</td>
</tr>
</table>

- iOS & Android: Optimized the continuity of mobile screen sharing, retaining the last frame sent during sharing pause, with a frame rate of 1-2 fps.

- iOS & Android: Adjusted the response behavior of gravity sensor, only responding to gravity sensor on or off.


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
