// ITXDeviceManager.h
#pragma once

#include "TXDeviceObserver.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

class TRTCVideoRenderCallbackImpl;

namespace ManageLiteAV {

    /// @defgroup ITXDeviceManager_csharp ITXDeviceManager
    /// 腾讯云视频通话功能的设备管理接口类
    /// @{

    /// <summary>
    /// 设备类型
    /// </summary>
    public enum class TRTCDeviceType {
        /// <summary>
        /// 未知设备
        /// </summary>
        TRTCDeviceTypeUnknow = -1,
        /// <summary>
        /// 麦克风
        /// </summary>
        TRTCDeviceTypeMic = 0,
        /// <summary>
        /// 扬声器
        /// </summary>
        TRTCDeviceTypeSpeaker = 1,
        /// <summary>
        /// 摄像头
        /// </summary>
        TRTCDeviceTypeCamera = 2,
    };

    /// <summary>
    /// 设备列表
    /// <para>【获取SDK设备信息接口 ITRTCDeviceCollection】</para>
    ///
    /// <para>您可以通过 getMicDevicesList()/getSpeakerDevicesList()/getMicDevicesList
    /// 枚举硬件设备列表，列表通过 ITRTCDeviceCollection 返回</para>
    /// </summary>
    public interface class ITRTCDeviceCollection {
        public:
         /// <returns>设备个数</returns>
         UInt32 getCount();

         /// <returns>设备名称，字符编码格式是UTF-8</returns>
         String ^ getDeviceName(UInt32 index);

         /// <returns>设备PID，字符编码格式是UTF-8</returns>
         String ^ getDevicePID(UInt32 index);

         /// <summary>
         /// 遍历完设备后，调用release释放资源。
         /// </summary>
         void release();
     };

    /// <summary>
    /// 设备Item信息
    /// <para>【获取SDK当前使用设备信息 ITRTCDeviceInfo】</para>
    ///
    /// <para>您可以通过 setCurrentCameraDevice()/getCurrentMicDevice()/getCurrentSpeakerDevice
    /// 获取当前使用设备。</para>
    /// </summary>
    public interface class ITRTCDeviceInfo {
        public:
         /// <returns>设备名称，字符编码格式是UTF-8</returns>
         String ^ getDeviceName();

         /// <returns>设备PID，字符编码格式是UTF-8</returns>
         String ^ getDevicePID();

         /// <summary>
         /// 获取完设备信息后，调用release释放资源。
         /// </summary>
         void release();
     };

    /// <summary>
    /// 设备类型（仅适用于桌面平台）
    /// </summary>
    public enum class TXMediaDeviceType {
        /// <summary>
        /// 未知设备
        /// </summary>
        TXMediaDeviceTypeUnknown = -1,
        /// <summary>
        /// 麦克风
        /// </summary>
        TXMediaDeviceTypeMic = 0,
        /// <summary>
        /// 扬声器
        /// </summary>
        TXMediaDeviceTypeSpeaker = 1,
        /// <summary>
        /// 摄像头
        /// </summary>
        TXMediaDeviceTypeCamera = 2,
    };

    /// <summary>
    /// 设备操作
    /// </summary>
    public enum class TXMediaDeviceState {
        /// <summary>
        /// 设备已被插入
        /// </summary>
        TXMediaDeviceStateAdd = 0,
        /// <summary>
        /// 设备已被移除
        /// </summary>
        TXMediaDeviceTypeMic = 1,
        /// <summary>
        /// 设备已启用
        /// </summary>
        TXMediaDeviceTypeSpeaker = 2,
    };

    public interface class ITXDeviceObserver {
        /// <summary>
        /// 本地设备的通断状态发生变化（仅适用于桌面系统）
        /// <para>当本地设备（包括摄像头、麦克风以及扬声器）被插入或者拔出时，SDK 便会抛出此事件回调</para>
        /// </summary>
        /// <param name="deviceId"></param>
        /// <param name="type"></param>
        /// <param name="state"></param>
        void onDeviceChanged(String^ deviceId, TXMediaDeviceType type, TXMediaDeviceState state);
    };


    /// <summary>
    /// 摄像头采集偏好
    /// <para>该枚举类型用于摄像头采集参数设置。</para>
    /// </summary>
    public enum class TXCameraCaptureMode {

        /// <summary>
        /// 自动调整采集参数。
        /// SDK
        /// 根据实际的采集设备性能及网络情况，选择合适的摄像头输出参数，在设备性能及视频预览质量之间，维持平衡。
        /// </summary>
        TXCameraResolutionStrategyAuto = 0,

        /// <summary>
        /// 优先保证设备性能。
        /// SDK 根据用户设置编码器的分辨率和帧率，选择最接近的摄像头输出参数，从而保证设备性能。
        /// </summary>
        TXCameraResolutionStrategyPerformance = 1,

        /// <summary>
        /// 优先保证视频预览质量。
        /// SDK选择较高的摄像头输出参数，从而提高预览视频的质量。在这种情况下，会消耗更多的 CPU
        /// 及内存做视频前处理。
        /// </summary>
        TXCameraResolutionStrategyHighQuality = 2,

        /// <summary>
        /// 允许用户设置本地摄像头采集的视频宽高。
        /// </summary>
        TXCameraCaptureManual = 3,

    };

    /// <summary>
    /// 摄像头采集参数
    /// <para> 该设置能决定本地预览图像画质。</para>
    /// </summary>
    public ref struct TXCameraCaptureParam {

        /// <summary>
        ///【字段含义】摄像头采集偏好
        /// </summary>
        TXCameraCaptureMode mode;

        /// <summary>
        ///【字段含义】采集图像长度
        /// </summary>
        int width;

        /// <summary>
        ///【字段含义】采集图像宽度
        /// </summary>
        int height;

        TXCameraCaptureParam()
            : mode(TXCameraCaptureMode::TXCameraResolutionStrategyAuto)
            , width(640)
            , height(360)
        {
        }
    };


    public ref class ITXDeviceManager {
    protected:
        ~ITXDeviceManager();

    public:
        ITXDeviceManager();

        /// <summary>
        /// 获取设备列表
        /// </summary>
        /// <param name="type">设备类型，指定需要获取哪种设备的列表。详见TRTCDeviceType定义。</param>
        /// <remarks>
        /// - SDK 会维护 ITXDeviceCollection 对象的生命周期，使用完毕后请调用 release 方法释放资源。
        /// - type 只支持 TRTCDeviceTypeMic、TRTCDeviceTypeSpeaker、TRTCDeviceTypeCamera
        /// </remarks>
        ITRTCDeviceCollection^ getDevicesList(TRTCDeviceType type);

        /// <summary>
        /// 指定当前设备
        /// </summary>
        /// <param name="type">设备类型，根据设备类型指定当前设备。详见 TRTCDeviceType 定义。</param>
        /// <param name="deviceId">从 getDevicesList 中得到的设备 ID</param>
        /// <returns>0：操作成功 负数：失败</returns>
        /// <remarks>
        /// type 只支持 TRTCDeviceTypeMic、TRTCDeviceTypeSpeaker、TRTCDeviceTypeCamera
        /// </remarks>
        int setCurrentDevice(TRTCDeviceType type, String^ deviceId);

        /// <summary>
        /// 获取当前使用的设备
        /// </summary>
        /// <param name="type">设备类型，根据设备类型获取当前设备信息。详见 TRTCDeviceType 定义</param>
        /// <returns>ITRTCDeviceInfo 设备信息，能获取设备 ID 和设备名称</returns>
        ITRTCDeviceInfo ^ getCurrentDevice(TRTCDeviceType type);

        /// <summary>
        /// 设置当前设备的音量
        /// </summary>
        /// <param name="type">设备类型，根据设备类型获取当前设备音量。详见TRTCDeviceType定义。</param>
        /// <param name="volume">音量大小</param>
        /// <returns>0：操作成功 负数：失败</returns>
        /// <remarks>type 只支持 TRTCDeviceTypeMic、TRTCDeviceTypeSpeaker</remarks>
        int setCurrentDeviceVolume(TRTCDeviceType type, UInt32 volume);

        /// <summary>
        /// 获取当前设备的音量
        /// </summary>
        /// <param name="type">设备类型，根据设备类型获取当前设备音量。详见TRTCDeviceType定义。</param>
        /// <remarks>type 只支持 TRTCDeviceTypeMic、TRTCDeviceTypeSpeaker</remarks>
        UInt32 getCurrentDeviceVolume(TRTCDeviceType type);

        /// <summary>
        /// 设置当前设备是否静音
        /// </summary>
        /// <param name="type">设备类型，根据设备类型设置当前设备状态。详见TRTCDeviceType定义。</param>
        /// <param name="mute">是否静音/禁画</param>
        /// <returns>0：操作成功 负数：失败</returns>
        /// <remarks>type 只支持 TRTCDeviceTypeMic、TRTCDeviceTypeSpeaker</remarks>
        int setCurrentDeviceMute(TRTCDeviceType type, bool mute);

        /// <summary>
        /// 查询当前设备是否静音
        /// </summary>
        /// <param name="type">设备类型，根据设备类型获取当前设备状态。详见TRTCDeviceType定义。</param>
        /// <returns>true : 当前设备已静音；false : 当前设备未静音</returns>
        /// <remarks>type 只支持 TRTCDeviceTypeMic、TRTCDeviceTypeSpeaker</remarks>
        bool getCurrentDeviceMute(TRTCDeviceType type);

        /// <summary>
        /// 开始摄像头测试
        /// </summary>
        /// <param name="view">预览控件所在的父控件</param>
        /// <returns>0：操作成功 负数：失败</returns>
        /// <remarks>在测试过程中可以使用 setCurrentCameraDevice 接口切换摄像头。</remarks>
        int startCameraDeviceTest(IntPtr view);

        /// <summary>
        /// 结束摄像头测试
        /// </summary>
        /// <returns>0：操作成功 负数：失败</returns>
        int stopCameraDeviceTest();

        /// <summary>
        /// 开始麦克风测试
        /// </summary>
        /// <param name="interval">音量回调间隔</param>
        /// <returns>0：操作成功 负数：失败</returns>
        int startMicDeviceTest(UInt32 interval);

        /// <summary>
        /// 结束麦克风测试
        /// </summary>
        /// <returns>0：操作成功 负数：失败</returns>
        int stopMicDeviceTest();

        /// <summary>
        /// 开始扬声器测试
        /// <para>该方法播放指定的音频文件测试播放设备是否能正常工作。如果能听到声音，说明播放设备能正常工作。</para>
        /// </summary>
        /// <param name="filePath">声音文件的路径</param>
        /// <returns>0：操作成功 负数：失败</returns>
        int startSpeakerDeviceTest(String^ filePath);

        /// <summary>
        /// 停止扬声器测试
        /// </summary>
        /// <returns>0：操作成功 负数：失败</returns>
        int stopSpeakerDeviceTest();

        /// <summary>
        /// 设置 onDeviceChanged 事件回调
        /// </summary>
        /// <param name="observer">回调</param>
        void setDeviceObserver(ITXDeviceObserver^ observer);

        /// <summary>
        /// 设置摄像头采集偏好
        /// </summary>
        /// <param name="params">采集参数，width height mode 详见TXCameraCaptureParam定义</param>
        void setCameraCapturerParam(TXCameraCaptureParam^ params);

        /// <summary>
        /// 设置 Windows 系统音量合成器中当前进程的音量
        /// </summary>
        /// <returns>0：操作成功</returns>
        /// <param name="volume">播放音量</param>
        int setApplicationPlayVolume(int volume);

        /// <summary>
        /// 获取 Windows 系统音量合成器中当前进程的音量
        /// </summary>
        /// <returns>返回音量</returns>
        int getApplicationPlayVolume();

        /// <summary>
        /// 设置 Windows 系统音量合成器中当前进程的静音状态
        /// </summary>
        /// <returns>0：操作成功</returns>
        /// <param name="bMute">是否静音</param>
        int setApplicationMuteState(bool bMute);

        /// <summary>
        /// 获取 Windows 系统音量合成器中当前进程的静音状态
        /// </summary>
        /// <returns>true：操作成功</returns>
        bool getApplicationMuteState();
    private:

    private:
        liteav::ITXDeviceManager* device_manager_;

        ITXDeviceObserver^ m_callback;

        TXDeviceObserverImpl* m_ObserverImpl;
        
        Object^ m_cb_locker;

        // ITXDeviceObserver
        delegate void DeviceChangedDelegate(const char* deviceId, liteav::TXMediaDeviceType type, liteav::TXMediaDeviceState state);
        DeviceChangedDelegate^ m_deviceChangeDelegate;
        void onDeviceChanged(const char* deviceId, liteav::TXMediaDeviceType type, liteav::TXMediaDeviceState state);
    };
    /// @}
}