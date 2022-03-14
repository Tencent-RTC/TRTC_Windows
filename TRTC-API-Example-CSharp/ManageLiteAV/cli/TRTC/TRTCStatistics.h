// TRTCStatistics.h

#pragma once

// C++/CLI Header
#include "cli/TRTC/TRTCCloudDef.h"

using namespace System;
using namespace System::Collections::Generic;

namespace ManageLiteAV {

    /// @addtogroup TRTCCloudDef_csharp
    /// @{

    /// <summary>
    /// 自己本地的音视频统计信息
    /// </summary>
    public ref struct TRTCLocalStatistics
    {
        /// <summary>
        /// 视频宽度
        /// </summary>
        UInt32 width;
        /// <summary>
        /// 视频高度
        /// </summary>
        UInt32 height;
        /// <summary>
        /// 帧率（fps）
        /// </summary>
        UInt32 frameRate;
        /// <summary>
        /// 视频发送码率（Kbps）
        /// </summary>
        UInt32 videoBitrate;
        /// <summary>
        /// 音频采样率（Hz）
        /// </summary>
        UInt32 audioSampleRate;
        /// <summary>
        /// 音频发送码率（Kbps）
        /// </summary>
        UInt32 audioBitrate;
        /// <summary>
        /// 流类型（大画面 | 小画面 | 辅路画面）
        /// </summary>
        TRTCVideoStreamType streamType;
    };

    /// <summary>
    /// 远端成员的音视频统计信息
    /// </summary>
    public ref struct TRTCRemoteStatistics
    {
        /// <summary>
        /// 用户 ID，指定是哪个用户的视频流
        /// </summary>
        String^ userId;
        /// <summary>
        /// 该线路的总丢包率（％）
        /// 
        /// <para>这个值越小越好，例如，丢包率为0表示网络很好。</para>
        /// <para>丢包率是该线路的 userId 从上行到服务器再到下行的总丢包率。</para>
        /// <para>如果 downLoss 为0，但是 finalLoss 不为0，说明该 userId 上行时出现了无法恢复的丢包。</para>
        /// </summary>
        UInt32 finalLoss;
        /// <summary>
        /// 视频宽度
        /// </summary>
        UInt32 width;
        /// <summary>
        /// 视频高度
        /// </summary>
        UInt32 height;
        /// <summary>
        /// 接收帧率（fps）
        /// </summary>
        UInt32 frameRate;
        /// <summary>
        /// 视频码率（Kbps）
        /// </summary>
        UInt32 videoBitrate;
        /// <summary>
        /// 音频采样率（Hz）
        /// </summary>
        UInt32 audioSampleRate;
        /// <summary>
        /// 音频码率（Kbps）
        /// </summary>
        UInt32 audioBitrate;
        /// <summary>
        /// 流类型（大画面 | 小画面 | 辅路画面）
        /// </summary>
        TRTCVideoStreamType streamType;
    };

    /// <summary>
    /// 统计数据
    /// </summary>
    public ref struct TRTCStatistics
    {
        /// <summary>
        /// C -> S 上行丢包率（％），
        /// <para>
        /// 该值越小越好，例如，丢包率为0表示网络很好，
        /// 丢包率为30@%则意味着 SDK 向服务器发送的数据包中会有30@%丢失在上行传输中。
        /// </para>
        /// </summary>
        UInt32 upLoss;
        /// <summary>
        /// S -> C 下行丢包率（％），
        /// 该值越小越好，例如，丢包率为0表示网络很好，
        /// 丢包率为30@%则意味着 SDK 向服务器发送的数据包中会有30@%丢失在下行传输中。
        /// </summary>
        UInt32 downLoss;
        /// <summary>
        /// 当前 App 的 CPU 使用率（％）
        /// </summary>
        UInt32 appCpu;
        /// <summary>
        /// 当前系统的 CPU 使用率（％）
        /// </summary>
        UInt32 systemCpu;
        /// <summary>
        /// 延迟（毫秒），指 SDK 到腾讯云服务器的一次网络往返时间，该值越小越好。
        /// <para>一般低于50ms的 rtt 相对理想，而高于100ms的 rtt 会引入较大的通话延时。</para>
        /// <para>由于数据上下行共享一条网络连接，所以 local 和 remote 的 rtt 相同。</para>
        /// </summary>
        UInt32 rtt;
        /// <summary>
        /// 总接收字节数（包含信令和音视频）
        /// </summary>
        UInt32 receivedBytes;
        /// <summary>
        /// 总发送字节总数（包含信令和音视频）
        /// </summary>
        UInt32 sentBytes;
        /// <summary>
        /// 本地的音视频统计信息，可能有主画面、小画面以及辅路画面等多路的情况，因此是一个数组
        /// </summary>
        array<TRTCLocalStatistics^>^ localStatisticsArray;
        /// <summary>
        /// 数组 localStatisticsArray 的大小
        /// </summary>
        UInt32 localStatisticsArraySize;
        /// <summary>
        /// 远端成员的音视频统计信息，可能有主画面、小画面以及辅路画面等多路的情况，因此是一个数组
        /// </summary>
        array<TRTCRemoteStatistics^>^ remoteStatisticsArray;
        /// <summary>
        /// 数组 remoteStatisticsArray 的大小
        /// </summary>
        UInt32 remoteStatisticsArraySize;
    };
    /// @}
}