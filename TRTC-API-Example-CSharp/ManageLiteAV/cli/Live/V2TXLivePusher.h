#pragma once
// C++
#include "include/Live2/V2TXLivePusher.hpp"

#include "cli/Live/V2TXLivePusherObserver.h"
#include "cli/Live/V2TXLiveDef.h"
#include "cli/ITRTCAudioEffectManager.h"
#include "cli/ITRTCDeviceManager.h"

using namespace System;
class V2TXLivePusherObserverImpl;

namespace ManageLiteAV
{
    /// @defgroup V2TXLivePusher_csharp V2TXLivePusher
    /// 腾讯云直播推流器
    /// @{
    /// <summary>
    /// Module:   V2TXLivePusher @ TXLiteAVSDK
    /// <para>主要负责将本地的音频和视频画面进行编码，并推送到指定的推流地址，支持任意的推流服务端。</para>
    /// 推流器包含如下能力：
    /// - 自定义的视频采集，让您可以根据项目需要定制自己的音视频数据源；
    /// - Qos 流量控制技术，具备上行网络自适应能力，可以根据主播端网络的具体情况实时调节音视频数据量；
    /// - 脸形调整、动效挂件，支持基于优图 AI 人脸识别技术的大眼、瘦脸、隆鼻等脸形微调以及动效挂件效果，只需要购买 **优图 License** 就可以轻松实现丰富的直播效果。  
    /// </summary>  
    public ref class V2TXLivePusher
    {
    public:
        /// <summary>
        /// 用于动态加载 dll 时，获取 V2TXLivePlayer 对象指针
        /// </summary>
        /// <returns>返回 V2TXLivePlayer 对象的指针，注意：请调用 releaseV2TXLivePlayer 析构</returns>
        /// <para>note 本接口适用于Windows、Mac、iOS平台</para> 
        static V2TXLivePusher^ createV2TXLivePusher(V2TXLiveMode mode);

        /// <summary>
        /// 释放 V2TXLivePlayer 单例对象。
        /// </summary>
        static void releaseV2TXLivePusher(V2TXLivePusher^ pusher);

        /// <summary>
        /// 设置推流器回调
        /// </summary>
        /// <param name="observer">推流器的回调目标对象，更多信息请查看 {@link V2TXLivePusherObserver}</param>
        void setObserver(V2TXLivePusherObserver^ observer);

        /// <summary>
        /// 设置本地摄像头预览 View。
        /// </summary>
        /// <param name="view">view 本地摄像头预览 View</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 setRenderView(IntPtr view);

        /// <summary>
        /// 设置本地摄像头预览镜像
        /// </summary>
        /// <param name="mirrorType">摄像头镜像类型 {@link V2TXLiveMirrorType}</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 setRenderMirror(V2TXLiveMirrorType mirrorType);

        /// <summary>
        /// 设置视频编码镜像
        /// </summary>
        /// <param name="mirror">是否镜像</param>
        /// <returns>编码镜像只影响观众端看到的视频效果。</returns>
        Int32 setEncoderMirror(bool mirror);

        /// <summary>
        /// 设置本地摄像头预览画面的旋转角度
        /// </summary>
        /// <param name="rotation">rotation 预览画面的旋转角度 {@link V2TXLiveRotation}</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 setRenderRotation(V2TXLiveRotation rotation);

        /// <summary>
        /// 打开本地摄像头
        /// </summary>
        /// <param name="cameraId">摄像头标识</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        /// <para>startVirtualCamera，startCamera，startScreenCapture，同一 Pusher 实例下，仅有一个能上行，三者为覆盖关系。例如先调用 startCamera，后调用 startVirtualCamera。此时表现为暂停摄像头推流，开启图片推流</para>
        Int32 startCamera(String^ cameraId);

        /// <summary>
        /// 关闭本地摄像头
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 stopCamera();

        /// <summary>
        /// 打开麦克风
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 startMicrophone();

        /// <summary>
        /// 关闭麦克风
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 stopMicrophone();

        /// <summary>
        /// 开启图片推流
        /// </summary>
        /// <param name="image">图片</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        /// <para>startVirtualCamera，startCamera，startScreenCapture，同一 Pusher 实例下，仅有一个能上行，三者为覆盖关系。例如先调用 startCamera，后调用 startVirtualCamera。此时表现为暂停摄像头推流，开启图片推流</para>
        Int32 startVirtualCamera(V2TXLiveImage^ image);

        /// <summary>
        /// 关闭图片推流
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 stopVirtualCamera();

        /// <summary>
        /// 暂停推流器的音频流
        /// </summary>
        /// <returns> 返回值 {@link V2TXLiveCode}</returns>
        Int32 pauseAudio();

        /// <summary>
        /// 恢复推流器的音频流
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 resumeAudio();

        /// <summary>
        /// 暂停推流器的视频流
        /// </summary>
        /// <returns> 返回值 {@link V2TXLiveCode}</returns>
        Int32 pauseVideo();

        /// <summary>
        /// 恢复推流器的视频流
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 resumeVideo();

        /// <summary>
        /// 开始音视频数据推流
        /// </summary>
        /// <param name="url">推流的目标地址，支持任意推流服务端</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 startPush(String^ url);

        /// <summary>
        /// 停止推送音视频数据
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 stopPush();

        /// <summary>
        /// 当前推流器是否正在推流中
        /// </summary>
        /// <returns>是否正在推流(1: 正在推流中,0: 已经停止推流)</returns>
        Int32 isPushing();

        /// <summary>
        /// 设置推流音频质量
        /// </summary>
        /// <param name="quality">音频质量 {@link V2TXLiveAudioQuality}</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 setAudioQuality(V2TXLiveAudioQuality quality);

        /// <summary>
        /// 设置推流视频编码参数
        /// </summary>
        /// <param name="param">视频编码参数 {@link V2TXLiveVideoEncoderParam}</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 setVideoQuality(V2TXLiveVideoEncoderParam^ param);

        /// <summary>
        /// 获取音效管理对象 {@link TXAudioEffectManager}
        /// 
        /// 通过音效管理，您可以使用以下功能：
        /// - 调整麦克风收集的人声音量。
        /// - 设置混响和变声效果。
        /// - 开启耳返，设置耳返音量。
        /// - 添加背景音乐，调整背景音乐的播放效果。
        /// </summary>
        /// <returns>返回值 {@link TXAudioEffectManager}</returns>
        ITXAudioEffectManager^ getAudioEffectManager();

        /// <summary>
        /// 获取设备管理对象 {@link TXDeviceManager}。
        /// 
        /// 通过设备管理，您可以使用以下功能：
        /// - 切换前后摄像头。
        /// - 设置自动聚焦。
        /// - 设置摄像头缩放倍数。
        /// - 打开或关闭闪光灯。
        /// - 切换耳机或者扬声器。
        /// - 修改音量类型(媒体音量或者通话音量)。
        /// </summary>
        /// <returns></returns>
        ITXDeviceManager^ getDeviceManager();

        /// <summary>
        /// 截取推流过程中的本地画面。
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 snapshot();

        /// <summary>
        /// 设置推流器水印。默认情况下，水印不开启。
        /// 
        /// 水印的位置是通过 x, y, scale 来指定的。
        /// - x：水印的坐标，取值范围为0 - 1的浮点数。
        /// - y：水印的坐标，取值范围为0 - 1的浮点数。
        /// - scale：水印的大小比例，取值范围为0 - 1的浮点数。
        /// </summary>
        /// <param name="watermarkPath">水印图片文件路径，为 nullptr 则等同于关闭水印</param>
        /// <param name="x">水印显示的左上角 x 轴偏移</param>
        /// <param name="y">水印显示的左上角 y 轴偏移</param>
        /// <param name="scale">水印显示的宽度占画面宽度比例（水印按该参数等比例缩放显示）</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 setWatermark(String^ watermarkPath, float x, float y, float scale);

        /// <summary>
        /// 启用采集音量大小提示
        /// 开启后可以在 {@link V2TXLivePusherObserver#onMicrophoneVolumeUpdate(int)} 回调中获取到 SDK 对音量大小值的评估。
        /// </summary>
        /// <param name="intervalMs">intervalMs 决定了 onMicrophoneVolumeUpdate 回调的触发间隔，单位为ms，最小间隔为100ms，如果小于等于0则会关闭回调，建议设置为300ms；【默认值】：0，不开启</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 enableVolumeEvaluation(Int32 intervalMs);

        /// <summary>
        /// 开启/关闭自定义视频采集。
        /// 在自定义视频采集模式下，SDK 不再从摄像头采集图像，只保留编码和发送能力。
        /// 需要在 [startPush](@ref V2TXLivePusher#startPush:) 之前调用，才会生效。
        /// </summary>
        /// <param name="enable">true：开启自定义采集；false：关闭自定义采集。【默认值】：false</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 enableCustomVideoCapture(bool enable);

        /// <summary>
        /// 开启/关闭视频自定义预处理
        /// </summary>
        /// <param name="enable">是否开启自定义视频预处理。【默认值】：false</param>
        /// <param name="pixelFormat">自定义视频预处理回调的视频像素格式 {@link V2TXLivePixelFormat}</param>
        /// <param name="bufferType">自定义视频预处理的视频数据格式 {@link V2TXLiveBufferType}</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 enableCustomVideoProcess(bool enable, V2TXLivePixelFormat pixelFormat, V2TXLiveBufferType bufferType);

        /// <summary>
        /// 设置本地视频自定义渲染回调
        /// 通过该方法，可以获取解码后的每一帧视频画面，进行自定义渲染处理，添加自定义显示效果。
        /// </summary>
        /// <param name="enable">是否开启自定义渲染。【默认值】：false</param>
        /// <param name="pixelFormat">自定义渲染回调的视频像素格式 {@link V2TXLivePixelFormat}。</param>
        /// <param name="bufferType">自定义渲染回调的视频数据格式 {@link V2TXLiveBufferType}。</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 enableCustomVideoRender(bool enable, V2TXLivePixelFormat pixelFormat, V2TXLiveBufferType bufferType);

        /// <summary>
        /// 开启/关闭自定义音频采集
        /// 在自定义音频采集模式下，SDK 不再从麦克风采集声音，只保留编码和发送能力。
        /// 开启/关闭自定义音频采集。
        /// 需要在 [startPush]({@link V2TXLivePusher#startPush(String)}) 前调用才会生效。
        /// </summary>
        /// <param name="enable"> 开启自定义采集; false: 关闭自定义采集。【默认值】: false</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 enableCustomAudioCapture(bool enable);

        /// <summary>
        /// 在自定义视频采集模式下，将采集的视频数据发送到SDK。
        /// 在自定义视频采集模式下，SDK不再采集摄像头数据，仅保留编码和发送功能。
        /// </summary>
        /// <param name="videoFrame">向 SDK 发送的 视频帧数据 {@link V2TXLiveVideoFrame}</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 sendCustomVideoFrame(V2TXLiveVideoFrame^ videoFrame);

        /// <summary>
        /// 在自定义音频采集模式下，将采集的音频数据发送到SDK
        /// 在自定义音频采集模式下，将采集的音频数据发送到SDK，SDK不再采集麦克风数据，仅保留编码和发送功能。
        /// </summary>
        /// <param name="audioFrame">向 SDK 发送的 音频帧数据 {@link V2TXLiveAudioFrame}</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 sendCustomAudioFrame(V2TXLiveAudioFrame^ audioFrame);

        /// <summary>
        /// 发送 SEI 消息
        /// 
        /// 播放端 {@link V2TXLivePlayer} 通过 {@link V2TXLivePlayerObserver} 中的 `onReceiveSeiMessage` 回调来接收该消息。
        /// </summary>
        /// <param name="payloadType">数据类型，支持 5、242。推荐填：242</param>
        /// <param name="data">待发送的数据</param>
        /// <param name="dataSize">数据大小</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 sendSeiMessage(int payloadType, array<Byte>^ data, UInt32 dataSize);

        /// <summary>
        /// 启动屏幕分享
        /// startVirtualCamera，startCamera，startScreenCapture，同一 Pusher 实例下，仅有一个能上行，三者为覆盖关系。例如先调用 startCamera，后调用 startVirtualCamera。此时表现为暂停摄像头推流，开启图片推流
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 startScreenCapture();

        /// <summary>
        /// 停止屏幕采集
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 stopScreenCapture();

        /// <summary>
        /// 打开系统声音采集
        /// 开启后可以采集整个操作系统的播放声音（path 为空）或某一个播放器（path 不为空）的声音，
        /// 并将其混入到当前麦克风采集的声音中一起发送到云端。
        /// </summary>
        /// <param name="path">
        ///    - path 为空，代表采集整个操作系统的声音。(Windows 平台)
        ///    - path 填写 exe 程序（如 QQ音乐）所在的路径，将会启动此程序并只采集此程序的声音。(Windows 平台，采集程序声音仅支持32位 SDK)
        ///    - path 默认为空，其他值未定义。（ Mac 平台）
        /// </param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 startSystemAudioLoopback(String^ path);

        /// <summary>
        /// 关闭系统声音采集
        /// 此接口目前仅适用于 Windows 、 Mac 平台
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 stopSystemAudioLoopback();

        /// <summary>
        /// 枚举可分享的屏幕窗口
        /// </summary>
        /// <param name="thumbSize">指定要获取的窗口缩略图大小，缩略图可用于绘制在窗口选择界面上</param>
        /// <param name="iconSize">指定要获取的窗口图标大小</param>
        /// <returns>窗口列表包括屏幕</returns>
        IV2TXLiveScreenCaptureSourceList^ getScreenCaptureSources(V2TXLiveSize^ thumbSize, V2TXLiveSize^ iconSize);

        /// <summary>
        /// 设置屏幕分享参数，该方法在屏幕分享过程中也可以调用
        /// 如果您期望在屏幕分享的过程中，切换想要分享的窗口，可以再次调用这个函数而不需要重新开启屏幕分享。
        /// 支持如下四种情况：
        /// - 共享整个屏幕：sourceInfoList 中 type 为 Screen 的 source，captureRect 设为 { 0, 0, 0, 0 }
        /// - 共享指定区域：sourceInfoList 中 type 为 Screen 的 source，captureRect 设为非 nullptr，例如 { 100, 100, 300, 300 }
        /// - 共享整个窗口：sourceInfoList 中 type 为 Window 的 source，captureRect 设为 { 0, 0, 0, 0 }
        /// - 共享窗口区域：sourceInfoList 中 type 为 Window 的 source，captureRect 设为非 nullptr，例如 { 100, 100, 300, 300 }
        /// </summary>
        /// <param name="source">指定分享源</param>
        /// <param name="captureRect">指定捕获的区域</param>
        /// <param name="property">指定屏幕分享目标的属性，包括捕获鼠标，高亮捕获窗口等，详情参考 V2TXLiveScreenCaptureProperty 定义</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 setScreenCaptureSource(V2TXLiveScreenCaptureSourceInfo^ source, V2TXLiveRect^ captureRect, V2TXLiveScreenCaptureProperty^ property);

        /// <summary>
        /// 显示仪表盘
        /// </summary>
        /// <param name="isShow"> 是否显示。【默认值】：false</param>
        void showDebugView(bool isShow);
    private:
        delegate void ErrorDelegate(int32_t code, const char* msg, void* extraInfo);
        delegate void WarningDelegate(int32_t code, const char* msg, void* extraInfo);
        delegate void CaptureFirstAudioFrameDelegate();
        delegate void CaptureFirstVideoFrameDelegate();
        delegate void MicrophoneVolumeUpdateDelegate(int32_t volume);
        delegate void PushStatusUpdateDelegate(liteav::V2TXLivePushStatus state, const char* msg, void* extraInfo);
        delegate void StatisticsUpdateDelegate(liteav::V2TXLivePusherStatistics statistics);
        delegate void SnapshotCompleteDelegate(const char* image, int length, int width, int height, liteav::V2TXLivePixelFormat format);
        delegate void RenderVideoFrameDelegate(const liteav::V2TXLiveVideoFrame* videoFrame);
        delegate int  ProcessVideoFrameDelegate(liteav::V2TXLiveVideoFrame* srcFrame, liteav::V2TXLiveVideoFrame* dstFrame);
        delegate void ScreenCaptureStartedDelegate();
        delegate void ScreenCaptureStopedDelegate(int reason);

        void onError(int32_t code, const char* msg, void* extraInfo);
        void onWarning(int32_t code, const char* msg, void* extraInfo);
        void onCaptureFirstAudioFrame();
        void onCaptureFirstVideoFrame();
        void onMicrophoneVolumeUpdate(int32_t volume);
        void onPushStatusUpdate(liteav::V2TXLivePushStatus state, const char* msg, void* extraInfo);
        void onStatisticsUpdate(liteav::V2TXLivePusherStatistics statistics);
        void onSnapshotComplete(const char* image, int length, int width, int height, liteav::V2TXLivePixelFormat format);
        void onRenderVideoFrame(const liteav::V2TXLiveVideoFrame* videoFrame);
        int  onProcessVideoFrame(liteav::V2TXLiveVideoFrame* srcFrame, liteav::V2TXLiveVideoFrame* dstFrame);
        void onScreenCaptureStarted();
        void onScreenCaptureStoped(int reason);
    private:
        V2TXLivePusher(V2TXLiveMode mode);
        ~V2TXLivePusher();

        liteav::V2TXLivePusher* m_pusher;
        ITXAudioEffectManager^ m_audioEffectManager;
        ITXDeviceManager ^ m_deviceManager;
        
        V2TXLivePusherObserver^ m_observer;
        V2TXLivePusherObserverImpl *m_V2TXLivePusherObserverImpl;

        Dictionary<String^, V2TXLiveVideoFrame^>^ m_video_frame_cache;

        ErrorDelegate^ m_ErrorDelegate;
        WarningDelegate^ m_WarningDelegate;
        CaptureFirstAudioFrameDelegate^ m_CaptureFirstAudioFrameDelegate;
        CaptureFirstVideoFrameDelegate^ m_CaptureFirstVideoFrameDelegate;
        MicrophoneVolumeUpdateDelegate^ m_MicrophoneVolumeUpdateDelegate;
        PushStatusUpdateDelegate^ m_PushStatusUpdateDelegate;
        StatisticsUpdateDelegate^ m_StatisticsUpdateDelegate;
        SnapshotCompleteDelegate^ m_SnapshotCompleteDelegate;
        RenderVideoFrameDelegate^ m_RenderVideoFrameDelegate;
        ProcessVideoFrameDelegate^ m_ProcessVideoFrameDelegate;
        ScreenCaptureStartedDelegate^ m_ScreenCaptureStartedDelegate;
        ScreenCaptureStopedDelegate^ m_ScreenCaptureStopedDelegate;
    };
    /// @}
}
