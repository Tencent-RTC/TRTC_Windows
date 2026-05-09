/**
 * Copyright (c) 2021 Tencent. All rights reserved.
 * Module:   V2TXLivePlayer @ TXLiteAVSDK
 * Function: 腾讯云直播播放器
 * <H2>功能
 * 腾讯云直播播放器。
 * 主要负责从指定的直播流地址拉取音视频数据，并进行解码和本地渲染播放。
 * <H2>介绍
 * 播放器包含如下能力：
 * - 支持 RTMP、HTTP-FLV、HLS、TRTC、WebRTC 协议。
 * - 屏幕截图，可以截取当前直播流的视频画面。
 * - 延时调节，可以设置播放器缓存自动调整的最小和最大时间。
 * - 自定义的视频数据处理，您可以根据项目需要处理直播流中的视频数据后，再进行渲染以及播放。
 */
using System;
namespace liteav {

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                    播放器相关接口
    //
    /////////////////////////////////////////////////////////////////////////////////

    public abstract class V2TXLivePlayer {
        /**
         * 创建 V2TXLivePlayer 实例
         */
        public static V2TXLivePlayer createLivePlayer() {
            return new V2TXLivePlayerImplement();
        }

        /**
         * 设置播放器回调
         *
         * 通过设置回调，可以监听 V2TXLivePlayer 播放器的一些回调事件，
         * 包括播放器状态、播放音量回调、音视频首帧回调、统计数据、警告和错误信息等。
         * @param observer 播放器的回调目标对象，更多信息请查看 {@link V2TXLivePlayerObserver}
         */
        public abstract void setCallback(V2TXLivePlayerObserver callback);

        /**
         * 设置播放器画面的旋转角度
         *
         * @param rotation 旋转角度 {@link V2TXLiveRotation}。
         *         - V2TXLiveRotation0【默认值】: 0度, 不旋转。
         *         - V2TXLiveRotation90:  顺时针旋转90度。
         *         - V2TXLiveRotation180: 顺时针旋转180度。
         *         - V2TXLiveRotation270: 顺时针旋转270度。
         * @return 返回值 {@link V2TXLiveCode}。
         *         - V2TXLIVE_OK: 成功。
         */
        public abstract int setRenderRotation(V2TXLiveRotation rotation);

        /**
         * 开始播放音视频流
         *
         * @param url 音视频流的播放地址，支持 RTMP, HTTP-FLV, TRTC。
         * @return 返回值 {@link V2TXLiveCode}。
         *         - V2TXLIVE_OK：操作成功，开始连接并播放。
         *         - V2TXLIVE_ERROR_INVALID_PARAMETER：操作失败，url 不合法。
         *         - V2TXLIVE_ERROR_REFUSED：RTC 不支持同一设备上同时推拉同一个 StreamId。
         */
        public abstract int startPlay(string url);

        /**
         * 停止播放音视频流
         *
         * @return 返回值 {@link V2TXLiveCode}。
         *         - V2TXLIVE_OK: 成功。
         */
        public abstract int stopPlay();

        /**
         * 播放器是否正在播放中
         *
         * @return 是否正在播放。
         *         - 1: 正在播放中。
         *         - 0: 已经停止播放。
         */
        public abstract int isPlaying();

        /**
         * 暂停播放器的音频流
         *
         * @return 返回值 {@link V2TXLiveCode}。
         *         - V2TXLIVE_OK: 成功。
         */
        public abstract int pauseAudio();

        /**
         * 恢复播放器的音频流
         *
         * @return 返回值 {@link V2TXLiveCode}。
         *         - V2TXLIVE_OK: 成功。
         */
        public abstract int resumeAudio();

        /**
         * 暂停播放器的视频流
         *
         * @return 返回值 {@link V2TXLiveCode}。
         *         - V2TXLIVE_OK: 成功。
         */
        public abstract int pauseVideo();

        /**
         * 恢复播放器的视频流
         *
         * @return 返回值 {@link V2TXLiveCode}。
         *         - V2TXLIVE_OK: 成功。
         */
        public abstract int resumeVideo();

        /**
         * 设置播放器音量
         *
         * @param volume 音量大小，取值范围0 - 100。【默认值】: 100。
         * @return 返回值 {@link V2TXLiveCode}。
         *         - V2TXLIVE_OK: 成功。
         */
        public abstract int setPlayoutVolume(int volume);

        /**
         * 开启/关闭对视频帧的监听回调
         *
         * SDK 在您开启此开关后将不再渲染视频画面，您可以通过 V2TXLivePlayerObserver 获得视频帧，并执行自定义的渲染逻辑。
         * @param enable      是否开启自定义渲染。【默认值】：false。
         * @param pixelFormat 自定义渲染回调的视频像素格式 {@link V2TXLivePixelFormat}。
         * @param bufferType  自定义渲染回调的视频数据格式 {@link V2TXLiveBufferType}。
         * @return 返回值 {@link V2TXLiveCode}。
         *         - V2TXLIVE_OK: 成功。
         *         - V2TXLIVE_ERROR_NOT_SUPPORTED: 像素格式或者数据格式不支持。
         */
        public abstract int enableObserveVideoFrame(bool enable, V2TXLivePixelFormat pixelFormat, V2TXLiveBufferType bufferType);

        /**
         * 是否显示播放器状态信息的调试浮层
         *
         * @param isShow 是否显示。【默认值】：false。
         */
        public abstract void showDebugView(bool isShow);
    }
}
