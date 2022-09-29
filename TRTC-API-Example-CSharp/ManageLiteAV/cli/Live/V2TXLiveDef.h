#pragma once

using namespace System;

namespace ManageLiteAV {

    /// <summary>
    /// 支持协议
    /// </summary>
    public enum class V2TXLiveMode {
        /// <summary>
        /// 支持协议: RTMP
        /// </summary>
        V2TXLiveModeRTMP,

        /// <summary>
        /// 支持协议: TRTC
        /// </summary>
        V2TXLiveModeRTC,

    };

    /// <summary>
    /// 视频画面顺时针旋转角度
    /// </summary>
    public enum class V2TXLiveRotation {

        /// <summary>
        /// 不旋转
        /// </summary>
        V2TXLiveRotation0,

        /// <summary>
        /// 顺时针旋转90度
        /// </summary>
        V2TXLiveRotation90,

        /// <summary>
        /// 顺时针旋转180度
        /// </summary>
        V2TXLiveRotation180,

        /// <summary>
        /// 顺时针旋转270度
        /// </summary>
        V2TXLiveRotation270
    };

    /// <summary>
    /// 视频画面填充模式
    /// </summary>
    public enum class V2TXLiveFillMode {

        /// <summary>        
        /// 图像铺满屏幕，超出显示视窗的视频部分将被裁剪，画面显示可能不完整
        /// </summary>
        V2TXLiveFillModeFill,

        /// <summary>  
        /// 图像长边填满屏幕，短边区域会被填充黑色，画面的内容完整
        /// </summary>
        V2TXLiveFillModeFit,

        /// <summary> 
        /// 图像拉伸铺满，因此长度和宽度可能不会按比例变化
        /// </summary>
        V2TXLiveFillModeScaleFill
    };

    /// <summary>
    /// 视频帧的像素格式
    /// </summary>
    public enum class V2TXLivePixelFormat {

        /// <summary>
        /// 未知
        /// </summary>
        V2TXLivePixelFormatUnknown,

        /// <summary>
        /// YUV420P I420
        /// </summary>
        V2TXLivePixelFormatI420,

        /// <summary>
        /// BGRA8888
        /// </summary>
        V2TXLivePixelFormatBGRA32
    };

    /// <summary>
    /// 视频数据包装格式
    /// @info 视频数据包装格式。
    /// @note 在自定义采集和自定义渲染功能，您需要用到下列枚举值来指定您希望以什么样的格式来包装视频数据。  
    /// </summary>
    public enum class V2TXLiveBufferType {

        /// <summary>
        /// 未知
        /// </summary>
        V2TXLiveBufferTypeUnknown,

        /// <summary>
        /// 二进制Buffer类型
        /// </summary>        
        V2TXLiveBufferTypeByteBuffer
    };

    /// <summary>
    /// 本地摄像头镜像类型
    /// </summary>
    public enum class V2TXLiveMirrorType {

        /// <summary>
        ///系统默认镜像类型，前置摄像头镜像，后置摄像头不镜像
        /// </summary>
        V2TXLiveMirrorTypeAuto,

        /// <summary>
        /// 前置摄像头和后置摄像头，都切换为镜像模式
        /// </summary>
        V2TXLiveMirrorTypeEnable,

        /// <summary>
        ///前置摄像头和后置摄像头，都切换为非镜像模式
        /// </summary>
        V2TXLiveMirrorTypeDisable
    };

    /// <summary>
    /// 图片类型
    /// </summary>
    public enum class V2TXLiveImageType {

        /// <summary>
        ///图片文件路径，支持 GIF、JPEG、PNG文件格式
        /// </summary>        
        V2TXLiveImageTypeFile = 0,

        /// <summary>
        /// BGRA32格式内存块
        /// </summary>
        V2TXLiveImageTypeBGRA32 = 1,

        /// <summary>
        /// RGBA32格式内存块
        /// </summary>
        V2TXLiveImageTypeRGBA32 = 2,
    };

    /// <summary>
    /// 声音音质
    /// </summary>
    public enum class V2TXLiveAudioQuality {

        /// <summary>
        /// 语音音质：采样率：16k；单声道；音频码率：16kbps；适合语音通话为主的场景，比如在线会议，语音通话
        /// </summary>
        V2TXLiveAudioQualitySpeech,

        /// <summary>
        /// 默认音质：采样率：48k；单声道；音频码率：50kbps；SDK 默认的音频质量，如无特殊需求推荐选择之
        /// </summary>
        V2TXLiveAudioQualityDefault,

        /// <summary>
        /// 音乐音质：采样率：48k；双声道 + 全频带；音频码率：128kbps；适合需要高保真传输音乐的场景，比如K歌、音乐直播等
        /// </summary>
        V2TXLiveAudioQualityMusic
    };

    /// <summary>
    /// 视频分辨率
    /// </summary>
    public enum class V2TXLiveVideoResolution {

        /// <summary>
        ///分辨率 160*160，码率范围：100Kbps ~ 150Kbps，帧率：15fps
        /// </summary> 
        V2TXLiveVideoResolution160x160,

        /// <summary>
        ///分辨率 270*270，码率范围：200Kbps ~ 300Kbps，帧率：15fps
        /// </summary> 
        V2TXLiveVideoResolution270x270,

        /// <summary>
        ///分辨率 480*480，码率范围：350Kbps ~ 525Kbps，帧率：15fps
        /// </summary> 
        V2TXLiveVideoResolution480x480,

        /// <summary>
        ///分辨率 320*240，码率范围：250Kbps ~ 375Kbps，帧率：15fps
        /// </summary> 
        V2TXLiveVideoResolution320x240,

        /// <summary>
        /// 分辨率 480*360，码率范围：400Kbps ~ 600Kbps，帧率：15fps
        /// </summary> 
        V2TXLiveVideoResolution480x360,

        /// <summary>
        /// 分辨率 640*480，码率范围：600Kbps ~ 900Kbps，帧率：15fps
        /// </summary> 
        V2TXLiveVideoResolution640x480,

        /// <summary>
        ///分辨率 320*180，码率范围：250Kbps ~ 400Kbps，帧率：15fps
        /// </summary> 
        V2TXLiveVideoResolution320x180,

        /// <summary>
        /// 分辨率 480*270，码率范围：350Kbps ~ 550Kbps，帧率：15fps
        /// </summary> 
        V2TXLiveVideoResolution480x270,

        /// <summary>
        ///分辨率 640*360，码率范围：500Kbps ~ 900Kbps，帧率：15fps
        /// </summary> 
        V2TXLiveVideoResolution640x360,

        /// <summary>
        /// 分辨率 960*540，码率范围：800Kbps ~ 1500Kbps，帧率：15fps
        /// </summary> 
        V2TXLiveVideoResolution960x540,

        /// <summary>
        /// 分辨率 1280*720，码率范围：1000Kbps ~ 1800Kbps，帧率：15fps
        /// </summary> 
        V2TXLiveVideoResolution1280x720,

        /// <summary>
        /// 分辨率 1920*1080，码率范围：2500Kbps ~ 3000Kbps，帧率：15fps
        /// </summary> 
        V2TXLiveVideoResolution1920x1080,
    };

    /// <summary>
    /// 图片信息
    /// </summary>
    public ref struct V2TXLiveImage {

        /// <summary>
        /// V2TXLiveImageTypeFile: 图片路径; 其他类型:图片内容
        /// </summary>
        String^ imageSrc;

        /// <summary>
        ///图片数据类型,默认为V2TXLiveImageFile {@link V2TXLiveImageType}
        /// </summary>
        V2TXLiveImageType imageType;

        /// <summary>
        ///图片宽度,默认为0(图片数据类型为V2TXLiveImageTypeFile,忽略该参数)
        /// </summary>
        UInt32 imageWidth;

        /// <summary>
        ///图片高度,默认为0(图片数据类型为V2TXLiveImageTypeFile,忽略该参数)
        /// </summary>
        UInt32 imageHeight;

        /// <summary>
        ///图片数据的长度;单位字节
        /// </summary>
        UInt32 imageLength;

        V2TXLiveImage() :
            imageSrc(nullptr),
            imageType(V2TXLiveImageType::V2TXLiveImageTypeBGRA32),
            imageWidth(0),
            imageHeight(0),
            imageLength(0) {
        }
    };

    /// <summary>
    /// 视频宽高比模式
    /// </summary>
    public enum class V2TXLiveVideoResolutionMode {

        /// <summary>
        /// 横屏模式
        /// </summary> 
        V2TXLiveVideoResolutionModeLandscape,

        /// <summary>
        /// 竖屏模式
        /// </summary>
        V2TXLiveVideoResolutionModePortrait,

    };

    /// <summary>
    /// 视频编码参数, 该设置决定远端用户看到的画面质量
    /// </summary>
    public ref struct V2TXLiveVideoEncoderParam {

        /// <summary>
        ///【字段含义】 视频分辨率
        ///【特别说明】如需使用竖屏分辨率，请指定 videoResolutionMode 为 Portrait，例如： 640 × 360 + Portrait = 360 × 640。
        ///【推荐取值】
        /// - 桌面平台（Win + Mac）：建议选择 640 × 360 及以上分辨率，videoResolutionMode 选择 Landscape，即横屏分辨率。
        /// </summary>
        V2TXLiveVideoResolution videoResolution;

        /// <summary>
        ///【字段含义】分辨率模式（横屏分辨率 or 竖屏分辨率）
        ///【推荐取值】桌面平台（Windows、Mac）建议选择 Landscape。
        ///【特别说明】如需使用竖屏分辨率，请指定 resMode 为 Portrait，例如： 640 × 360 + Portrait = 360 × 640。
        /// </summary>
        V2TXLiveVideoResolutionMode videoResolutionMode;

        /// <summary>
        ///【字段含义】视频采集帧率
        ///【推荐取值】15fps 或 20fps。5fps 以下，卡顿感明显。10fps 以下，会有轻微卡顿感。20fps 以上，会浪费带宽（电影的帧率为 24fps）。
        /// </summary>
        Int32 videoFps;

        /// <summary>
        ///【字段含义】目标视频码率，SDK 会按照目标码率进行编码，只有在弱网络环境下才会主动降低视频码率。
        ///【推荐取值】请参考 V2TXLiveVideoResolution 在各档位注释的最佳码率，也可以在此基础上适当调高。
        ///           比如：V2TXLiveVideoResolution1280x720 对应 1200kbps 的目标码率，您也可以设置为 1500kbps 用来获得更好的观感清晰度。
        ///【特别说明】您可以通过同时设置 videoBitrate 和 minVideoBitrate 两个参数，用于约束 SDK 对视频码率的调整范围：
        /// - 如果您将 videoBitrate 和 minVideoBitrate 设置为同一个值，等价于关闭 SDK 对视频码率的自适应调节能力。
        /// </summary>
        Int32 videoBitrate;

        /// <summary>
        ///【字段含义】最低视频码率，SDK 会在网络不佳的情况下主动降低视频码率以保持流畅度，最低会降至 minVideoBitrate 所设定的数值。
        ///【推荐取值】您可以通过同时设置 videoBitrate 和 minVideoBitrate 两个参数，用于约束 SDK 对视频码率的调整范围：
        /// - 如果您将 videoBitrate 和 minVideoBitrate 设置为同一个值，等价于关闭 SDK 对视频码率的自适应调节能力。
        /// </summary>
        Int32 minVideoBitrate;

        V2TXLiveVideoEncoderParam(V2TXLiveVideoResolution resolution);
    };

    /// <summary>
    /// 
    /// </summary>
    public ref struct V2TXLiveVideoFrame {
        /// 【字段含义】视频帧像素格式
        V2TXLivePixelFormat pixelFormat;

        /// 【字段含义】视频数据包装格式
        V2TXLiveBufferType bufferType;

        /// 【字段含义】bufferType 为 V2TXLiveBufferTypeByteBuffer 时的视频数据。
        array<Byte>^ data;

        /// 【字段含义】视频数据的长度，单位是字节
        Int32 length;

        /// 【字段含义】视频宽度
        Int32 width;

        /// 【字段含义】视频高度
        Int32 height;

        /// 【字段含义】视频帧的顺时针旋转角度
        V2TXLiveRotation rotation;

        V2TXLiveVideoFrame() :
            pixelFormat(V2TXLivePixelFormat::V2TXLivePixelFormatUnknown),
            bufferType(V2TXLiveBufferType::V2TXLiveBufferTypeUnknown),
            data(nullptr),
            length(0),
            width(0),
            height(0),
            rotation(V2TXLiveRotation::V2TXLiveRotation0) {
        }
    };

    /// <summary>
    /// 音频帧数据
    /// </summary>
    public ref struct V2TXLiveAudioFrame {
        /// <summary>
        /// 【字段含义】音频数据
        /// </summary>
        array<Byte>^ data;

        /// <summary>
        /// 【字段含义】音频数据的长度
        /// </summary>
        Int32 length;

        /// <summary>
        /// 【字段含义】采样率
        /// </summary>
        Int32 sampleRate;

        /// <summary>
        /// 【字段含义】声道数
        /// </summary>
        Int32 channel;

        V2TXLiveAudioFrame() :
            data(nullptr),
            length(0),
            sampleRate(48000),
            channel(1) {
        }
    };

    /// <summary>
    /// 屏幕分享有关的定义
    /// </summary>
    public enum class V2TXLiveScreenCaptureSourceType {

        /// <summary>
        /// 未知
        /// </summary>
        V2TXLiveScreenCaptureSourceTypeUnknown = -1,

        /// <summary>
        /// 该分享目标是某一个窗口
        /// </summary>
        V2TXLiveScreenCaptureSourceTypeWindow = 0,

        /// <summary>
        /// 该分享目标是整个桌面
        /// </summary>
        V2TXLiveScreenCaptureSourceTypeScreen = 1,

        /// <summary>
        /// 自定义窗口类型
        /// </summary>
        V2TXLiveScreenCaptureSourceTypeCustom = 2,

    };

    /// <summary>
    /// 屏幕分享窗口信息
    /// </summary>
    public ref struct V2TXLiveScreenCaptureSourceInfo {
        /// 采集源类型
        V2TXLiveScreenCaptureSourceType sourceType;

        /// 采集源 ID；对于窗口，该字段指示窗口句柄；对于屏幕，该字段指示屏幕 ID
        IntPtr sourceId;

        /// 采集源名称，UTF8 编码
        String^ sourceName;

        /// 缩略图内容
        V2TXLiveImage ^thumbBGRA;

        /// 图标内容
        V2TXLiveImage ^iconBGRA;

        /// 是否为最小化窗口，通过 getScreenCaptureSources 获取列表时的窗口状态，仅采集源为 Window 时才可用
        bool isMinimizeWindow;

        /// 是否为主屏，是否为主屏，仅采集源类型为 Screen 时才可用
        bool isMainScreen;

        V2TXLiveScreenCaptureSourceInfo() :
            sourceType(V2TXLiveScreenCaptureSourceType::V2TXLiveScreenCaptureSourceTypeUnknown),
            sourceId(nullptr),
            sourceName(nullptr),
            isMinimizeWindow(false),
            isMainScreen(false) {
        }
    };

    /// <summary>
    /// 屏幕分享窗口列表
    /// </summary>
    public interface class IV2TXLiveScreenCaptureSourceList
    {
    public:
        /// <summary>
        /// 窗口个数
        /// </summary>
        UInt32 getCount();

        /// <summary>
        /// 窗口信息
        /// </summary>
        V2TXLiveScreenCaptureSourceInfo^ getSourceInfo(UInt32 index);

        /// <summary>
        /// 遍历完窗口列表后，调用release释放资源。
        /// </summary>
        void release();
    };

    /// <summary>
    /// 屏幕分享参数
    /// </summary>
    public ref struct V2TXLiveScreenCaptureProperty {
        /// <summary>
        ///是否采集目标内容时顺带采集鼠标，默认为 true
        /// </summary>
        bool enableCaptureMouse;

        /// <summary>
        /// 是否高亮正在共享的窗口，默认为 true
        /// </summary>
        bool enableHighlightBorder;

        /// <summary>
        /// 是否开启高性能模式（只会在分享屏幕时会生效），开启后屏幕采集性能最佳，但无法过滤远端的高亮边框，默认为 true
        /// </summary>
        bool enableHighPerformance;

        /// <summary>
        /// 指定高亮边框颜色，RGB 格式，传入 0 时采用默认颜色，默认颜色为 #8CBF26
        /// </summary>
        int highlightBorderColor;

        /// <summary>
        /// 指定高亮边框的宽度，传入0时采用默认描边宽度，默认宽度为 5，最大值为 50
        /// </summary>
        int highlightBorderSize;

        /// <summary>
        /// 窗口采集时是否采集子窗口（与采集窗口具有 Owner 或 Popup 属性），默认为 false
        /// </summary>
        bool enableCaptureChildWindow;

        V2TXLiveScreenCaptureProperty() :
            enableCaptureMouse(true),
            enableHighlightBorder(true),
            enableHighPerformance(true),
            highlightBorderColor(0),
            highlightBorderSize(0),
            enableCaptureChildWindow(false) {
        }
    };

    /// <summary>
    /// 大小
    /// </summary>
    public ref struct V2TXLiveSize {
        /// <summary>
        /// 宽
        /// </summary>
        UInt64 width;

        /// <summary>
        /// 高
        /// </summary>
        UInt64 height;

        V2TXLiveSize() :
            width(0),
            height(0) {
        }
    };

    /// <summary>
    /// 区域
    /// </summary>
    public ref struct V2TXLiveRect {
        /// <summary>
        /// 左
        /// </summary>
        UInt64 left;

        /// <summary>
        /// 上
        /// </summary>
        UInt64 top;

        /// <summary>
        /// 右
        /// </summary>
        UInt64 right;

        /// <summary>
        /// 下
        /// </summary>
        UInt64 bottom;

        V2TXLiveRect() :
            left(0),
            top(0),
            right(0),
            bottom(0) {
        }
    };

    /// <summary>
    /// 日志级别枚举值
    /// </summary>
    public enum class V2TXLiveLogLevel {

        /// <summary>
        /// 输出所有级别的 log
        /// </summary>
        V2TXLiveLogLevelAll = 0,

        /// <summary>
        /// 输出 DEBUG，INFO，WARNING，ERROR 和 FATAL 级别的 log
        /// </summary>
        V2TXLiveLogLevelDebug = 1,

        /// <summary>
        /// 输出 INFO，WARNING，ERROR 和 FATAL 级别的 log
        /// </summary>
        V2TXLiveLogLevelInfo = 2,

        /// <summary>
        /// 只输出 WARNING，ERROR 和 FATAL 级别的 log
        /// </summary>
        V2TXLiveLogLevelWarning = 3,

        /// <summary>
        /// 只输出 ERROR 和 FATAL 级别的 log
        /// </summary>
        V2TXLiveLogLevelError = 4,

        /// <summary>
        /// 只输出 FATAL 级别的 log
        /// </summary>
        V2TXLiveLogLevelFatal = 5,

        /// <summary>
        /// 不输出任何 sdk log
        /// </summary>
        V2TXLiveLogLevelNULL = 6,
    };

    /// <summary>
    /// Log配置
    /// </summary>
    public ref struct V2TXLiveLogConfig {
        /// <summary>
        /// 【字段含义】设置 Log 级别
        /// 【推荐取值】默认值：V2TXLiveLogLevelAll
        /// </summary>
        V2TXLiveLogLevel logLevel;

        /// <summary>
        /// 【字段含义】是否通过 V2TXLivePremierObserver 接收要打印的 Log 信息
        /// 【特殊说明】如果您希望自己实现 Log 写入，可以打开此开关，Log 信息会通过 V2TXLivePremierObserver#onLog 回调给您。
        /// 【推荐取值】默认值：false
        /// </summary>
        bool enableObserver;

        /// <summary>
        /// 【字段含义】是否允许 SDK 在编辑器（XCoder、Android Studio、Visual Studio 等）的控制台上打印 Log
        /// 【推荐取值】默认值：false
        /// </summary>
        bool enableConsole;

        /// <summary>
        /// 【字段含义】是否启用本地 Log 文件
        /// 【特殊说明】如非特殊需要，请不要关闭本地 Log 文件，否则腾讯云技术团队将无法在出现问题时进行跟踪和定位。
        /// 【推荐取值】默认值：true
        /// </summary>
        bool enableLogFile;

        /// <summary>
        /// 【字段含义】设置本地 Log 的存储目录，默认 Log 存储位置：
        ///  Windows：%appdata%/liteav/log
        /// </summary>
        String^ logPath;

        V2TXLiveLogConfig() :
            logLevel(V2TXLiveLogLevel::V2TXLiveLogLevelAll),
            enableObserver(false),
            enableConsole(false),
            enableLogFile(true),
            logPath(nullptr) {
        }
    };

    /// <summary>
    /// 音频帧回调格式
    /// </summary>
    public ref struct V2TXLiveAudioFrameObserverFormat {
        /// <summary>
        ///【字段含义】采样率
        ///【推荐取值】默认值：48000Hz。支持 16000, 32000, 44100, 48000。
        /// </summary>
        int sampleRate;

        /// <summary>
        ///【字段含义】声道数
        ///【推荐取值】默认值：1，代表单声道。可设定的数值只有两个数字：1-单声道，2-双声道。
        /// </summary>
        int channel;

        /// <summary>
        ///【字段含义】采样点数
        ///【推荐取值】取值必须是 sampleRate/100 的整数倍。
        /// </summary>
        int samplesPerCall;

        V2TXLiveAudioFrameObserverFormat() :
            sampleRate(0),
            channel(0),
            samplesPerCall(0) {
        }
    };

    /// <summary>
    /// 播放器的统计数据
    /// </summary>
    public ref struct V2TXLivePlayerStatistics {

        /// <summary>
        /// 【字段含义】当前 App 的 CPU 使用率（％）
        /// </summary> 
        Int32 appCpu;

        /// <summary>
        /// 【字段含义】当前系统的 CPU 使用率（％）
        /// </summary> 
        Int32 systemCpu;

        /// <summary>
        /// 【字段含义】视频宽度
        /// </summary> 
        Int32 width;

        /// <summary>
        /// 【字段含义】视频高度
        /// </summary> 
        Int32 height;

        /// <summary>
        /// 【字段含义】帧率（fps）
        /// </summary> 
        Int32 fps;

        /// <summary>
        /// 【字段含义】视频码率（Kbps）
        /// </summary> 
        Int32 videoBitrate;

        /// <summary>
        /// 【字段含义】音频码率（Kbps）
        /// </summary> 
        Int32 audioBitrate;

        V2TXLivePlayerStatistics() :
            appCpu(0),
            systemCpu(0),
            width(0),
            height(0),
            fps(0),
            videoBitrate(0),
            audioBitrate(0) {
        }
    };

    /// <summary>
    /// 直播流的连接状态
    /// </summary>
    public enum class V2TXLivePushStatus {

        /// <summary>
        /// 与服务器断开连接
        /// </summary>
        V2TXLivePushStatusDisconnected,

        /// <summary>
        /// 正在连接服务器
        /// </summary>
        V2TXLivePushStatusConnecting,

        /// <summary>
        /// 连接服务器成功
        /// </summary>
        V2TXLivePushStatusConnectSuccess,

        /// <summary>
        /// 重连服务器中
        /// </summary>
        V2TXLivePushStatusReconnecting

    };

    /// <summary>
    /// 推流器的统计数据
    /// </summary>
    public ref struct V2TXLivePusherStatistics {

        /// <summary>
        /// 【字段含义】当前 App 的 CPU 使用率（％）
        /// </summary> 
        Int32 appCpu;

        /// <summary>
        /// 【字段含义】当前系统的 CPU 使用率（％）
        /// </summary> 
        Int32 systemCpu;

        /// <summary>
        /// 【字段含义】视频宽度
        /// </summary> 
        Int32 width;

        /// <summary>
        /// 【字段含义】视频高度
        /// </summary> 
        Int32 height;

        /// <summary>
        /// 【字段含义】帧率（fps）
        /// </summary> 
        Int32 fps;

        /// <summary>
        /// 【字段含义】视频码率（Kbps）
        /// </summary> 
        Int32 videoBitrate;

        /// <summary>
        /// 【字段含义】音频码率（Kbps）
        /// </summary> 
        Int32 audioBitrate;

        V2TXLivePusherStatistics() :
            appCpu(0),
            systemCpu(0),
            width(0),
            height(0),
            fps(0),
            videoBitrate(0),
            audioBitrate(0) {
        }
    };
}