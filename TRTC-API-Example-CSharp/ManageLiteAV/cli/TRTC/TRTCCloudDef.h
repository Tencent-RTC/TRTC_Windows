// TRTCCloudDef.h

#pragma once

// C++/CLI Header
#include "cli/ITRTCDeviceManager.h"

using namespace System;
using namespace System::Collections::Generic;

namespace ManageLiteAV {

    /// @defgroup TRTCCloudDef_csharp 关键类型定义
    /// 腾讯云视频通话功能的关键类型定义
    /// @{
    /////////////////////////////////////////////////////////////////////////////////
    //
    //                    【（一）视频相关枚举值定义】
    //                   
    /////////////////////////////////////////////////////////////////////////////////

    /// <summary>
    /// 1.1 视频分辨率
    /// </summary>
    /// <remarks>
    /// 此处仅定义横屏分辨率，如需使用竖屏分辨率（例如360 × 640），需要同时指定 TRTCVideoResolutionMode 为 Portrait。
    /// </remarks>
    public enum class TRTCVideoResolution
    {
        // 宽高比1:1
        /// <summary>
        /// 建议码率80kbps
        /// </summary>
        TRTCVideoResolution_120_120 = 1,
        /// <summary>
        /// 建议码率100kbps
        /// </summary>
        TRTCVideoResolution_160_160 = 3,
        /// <summary>
        /// 建议码率200kbps
        /// </summary>
        TRTCVideoResolution_270_270 = 5,
        /// <summary>
        /// 建议码率350kbps
        /// </summary>
        TRTCVideoResolution_480_480 = 7,

        // 宽高比4:3
        /// <summary>
        /// 建议码率100kbps
        /// </summary>
        TRTCVideoResolution_160_120 = 50,
        /// <summary>
        /// 建议码率150kbps
        /// </summary>
        TRTCVideoResolution_240_180 = 52,
        /// <summary>
        /// 建议码率200kbps
        /// </summary>
        TRTCVideoResolution_280_210 = 54,
        /// <summary>
        /// 建议码率250kbps
        /// </summary>
        TRTCVideoResolution_320_240 = 56,
        /// <summary>
        /// 建议码率300kbps
        /// </summary>
        TRTCVideoResolution_400_300 = 58,
        /// <summary>
        /// 建议码率400kbps
        /// </summary>
        TRTCVideoResolution_480_360 = 60,
        /// <summary>
        /// 建议码率600kbps
        /// </summary>
        TRTCVideoResolution_640_480 = 62,
        /// <summary>
        /// 建议码率1000kbps
        /// </summary>
        TRTCVideoResolution_960_720 = 64,


        // 宽高比16:9
        /// <summary>
        /// 建议码率150kbps
        /// </summary>
        TRTCVideoResolution_160_90 = 100,
        /// <summary>
        /// 建议码率200kbps
        /// </summary>
        TRTCVideoResolution_256_144 = 102,
        /// <summary>
        /// 建议码率250kbps
        /// </summary>
        TRTCVideoResolution_320_180 = 104,
        /// <summary>
        /// 建议码率350kbps
        /// </summary>
        TRTCVideoResolution_480_270 = 106,
        /// <summary>
        /// 建议码率550kbps
        /// </summary>
        TRTCVideoResolution_640_360 = 108,
        /// <summary>
        /// 建议码率850kbps
        /// </summary>
        TRTCVideoResolution_960_540 = 110,
        /// <summary>
        /// 摄像头采集 - 建议码率1200kbps
        /// <para>屏幕分享 - 建议码率 低清：1000kbps 高清：1600kbps</para>
        /// </summary>
        TRTCVideoResolution_1280_720 = 112,
        /// <summary>
        /// 屏幕分享 - 建议码率2000kbps
        /// </summary>
        TRTCVideoResolution_1920_1080 = 114,
    };

    /// <summary>
    /// 1.2 视频分辨率模式
    /// 
    /// <para>- 横屏分辨率：TRTCVideoResolution_640_360 + TRTCVideoResolutionModeLandscape = 640 × 360</para>
    /// <para>- 竖屏分辨率：TRTCVideoResolution_640_360 + TRTCVideoResolutionModePortrait  = 360 × 640</para>
    /// </summary>
    public enum class TRTCVideoResolutionMode
    {
        /// <summary>
        /// 横屏分辨率
        /// </summary>
        TRTCVideoResolutionModeLandscape = 0,
        /// <summary>
        /// 竖屏分辨率
        /// </summary>
        TRTCVideoResolutionModePortrait = 1,
    };

    /// <summary>
    /// 1.3 视频流类型
    /// 
    /// <para>TRTC 内部有三种不同的音视频流，分别为：</para>
    /// <para>-主画面：最常用的一条线路，一般用来传输摄像头的视频数据。</para>
    /// <para>-小画面：跟主画面的内容相同，但是分辨率和码率更低。</para>
    /// <para>-辅流画面：一般用于屏幕分享或远程播片（例如老师播放视频给学生观看）。</para>
    /// </summary>
    /// <remarks>
    /// - 如果主播的上行网络和性能比较好，则可以同时送出大小两路画面。
    /// <para>- SDK 不支持单独开启小画面，小画面必须依附于主画面而存在。</para>
    /// </remarks>
    public enum class TRTCVideoStreamType
    {
        /// <summary>
        /// 主画面视频流
        /// </summary>
        TRTCVideoStreamTypeBig = 0,
        /// <summary>
        /// 小画面视频流
        /// </summary>
        TRTCVideoStreamTypeSmall = 1,
        /// <summary>
        /// 辅流（屏幕分享）
        /// </summary>
        TRTCVideoStreamTypeSub = 2,
    };

    /// <summary>
    /// 1.4 画质级别
    ///
    /// <para>TRTC SDK 对画质定义了六种不同的级别，Excellent 表示最好，Down 表示不可用。</para>
    /// </summary>
    public enum class TRTCQuality
    {
        /// <summary>
        /// 未定义
        /// </summary>
        TRTCQuality_Unknown = 0,
        /// <summary>
        /// 最好
        /// </summary>
        TRTCQuality_Excellent = 1,
        /// <summary>
        /// 好
        /// </summary>
        TRTCQuality_Good = 2,
        /// <summary>
        /// 一般
        /// </summary>
        TRTCQuality_Poor = 3,
        /// <summary>
        /// 差
        /// </summary>
        TRTCQuality_Bad = 4,
        /// <summary>
        /// 很差
        /// </summary>
        TRTCQuality_Vbad = 5,
        /// <summary>
        /// 不可用
        /// </summary>
        TRTCQuality_Down = 6,
    };

    /// <summary>
    /// 1.5 视频画面填充模式
    ///
    /// <para>如果画面的显示分辨率不等于画面的原始分辨率，就需要您设置画面的填充模式:</para>
    /// <para>- TRTCVideoFillMode_Fill，超出显示视窗的视频部分将被裁剪，画面显示可能不完整。</para>
    /// <para>- TRTCVideoFillMode_Fit，图像长边填满屏幕，短边区域会被填充黑色，但画面的内容肯定是完整的。</para>
    /// </summary>
    public enum class TRTCVideoFillMode
    {
        /// <summary>
        /// 图像铺满屏幕，超出显示视窗的视频部分将被裁剪
        /// </summary>
        TRTCVideoFillMode_Fill = 0,
        /// <summary>
        /// 图像长边填满屏幕，短边区域会被填充黑色
        /// </summary>
        TRTCVideoFillMode_Fit = 1,
    };

    /// <summary>
    /// 1.6 视频画面旋转方向
    ///
    /// <para>TRTC SDK 提供了对本地和远程画面的旋转角度设置 API，下列的旋转角度都是指顺时针方向的。</para>
    /// </summary>
    public enum class TRTCVideoRotation
    {
        /// <summary>
        /// 顺时针旋转0度
        /// </summary>
        TRTCVideoRotation0 = 0,
        /// <summary>
        /// 顺时针旋转90度
        /// </summary>
        TRTCVideoRotation90 = 1,
        /// <summary>
        /// 顺时针旋转180度
        /// </summary>
        TRTCVideoRotation180 = 2,
        /// <summary>
        /// 顺时针旋转270度
        /// </summary>
        TRTCVideoRotation270 = 3,
    };

    /// <summary>
    /// 1.7 美颜（磨皮）算法
    ///
    /// <para>TRTC SDK 内置多种不同的磨皮算法，您可以选择最适合您产品定位的方案。</para>
    /// </summary>
    public enum class TRTCBeautyStyle
    {
        /// <summary>
        /// 光滑，适用于美女秀场，效果比较明显。
        /// </summary>
        TRTCBeautyStyleSmooth = 0,
        /// <summary>
        /// 自然，磨皮算法更多地保留了面部细节，主观感受上会更加自然。
        /// </summary>
        TRTCBeautyStyleNature = 1,
    };

    /// <summary>
    /// 1.8 视频像素格式
    ///
    /// <para>TRTC SDK 提供针对视频的自定义采集和自定义渲染功能，在自定义采集功能中，您可以用下列枚举值描述您采集的视频像素格式。</para>
    /// <para>在自定义渲染功能中，您可以指定您期望 SDK 回调的视频像素格式。</para>
    /// </summary>
    public enum class TRTCVideoPixelFormat
    {
        /// <summary>
        /// 【字段含义】未知类型
        /// </summary>
        TRTCVideoPixelFormat_Unknown = 0,
        /// <summary>
        /// 【字段含义】I420
        /// </summary>
        TRTCVideoPixelFormat_I420 = 1,
        /// <summary>
        /// 【字段含义】OpenGL 2D 纹理
        /// </summary>
        TRTCVideoPixelFormat_Texture_2D = 2,
        /// <summary>
        /// 【字段含义】BGRA32
        /// </summary>
        TRTCVideoPixelFormat_BGRA32 = 3,
        /// <summary>
        /// 【字段含义】RGBA32
        /// </summary>
        TRTCVideoPixelFormat_RGBA32 = 5,
    };

    /// <summary>
    /// 1.9 视频数据包装格式
    /// </summary>
    public enum class TRTCVideoBufferType
    {
        /// <summary>
        /// 【字段含义】未知类型
        /// </summary>
        TRTCVideoBufferType_Unknown = 0,
        /// <summary>
        /// 【字段含义】二进制Buffer类型
        /// </summary>
        TRTCVideoBufferType_Buffer = 1,
        /// <summary>
        /// 【字段含义】纹理类型
        /// </summary>
        TRTCVideoBufferType_Texture = 3,
    };

    /// <summary>
    /// 1.10 画面渲染镜像类型
    /// 
    /// <para>TRTC 的画面镜像提供下列设置模式</para>
    /// </summary>
    public enum class TRTCVideoMirrorType {
        /// <summary>
        /// 【字段含义】所有画面均镜像
        /// </summary>
        TRTCVideoMirrorType_Enable = 1,

        /// <summary>
        /// 【字段含义】所有画面均不镜像
        /// </summary>
        TRTCVideoMirrorType_Disable = 2,
    };

    /// <summary>
    /// 1.11 视频截图来源
    /// 
    /// </summary>
    public enum class TRTCSnapshotSourceType
    {
        /// <summary>
        /// 【字段含义】从视频流上截取视频画面
        /// </summary>
        TRTCSnapshotSourceTypeStream = 0,

        /// <summary>
        /// 【字段含义】从渲染 View 上截取视频画面
        /// </summary>
        TRTCSnapshotSourceTypeView = 1,
    };

    /// <summary>
    /// 1.12 视频状态类型
    /// 该枚举类型用于视频状态变化回调接口{@link onRemoteVideoStatusUpdated}，用于指定当前的视频状态。
    /// </summary>
    public enum class TRTCAVStatusType
    {
        /// <summary>
        /// 【字段含义】停止播放
        /// </summary>
        TRTCAVStatusStopped = 0,

        /// <summary>
        /// 【字段含义】正在播放
        /// </summary>
        TRTCAVStatusPlaying = 1,

        /// <summary>
        /// 【字段含义】正在加载
        /// </summary>
        TRTCAVStatusLoading = 2,
    };

    /// <summary>
    /// 1.13 视频状态类型
    /// 该枚举类型用于视频状态变化回调接口{@link
    /// onRemoteVideoStatusUpdated}，用于指定当前的视频状态。
    /// </summary>
    public enum class TRTCAVStatusChangeReason
    {
        /// <summary>
        /// 【字段含义】缺省值
        /// </summary>
        TRTCAVStatusChangeReasonInternal = 0,
        /// <summary>
        /// 【字段含义】网络缓冲
        /// </summary>
        TRTCAVStatusChangeReasonBufferingBegin = 1,
        /// <summary>
        /// 【字段含义】结束缓冲
        /// </summary>
        TRTCAVStatusChangeReasonBufferingEnd = 2,
        /// <summary>
        /// 【字段含义】本地启动视频流播放
        /// </summary>
        TRTCAVStatusChangeReasonLocalStarted = 3,
        /// <summary>
        /// 【字段含义】本地停止视频流播放
        /// </summary>
        TRTCAVStatusChangeReasonLocalStopped = 4,
        /// <summary>
        /// 【字段含义】远端视频流开始（或继续）
        /// </summary>
        TRTCAVStatusChangeReasonRemoteStarted = 5,
        /// <summary>
        /// 【字段含义】远端视频流停止（或中断)
        /// </summary>
        TRTCAVStatusChangeReasonRemoteStopped = 6,
    };
    /////////////////////////////////////////////////////////////////////////////////
    //
    //                    【（二）网络相关枚举值定义】
    //                   
    /////////////////////////////////////////////////////////////////////////////////

    /// <summary>
    /// 2.1 应用场景
    /// 
    /// <para>TRTC 可用于视频会议和在线直播等多种应用场景，针对不同的应用场景，TRTC SDK 的内部会进行不同的优化配置：</para>
    /// <para>- TRTCAppSceneVideoCall    ：视频通话场景，适合[1对1视频通话]、[300人视频会议]、[在线问诊]、[视频聊天]、[远程面试]等。</para>
    /// <para>- TRTCAppSceneLIVE         ：视频互动直播，适合[视频低延时直播]、[十万人互动课堂]、[视频直播 PK]、[视频相亲房]、[互动课堂]、[远程培训]、[超大型会议]等。</para>
    /// <para>- TRTCAppSceneAudioCall    ：语音通话场景，适合[1对1语音通话]、[300人语音会议]、[语音聊天]、[语音会议]、[在线狼人杀]等。</para>
    /// <para>- TRTCAppSceneVoiceChatRoom：语音互动直播，适合：[语音低延时直播]、[语音直播连麦]、[语聊房]、[K 歌房]、[FM 电台]等。</para>
    /// </summary>
    public enum class TRTCAppScene
    {
        /// <summary>
        /// 视频通话场景，支持720P、1080P高清画质，单个房间最多支持300人同时在线，最高支持50人同时发言。
        /// <para>适合：[1对1视频通话]、[300人视频会议]、[在线问诊]、[视频聊天]、[远程面试]等。</para>
        /// </summary>
        TRTCAppSceneVideoCall = 0,
        /// <summary>
        /// 视频互动直播，支持平滑上下麦，切换过程无需等待，主播延时小于300ms；支持十万级别观众同时播放，播放延时低至1000ms。
        /// <para>适合：[视频低延时直播]、[十万人互动课堂]、[视频直播 PK]、[视频相亲房]、[互动课堂]、[远程培训]、[超大型会议]等。</para>
        /// <para>注意：此场景下，您必须通过 TRTCParams 中的 role 字段指定当前用户的角色。</para>
        /// </summary>
        TRTCAppSceneLIVE = 1,
        /// <summary>
        /// 语音通话场景，支持 48kHz，支持双声道。单个房间最多支持300人同时在线，最高支持50人同时发言。
        /// <para>适合：[1对1语音通话]、[300人语音会议]、[语音聊天]、[语音会议]、[在线狼人杀]等。</para>
        /// </summary>
        TRTCAppSceneAudioCall = 2,
        /// <summary>
        /// 语音互动直播，支持平滑上下麦，切换过程无需等待，主播延时小于300ms；支持十万级别观众同时播放，播放延时低至1000ms。
        /// <para>适合：[语音低延时直播]、[语音直播连麦]、[语聊房]、[K 歌房]、[FM 电台]等。</para>
        /// <para>注意：此场景下，您必须通过 TRTCParams 中的 role 字段指定当前用户的角色。</para>
        /// </summary>
        TRTCAppSceneVoiceChatRoom = 3,
    };

    /// <summary>
    /// 2.2 角色，仅适用于直播场景（TRTCAppSceneLIVE 和 TRTCAppSceneVoiceChatRoom）
    ///
    /// <para>在直播场景中，多数用户仅为观众，个别用户是主播，这种角色区分有利于 TRTC 进行更好的定向优化。</para>
    ///
    /// <para>- Anchor：主播，可以上行视频和音频，一个房间里最多支持50个主播同时上行音视频。</para>
    /// <para>- Audience：观众，只能观看，不能上行视频和音频，一个房间里的观众人数没有上限。</para>
    /// </summary>
    public enum class TRTCRoleType
    {
        /// <summary>
        /// 【字段含义】主播
        /// </summary>
        TRTCRoleAnchor = 20,
        /// <summary>
        /// 【字段含义】观众
        /// </summary>
        TRTCRoleAudience = 21,
    };

    /// <summary>
    /// 2.3 流控模式
    ///
    /// <para>TRTC SDK 内部需要时刻根据网络情况调整内部的编解码器和网络模块，以便能够对网络的变化做出反应。</para>
    /// <para>为了支持快速算法升级，SDK 内部设置了两种不同的流控模式：</para>
    /// <para>- ModeServer： 云端控制，默认模式，推荐选择。</para>
    /// <para>- ModeClient： 本地控制，用于 SDK 开发内部调试，客户请勿使用。</para>
    /// </summary>
    /// <remarks>
    /// 推荐您使用云端控制，这样每当我们升级 Qos 算法时，您无需升级 SDK 即可体验更好的效果。
    /// </remarks>
    public enum class TRTCQosControlMode
    {
        /// <summary>
        ///【字段含义】 客户端控制（用于 SDK 开发内部调试，客户请勿使用）
        /// </summary>
        TRTCQosControlModeClient,
        /// <summary>
        ///【字段含义】 云端控制 （默认）
        /// </summary>
        TRTCQosControlModeServer,
    };

    /// <summary>
    /// 2.4 画质偏好
    ///
    /// <para>指当 TRTC SDK 在遇到弱网络环境时，您期望“保清晰”或“保流畅”：</para>
    /// <para>- Smooth：弱网下保流畅。即在遭遇弱网环境时首先确保声音的流畅和优先发送，画面会变得模糊且会有较多马赛克，但可以保持流畅不卡顿。</para>
    /// <para>- Clear：弱网下保清晰。即在遭遇弱网环境时，画面会尽可能保持清晰，但可能会更容易出现卡顿。</para>
    /// </summary>
    public enum class TRTCVideoQosPreference
    {
        /// <summary>
        ///【字段含义】 弱网下保流畅
        /// </summary>
        TRTCVideoQosPreferenceSmooth = 1,
        /// <summary>
        /// 弱网下保清晰
        ///【字段含义】 </summary>
        TRTCVideoQosPreferenceClear = 2,
    };


    /////////////////////////////////////////////////////////////////////////////////
    //
    //                    【（三）声音相关枚举值定义】
    //
    /////////////////////////////////////////////////////////////////////////////////

    /// <summary>
    /// 3.1 音频帧的格式
    /// </summary>
    public enum class TRTCAudioFrameFormat
    {
        TRTCAudioFrameFormatNone = 0,
        /// <summary>
        ///【字段含义】 PCM，每个采样点占16bit数据量。
        /// </summary>
        TRTCAudioFrameFormatPCM,
    };

    /// <summary>
    /// 3.2 声音音质
    /// </summary>
    /// <remarks>
    /// 音频音质用来衡量声音的保真程度，TRTCAudioQualitySpeech 适用于通话场景，TRTCAudioQualityMusic 适用于高音质音乐场景。
    /// </remarks>
    public enum class TRTCAudioQuality
    {
        ///<summary>
        ///【字段含义】 流畅音质：采样率：16k；单声道；音频裸码率：16kbps；适合语音通话为主的场景，比如在线会议，语音通话。
        ///</summary>
        TRTCAudioQualitySpeech = 1,
        ///<summary>
        ///【字段含义】 默认音质：采样率：48k；单声道；音频裸码率：50kbps；SDK 默认的音频质量，如无特殊需求推荐选择之。
        ///</summary>
        TRTCAudioQualityDefault = 2,
        ///<summary>
        ///【字段含义】 高音质：采样率：48k；双声道 + 全频带；音频裸码率：128kbps；适合需要高保真传输音乐的场景，比如K歌、音乐直播等。
        ///</summary>
        TRTCAudioQualityMusic = 3,
    };

    /// <summary>
    /// 3.3 音频自定义回调的格式参数
    /// </summary>
    /// <remarks>
    /// 该参数用于在音频自定义回调相关的接口中，设置 SDK 回调出来的音频数据的相关格式（包括采样率、声道数等）。
    /// </remarks>
    public ref struct TRTCAudioFrameCallbackFormat
    {
        ///< summary>
        ///【字段含义】采样率
        ///【推荐取值】默认值：48000Hz。支持 16000, 32000, 44100, 48000。
        /// </summary>
        int sampleRate;

        ///< summary>
        ///【字段含义】声道数
        ///【推荐取值】默认值：1，代表单声道。可设定的数值只有两个数字：1-单声道，2-双声道。
        /// </summary>
        int channel;

        ///< summary>
        ///【字段含义】采样点数
        ///【推荐取值】取值必须是 sampleRate/100 的整数倍。
        /// </summary>
        int samplesPerCall;

        TRTCAudioFrameCallbackFormat()
            : sampleRate(0)
            , channel(0)
            , samplesPerCall(0)
        {}
    };

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                    【（四）更多枚举值定义】
    //
    /////////////////////////////////////////////////////////////////////////////////

    /// <summary>
     /// 4.1 Log 级别
     /// </summary>
    public
        enum class TRTCLogLevel {
        /// <summary>
        ///【字段含义】 输出所有级别的 Log
        /// </summary>
        TRTCLogLevelVerbose = 0,
        /// <summary>
        ///【字段含义】 输出 DEBUG，INFO，WARNING，ERROR 和 FATAL 级别的 Log
        /// </summary>
        TRTCLogLevelDebug = 1,
        /// <summary>
        ///【字段含义】 输出 INFO，WARNING，ERROR 和 FATAL 级别的 Log
        /// </summary>
        TRTCLogLevelInfo = 2,
        /// <summary>
        ///【字段含义】 只输出 WARNING，ERROR 和 FATAL 级别的 Log
        /// </summary>
        TRTCLogLevelWarn = 3,
        /// <summary>
        ///【字段含义】 只输出ERROR 和 FATAL 级别的 Log
        /// </summary>
        TRTCLogLevelError = 4,
        /// <summary>
        ///【字段含义】 只输出 FATAL 级别的 Log
        /// </summary>
        TRTCLogLevelFatal = 5,
        /// <summary>
        ///【字段含义】 不输出任何 SDK Log
        /// </summary>
        TRTCLogLevelNone = 6,
    };

    /// <summary>
    /// 4.2 设备操作
    /// </summary>
    public
        enum class TRTCDeviceState {
        /// <summary>
        ///【字段含义】 添加设备
        /// </summary>
        TRTCDeviceStateAdd = 0,
        /// <summary>
        ///【字段含义】 移除设备
        /// </summary>
        TRTCDeviceStateRemove = 1,
        /// <summary>
        ///【字段含义】 设备已启用
        /// </summary>
        TRTCDeviceStateActive = 2,
    };

    /// <summary>
    /// 4.3 水印图片的源类型
    /// </summary>
    public
        enum class TRTCWaterMarkSrcType {
        /// <summary>
        ///【字段含义】 图片文件路径，支持 BMP、GIF、JPEG、PNG、TIFF、Exif、WMF 和 EMF 文件格式
        /// </summary>
        TRTCWaterMarkSrcTypeFile = 0,
        /// <summary>
        ///【字段含义】 BGRA32格式内存块
        /// </summary>
        TRTCWaterMarkSrcTypeBGRA32 = 1,
        /// <summary>
        ///【字段含义】 RGBA32格式内存块
        /// </summary>
        TRTCWaterMarkSrcTypeRGBA32 = 2,
    };

    /// <summary>
    /// 4.4 混流输入类型
    /// </summary>
    public
        enum class TRTCMixInputType {
        /// <summary>
        ///【字段含义】 不指定，根据pureAudio值决定混流类型
        /// </summary>
        TRTCMixInputTypeUndefined = 0,
        /// <summary>
        ///【字段含义】 混入音视频
        /// </summary>
        TRTCMixInputTypeAudioVideo = 1,
        /// <summary>
        ///【字段含义】 只混入视频
        /// </summary>
        TRTCMixInputTypePureVideo = 2,
        /// <summary>
        ///【字段含义】 只混入音频
        /// </summary>
        TRTCMixInputTypePureAudio = 3,
    };

    /// <summary>
    /// 4.5 音频录制内容类型
    /// 该枚举类型用于音频录制接口{@link startAudioRecording}，用于指定录制音频的内容。
    /// </summary>
    public
        enum class TRTCAudioRecordingContent {
        /// <summary>
        ///【字段含义】 录制本地和远端所有音频
        /// </summary>
        TRTCAudioRecordingContentAll = 0,
        /// <summary>
        ///【字段含义】 仅录制本地音频
        /// </summary>
        TRTCAudioRecordingContentLocal = 1,
        /// <summary>
        ///【字段含义】 仅录制远端音频
        /// </summary>
        TRTCAudioRecordingContentRemote = 2,
    };

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      【（五）屏幕分享相关类型定义】
    //                   
    /////////////////////////////////////////////////////////////////////////////////
    /// <summary>
    /// 5.1 屏幕分享目标信息
    /// 
    /// <para>您可以通过 getScreenCaptureSources() 枚举可共享的窗口列表，列表通过 ITRTCScreenCaptureSourceList 返回</para>
    /// </summary>
    public enum class TRTCScreenCaptureSourceType
    {
        TRTCScreenCaptureSourceTypeUnknown = -1,
        /// <summary>
        /// 该分享目标是某一个Windows窗口
        /// </summary>
        TRTCScreenCaptureSourceTypeWindow = 0,
        /// <summary>
        /// 该分享目标是整个Windows桌面
        /// </summary>
        TRTCScreenCaptureSourceTypeScreen = 1,
        TRTCAVScreenCaptureSourceTypeCustom = 2,
    };

    /// <summary>
    /// 5.2 图缓存
    /// </summary>
    public ref struct TRTCImageBuffer
    {
        /// <summary>
        /// 图内容
        /// </summary>
        array<Byte>^ buffer;
        /// <summary>
        /// 图缓存大小
        /// </summary>
        UInt32 length;
        /// <summary>
        /// 图宽
        /// </summary>
        UInt32 width;
        /// <summary>
        /// 图高
        /// </summary>
        UInt32 height;

        TRTCImageBuffer()
            : buffer(nullptr)
            , length(0)
            , width(0)
            , height(0)
        {}
    };

    /// <summary>
    /// 5.3 屏幕采集信息
    /// </summary>
    public ref struct TRTCScreenCaptureSourceInfo
    {
        /// <summary>
        /// 采集源类型
        /// </summary>
        TRTCScreenCaptureSourceType type;
        /// <summary>
        /// 采集源ID；对于窗口，该字段指示窗口句柄；对于屏幕，该字段指示屏幕ID
        /// </summary>
        IntPtr sourceId;
        /// <summary>
        /// 采集源名称，UTF8编码
        /// </summary>
        String^ sourceName;
        /// <summary>
        /// 缩略图内容
        /// </summary>
        TRTCImageBuffer^ thumbBGRA;
        /// <summary>
        /// 图标内容
        /// </summary>
        TRTCImageBuffer^ iconBGRA;
        /// <summary>
        /// 【字段含义】是否为最小化窗口
        /// </summary>
        bool isMinmizeWindow;
        /// <summary>
        ///【字段含义】是否为主显示屏（适用于多显示器的情况）
        /// </summary>
        bool isMainScreen;
        TRTCScreenCaptureSourceInfo()
            : type(TRTCScreenCaptureSourceType::TRTCScreenCaptureSourceTypeUnknown)
            , sourceId(0)
            , sourceName(nullptr)
            , thumbBGRA(nullptr)
            , iconBGRA(nullptr)
            , isMinmizeWindow(false)
            , isMainScreen(false)
        {}
    };

    /// <summary>
    /// 5.4 记录 buffer 的长宽
    /// <para>为了兼容 c++ 使用的 SIZE 类型</para>
    /// </summary>
    public ref struct SIZE
    {
        /// <summary>
        /// 宽
        /// </summary>
        long cx;

        /// <summary>
        /// 高
        /// </summary>
        long cy;
    };

    /// <summary>
    /// 5.5 屏幕采集属性
    /// </summary>
    public ref struct TRTCScreenCaptureProperty
    {
        /// <summary>
        /// 是否采集目标内容时顺带采集鼠标，默认为 true
        /// </summary>
        bool enableCaptureMouse;
        /// <summary>
        /// 是否高亮正在共享的窗口，默认为 true
        /// </summary>
        bool enableHighLight;
        /// <summary>
        /// 是否开启高性能模式（只会在分享屏幕时会生效），开启后屏幕采集性能最佳，但无法过滤远端的高亮边框，默认为 true
        /// </summary>
        bool enableHighPerformance;
        /// <summary>
        /// 【字段含义】窗口采集时是否采集子窗口（需要子窗口与被采集窗口具有 Owner 或 Popup
        /// 属性），默认为 false。
        /// </summary>
        bool enableCaptureChildWindow;
        /// <summary>
        /// 指定高亮边框颜色，RGB格式，传入0时采用默认颜色，默认颜色为 #8CBF26
        /// </summary>
        int  highLightColor;
        /// <summary>
        /// 指定高亮边框的宽度，传入0时采用默认描边宽度，默认宽度为 5，最大值为 50
        /// </summary>
        int  highLightWidth;

        TRTCScreenCaptureProperty()
            : enableCaptureMouse(true)
            , enableHighLight(true)
            , enableHighPerformance(true)
            , highLightColor(0)
            , highLightWidth(0)
            , enableCaptureChildWindow(false)
        {}
    };

    /// <summary>
    /// 5.6 本地录制类型
    /// </summary>
    public enum class TRTCLocalRecordType {
        /// <summary>
        /// 仅录制音频
        /// </summary>
        TRTCLocalRecordType_Audio = 0,

        /// <summary>
        /// 仅录制视频
        /// </summary>
        TRTCLocalRecordType_Video = 1,

        /// <summary>
        /// 同时录制音频、视频
        /// </summary>
        TRTCLocalRecordType_Both = 2,

    };

    /// <summary>
    /// 5.7 本地录制参数
    /// </summary>
    public ref struct TRTCLocalRecordingParams {
        /// <summary>
        ///【字段含义】文件路径（必填），录制的文件地址，请自行指定，确保路径有读写权限且合法，否则录制文件无法生成。
        ///【特别说明】该路径需精确到文件名及格式后缀，格式后缀决定录制文件的格式，目前支持的格式只有 mp4。
        /// Windows建议在应用的私有数据目录中指定存放路径。
        ///【示例代码】在 %appdata%\\test 目录下录制 example.mp4 文件
        /// std::string filePath;
        /// std::wstring path;
        /// wchar_t fullPath[MAX_PATH] = { 0 };
        /// ::SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, fullPath);
        /// path=fullPath;
        /// path += L"\\test\\example.mp4";
        /// filePath = txf_wstr2utf8(path);
        /// </summary>
        String^ filePath;

        /// <summary>
        ///【字段含义】媒体录制类型，默认为同时录制音频和视频。
        /// </summary>
        TRTCLocalRecordType recordType = TRTCLocalRecordType::TRTCLocalRecordType_Both;
        /// <summary>
        ///interval 录制中事件（onLocalRecordDoing）的回调频率，单位毫秒，有效范围：1000-10000，默认为-1表示不回调
        /// </summary>
        int interval = -1;
    };
    /// <summary>
    /// 5.7 屏幕窗口列表
    /// </summary>
    public interface class ITRTCScreenCaptureSourceList
    {
    public:
        /// <summary>
        /// 窗口个数
        /// </summary>
        UInt32 getCount();

        /// <summary>
        /// 窗口信息
        /// </summary>
        TRTCScreenCaptureSourceInfo^ getSourceInfo(UInt32 index);

        /// <summary>
        /// 遍历完窗口列表后，调用release释放资源。
        /// </summary>
        void release();
    };

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      【（六）TRTC 核心类型定义】
    //                   
    /////////////////////////////////////////////////////////////////////////////////

    /// <summary>
    /// 6.1 进房相关参数
    /// 
    /// <para>只有该参数填写正确，才能顺利调用 enterRoom 进入 roomId 所指定的音视频房间。</para>
    /// </summary>
    public ref struct TRTCParams
    {
        /// <summary>
        ///【字段含义】应用标识 [必填]，腾讯视频云基于 sdkAppId 进行计费统计。
        /// <para>在 [实时音视频控制台](https://console.cloud.tencent.com/rav/) 创建应用后可在帐号信息页面中得到该 ID。</para>
        /// </summary>
        UInt32 sdkAppId;
        /// <summary>
        ///【字段含义】用户标识 [必填]，当前用户的 userId，相当于用户名，使用 UTF-8 编码。
        /// <para>【推荐取值】如果一个用户在您的帐号系统中的 ID 为“abc”，则 userId 即可设置为“abc”。</para>
        /// </summary>
        String^ userId;
        /// <summary>
        ///【字段含义】用户签名 [必填]，当前 userId 对应的验证签名，相当于登录密码。
        /// <para>【推荐取值】具体计算方法请参见 [如何计算UserSig](https://cloud.tencent.com/document/product/647/17275)。</para>
        /// </summary>
        String^ userSig;
        /// <summary>
        ///【字段含义】房间号码 [必填]，在同一个房间内的用户可以看到彼此并进行视频通话。
        /// <para>【推荐取值】您可以自定义设置该参数值，但不可重复。如果您的用户帐号 ID （userId）为数字类型，可直接使用创建者的用户 ID 作为 roomId。</para>
        /// </summary>
        UInt32 roomId;
        /// <summary>
        ///【字段含义】字符串房间号码，在同一个房间里的用户（userId）可以彼此看到对方并进行视频通话。
        /// <para>【推荐取值】限制长度为64字节。以下为支持的字符集范围（共 89 个字符）:</para>
        /// - 大小写英文字母（a-zA-Z）；
        /// - 数字（0-9）；
        /// - 空格、"!"、"#"、"$"、"%"、"&amp"、"("、")"、"+"、"-"、":"、";"、"&lt;"、"="、"."、"&gt;"、"?"、"@"、"["、"]"、"^"、"_"、" {"、"}"、"|"、"~"、","。
        /// <para>【特殊说明】roomId 与 strRoomId 必填一个，若您选用 strRoomId，则 roomId 需要填写为0。若两者都填，将优先选用 roomId。</para>
        /// </summary>
        String^ strRoomId;
        /// <summary>
        ///【字段含义】直播场景下的角色，仅适用于直播场景（TRTCAppSceneLIVE），视频通话场景下指定无效。
        /// <para>【推荐取值】默认值：主播（TRTCRoleAnchor）</para>
        /// </summary>
        TRTCRoleType role;
        /// <summary>
        ///【字段含义】绑定腾讯云直播 CDN 流 ID[非必填]，设置之后，您就可以在腾讯云直播 CDN 上通过标准直播方案（FLV或HLS）播放该用户的音视频流。
        /// <para>【推荐取值】限制长度为64字节，可以不填写，一种推荐的方案是使用 “sdkappid_roomid_userid_main” 作为 streamid，这样比较好辨认且不会在您的多个应用中发生冲突。</para>
        /// <para>【特殊说明】要使用腾讯云直播 CDN，您需要先在[控制台](https://console.cloud.tencent.com/trtc/) 中的功能配置页开启“启用旁路推流”开关。</para>
        /// <para>【参考文档】[CDN 旁路直播](https://cloud.tencent.com/document/product/647/16826)。</para>
        /// </summary>
        String^ streamId;
        /// <summary>
        ///【字段含义】设置云端录制完成后的回调消息中的 "userdefinerecordid"  字段内容，便于您更方便的识别录制回调。
        /// <para>【推荐取值】限制长度为64字节，只允许包含大小写英文字母（a-zA-Z）、数字（0-9）及下划线和连词符。</para>
        /// <para>【参考文档】[云端录制](https://cloud.tencent.com/document/product/647/16823)。</para>
        /// </summary>
        String^ userDefineRecordId;
        /// <summary>
        ///【字段含义】房间签名 [非必填]，当您希望某个房间只能让特定的 userId 进入时，需要使用 privateMapKey 进行权限保护。
        /// <para>【推荐取值】仅建议有高级别安全需求的客户使用，更多详情请参见 [进房权限保护](https://cloud.tencent.com/document/product/647/32240)。</para>
        /// </summary>
        String^ privateMapKey;
        /// <summary>
        ///【字段含义】业务数据 [非必填]，部分高级特性才需要使用该字段。
        /// <para>【推荐取值】不建议使用</para>
        /// </summary>
        String^ businessInfo;

        TRTCParams()
            : sdkAppId(0)
            , roomId(0)
            , strRoomId(nullptr)
            , userId(nullptr)
            , userSig(nullptr)
            , role(TRTCRoleType::TRTCRoleAnchor)
            , privateMapKey(nullptr)
            , businessInfo(nullptr)
            , userDefineRecordId(nullptr)
            , streamId(nullptr)
        {}
    };

    /// <summary>
    /// 6.2 视频编码参数
    ///
    /// <para>该设置决定了远端用户看到的画面质量（同时也是云端录制出的视频文件的画面质量）。</para>
    /// </summary>
    public ref struct TRTCVideoEncParam
    {
        /// <summary>
        ///【字段含义】 视频分辨率
        /// <para>【推荐取值】</para>
        /// <para>  - 视频通话建议选择360 × 640及以下分辨率，resMode 选择 Portrait。</para>
        /// <para>  - 手机直播建议选择540 × 960，resMode 选择 Portrait。</para>
        /// <para>  - Window 和 iMac 建议选择640 × 360 及以上分辨率，resMode 选择 Landscape。</para>
        /// <para>【特别说明】 TRTCVideoResolution 默认只有横屏模式的分辨率，例如640 × 360。</para>
        /// <para>            如需使用竖屏分辨率，请指定 resMode 为 Portrait，例如640 × 360结合 Portrait 则为360 × 640。</para>
        /// </summary>
        TRTCVideoResolution videoResolution;
        /// <summary>
        ///【字段含义】分辨率模式（横屏分辨率 - 竖屏分辨率）
        /// <para>【推荐取值】手机直播建议选择 Portrait，Window 和 Mac 建议选择 Landscape。</para>
        /// <para>【特别说明】如果 videoResolution 指定分辨率 640 × 360，resMode 指定模式为 Portrait，则最终编码出的分辨率为360 × 640。</para>
        /// </summary>
        TRTCVideoResolutionMode resMode;
        /// <summary>
        ///【字段含义】视频采集帧率
        /// <para>【推荐取值】15fps 或 20fps。5fps以下，卡顿感明显。10fps以下，会有轻微卡顿感。20fps以上，则过于浪费（电影的帧率为24fps）。</para>
        /// <para>【特别说明】很多 Android 手机的前置摄像头并不支持15fps以上的采集帧率，部分过于突出美颜功能的 Android 手机前置摄像头的采集帧率可能低于10fps。</para>
        /// </summary>
        UInt32 videoFps;
        /// <summary>
        ///【字段含义】目标视频码率，SDK 会按照目标码率进行编码，只有在网络不佳的情况下才会主动降低视频码率。
        /// <para>【推荐取值】请参考本 TRTCVideoResolution 在各档位注释的最佳码率，也可以在此基础上适当调高。</para>
        /// <para>            比如 TRTCVideoResolution_1280_720 对应 1200kbps 的目标码率，您也可以设置为 1500kbps 用来获得更好的清晰度观感。</para>
        /// <para>【特别说明】SDK 会努力按照 videoBitrate 指定的码率进行编码，只有在网络不佳的情况下才会主动降低视频码率，最低会降至 minVideoBitrate 所设定的数值。</para>
        /// <para>            如果您追求“允许卡顿但要保持清晰”的效果，可以设置 minVideoBitrate 为 videoBitrate 的 60%；</para>
        /// <para>            如果您追求“允许模糊但要保持流畅”的效果，可以设置 minVideoBitrate 为 200kbps；</para>
        /// <para>            如果您将 videoBitrate 和 minVideoBitrate 设置为同一个值，等价于关闭 SDK 的自适应调节能力。</para>
        /// </summary>
        UInt32 videoBitrate;
        /// <summary>
        ///【字段含义】最低视频码率，SDK 会在网络不佳的情况下主动降低视频码率，最低会降至 minVideoBitrate 所设定的数值。
        /// <para>【推荐取值】</para>
        /// <para>     - 如果您追求“允许卡顿但要保持清晰”的效果，可以设置 minVideoBitrate 为 videoBitrate 的 60%；</para>
        /// <para>     - 如果您追求“允许模糊但要保持流畅”的效果，可以设置 minVideoBitrate 为 200kbps；</para>
        /// <para>     - 如果您将 videoBitrate 和 minVideoBitrate 设置为同一个值，等价于关闭 SDK 的自适应调节能力；</para>
        /// <para>     - 默认值：0，此时最低码率由 SDK 根据分辨率情况，自动设置合适的数值。</para>
        /// <para>【特别说明】</para>
        /// <para>     - 当您把分辨率设置的比较高时，minVideoBitrate 不适合设置的太低，否则会出现画面模糊和大范围的马赛克宏块。</para>
        /// <para>       比如把分辨率设置为 720p，把码率设置为 200kbps，那么编码出的画面将会出现大范围区域性马赛克。</para>
        /// </summary>
        UInt32 minVideoBitrate;
        /// <summary>
        ///【字段含义】是否允许调整分辨率
        /// <para>【推荐取值】</para>
        /// <para>   - 手机直播建议选择 NO。</para>
        /// <para>   - 视频通话模式，若更关注流畅性，建议选择 YES，此时若遇到带宽有限的弱网，SDK 会自动降低分辨率以保障更好的流畅度（仅针对 TRTCVideoStreamTypeBig 生效）。</para>
        /// <para>   - 默认值：NO。</para>
        /// <para>【特别说明】若有录制需求，选择 YES 时，请确保通话过程中，调整分辨率不会影响您的录制效果。</para>
        /// </summary>
        bool enableAdjustRes;

        TRTCVideoEncParam()
            : videoResolution(TRTCVideoResolution::TRTCVideoResolution_640_360)
            , resMode(TRTCVideoResolutionMode::TRTCVideoResolutionModeLandscape)
            , videoFps(15)
            , videoBitrate(550)
            , minVideoBitrate(0)
            , enableAdjustRes(false)
        {}
    };

    /// <summary>
    /// 6.3 网络流控相关参数
    ///
    /// <para>网络流控相关参数，该设置决定了SDK在各种网络环境下的调控方向（例如弱网下是“保清晰”或“保流畅”）</para>
    /// </summary>
    public ref struct TRTCNetworkQosParam
    {
        /// <summary>
        ///【字段含义】弱网下是“保清晰”或“保流畅”
        /// <para>【特别说明】</para>
        /// <para>   - 弱网下保流畅：在遭遇弱网环境时，画面会变得模糊，且出现较多马赛克，但可以保持流畅不卡顿</para>
        /// <para>   - 弱网下保清晰：在遭遇弱网环境时，画面会尽可能保持清晰，但可能容易出现卡顿</para>
        /// </summary>
        TRTCVideoQosPreference preference;
        /// <summary>
        ///【字段含义】视频分辨率（云端控制 - 客户端控制）
        /// <para>【推荐取值】云端控制</para>
        /// <para>【特别说明】</para>
        /// <para>   - Server 模式（默认）：云端控制模式，若无特殊原因，请直接使用该模式</para>
        /// <para>   - Client 模式：客户端控制模式，用于 SDK 开发内部调试，客户请勿使用</para>
        /// </summary>
        TRTCQosControlMode controlMode;

        TRTCNetworkQosParam()
            : preference(TRTCVideoQosPreference::TRTCVideoQosPreferenceClear)
            , controlMode(TRTCQosControlMode::TRTCQosControlModeServer)
        {}
    };

    /// <summary>
    /// 6.4 视频质量
    /// 
    /// <para>表示视频质量的好坏，通过这个数值，您可以在 UI 界面上用图标表征 userId 的通话线路质量</para>
    /// </summary>
    public ref struct TRTCQualityInfo
    {
        /// <summary>
        /// 用户标识
        /// </summary>
        String^ userId;
        /// <summary>
        /// 视频质量
        /// </summary>
        TRTCQuality quality;

        TRTCQualityInfo()
            : userId(nullptr)
            , quality(TRTCQuality::TRTCQuality_Unknown)
        {}
    };

    /// <summary>
    /// 6.5 音量大小
    /// 
    /// <para>表示语音音量的评估大小，通过这个数值，您可以在 UI 界面上用图标表征 userId 是否有在说话。</para>
    /// </summary>
    public ref struct TRTCVolumeInfo
    {
        /// <summary>
        /// 说话者的 userId，字符编码格式为 UTF-8
        /// </summary>
        String^ userId;
        /// <summary>
        /// 说话者的音量， 取值范围0 - 100              
        /// </summary>
        UInt32 volume;

        TRTCVolumeInfo()
            : userId(nullptr)
            , volume(0)
        {}
    };

    /// <summary>
    /// 6.6 视频纹理数据
    /// </summary>
    public ref struct TRTCTexture {
        ///【字段含义】视频纹理 ID。
        int glTextureId;

        TRTCTexture() : glTextureId(0) {}
    };

    /// <summary>
    /// 6.7 视频帧数据
    /// </summary>
    public ref struct TRTCVideoFrame
    {
        /// <summary>
        /// 视频帧的格式
        /// </summary>
        TRTCVideoPixelFormat videoFormat;
        /// <summary>
        /// 视频数据结构类型
        /// </summary>
        TRTCVideoBufferType bufferType;
        /// <summary>
        /// 视频数据，字段bufferType是LiteAVVideoBufferType_Buffer时生效
        /// </summary>
        array<Byte>^ data;
        /// <summary>
        /// 视频纹理ID，字段bufferType是LiteAVVideoBufferType_Texture时生效
        /// </summary>
        TRTCTexture texture;
        /// <summary>
        /// 视频数据的长度，单位是字节，对于i420而言， length = width * height * 3 / 2，对于BGRA32而言， length = width * height * 4
        /// </summary>
        UInt32 length;
        /// <summary>
        /// 画面的宽度
        /// </summary>
        UInt32 width;
        /// <summary>
        /// 画面的高度
        /// </summary>
        UInt32 height;
        /// <summary>
        /// 时间戳，单位ms
        /// </summary>
        UInt64 timestamp;
        /// <summary>
        /// 画面旋转角度
        /// </summary>
        TRTCVideoRotation rotation;

        TRTCVideoFrame()
            : videoFormat(TRTCVideoPixelFormat::TRTCVideoPixelFormat_Unknown)
            , bufferType(TRTCVideoBufferType::TRTCVideoBufferType_Unknown)
            , data(nullptr)
            , length(0)
            , width(640)
            , height(360)
            , timestamp(0)
            , rotation(TRTCVideoRotation::TRTCVideoRotation0)
        {}
    };

    /// <summary>
    /// 6.8 音频帧数据
    /// </summary>
    public ref struct TRTCAudioFrame
    {
        /// <summary>
        /// 音频帧的格式
        /// </summary>
        TRTCAudioFrameFormat audioFormat;
        /// <summary>
        /// 音频数据
        /// </summary>
        array<Byte>^ data;
        /// <summary>
        /// 音频数据的长度
        /// </summary>
        UInt32 length;
        /// <summary>
        /// 采样率
        /// </summary>
        UInt32 sampleRate;
        /// <summary>
        /// 声道数
        /// </summary>
        UInt32 channel;
        /// <summary>
        /// 时间戳，单位ms
        /// </summary>
        UInt64 timestamp;

        TRTCAudioFrame()
            : audioFormat(TRTCAudioFrameFormat::TRTCAudioFrameFormatNone)
            , data(nullptr)
            , length(0)
            , sampleRate(48000)
            , channel(1)
            , timestamp(0)
        {}
    };

    /// <summary>
    /// 6.9 网络测速结果
    /// <para>
    /// 您可以在用户进入房间前通过 TRTCCloud 的 startSpeedTest 接口进行测速 （注意：请不要在通话中调用），
    /// 测速结果会每2 - 3秒钟返回一次，每次返回一个 IP 地址的测试结果。
    /// </para>
    /// </summary>
    /// <remarks>
    /// - quality 是内部通过评估算法测算出的网络质量，loss 越低，rtt 越小，得分便越高。
    /// <para>- upLostRate 是指上行丢包率。例如，0.3表示每向服务器发送10个数据包可能会在中途丢失3个。</para>
    /// <para>- downLostRate 是指下行丢包率。例如，0.2表示每从服务器收取10个数据包可能会在中途丢失2个。</para>
    /// <para>- rtt 是指当前设备到腾讯云服务器的一次网络往返时间，该值越小越好，正常数值范围是10ms - 100ms。</para>
    /// </remarks>
    public ref struct TRTCSpeedTestResult
    {
        /// <summary>
        /// 服务器 IP 地址
        /// </summary>
        String^ ip;
        /// <summary>
        /// 网络质量，内部通过评估算法测算出的网络质量，loss 越低，rtt 越小，得分便越高
        /// </summary>
        TRTCQuality quality;
        /// <summary>
        /// 上行丢包率，范围是0 - 1.0，例如，0.3表示每向服务器发送10个数据包可能会在中途丢失3个。
        /// </summary>
        float upLostRate;
        /// <summary>
        /// 下行丢包率，范围是0 - 1.0，例如，0.2表示每从服务器收取10个数据包可能会在中途丢失2个。
        /// </summary>
        float downLostRate;
        /// <summary>
        /// 延迟（毫秒），指当前设备到腾讯云服务器的一次网络往返时间，该值越小越好，正常数值范围是10ms - 100ms
        /// </summary>
        int rtt;

        TRTCSpeedTestResult()
            : ip(nullptr)
            , upLostRate(0.0f)
            , downLostRate(0.0f)
            , rtt(0)
        {}
    };

    /// <summary>
    /// 记录矩形的四个点坐标
    /// <para>为了兼容 c++ 使用的 RECT 类型</para>
    /// </summary>
    public ref struct RECT
    {
        /// <summary>
        /// 左坐标点
        /// </summary>
        int left;
        /// <summary>
        /// 上坐标点
        /// </summary>
        int top;
        /// <summary>
        /// 右坐标点
        /// </summary>
        int right;
        /// <summary>
        /// 下坐标点
        /// </summary>
        int bottom;
    };

    /// <summary>
    /// 6.10 云端混流中每一路子画面的位置信息
    /// 
    /// <para>TRTCMixUser 用于指定每一路（即每一个 userId）视频画面的具体摆放位置</para>
    /// </summary>
    public ref struct TRTCMixUser
    {
        /// <summary>
        /// 参与混流的 userId
        /// </summary>
        String^ userId;
        /// <summary>
        /// 参与混流的 roomId，跨房流传入的实际 roomId，当前房间流传入 roomId = NULL
        /// </summary>
        String^ roomId;
        /// <summary>
        /// 图层位置坐标以及大小，左上角为坐标原点(0,0) （绝对像素值）
        /// </summary>
        RECT^ rect;
        /// <summary>
        /// 图层层次（1 - 15）不可重复
        /// </summary>
        int zOrder;
        /// <summary>
        /// 是否纯音频
        /// </summary>
        bool pureAudio;
        /// <summary>
        /// 参与混合的是主路画面（TRTCVideoStreamTypeBig）或屏幕分享（TRTCVideoStreamTypeSub）画面
        /// </summary>
        TRTCVideoStreamType streamType;
        /// <summary>
        /// 【字段含义】该用户的输入流类型（该字段是对 pureAudio 字段的升级）
        /// 【推荐取值】
        ///     - 默认值：TRTCMixInputTypeUndefined
        ///     - 如果您没有对 pureAudio 字段进行设置，您可以根据实际需要设置该字段
        ///     - 如果您已经设置了 pureAudio 为 YES，请设置该字段为 TRTCMixInputTypeUndefined
        /// </summary>
        TRTCMixInputType inputType;

        TRTCMixUser()
            : userId(nullptr)
            , roomId(nullptr)
            , rect(nullptr)
            , zOrder(0)
            , pureAudio(false)
            , streamType(TRTCVideoStreamType::TRTCVideoStreamTypeBig)
            , inputType(TRTCMixInputType::TRTCMixInputTypeUndefined)
        {}
    };

    /// <summary>
    /// 6.11 混流参数配置模式
    /// 
    /// <para>目前暂仅支持手动配置这一种模式，即需要指定 TRTCTranscodingConfig 的全部参数。</para>
    /// </summary>
    public enum class TRTCTranscodingConfigMode
    {
        /// <summary>
        /// 非法值
        /// </summary>
        TRTCTranscodingConfigMode_Unknown = 0,

        /// <summary>
        /// <para>全手动模式，灵活性最高，可以自由组合出各种混流方案，但易用性最差。</para>
        /// <para>此模式下，您需要填写 TRTCTranscodingConfig 中的所有参数，并需要监听 TRTCCloudDelegate 中的 onUserVideoAvailable() 和 onUserAudioAvailable() 回调，</para>
        /// <para>以便根据当前房间中各个上麦用户的音视频状态不断地调整 mixUsers 参数，否则会导致混流失败。</para>
        /// </summary>
        TRTCTranscodingConfigMode_Manual = 1,

        /// <summary>
        /// <para>纯音频模式，适用于语音通话（AudioCall）和语音聊天室（VoiceChatRoom）等纯音频场景。</para>
        /// <para>只需要在进房后通过 setMixTranscodingConfig() 接口设置一次，之后 SDK 就会自动把房间内所有上麦用户的声音混流到当前用户的直播流上。</para>
        /// <para>此模式下，您无需设置 TRTCTranscodingConfig 中的 mixUsers 参数，只需设置 audioSampleRate、audioBitrate 和 audioChannels 等参数。</para>
        /// </summary>
        TRTCTranscodingConfigMode_Template_PureAudio = 2,

        /// <summary>
        /// <para>预排版模式，通过占位符提前对各路画面进行排布</para>
        /// <para>此模式下，您依然需要设置 mixUsers 参数，但可以将 userId 设置为占位符，可选的占位符有：</para>
        /// <para> - "$PLACE_HOLDER_REMOTE$"     :  指代远程用户的画面，可以设置多个。</para>
        /// <para> - "$PLACE_HOLDER_LOCAL_MAIN$" :  指代本地摄像头画面，只允许设置一个。</para>
        /// <para> - "$PLACE_HOLDER_LOCAL_SUB$"  :  指代本地屏幕分享画面，只允许设置一个。</para>
        /// <para>但是您可以不需要监听 TRTCCloudDelegate 中的 onUserVideoAvailable() 和 onUserAudioAvailable() 回调进行实时调整，</para>
        /// <para>只需要在进房成功后调用一次 setMixTranscodingConfig() 即可，之后 SDK 会自动将真实的 userId 补位到您设置的占位符上。</para>
        /// </summary>
        TRTCTranscodingConfigMode_Template_PresetLayout = 3,

        /// <summary>
        /// <para>屏幕分享模式，适用于在线教育场景等以屏幕分享为主的应用场景，仅支持 Windows 和 Mac 两个平台的 SDK。</para>
        /// <para>SDK 会先根据您（通过 videoWidth 和 videoHeight 参数）设置的目标分辨率构建一张画布，</para>
        /// <para>当老师未开启屏幕分享时，SDK 会将摄像头画面等比例拉伸绘制到该画布上；当老师开启屏幕分享之后，SDK 会将屏幕分享画面绘制到同样的画布上。</para>
        /// <para>这样操作的目的是为了确保混流模块的输出分辨率一致，避免课程回放和网页观看的花屏问题（网页播放器不支持可变分辨率）。</para>
        /// <para>由于教学模式下的视频内容以屏幕分享为主，因此同时传输摄像头画面和屏幕分享画面是非常浪费带宽的。</para>
        /// <para>推荐的做法是直接将摄像头画面通过 setLocalVideoRenderCallback 接口自定义绘制到当前屏幕上。</para>
        /// <para>在该模式下，您无需设置 TRTCTranscodingConfig 中的 mixUsers 参数，SDK 不会混合学生的画面，以免干扰屏幕分享的效果。</para>
        /// <para>您可以将 TRTCTranscodingConfig 中的 width × height 设为 0px × 0px，SDK 会自动根据用户当前屏幕的宽高比计算出一个合适的分辨率：</para>
        /// <para> - 如果老师当前屏幕宽度 &lt;= 1920px，SDK 会使用老师当前屏幕的实际分辨率。</para>
        /// <para> - 如果老师当前屏幕宽度 &gt; 1920px，SDK 会根据当前屏幕宽高比，选择 1920x1080(16:9)、1920x1200(16:10)、1920x1440(4:3) 三种分辨率中的一种。</para>
        /// </summary>
        TRTCTranscodingConfigMode_Template_ScreenSharing = 4,
    };

    /// <summary>
    /// 6.12 云端混流（转码）配置
    ///
    /// <para>包括最终编码质量和各路画面的摆放位置</para>
    /// </summary>
    public ref struct TRTCTranscodingConfig
    {
        /// <summary>
        ///【字段含义】转码 config 模式
        /// </summary>
        TRTCTranscodingConfigMode mode;
        /// <summary>
        ///【字段含义】腾讯云直播 AppID
        /// <para>【推荐取值】请在 [实时音视频控制台](https://console.cloud.tencent.com/rav) 选择已经创建的应用，单击【帐号信息】后，在“直播信息”中获取</para>
        /// </summary>
        UInt32 appId;
        /// <summary>
        ///【字段含义】腾讯云直播 bizid
        /// <para>【推荐取值】请在 [实时音视频控制台](https://console.cloud.tencent.com/rav) 选择已经创建的应用，单击【帐号信息】后，在“直播信息”中获取</para>
        /// </summary>
        UInt32 bizId;
        /// <summary>
        ///【字段含义】最终转码后的视频分辨率的宽度（px）
        /// <para>【推荐取值】推荐值：360px ，如果你是纯音频推流，请将 width × height 设为 0px × 0px，否则混流后会携带一条画布背景的视频流。</para>
        /// </summary>
        UInt32 videoWidth;
        /// <summary>
        ///【字段含义】最终转码后的视频分辨率的高度（px）
        /// <para>【推荐取值】推荐值：640px ，如果你是纯音频推流，请将 width × height 设为 0px × 0px，否则混流后会携带一条画布背景的视频流。</para>
        /// </summary>
        UInt32 videoHeight;
        /// <summary>
        ///【字段含义】最终转码后的视频分辨率的码率（kbps）
        /// <para>【推荐取值】如果填0，后台会根据videoWidth和videoHeight来估算码率，您也可以参考枚举定义TRTCVideoResolution_640_480的注释。</para>
        /// </summary>
        UInt32 videoBitrate;
        /// <summary>
        ///【字段含义】最终转码后的视频分辨率的帧率（FPS）
        /// <para>【推荐取值】默认值：15fps，取值范围是 (0,30]。</para>
        /// </summary>
        UInt32 videoFramerate;
        /// <summary>
        ///【字段含义】最终转码后的视频分辨率的关键帧间隔（又称为 GOP），单位秒
        /// <para>【推荐取值】默认值：2，单位为秒，取值范围是 [1,8]。</para>
        /// </summary>
        UInt32 videoGOP;
        /// <summary>
        ///【字段含义】混流背景颜色，格式为 0xRRGGBB。
        /// <para>【推荐取值】默认值：0x000000，黑色。</para>
        /// </summary>
        UInt32 backgroundColor;
        /// <summary>
        ///【字段含义】混合后画面的背景图。
        /// <para>【推荐取值】默认值：null，即不设置背景图</para>
        /// <para>【特别说明】背景图需要您事先在 “[控制台](https://console.cloud.tencent.com/trtc) => 应用管理 => 功能配置 => 素材管理” 中上传，</para>
        /// <para>           上传成功后可以获得对应的“图片ID”，然后将“图片ID”转换成字符串类型并设置到 backgroundImage 里即可。</para>
        /// <para>           例如：假设“图片ID” 为 63，可以设置 backgroundImage = "63";</para>
        /// </summary>
        String^ backgroundImage;
        /// <summary>
        ///【字段含义】最终转码后的音频采样率。
        /// <para>【推荐取值】默认值：48000Hz。支持12000HZ、16000HZ、22050HZ、24000HZ、32000HZ、44100HZ、48000HZ。</para>
        /// </summary>
        UInt32 audioSampleRate;
        /// <summary>
        ///【字段含义】最终转码后的音频码率。
        /// <para>【推荐取值】默认值：64kbps，取值范围是 [32，192]。</para>
        /// </summary>
        UInt32 audioBitrate;
        /// <summary>
        ///【字段含义】最终转码后的音频声道数
        /// <para>【推荐取值】默认值：1。取值范围为 [1,2] 中的整型。</para>
        /// </summary>
        UInt32 audioChannels;
        /// <summary>
        ///【字段含义】每一路子画面的位置信息
        /// </summary>
        array<TRTCMixUser^>^ mixUsersArray;
        /// <summary>
        ///【字段含义】 数组 mixUsersArray 的大小
        /// </summary>
        UInt32 mixUsersArraySize;
        /// <summary>
        ///【字段含义】输出到 CDN 上的直播流 ID
        ///          如不设置该参数，SDK
        ///          会执行默认逻辑，即房间里的多路流会混合到该接口调用者的视频流上，也就是 A + B => A；
        ///          如果设置该参数，SDK 会将房间里的多路流混合到您指定的直播流 ID 上，也就是 A + B => C。
        ///【推荐取值】默认值：null，即房间里的多路流会混合到该接口调用者的视频流上。
        /// </summary>
        String^ streamId;

        TRTCTranscodingConfig()
            : mode(TRTCTranscodingConfigMode::TRTCTranscodingConfigMode_Unknown)
            , videoWidth(0)
            , videoHeight(0)
            , videoBitrate(0)
            , videoFramerate(15)
            , videoGOP(2)
            , audioSampleRate(64)
            , audioBitrate(48000)
            , audioChannels(1)
            , mixUsersArray(nullptr)
            , mixUsersArraySize(0)
            , backgroundColor(0)
            , backgroundImage(nullptr)
            , streamId(nullptr)
        {}
    };

    /// <summary>
    /// 6.13 CDN 旁路推流参数 
    /// </summary>
    public ref struct TRTCPublishCDNParam
    {
        /// <summary>
        /// 腾讯云 AppID，请在 [实时音视频控制台](https://console.cloud.tencent.com/rav) 选择已经创建的应用，单击【帐号信息】，在“直播信息”中获取
        /// </summary>
        UInt32 appId;
        /// <summary>
        /// 腾讯云直播 bizid，请在 [实时音视频控制台](https://console.cloud.tencent.com/rav) 选择已经创建的应用，单击【帐号信息】，在“直播信息”中获取
        /// </summary>
        UInt32 bizId;
        /// <summary>
        /// 旁路转推的 URL
        /// </summary>
        String^ url;

        TRTCPublishCDNParam()
            : url(nullptr)
            , appId(0)
            , bizId(0)
        {

        }
    };

    /// <summary>
    /// 6.14 录音参数
    ///
    /// <para>请正确填写参数，确保录音文件顺利生成。</para>
    /// </summary>
    public ref struct TRTCAudioRecordingParams {

        /// <summary>
        ///【字段含义】文件路径（必填），录音文件的保存路径。该路径需要用户自行指定，请确保路径存在且可写。
        ///【特别说明】该路径需精确到文件名及格式后缀，格式后缀决定录音文件的格式，目前支持的格式有 PCM、WAV 和 AAC。
        ///           例如，指定路径为 path/to/audio.aac，则会生成一个 AAC 格式的文件。
        ///           请指定一个有读写权限的合法路径，否则录音文件无法生成。
        /// </summary>
        String^ filePath;
        /// <summary>
        ///【字段含义】音频录制内容类型。
        ///【特别说明】默认录制所有本地和远端音频
        /// </summary>
        TRTCAudioRecordingContent recordingContent;

        TRTCAudioRecordingParams()
            : filePath(nullptr)
            , recordingContent(TRTCAudioRecordingContent::TRTCAudioRecordingContentAll)
        {

        }
    };

    /// <summary>
    /// 6.15 音效播放（已废弃）
    /// </summary>
    public ref struct TRTCAudioEffectParam {

        /// <summary>
        ///【字段含义】音效 ID
        ///【特别说明】SDK 允许播放多路音效，因此需要音效 ID 进行标记，用于控制音效的开始、停止、音量等
        /// </summary>
        int effectId;

        /// <summary>
        ///【字段含义】音效路径
        /// </summary>
        String^ path;

        /// <summary>
        ///【字段含义】循环播放次数
        ///【推荐取值】取值范围为0 - 任意正整数，默认值：0。0表示播放音效一次；1表示播放音效两次；以此类推
        /// </summary>
        int loopCount;

        /// <summary>
        ///【字段含义】音效是否上行
        ///【推荐取值】YES：音效在本地播放的同时，会上行至云端，因此远端用户也能听到该音效；NO：音效不会上行
        /// </summary>
        bool publish;

        /// <summary>
        ///【字段含义】音效音量
        ///【推荐取值】取值范围为0 - 100；默认值：100
        /// </summary>
        int volume;

        TRTCAudioEffectParam(const int _effectId, String^ _path)
            : loopCount(0)
            , publish(false)
            , volume(100)
        {
            effectId = _effectId;
            path = _path;
        }
    };

    /// <summary>
    /// 6.16 切换房间参数
    /// </summary>
    public ref struct TRTCSwitchRoomConfig {
        /// <summary>
        ///【字段含义】数字房间号码 [选填]，在同一个房间内的用户可以看到彼此并进行视频通话。
        ///【推荐取值】取值范围：1 - 4294967294。
        ///【特别说明】roomId 和 strRoomId 必须并且只能填一个。若两者都填，则优先选择 roomId。
        /// </summary>
        uint32_t roomId;

        /// <summary>
        ///【字段含义】字符串房间号码 [选填]，在同一个房间内的用户可以看到彼此并进行视频通话。
        ///【特别说明】roomId 和 strRoomId 必须并且只能填一个。若两者都填，则优先选择 roomId。
        /// </summary>
        String^ strRoomId;

        /// <summary>
        ///【字段含义】用户签名 [选填]，当前 userId 对应的验证签名，相当于登录密码。不填时，SDK 会继续使用旧的 userSig，
        ///          但用户必须保证旧的 userSig 仍在有效期内，否则会造成进房失败等后果。
        ///【推荐取值】具体计算方法请参见 [如何计算UserSig](https://cloud.tencent.com/document/product/647/17275)。
        /// </summary>
        String^ userSig;

        /// <summary>
        ///【字段含义】房间签名 [选填]，当您希望某个房间只能让特定的 userId 进入时，需要使用 privateMapKey 进行权限保护。
        ///【推荐取值】仅建议有高级别安全需求的客户使用，更多详情请参见 [进房权限保护](https://cloud.tencent.com/document/product/647/32240)。
        /// </summary>
        String^ privateMapKey;

        TRTCSwitchRoomConfig()
            : roomId(0)
            , strRoomId(nullptr)
            , userSig(nullptr)
            , privateMapKey(nullptr)
        {}
    };

    /// <summary>
    /// 6.17 画面渲染参数
    /// 
    /// <para>您可以通过设置此参数来控制画面的旋转、填充、镜像模式</para>
    /// </summary>
    public ref struct TRTCRenderParams {
        /// <summary>
        /// 视频画面旋转方向，默认值为 TRTCVideoRotation0
        /// </summary>
        TRTCVideoRotation rotation;

        /// <summary>
        /// 视频画面填充模式，默认值为 TRTCVideoFillMode_Fit
        /// </summary>
        TRTCVideoFillMode fillMode;

        /// <summary>
        /// 视频画面镜像模式，默认值为 TRTCVideoMirrorType_Disable
        /// </summary>
        TRTCVideoMirrorType mirrorType;

        TRTCRenderParams()
            : rotation(TRTCVideoRotation::TRTCVideoRotation0)
            , fillMode(TRTCVideoFillMode::TRTCVideoFillMode_Fit)
            , mirrorType(TRTCVideoMirrorType::TRTCVideoMirrorType_Disable) {
        }
    };

    /// <summary>
    /// 6.18 测速参数
    ///
    /// <para>您可以在用户进入房间前通过 startSpeedTest 接口测试网速（注意：请不要在通话中调用）。</para>
    /// </summary>
    public ref struct TRTCSpeedTestParams {
        /// 应用标识，请参考 {@link TRTCParams} 中的相关说明。
        int sdkAppId;

        /// 用户标识，请参考 {@link TRTCParams} 中的相关说明。
        String^ userId;

        /// 用户签名，请参考 {@link TRTCParams} 中的相关说明。
        String^ userSig;

        /// 预期的上行带宽（kbps，取值范围： 10 ～ 5000，为 0 时不测试）。
        int expectedUpBandwidth;

        /// 预期的下行带宽（kbps，取值范围： 10 ～ 5000，为 0 时不测试）。
        int expectedDownBandwidth;

        TRTCSpeedTestParams()
            : sdkAppId(0),
            userId(nullptr),
            userSig(nullptr),
            expectedUpBandwidth(0),
            expectedDownBandwidth(0) {
        }
    };
    /// @}
}
