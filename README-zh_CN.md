# 腾讯云实时音视频 TRTC SDK

_[English](README.md) | 简体中文_

## 产品介绍

腾讯实时音视频（Tencent Real-Time Communication，TRTC），将腾讯多年来在网络与音视频技术上的深度积累，以多人音视频通话和低延时互动直播两大场景化方案，通过腾讯云服务向开发者开放，致力于帮助开发者快速搭建低成本、低延时、高品质的音视频互动解决方案，[更多](https://cloud.tencent.com/document/product/647/16788)...

> TRTC SDK 支持Web、Android、iOS、Windows以及Flutter、小程序等所有主流平台， [更多平台](https://github.com/LiteAVSDK?q=TRTC_&type=all&sort=)...



## 更新日志
### Version 11.1 @ 2023.04.17

**新特性**

- 全平台：新增 onVoiceEarMonitorAudioFrame 数据回调，用于获取或修改耳返数据。
- 全平台：数据回调 onCapturedRawAudioFrame 优化命名为 onCapturedAudioFrame
- Mac：窗口分享支持 PPT 放映模式

**功能优化**

- 全平台：优化 log 文件自动清理逻辑，防止 log 文件夹体积超标。
- iOS & Android：优化解码渲染时的颜色矩阵兼容性，避免引入色彩偏差。
- Android：优化低端机在高分辨率场景下偶现硬编无法启动导致性能开销增大的问题。
- Android：优化 Android 12以上系统偶现硬编码率不受控的问题。
- Android：优化在合唱场景中少量机型主播采集声音小的问题。
- Android：优化在合唱场景中偶现的声音剪切严重的问题。

**缺陷修复**
- Android：修复蓝牙耳机连接状态但未启用时漏回声。
- Windows：修复开关系统混音偶现漏回声的问题。

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
