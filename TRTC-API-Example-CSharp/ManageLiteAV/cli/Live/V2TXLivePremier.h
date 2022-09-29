/// @defgroup V2TXLivePremier V2TXLivePremier
///
/// @{

#pragma once
#include "include/Live2/V2TXLivePremier.hpp"
#include "cli/Live/V2TXLiveDef.h"

using namespace System;
class V2TXLivePremierObserverImpl;
namespace ManageLiteAV {
    public interface class V2TXLivePremierObserver {
    public:
        /// <summary>
        /// 自定义 Log 输出回调接口
        /// </summary>
        /// <param name="level">日志级别</param>
        /// <param name="log">日志</param>
        void onLog(V2TXLiveLogLevel level, String^ log);

        /// <summary>
        /// 本地麦克风采集到的音频数据回调
        /// - 请不要在此回调函数中做任何耗时操作，建议直接拷贝到另一线程进行处理，否则会导致各种声音问题
        /// - 此接口回调出的音频数据支持修改
        /// - 此接口回调出的音频时间帧长固定为0.02s
        ///         由时间帧长转化为字节帧长的公式为【采样率 × 时间帧长 × 声道数 × 采样点位宽】。
        ///         以SDK默认的音频录制格式48000采样率、单声道、16采样点位宽为例，字节帧长为【48000 × 0.02s × 1 × 16bit
        /// - 此接口回调出的音频数据**不包含**背景音、音效、混响等前处理效果，延迟极低。
        /// - 需要您调用 [enableAudioCaptureObserver](@ref V2TXLivePremier#enableAudioCaptureObserver) 开启回调开关
        /// </summary>
        /// <param name="frame">音频数据</param>
        void onCaptureAudioFrame(V2TXLiveAudioFrame^ frame);
    };
    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      V2TXLive 高级接口
    //
    /////////////////////////////////////////////////////////////////////////////////    
    public ref class V2TXLivePremier {
    public:
        /// <summary>
        /// 创建 V2TXLivePremier 实例
        /// </summary>
        /// <returns>V2TXLivePremier 对象</returns>
        static V2TXLivePremier^ getV2TXLivePremierShareInstance();

        /// <summary>
        /// 销毁 V2TXLivePremier 实例
        /// </summary>
        static void destroyV2TXLivePremierShareInstance();

        /// <summary>
        /// 获取 SDK 版本号
        /// </summary>
        /// <returns></returns>
        String^% getSDKVersionStr();

        /// <summary>
        /// 设置 V2TXLivePremier 回调接口
        /// </summary>
        /// <param name="observer"></param>
        void setObserver(V2TXLivePremierObserver^ observer);

        /// <summary>
        /// 设置 Log 的配置信息
        /// </summary>
        /// <param name="config"></param>
        /// <returns></returns>
        Int32 setLogConfig(V2TXLiveLogConfig^ config);

        /// <summary>
        /// 设置 SDK 接入环境
        /// </summary>
        /// <param name="env">目前支持 “default” 和 “GDPR” 两个参数</param>
        /// <returns></returns>
        Int32 setEnvironment(String^ env);

        /// <summary>
        /// 设置 SDK sock5 代理配置
        /// </summary>
        /// <param name="host">代理服务器的地址</param>
        /// <param name="port">代理服务器的端口</param>
        /// <param name="username">代理服务器的验证的用户名</param>
        /// <param name="password">代理服务器的验证的密码</param>
        /// <returns></returns>
        Int32 setSocks5Proxy(String^ host, UInt16 port, String^ username, String^ password);

        /// <summary>
        /// 开启/关闭对音频采集数据的监听回调（可读写）
        /// 需要在 {@link V2TXLivePusher#startPush} 之前调用，才会生效。
        /// </summary>
        /// <param name="enable">是否开启。 【默认值】：false</param>
        /// <param name="format">设置回调出的 AudioFrame 的格式</param>
        /// <returns></returns>
        Int32 enableAudioCaptureObserver(bool enable, V2TXLiveAudioFrameObserverFormat^ format);
    private:
        V2TXLivePremier();
        ~V2TXLivePremier();
        void onError(liteav::V2TXLiveLogLevel level, const char* log);
        void onCaptureAudioFrame(liteav::V2TXLiveAudioFrame* frame);
    private:

        static V2TXLivePremier^ sInstance;
        static Object^ slocker = gcnew Object();
        V2TXLivePremierObserver^ m_observer;
        delegate void LogDelegate(liteav::V2TXLiveLogLevel, const char*);
        delegate void CaptureAudioFrameDelegate(liteav::V2TXLiveAudioFrame*);

        LogDelegate^ m_LogDelegate;
        CaptureAudioFrameDelegate^ m_CaptureAudioFrameDelegate;
        V2TXLivePremierObserverImpl *m_V2TXLivePremierObserverImpl;
        liteav::V2TXLivePremier* m_LivePremier;
        ManageLiteAV::V2TXLiveAudioFrame^ m_frame;
    };
}  // namespace ManageLiteAV

/// @}