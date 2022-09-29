#pragma once

// C++
#include "include/Live2/V2TXLivePlayer.hpp"

// C++/CLI Header
#include "cli/Live/V2TXLivePlayerObserver.h"
#include "cli/Live/V2TXLiveDef.h"

using namespace System;

class V2TXLivePlayerObserverImpl;

namespace ManageLiteAV
{
    /// @defgroup V2TXLivePlayer_csharp V2TXLivePlayer
    /// 腾讯云直播播放器
    /// @{
    /// <summary>
    /// Module:   V2TXLivePlayer @ TXLiteAVSDK
    /// <para>主要负责从指定的直播流地址拉取音视频数据，并进行解码和本地渲染播放。</para>
    /// 播放器包含如下能力:
    /// - 支持RTMP, HTTP-FLV，TRTC；
    /// - 延时调节，可以设置播放器缓存自动调整的最小和最大时间；
    /// - 自定义的视频数据处理，让您可以根据项目需要处理直播流中的视频数据后，进行渲染以及播放。  
    /// </summary>    
    public ref class V2TXLivePlayer
    {
    public:
        /// <summary>
        /// 用于动态加载 dll 时，获取 V2TXLivePlayer 对象指针
        /// </summary>
        /// <returns>返回 V2TXLivePlayer 对象的指针，注意：请调用 releaseV2TXLivePlayer 析构</returns>
        /// <para>note 本接口适用于Windows、Mac、iOS平台</para> 
        static V2TXLivePlayer^ createV2TXLivePlayer();

        /// <summary>
        /// 释放 V2TXLivePlayer 单例对象。
        /// </summary>
        static void releaseV2TXLivePlayer(V2TXLivePlayer^ player);

        /// <summary>
        /// 设置播放器回调。
        /// 通过设置回调，可以监听 V2TXLivePlayer 播放器的一些回调事件，
        /// 包括播放器状态、播放音量回调、音视频首帧回调、统计数据、警告和错误信息等。
        /// </summary>
        /// <param name="observer">播放器的回调目标对象</param>
        void setObserver(V2TXLivePlayerObserver^ observer);

        /// <summary>
        /// 设置播放器的视频渲染 View。 该控件负责显示视频内容。
        /// </summary>
        /// <param name="view">播放器渲染 View</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 setRenderView(IntPtr view);

        /// <summary>
        /// 设置播放器画面的旋转角度。
        /// </summary>
        /// <param name="rotation">rotation 旋转角度 {@link V2TXLiveRotation}</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 setRenderRotation(V2TXLiveRotation rotation);

        /// <summary>
        /// 设置画面的填充模式
        /// </summary>
        /// <param name="mode">mode 画面填充模式 {@link V2TXLiveFillMode}</param>
        /// <returns>返回值 {@link V2TXLiveCode} </returns>
        Int32 setRenderFillMode(V2TXLiveFillMode mode);

        /// <summary>
        /// 开始播放音视频流
        /// </summary>
        /// <param name="url">url 音视频流的播放地址，支持 RTMP, HTTP-FLV, TRTC</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 startPlay(String^ url);

        /// <summary>
        /// 停止播放音视频流
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 stopPlay();

        /// <summary>
        /// 播放器是否正在播放中
        /// </summary>
        /// <returns>是否正在播放(1: 正在播放中,0: 已经停止播放)</returns>
        Int32 isPlaying();

        /// <summary>
        /// 暂停播放器的音频流
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 pauseAudio();

        /// <summary>
        /// 恢复播放器的音频流
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 resumeAudio();

        /// <summary>
        /// 暂停播放器的视频流
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 pauseVideo();

        /// <summary>
        /// 恢复播放器的视频流
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 resumeVideo();

        /// <summary>
        /// 设置播放器音量
        /// </summary>
        /// <param name="volume">音量大小，取值范围0 - 100。【默认值】: 100</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 setPlayoutVolume(Int32 volume);

        /// <summary>
        /// 设置播放器缓存自动调整的最小和最大时间 ( 单位：秒 )。
        /// </summary>
        /// <param name="minTime">缓存自动调整的最小时间，取值需要大于0。【默认值】：1</param>
        /// <param name="maxTime">缓存自动调整的最大时间，取值需要大于0。【默认值】：5</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 setCacheParams(float minTime, float maxTime);

        /// <summary>
        /// 直播流无缝切换，支持 FLV 和 LEB。
        /// </summary>
        /// <param name="newUrl">newUrl 新的拉流地址。</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 switchStream(String^ newUrl);

        /// <summary>
        /// 启用播放音量大小提示
        /// </summary>
        /// <param name="intervalMs">开启后可以在 {@link V2TXLivePlayerObserver} 回调中获取到 SDK 对音量大小值的评估。</param>
        /// <returns>intervalMs 决定了 onPlayoutVolumeUpdate 回调的触发间隔，单位为ms，最小间隔为100ms，如果小于等于0则会关闭回调，建议设置为300ms；【默认值】：0，不开启</returns>
        Int32 enableVolumeEvaluation(Int32 intervalMs);

        /// <summary>
        /// 截取播放过程中的视频画面
        /// </summary>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 snapshot();

        /// <summary>
        /// 开启/关闭对视频帧的监听回调。
        /// SDK 在您开启次此开关后将不再渲染视频画面，您可以通过 V2TXLivePlayerObserver 获得视频帧，并执行自定义的渲染逻辑。 
        /// </summary>
        /// <param name="enable">是否开启自定义渲染。【默认值】：false</param>
        /// <param name="pixelFormat">自定义渲染回调的视频像素格式 {@link V2TXLivePixelFormat}。</param>
        /// <param name="bufferType">自定义渲染回调的视频数据格式 {@link V2TXLiveBufferType}。</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 enableObserveVideoFrame(bool enable, V2TXLivePixelFormat pixelFormat, V2TXLiveBufferType bufferType);

        /// <summary>
        /// 开启接收 SEI 消息
        /// </summary>
        /// <param name="enable">true: 开启接收 SEI 消息; false: 关闭接收 SEI 消息。【默认值】: false</param>
        /// <param name="payloadType">指定接收 SEI 消息的 payloadType，支持 5、242，请与发送端的 payloadType 保持一致。</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 enableReceiveSeiMessage(bool enable, int payloadType);

        /// <summary>
        /// 是否显示播放器状态信息的调试浮层
        /// </summary>
        /// <param name="isShow">是否显示。【默认值】：false</param>
        void showDebugView(bool isShow);

        /// <summary>
        /// 调用 V2TXLivePlayer 的高级 API 接口。
        /// @note  该接口用于调用一些高级功能。
        /// </summary>
        /// <param name="key">高级 API 对应的 key。</param>
        /// <param name="value">调用 key 所对应的高级 API 时，需要的参数</param>
        /// <returns>返回值 {@link V2TXLiveCode}</returns>
        Int32 setProperty(String^ key, String^ value);
    private:
        delegate void ErrorDelegate(liteav::V2TXLivePlayer*, int32_t, const char*, void*);
        delegate void WarningDelegate(liteav::V2TXLivePlayer*, int32_t, const char*, void*);
        delegate void VideoResolutionChangedDelegate(liteav::V2TXLivePlayer*, int, int);
        delegate void ConnectedDelegate(liteav::V2TXLivePlayer*, void*);
        delegate void VideoPlayingDelegate(liteav::V2TXLivePlayer*, bool, void*);
        delegate void AudioPlayingDelegate(liteav::V2TXLivePlayer*, bool, void*);
        delegate void VideoLoadingDelegate(liteav::V2TXLivePlayer*, void*);
        delegate void AudioLoadingDelegate(liteav::V2TXLivePlayer*, void*);
        delegate void PlayoutVolumeUpdateDelegate(liteav::V2TXLivePlayer*, int32_t);
        delegate void StatisticsUpdateDelegate(liteav::V2TXLivePlayer*, liteav::V2TXLivePlayerStatistics);
        delegate void SnapshotCompleteDelegate(liteav::V2TXLivePlayer*, const char*, int, int, int, liteav::V2TXLivePixelFormat);
        delegate void RenderVideoFrameDelegate(liteav::V2TXLivePlayer*, const liteav::V2TXLiveVideoFrame*);
        delegate void ReceiveSeiMessageDelegate(liteav::V2TXLivePlayer*, int, const uint8_t*, uint32_t);

        void onError(liteav::V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo);
        void onWarning(liteav::V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo);
        void onVideoResolutionChanged(liteav::V2TXLivePlayer* player, int width, int height);
        void onConnected(liteav::V2TXLivePlayer* player, void* extraInfo);
        void onVideoPlaying(liteav::V2TXLivePlayer* player, bool firstPlay, void* extraInfo);
        void onAudioPlaying(liteav::V2TXLivePlayer* player, bool firstPlay, void* extraInfo);
        void onVideoLoading(liteav::V2TXLivePlayer* player, void* extraInfo);
        void onAudioLoading(liteav::V2TXLivePlayer* player, void* extraInfo);
        void onPlayoutVolumeUpdate(liteav::V2TXLivePlayer* player, int32_t volume);
        void onStatisticsUpdate(liteav::V2TXLivePlayer* player, liteav::V2TXLivePlayerStatistics statistics);
        void onSnapshotComplete(liteav::V2TXLivePlayer* player, const char* image, int length, int width, int height, liteav::V2TXLivePixelFormat format);
        void onRenderVideoFrame(liteav::V2TXLivePlayer* player, const liteav::V2TXLiveVideoFrame* videoFrame);
        void onReceiveSeiMessage(liteav::V2TXLivePlayer* player, int payloadType, const uint8_t* data, uint32_t dataSize);
    private:
        V2TXLivePlayer();
        ~V2TXLivePlayer();

        liteav::V2TXLivePlayer* m_player;
        V2TXLivePlayerObserver^ m_V2TXLivePlayerObserver;
        V2TXLivePlayerObserverImpl* m_V2TXLivePlayerObserverImpl;

        ErrorDelegate^ m_ErrorDelegate;
        WarningDelegate^ m_WarningDelegate;
        VideoResolutionChangedDelegate^ m_VideoResolutionChangedDelegate;
        ConnectedDelegate^ m_ConnectedDelegate;
        VideoPlayingDelegate^ m_VideoPlayingDelegate;
        AudioPlayingDelegate^ m_AudioPlayingDelegate;
        VideoLoadingDelegate^ m_VideoLoadingDelegate;
        AudioLoadingDelegate^ m_AudioLoadingDelegate;
        PlayoutVolumeUpdateDelegate^ m_PlayoutVolumeUpdateDelegate;
        StatisticsUpdateDelegate^ m_StatisticsUpdateDelegate;
        SnapshotCompleteDelegate^ m_SnapshotCompleteDelegate;
        RenderVideoFrameDelegate^ m_RenderVideoFrameDelegate;
        ReceiveSeiMessageDelegate^ m_ReceiveSeiMessageDelegate;

        ManageLiteAV::V2TXLiveVideoFrame^ m_Videoframe;
    };
    /// @}
}
