/**
 * Copyright (c) 2021 Tencent. All rights reserved.
 * Module:   V2TXLivePlayerObserver @ TXLiteAVSDK
 * Function: 腾讯云直播的播放器回调通知
 * <H2>功能
 * 腾讯云直播的播放器回调通知。
 * <H2>介绍
 * 可以接收 {@link V2TXLivePlayer} 播放器的一些回调通知，包括播放器状态、播放音量回调、音视频首帧回调、统计数据、警告和错误信息等。
 */
using System;
namespace liteav {
    public interface V2TXLivePlayerObserver {
        /////////////////////////////////////////////////////////////////////////////////
        //
        //                   直播播放器事件回调
        //
        /////////////////////////////////////////////////////////////////////////////////

        /**
         * 直播播放器错误通知，播放器出现错误时，会回调该通知
         *
         * @param player    回调该通知的播放器对象。
         * @param code      错误码 {@link V2TXLiveCode}。
         * @param msg       错误信息。
         * @param extraInfo 扩展信息。
         */
        void onError(V2TXLivePlayer player, V2TXLiveCode code, string msg, IntPtr extraInfo);

        /**
         * 直播播放器警告通知
         *
         * @param player    回调该通知的播放器对象。
         * @param code      警告码 {@link V2TXLiveCode}。
         * @param msg       警告信息。
         * @param extraInfo 扩展信息。
         */
        void onWarning(V2TXLivePlayer player, V2TXLiveCode code, string msg, IntPtr extraInfo);

        /**
         * 直播播放器分辨率变化通知
         *
         * @param player    回调该通知的播放器对象。
         * @param width     视频宽。
         * @param height    视频高。
         */
        void onVideoResolutionChanged(V2TXLivePlayer player, int width, int height);

        /**
         * 已经成功连接到服务器
         *
         * @param player    回调该通知的播放器对象。
         * @param extraInfo 扩展信息。
         */
        void onConnected(V2TXLivePlayer player, IntPtr extraInfo);

        /**
         * 视频播放事件
         *
         * @param player    回调该通知的播放器对象。
         * @param firstPlay 第一次播放标志。
         * @param extraInfo 扩展信息。
         */
        void onVideoPlaying(V2TXLivePlayer player, bool firstPlay, IntPtr extraInfo);

        /**
         * 音频播放事件
         *
         * @param player    回调该通知的播放器对象。
         * @param firstPlay 第一次播放标志。
         * @param extraInfo 扩展信息。
         */
        void onAudioPlaying(V2TXLivePlayer player, bool firstPlay, IntPtr extraInfo);

        /**
         * 视频加载事件
         *
         * @param player    回调该通知的播放器对象。
         * @param extraInfo 扩展信息。
         */
        void onVideoLoading(V2TXLivePlayer player, IntPtr extraInfo);

        /**
         * 音频加载事件
         *
         * @param player    回调该通知的播放器对象。
         * @param extraInfo 扩展信息。
         */
        void onAudioLoading(V2TXLivePlayer player, IntPtr extraInfo);

        /**
         * 直播播放器统计数据回调
         *
         * @param player     回调该通知的播放器对象。
         * @param statistics 播放器统计数据 {@link V2TXLivePlayerStatistics}。
         */
        void onStatisticsUpdate(V2TXLivePlayer player, V2TXLivePlayerStatistics statistics);

        /**
         * 自定义视频渲染回调
         *
         * @param player     回调该通知的播放器对象。
         * @param videoFrame 视频帧数据 {@link V2TXLiveVideoFrame}。
         * @note  需要您调用 {@link enableObserveVideoFrame} 开启回调开关。
         */
        void onRenderVideoFrame(V2TXLivePlayer player, V2TXLiveVideoFrame videoFrame);
    }
}
