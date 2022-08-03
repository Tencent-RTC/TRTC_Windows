# 腾讯云实时音视频 TRTC SDK

_[English](README.md) | 简体中文_

## 产品介绍

腾讯实时音视频（Tencent Real-Time Communication，TRTC），将腾讯多年来在网络与音视频技术上的深度积累，以多人音视频通话和低延时互动直播两大场景化方案，通过腾讯云服务向开发者开放，致力于帮助开发者快速搭建低成本、低延时、高品质的音视频互动解决方案，[更多](https://cloud.tencent.com/document/product/647/16788)...

> TRTC SDK 支持Web、Android、iOS、Windows以及Flutter、小程序等所有主流平台， [更多平台](https://github.com/LiteAVSDK?q=TRTC_&type=all&sort=)...



## 更新日志
### Version 10.4 @ 2022.07.21

**新特性：**
- iOS & Android：自定义视频采集支持 RGBA32 格式，详见：sendCustomVideoData；
- Windows & Mac：水印设置支持本地预览，详见：setWaterMark；

**功能优化:**
- Android：优化低延迟耳返及双声道采集的兼容性；
- Android：优化硬解码切软解码的策略，提升解码性能；
- iOS：优化 iPad 采集音量小的问题；

**缺陷修复:**

- 全平台：修复偶现的进退房回调异常的问题；
- Windows：修复切换分享窗口，新窗口的内容被剪裁的问题；
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
