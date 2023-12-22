# 腾讯云实时音视频 TRTC SDK

_[English](README.md) | 简体中文_

## 产品介绍

腾讯实时音视频（Tencent Real-Time Communication，TRTC），将腾讯多年来在网络与音视频技术上的深度积累，以多人音视频通话和低延时互动直播两大场景化方案，通过腾讯云服务向开发者开放，致力于帮助开发者快速搭建低成本、低延时、高品质的音视频互动解决方案，[更多](https://cloud.tencent.com/document/product/647/16788)...

> TRTC SDK 支持Web、Android、iOS、Windows以及Flutter、小程序等所有主流平台， [更多平台](https://github.com/LiteAVSDK?q=TRTC_&type=all&sort=)...



## 更新日志
### Version 11.5 @ 2023.11.27

#### 功能优化：
- 全平台：优化视频模块的整体性能及稳定性。

- 全平台：优化音频模块的整体稳定性。

- 全平台：优化部分 API 接口行为策略，详情见接口行为调整。

- 全平台：优化音频背景音乐模块整体策略及性能占用，减少背景音乐播放异常的情况。

- Windows：优化 HEVC 硬件解码策略，适配 AMD、Nvidia 显卡。

- Windows：优化屏幕共享整体性能，提升屏幕采集帧率及稳定性。

- Android：优化 TRTC + VODPlayer 场景下的播放效果。

- iOS&Mac：优化使用 Metal 进行预处理及渲染的性能。


#### 接口行为调整：
- 全平台：视频设置分辨率为540P竖屏时（预期540x960），具体的编码处理分辨率由544x960调整为536x960。

- 全平台：背景音乐进度回调`onPlayProgress`回调间隔由200ms 调整为300ms。

- 全平台：背景音乐模块内部实现调整为单例，多实例情况下 musicID 需要全局唯一。开发者在使用子实例播放背景音乐时，请确保不同实例使用不同的 musicID。

- 全平台：本地录制事件状态码调整为异步返回，相关接口调用后默认返回 0，具体的状态码通过相应的事件回调获取。

- 全平台：启动录制事件回调`onLocalRecordBegin`调整以下状态码：

<table>
<tr>
<td rowspan="1" colSpan="1" ><br>事件</td>

<td rowspan="1" colSpan="1" >11.5 之前版本状态码</td>

<td rowspan="1" colSpan="1" >11.5 版本状态码</td>
</tr>

<tr>
<td rowspan="1" colSpan="1" >录制已经启动，需要先停止录制</td>

<td rowspan="1" colSpan="1" >-1</td>

<td rowspan="1" colSpan="1" >-6</td>
</tr>

<tr>
<td rowspan="1" colSpan="1" >录制目录无写入权限，请检查目录权限问题</td>

<td rowspan="1" colSpan="1" >-2</td>

<td rowspan="1" colSpan="1" >-8</td>
</tr>

<tr>
<td rowspan="1" colSpan="1" >文件后缀名有误（比如不支持的录制格式）</td>

<td rowspan="1" colSpan="1" >-3</td>

<td rowspan="1" colSpan="1" >-2</td>
</tr>
</table>

- iOS&Android：优化移动端屏幕共享连续性，在共享暂停时保留发送最后一帧，发送帧率为1 - 2fps。

- iOS&Android：调整重力感应响应行为，只响应重力感应开或者关。

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
