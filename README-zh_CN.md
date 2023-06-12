# 腾讯云实时音视频 TRTC SDK

_[English](README.md) | 简体中文_

## 产品介绍

腾讯实时音视频（Tencent Real-Time Communication，TRTC），将腾讯多年来在网络与音视频技术上的深度积累，以多人音视频通话和低延时互动直播两大场景化方案，通过腾讯云服务向开发者开放，致力于帮助开发者快速搭建低成本、低延时、高品质的音视频互动解决方案，[更多](https://cloud.tencent.com/document/product/647/16788)...

> TRTC SDK 支持Web、Android、iOS、Windows以及Flutter、小程序等所有主流平台， [更多平台](https://github.com/LiteAVSDK?q=TRTC_&type=all&sort=)...



## 更新日志

### Version 11.2 @ 2023.06.05

**新特性**

- 全平台：支持合唱场景下 BGM 伴奏、原唱无缝切换，详见 setMusicTrack。
- Android：全功能版（Professional）、直播基础版（Smart）支持x86架构，并支持通过Maven获取。
- Android：满足 Android 12 及以上版本的操作系统要求，在屏幕采集时启动前台服务，详见：`enableForegroundService`。
- iOS：支持在 Apple 芯片设备上通过 Xcode 模拟器运行 SDK。
- Mac：支持获取屏幕窗口信息时返回宽高等信息，对齐 Windows，详见：TXCScreenSourceInfo。

**功能优化**

- 全平台：优化合唱场景的整体音质，提升合唱效果，减少合唱延迟。
- 全平台：优化上下麦时的音频效果，上下麦体验更平滑。
- 全平台：优化极限弱网下的音频体验。
- 全平台：优化直播单主播推流时的弱网体验。
- 全平台：优化视频通话场景大小流切换过程的流畅度。
- Android&iOS：优化音乐场景下的音质表现，提升合唱体验。
- Android&iOS：优化不同音量类型下使用蓝牙耳机的体验。
- Android：优化硬件解码延时，提升首帧体验。
- Android：优化耳返功能，提升开关耳返时的体验。
- Android：优化 Android 设备的采集兼容性，减少音频异常问题。
- iOS：优化画质表现，提升视频体验。

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
├─ TRTC-API-Example-ActiveX          // 提供 ActiveX SDK 的使用Demo
│  ├─TRTC-API-Example-ActiveX        // 提供 ActiveX 版本的Demo
│  ├─SDK                             // 生成的 ActiveX SDK 目录
│  │  ├─ActiveX              
│  │  ├─README.md
```



## 提示

- 目前只提供了的TRTC-API-Example-Qt工程、TRTC-API-Example-CSharp工程和TRTC-API-Example-ActiveX工程，如果你想使用其他的Windows平台的Demo，请参考[TRTCSDK](https://github.com/tencentyun/TRTCSDK)；



## 联系我们
- 如果你遇到了困难，可以先参阅 [常见问题](https://cloud.tencent.com/document/product/647/43018)；

- 如果你想了解TRTC SDK在复杂场景下的应用，可以参考[更多场景案例](https://cloud.tencent.com/document/product/647/57486)；

- 完整的 API 文档见 [SDK 的 API 文档](http://doc.qcloudtrtc.com/md_introduction_trtc_Windows_cpp_%E6%A6%82%E8%A7%88.html)；
- 如果需要售后技术支持, 你可以点击[这里](https://cloud.tencent.com/document/product/647/19906)；
- 如果发现了示例代码的 bug，欢迎提交 issue；
