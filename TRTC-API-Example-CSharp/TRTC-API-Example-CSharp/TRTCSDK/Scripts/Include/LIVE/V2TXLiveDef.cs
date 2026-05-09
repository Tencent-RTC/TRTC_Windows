/**
 * Copyright (c) 2021 Tencent. All rights reserved.
 * Module:   V2TXLiveDef @ TXLiteAVSDK
 * Function: 腾讯云直播服务(LVB)关键类型定义
 */
namespace liteav {

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                              支持协议
    //
    /////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////
    //
    //           （一）视频相关类型定义
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 视频画面填充模式
     */
    public enum V2TXLiveFillMode {

        ///  图像铺满屏幕，超出显示视窗的视频部分将被裁剪，画面显示可能不完整。
        V2TXLiveFillModeFill = 0,

        ///  图像长边填满屏幕，短边区域会被填充黑色，画面的内容完整。
        V2TXLiveFillModeFit = 1,

        ///  图像拉伸铺满，因此长度和宽度可能不会按比例变化。
        V2TXLiveFillModeScaleFill = 2,

    }

    /**
     * 视频画面顺时针旋转角度
     */
    public enum V2TXLiveRotation {

        ///  不旋转。
        V2TXLiveRotation0 = 0,

        ///  顺时针旋转90度。
        V2TXLiveRotation90 = 1,

        ///  顺时针旋转180度。
        V2TXLiveRotation180 = 2,

        ///  顺时针旋转270度。
        V2TXLiveRotation270 = 3,

    }

    /**
     * 视频帧的像素格式
     */
    public enum V2TXLivePixelFormat {

        ///  未知。
        V2TXLivePixelFormatUnknown = 0,

        ///  YUV420P I420。
        V2TXLivePixelFormatI420 = 1,

        ///  BGRA8888。
        V2TXLivePixelFormatBGRA32 = 2,

        ///  RGBA32
        V2TXLivePixelFormatRGBA32 = 3,

    }

    /**
     * 视频数据包装格式
     *
     * @info 视频数据包装格式。
     * @note 在自定义采集和自定义渲染功能，您需要用到下列枚举值来指定您希望以什么类型的容器来包装视频数据。
     * - Texture: 直接使用时效率最高。
     */
    public enum V2TXLiveBufferType {

        ///  未知。
        V2TXLiveBufferTypeUnknown = 0,

        ///  DirectBuffer，装载 I420 等 buffer，在 native 层使用。
        V2TXLiveBufferTypeByteBuffer = 1,

    }

    /**
     * 视频帧信息
     *
     * @info 视频帧信息。
     *         V2TXLiveVideoFrame 用来描述一帧视频画面的裸数据，它可以是一帧编码前的画面，也可以是一帧解码后的画面。
     * @note  自定义采集和自定义渲染时使用。自定义采集时，需要使用 V2TXLiveVideoFrame 来包装待发送的视频帧；自定义渲染时，会返回经过 V2TXLiveVideoFrame 包装的视频帧。
     */
    public struct V2TXLiveVideoFrame {
        ///  【字段含义】视频帧像素格式。
        public V2TXLivePixelFormat pixelFormat;

        ///  【字段含义】视频数据包装格式。
        public V2TXLiveBufferType bufferType;

        ///  【字段含义】bufferType 为 V2TXLiveBufferTypeByteBuffer 时的视频数据。
        public byte[] data;

        ///  【字段含义】视频数据的长度，单位是字节。
        public int length;

        ///  【字段含义】视频宽度。
        public int width;

        ///  【字段含义】视频高度。
        public int height;

        ///  【字段含义】视频帧的顺时针旋转角度。
        public V2TXLiveRotation rotation;
    }

    /////////////////////////////////////////////////////////////////////////////////
    //
    //          （二）音频相关类型定义
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 音频帧数据
     */
    public struct V2TXLiveAudioFrame {
        ///  【字段含义】音频数据。
        public byte[] data;

        ///  【字段含义】音频数据的长度。
        public int length;

        ///  【字段含义】采样率。
        public int sampleRate;

        ///  【字段含义】声道数。
        public int channel;
    }

    /////////////////////////////////////////////////////////////////////////////////
    //
    //          （三）推流器和播放器的一些统计指标数据定义
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 播放器的统计数据
     */
    public struct V2TXLivePlayerStatistics {
        ///  【字段含义】当前 App 的 CPU 使用率（％）。
        public int appCpu;

        ///  【字段含义】当前系统的 CPU 使用率（％）。
        public int systemCpu;

        ///  【字段含义】视频宽度。
        public int width;

        ///  【字段含义】视频高度。
        public int height;

        ///  【字段含义】帧率（fps）。
        public int fps;

        ///  【字段含义】视频码率（Kbps）。
        public int videoBitrate;

        ///  【字段含义】音频码率（Kbps）。
        public int audioBitrate;

        ///  【字段含义】网络音频丢包率（％），注：仅支持前缀为 [trtc://] 或 [webrtc://] 的播放地址。
        public int audioPacketLoss;

        ///  【字段含义】网络视频丢包率（％），注：仅支持前缀为 [trtc://] 或 [webrtc://] 的播放地址。
        public int videoPacketLoss;

        ///  【字段含义】播放延迟（ms）。
        public int jitterBufferDelay;

        ///  【字段含义】音频播放的累计卡顿时长（ms）。
        /// 该时长为区间（2s）内的卡顿时长。
        public int audioTotalBlockTime;

        ///  【字段含义】音频播放卡顿率，单位（％）。
        /// 音频播放卡顿率（audioBlockRate） = 音频播放的累计卡顿时长（audioTotalBlockTime） / 音频播放的区间时长（2000ms）。
        public int audioBlockRate;

        ///  【字段含义】视频播放的累计卡顿时长（ms）。
        /// 该时长为区间（2s）内的卡顿时长。
        public int videoTotalBlockTime;

        ///  【字段含义】视频播放卡顿率，单位（％）。
        /// 视频播放卡顿率（videoBlockRate） = 视频播放的累计卡顿时长（videoTotalBlockTime） / 视频播放的区间时长（2000ms）。
        public int videoBlockRate;

        ///  【字段含义】从 SDK 到云端的往返延时（ms），注：仅支持前缀为 [trtc://] 或 [webrtc://] 的播放地址。
        public int rtt;

        ///  【字段含义】下载速度（Kbps）
        public int netSpeed;
    }

    /////////////////////////////////////////////////////////////////////////////////
    //
    //          （四）连接状态相关枚举值定义
    //
    /////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////
    //
    //          (五) 公共配置组件
    //
    /////////////////////////////////////////////////////////////////////////////////

}
