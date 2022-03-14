#pragma once

// C++ Header
#include "include/TRTC/ITRTCCloud.h"
#include "include/Vod/ITXVodPlayer.h"

// C++/CLI Header
#include "cli/TRTC/TRTCCloudDef.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

class ITXVodPlayerDataCallbackImpl;
class ITXVodPlayerEventCallbackImpl;


namespace ManageLiteAV
{

    public interface class  ITXVodPlayerDataCallback
    {
    public:
        /// <summary>
        /// Vod视频帧回调
        /// </summary>
        /// <param name="frame"></param>
        /// <returns>视频帧数据</returns>
        Int32 onVodVideoFrame(TRTCVideoFrame^% frame);

        /// <summary>
        /// Vod音频帧回调
        /// </summary>
        /// <param name="frame"></param>
        /// <returns>音频帧数据</returns>
        Int32 onVodAudioFrame(TRTCAudioFrame^% frame);
    };

    public interface class ITXVodPlayerEventCallback
    {
    public:
        /// <summary>
        /// 当多媒体文件播放开始时，SDK会通过此回调通知
        /// </summary>
        /// <param name="msLength">多媒体文件总长度，单位毫秒</param>
        void onVodPlayerStarted(UInt64 msLength);

        /// <summary>
        /// 当多媒体文件播放进度改变时，SDK会通过此回调通知
        /// </summary>
        /// <param name="msPos">多媒体文件播放进度，单位毫秒</param>
        void onVodPlayerProgress(UInt64 msPos);

        /// <summary>
        /// 当多媒体文件播放暂停时，SDK会通过此回调通知
        /// </summary>
        void onVodPlayerPaused();

        /// <summary>
        /// 当多媒体文件播放恢复时，SDK会通过此回调通知
        /// </summary>
        void onVodPlayerResumed();

        /// <summary>
        /// 当多媒体文件播放停止时，SDK会通过此回调通知
        /// </summary>
        /// <param name="reason">停止原因，0表示用户主动停止，1表示文件播放完，2表示视频断流</param>
        void onVodPlayerStoped(Int32 reason);

        /// <summary>
        /// 当多媒体文件播放出错时，SDK会通过此回调通知
        /// </summary>
        /// <param name="error">错误码</param>
        void onVodPlayerError(Int32 error);
    };

    /// @defgroup VodPlayerController_csharp VodPlayerController
    /// 腾讯云Vod播放器的主要接口类
    /// @{
    /// <summary>
    /// Module:   VodPlayerController @ TXLiteAVSDK
    ///
    /// <para>Function: 腾讯云Vod播放器的主要接口类</para>
    /// </summary>
    /// <example>
    /// 创建/使用/销毁 VodPlayerController 对象的示例代码：
    /// <code>
    /// VodPlayerController playerController = VodPlayerController.getVodPlayerControllerInstance();
    /// </code>
    ///	程序退出或者后续程序流程不需要使用 VodPlayerController 对象时，释放单例对象 VodPlayerController
    /// <code>
    /// VodPlayerController.destroyVodPlayerControllerInstance();
    /// playerController = null;
    /// </code>
    /// </example>
    public ref class ITXVodPlayer
    {
    public:
        /// <summary>
        /// 创建一个播放器
        /// </summary>
        /// <param name="mediaFile">音视频文件名</param>
        /// <param name="repeat">是否循环</param>
        static ITXVodPlayer^ createTXVodPlayer(String^ mediaFile, bool repeat);

        /// <summary>
        /// 销毁一个播放器
        /// </summary>
        static void destroyTXVodPlayer(ITXVodPlayer^);

        /// <summary>
        /// 绑定 TRTC 
        /// </summary>
        /// <para>用于辅流推送，绑定后音频播放由TRTC接管</para>
        void attachTRTC();

        /// <summary>
        /// 解绑TRTC
        /// </summary>
        void detachTRTC();

        /// <summary>
        /// 开始向TRTC发布辅路视频流
        /// </summary>
        void publishVideo();

        /// <summary>
        /// 开始向TRTC发布辅路音频流
        /// </summary>
        void publishAudio();

        /// <summary>
        /// 结束向TRTC发布辅路视频流
        /// </summary>
        void unpublishVideo();

        /// <summary>
        /// 结束向TRTC发布辅路音频流
        /// </summary>
        void unpublishAudio();

        /// <summary>
        /// 设置多媒体事件回调
        /// </summary>
        /// <param name="callback">要使用的多媒体文件播放回调接收实例</param>
        void setEventCallback(ITXVodPlayerEventCallback^ callback);

        /// <summary>
        /// 设置多媒体文件播放回调
        /// </summary>
        /// <param name="callback">要使用的多媒体文件播放回调接收实例</param>
        void setDataCallback(ITXVodPlayerDataCallback^ callback);

        /// <summary>
        /// 开始多媒体文件播放
        ///<para>支持的视频格式包括：mp4、avi、mkv、wmv、m4v</para>
        ///<para>支持的音频格式包括：mp3、wav、wma、aac</para>
        /// </summary>
        void start();

        /// <summary>
        /// 暂停多媒体文件播放
        /// </summary>
        void pause();

        /// <summary>
        /// 恢复多媒体文件播放
        /// </summary>
        void resume();

        /// <summary>
        /// 停止多媒体文件播放
        /// </summary>
        void stop();

        /// <summary>
        /// 设置多媒体文件播放进度
        /// </summary>
        /// <param name="msPos">播放进度（单位毫秒）</param>
        void seek(UInt64 msPos);

        /// <summary>
        /// 设置多媒体文件播放速度
        /// </summary>
        /// <param name="rate">播放速度（0.5-2.0）</param>
        void setRate(float rate);

        /// <summary>
        /// 设置播放渲染句柄
        /// </summary>
        /// <param name="hwnd">窗口句柄</param>
        void setView(IntPtr hwnd);

        /// <summary>
        /// 获取多媒体文件总时长
        /// </summary>
        Int32 getDuration();

        /// <summary>
        /// 获取视频宽度
        /// </summary>
        Int32 getWidth();

        /// <summary>
        /// 获取视频高度
        /// </summary>
        Int32 getHeight();

        /// <summary>
        /// 设置多媒体音量
        /// </summary>
        /// <param name="volume">音量大小，100为原始音量，范围是：[0 ~ 150]，默认值为100</param>
        void setVolume(Int32 volume);

        /// <summary>
        /// 静音/取消静音
        /// </summary>
        /// <param name="mute">true：静音；false：取消静音</param>
        void mute(bool mute);

        /// <summary>
        /// 设置本地图像的顺时针旋转角度
        /// <para>param rotation 支持 TRTCVideoRotation90 、 TRTCVideoRotation180 以及 TRTCVideoRotation270</para>
        /// <para>旋转角度，默认值：TRTCVideoRotation0</para>
        /// </summary>
        /// <param name="rotation">视频画面旋转方向</param>
        void setRenderRotation(TRTCVideoRotation rotation);

        /// <summary>
        /// 设置本地图像的填充模式
        /// <para>填充（画面可能会被拉伸裁剪）或适应（画面可能会有黑边），默认值：TRTCVideoFillMode_Fit</para>
        /// </summary>
        /// <param name="rotation">视频画面填充模式</param>
        void setFillMode(TRTCVideoFillMode mode);

        /// <summary>
        /// 设置本地图像的镜像模式
        /// </summary>
        /// <param name="mirror">镜像模式，默认值：false（非镜像模式）</param>
        void setMirror(bool mirror);

    private:
        delegate int  VodVideoFrameDelegate(::TRTCVideoFrame& frame);
        delegate int  VodAudioFrameDelegate(::TRTCAudioFrame& frame);
        delegate void VodPlayerStartedDelegate(uint64_t msLength);
        delegate void VodPlayerProgressDelegate(uint64_t msPos);
        delegate void VodPlayerPausedDelegate();
        delegate void VodPlayerResumedDelegate();
        delegate void VodPlayerStopedDelegate(int reason);
        delegate void VodPlayerErrorDelegate(int error);


        int onVodVideoFrame(::TRTCVideoFrame& frame);
        int onVodAudioFrame(::TRTCAudioFrame& frame);
        void onVodPlayerStarted(uint64_t msLength);
        void onVodPlayerProgress(uint64_t msPos);
        void onVodPlayerPaused();
        void onVodPlayerResumed();
        void onVodPlayerStoped(int reason);
        void onVodPlayerError(int error);
    private:
        ITXVodPlayer(String^ mediaFile, bool repeat);
        ~ITXVodPlayer();

        ::ITXVodPlayer* m_player;
        
        Object ^ m_cb_locker_event;
        Object ^ m_cb_locker_data;

        ITXVodPlayerDataCallbackImpl* m_vodPlayerDataCallbackImpl;
        ITXVodPlayerEventCallbackImpl* m_vodPlayerEventCallbackImpl;

        ITXVodPlayerEventCallback^ m_vodPlayerEventCallback;
        ITXVodPlayerDataCallback^ m_vodPlayerDataCallback;

        //  VodPlayerDataCallback
        VodVideoFrameDelegate^ m_vodVideoFrameDeleate;
        VodAudioFrameDelegate^ m_vodAudioFrameDeleate;
        //  VodPlayerEventCallback
        VodPlayerStartedDelegate^ m_vodPlayerStartedDelegate;
        VodPlayerProgressDelegate^ m_vodPlayerProcessDelegate;
        VodPlayerPausedDelegate^ m_vodPlayerPausedDelegate;
        VodPlayerResumedDelegate^ m_vodPlayerResumedDelegate;
        VodPlayerStopedDelegate^ m_vodPlayerStopedDelegate;
        VodPlayerErrorDelegate^ m_vodPlayerErrorDelegate;
    };
}
