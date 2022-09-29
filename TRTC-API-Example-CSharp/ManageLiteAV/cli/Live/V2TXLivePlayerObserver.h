#pragma once
#include "cli/Live/V2TXLiveDef.h"
using namespace System;

namespace ManageLiteAV
{
    ref class V2TXLivePlayer;
    /// @defgroup ITRTCCloudCallback_csharp ITRTCCloudCallback
    /// 腾讯云直播的播放器回调通知
    /// @{
    /// <summary>
    /// 可以接收 V2TXLivePlayer 播放器的一些回调通知，包括播放器状态、播放音量回调、音视频首帧回调、统计数据、警告和错误信息等。
    /// </summary>
    public interface class V2TXLivePlayerObserver {
        /////////////////////////////////////////////////////////////////////////////////
        //
        //                   直播播放器事件回调
        //
        /////////////////////////////////////////////////////////////////////////////////

         /// <summary>
         /// 直播播放器错误通知，播放器出现错误时，会回调该通知
         /// </summary>
         /// <param name="player">回调该通知的播放器对象</param>
         /// <param name="code">错误码 {@link V2TXLiveCode}</param>
         /// <param name="msg">错误信息</param>
         /// <param name="extraInfo">扩展信息</param>
        void onError(ManageLiteAV::V2TXLivePlayer^ player, Int32 code, String^% msg, IntPtr extraInfo);

        /// <summary>
        /// 直播播放器警告通知
        /// </summary>
        /// <param name="player">回调该通知的播放器对象</param>
        /// <param name="code">警告码 {@link V2TXLiveCode}</param>
        /// <param name="msg">警告信息</param>
        /// <param name="extraInfo">扩展信息</param>
        void onWarning(ManageLiteAV::V2TXLivePlayer^ player, Int32 code, String^% msg, IntPtr extraInfo);

        /// <summary>
        /// 直播播放器分辨率变化通知
        /// </summary>
        /// <param name="player">回调该通知的播放器对象</param>
        /// <param name="width">视频宽</param>
        /// <param name="height">视频高</param>
        void onVideoResolutionChanged(ManageLiteAV::V2TXLivePlayer^ player, int width, int height);

        /// <summary>
        /// 已经成功连接到服务器
        /// </summary>
        /// <param name="player">回调该通知的播放器对象</param>
        /// <param name="extraInfo">扩展信息</param>
        void onConnected(ManageLiteAV::V2TXLivePlayer^ player, IntPtr extraInfo);

        /// <summary>
        /// 视频播放事件
        /// </summary>
        /// <param name="player">回调该通知的播放器对象</param>
        /// <param name="firstPlay">第一次播放标志</param>
        /// <param name="extraInfo">扩展信息</param>
        void onVideoPlaying(ManageLiteAV::V2TXLivePlayer^ player, bool firstPlay, IntPtr extraInfo);

        /// <summary>
        /// 音频播放事件
        /// </summary>
        /// <param name="player">回调该通知的播放器对象</param>
        /// <param name="firstPlay">第一次播放标志</param>
        /// <param name="extraInfo">扩展信息</param>
        void onAudioPlaying(ManageLiteAV::V2TXLivePlayer^ player, bool firstPlay, IntPtr extraInfo);

        /// <summary>
        /// 视频加载事件
        /// </summary>
        /// <param name="player">回调该通知的播放器对象</param>
        /// <param name="extraInfo">扩展信息</param>
        void onVideoLoading(ManageLiteAV::V2TXLivePlayer^ player, IntPtr extraInfo);

        /// <summary>
        /// 音频加载事件
        /// </summary>
        /// <param name="player">回调该通知的播放器对象</param>
        /// <param name="extraInfo">扩展信息</param>
        void onAudioLoading(ManageLiteAV::V2TXLivePlayer^ player, IntPtr extraInfo);

        /// <summary>
        /// 播放器音量大小回调
        /// </summary>
        /// <param name="player">回调该通知的播放器对象</param>
        /// <param name="volume">音量大小</param>
        void onPlayoutVolumeUpdate(ManageLiteAV::V2TXLivePlayer^ player, Int32 volume);

        /// <summary>
        /// 直播播放器统计数据回调
        /// </summary>
        /// <param name="player">回调该通知的播放器对象</param>
        /// <param name="statistics">播放器统计数据 {@link V2TXLivePlayerStatistics}</param>
        void onStatisticsUpdate(ManageLiteAV::V2TXLivePlayer^ player, V2TXLivePlayerStatistics^ statistics);

        /// <summary>
        /// 截图回调
        /// </summary>
        /// <param name="player">回调该通知的播放器对象</param>
        /// <param name="image">已截取的视频画面</param>
        /// <param name="length">截图数据长度，对于BGRA32而言，length = width * height * 4</param>
        /// <param name="width">截图画面的宽度</param>
        /// <param name="height">截图画面的高度</param>
        /// <param name="format">截图数据格式，目前只支持 V2TXLivePixelFormatBGRA32</param>
        void onSnapshotComplete(ManageLiteAV::V2TXLivePlayer^ player, array<Byte>^% image, int length, int width, int height, V2TXLivePixelFormat format);

        /// <summary>
        /// 自定义视频渲染回调
        /// </summary>
        /// <param name="player">回调该通知的播放器对象</param>
        /// <param name="videoFrame">视频帧数据 {@link V2TXLiveVideoFrame}</param>
        void onRenderVideoFrame(ManageLiteAV::V2TXLivePlayer^ player, V2TXLiveVideoFrame^% videoFrame);

        /// <summary>
        /// 收到 SEI 消息的回调，发送端通过 {@link V2TXLivePusher} 中的 `sendSeiMessage` 来发送 SEI 消息。
        /// </summary>
        /// <param name="player">回调该通知的播放器对象</param>
        /// <param name="payloadType">收到 SEI 消息的 payloadType</param>
        /// <param name="data">数据</param>
        /// <param name="dataSize">数据大小</param>
        void onReceiveSeiMessage(ManageLiteAV::V2TXLivePlayer^ player, int payloadType, array<Byte>^% data, UInt32 dataSize);
    };
}