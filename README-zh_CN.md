# 腾讯云实时音视频 TRTC SDK

_[English](README.md) | 简体中文_

## 产品介绍

腾讯实时音视频（Tencent Real-Time Communication，TRTC），将腾讯多年来在网络与音视频技术上的深度积累，以多人音视频通话和低延时互动直播两大场景化方案，通过腾讯云服务向开发者开放，致力于帮助开发者快速搭建低成本、低延时、高品质的音视频互动解决方案，[更多](https://cloud.tencent.com/document/product/647/16788)...

> TRTC SDK 支持Web、Android、iOS、Windows以及Flutter、小程序等所有主流平台， [更多平台](https://github.com/LiteAVSDK?q=TRTC_&type=all&sort=)...



## 更新日志
## Version 10.2 @ 2022.06.26

**新特性：**
- 全平台：全新推出更加灵活，且功能强大的混流转推 API。详见：startPublishMediaStream；
- 全平台：新增 3D 音频特效功能，详见：enable3DSpatialAudioEffect；
- 全平台：新增人声检测功能，当 muteLoalAudio 和 setAudioCaptureVolume 为 0 时不会影响人声检测结果。详见 enableAudioVolumeEvaluation，Tips：方便提示用户开麦；
- 全平台：切换角色时，增加支持权限校验的功能。详见：switchRole(TRTCRoleType role, const char* privateMapKey)；
- iOS & Mac：自定义预处理的 C++ 接口，支持以纹理方式对接视频处理；

**功能优化:**
- Android：优化耳返效果，降低延迟；
- Android：优化音频的采集链路，解决部分机型存在的杂音问题；
- iOS：优化上行视频处理链路，节省CPU、GPU占用；
- Windows & Mac：优化窗口分享时的编码性能，编码宽高不再受采集窗口大小的影响；
- Windows：优化性能，减少内存碎片及其分配时造成性能开销；

**缺陷修复:**
- 全平台：修复切换网络类型时，偶现的上行失败问题；
- iOS：修复在部分iOS 14系统上，本地录制文件存在的杂音问题；

更早期的版本更新历史请点击  [更多](https://cloud.tencent.com/document/product/647/46907)...


## 目录说明

```bash
├─ TRTC-API-Example-C++             // 提供 C++ 语言的Demo
│  ├─TRTC-API-Example-Qt            // 提供 Qt 框架的Demo
│  ├─SDK                            // C++ SDK目录
│  │  ├─CPlusPlus
│  │  ├─README.md
├─ TRTC-API-Example-CSharp          // 提供 C# 语言的Demo
│  ├─TRTC-Example-CSharp.sln        // 工程解决方案
│  ├─TRTC-API-Example-CSharp        // 提供 C# 版本的Demo
│  ├─ManageLiteAV                   // C# SDK 的 Adapter 工程
│  │  ├─thirdparty                  // 第三方库目录
│  │  │  ├─liteav              
│  │  │  │  ├─CPlusPlus             // C++SDK 以第三方库方式接入   
│  ├─SDK                            // C# SDK 目录
│  │  ├─CSharp              
│  │  ├─README.md
```



## 提示

- 目前只提供了的TRTC-API-Example-Qt工程和TRTC-API-Example-CSharp工程，如果你想使用其他的Windows平台的Demo，请参考[TRTCSDK](https://github.com/tencentyun/TRTCSDK)；



## 联系我们
- 如果你遇到了困难，可以先参阅 [常见问题](https://cloud.tencent.com/document/product/647/43018)；

- 如果你想了解TRTC SDK在复杂场景下的应用，可以参考[更多场景案例](https://cloud.tencent.com/document/product/647/57486)；

- 完整的 API 文档见 [SDK 的 API 文档](http://doc.qcloudtrtc.com/md_introduction_trtc_Windows_cpp_%E6%A6%82%E8%A7%88.html)；
- 如果需要售后技术支持, 你可以点击[这里](https://cloud.tencent.com/document/product/647/19906)；
- 如果发现了示例代码的 bug，欢迎提交 issue；
