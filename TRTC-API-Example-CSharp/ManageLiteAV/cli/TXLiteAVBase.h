#pragma once

using namespace System;

namespace ManageLiteAV {
    public interface class NetworkTimeCallback
    {
        void onUpdateNetworkTime(int errCode, String^ errMsg);
    };

    public ref class TXLiteAVBase
    {
    public:
        /// <summary>
        /// 获取 ITRTCCloud 单例对象。
        /// </summary>
        /// <returns>返回 ITRTCCloud 单例对象</returns>
        static TXLiteAVBase^ getInstance();

        /// <summary>
        /// 获取 SDK 版本号
        /// </summary>
        /// <returns>返回 UTF-8 编码的版本号</returns>
        static String^ getLiteAvSDKVersion();

        /// <summary>
        /// 设置 liteav SDK 接入的环境。
        /// </summary>
        /// <param name="env_config">需要接入的环境，SDK 默认接入的环境是：默认正式环境</param>		
        /// <returns>0：成功；其他：错误</returns>
        static int setGlobalEnv(String^ env_config);

        /// <summary>
        /// 启动 NTP 校时服务
        /// </summary>
        static int updateNetworkTime();

        /// <summary>
        /// 获取 NTP 时间戳（毫秒），请在收到 onUpdateNetworkTime 回调后使用
        /// </summary>
        /// <returns>NTP 时间戳（毫秒），若返回 0：未启动 NTP 校时或校时失败，请重启校时</returns>
        static Int64 getNetworkTimestamp();

        /// <summary>
        /// 设置 NTP 校时事件回调。
        /// </summary>
        /// <param name="callback">NetworkTimeCallback回调指针。</param>
        void setUpdateNetworkTimeCallback(NetworkTimeCallback^ callback);
    private:
        TXLiteAVBase();
        ~TXLiteAVBase();

        static TXLiteAVBase^ sInstance;
        static Object^ slocker = gcnew Object();

        delegate void UpdateNetworkTimeDelegate(int errCode, const char* errMsg);
        UpdateNetworkTimeDelegate ^ m_updateNetworkTimeDeleagate;
    
        void onUpdateNetworkTimeDeleagate(int errCode, const char* errMsg);

        NetworkTimeCallback ^ m_callback = nullptr;
    };
};