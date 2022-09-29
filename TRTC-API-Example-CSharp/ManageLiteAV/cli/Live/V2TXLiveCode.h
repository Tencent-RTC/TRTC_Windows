// Copyright (c) 2020 Tencent. All rights reserved.

/// @defgroup V2TXLiveCode V2TXLiveCode
/// 腾讯云直播服务(LVB)错误码和警告码的定义
/// @{
#pragma once

namespace ManageLiteAV {
    /////////////////////////////////////////////////////////////////////////////////
    //
    //                             V2 错误码和警告码
    //
    /////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// V2 错误码和警告码
        /// </summary>
    enum V2TXLiveCode {

        /// <summary>
        /// 没有错误
        /// </summary>
        V2TXLIVE_OK = 0,

        /// <summary>
        /// 暂未归类的通用错误
        /// </summary>
        V2TXLIVE_ERROR_FAILED = -1,

        /// <summary>
        ///调用 API 时，传入的参数不合法
        /// </summary>
        V2TXLIVE_ERROR_INVALID_PARAMETER = -2,

        /// <summary>
        /// API 调用被拒绝
        /// </summary>
        V2TXLIVE_ERROR_REFUSED = -3,

        /// <summary>
        /// 当前 API 不支持调用
        /// </summary>
        V2TXLIVE_ERROR_NOT_SUPPORTED = -4,

        /// <summary>
        /// license 不合法，调用失败
        /// </summary>
        V2TXLIVE_ERROR_INVALID_LICENSE = -5,

        /// <summary>
        /// 请求服务器超时
        /// </summary>
        V2TXLIVE_ERROR_REQUEST_TIMEOUT = -6,

        /// <summary>
        /// 服务器无法处理您的请求
        /// </summary>
        V2TXLIVE_ERROR_SERVER_PROCESS_FAILED = -7,

        /// <summary>
        /// 连接断开
        /// </summary>
        V2TXLIVE_ERROR_DISCONNECTED = -8,

        /// <summary>
        /// 找不到可用的 HEVC 解码器
        /// </summary>
        V2TXLIVE_ERROR_NO_AVAILABLE_HEVC_DECODERS = -2304,

        /////////////////////////////////////////////////////////////////////////////////
        //
        //      网络相关的警告码
        //
        /////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 网络状况不佳：上行带宽太小，上传数据受阻
        /// </summary>
        V2TXLIVE_WARNING_NETWORK_BUSY = 1101,

        /// <summary>
        /// 当前视频播放出现卡顿
        /// </summary>
        V2TXLIVE_WARNING_VIDEO_BLOCK = 2105,

        /////////////////////////////////////////////////////////////////////////////////
        //
        //             摄像头相关的警告码
        //
        /////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        ///摄像头打开失败
        /// </summary>
        V2TXLIVE_WARNING_CAMERA_START_FAILED = -1301,

        /// <summary>
        /// 摄像头正在被占用中，可尝试打开其他摄像头
        /// </summary>
        V2TXLIVE_WARNING_CAMERA_OCCUPIED = -1316,

        /// <summary>
        /// 摄像头设备未授权，通常在移动设备出现，可能是权限被用户拒绝了
        /// </summary>
        V2TXLIVE_WARNING_CAMERA_NO_PERMISSION = -1314,

        /////////////////////////////////////////////////////////////////////////////////
        //
        //             麦克风相关的警告码
        //
        /////////////////////////////////////////////////////////////////////////////////

        /// <summary>        
        ///麦克风打开失败
        /// </summary>
        V2TXLIVE_WARNING_MICROPHONE_START_FAILED = -1302,

        /// <summary>
        /// 麦克风正在被占用中，例如移动设备正在通话时，打开麦克风会失败
        /// </summary>
        V2TXLIVE_WARNING_MICROPHONE_OCCUPIED = -1319,

        /// <summary>
        /// 麦克风设备未授权，通常在移动设备出现，可能是权限被用户拒绝了
        /// </summary>
        V2TXLIVE_WARNING_MICROPHONE_NO_PERMISSION = -1317,

        /////////////////////////////////////////////////////////////////////////////////
        //
        //             屏幕分享相关警告码
        //
        /////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 当前系统不支持屏幕分享
        /// </summary>
        V2TXLIVE_WARNING_SCREEN_CAPTURE_NOT_SUPPORTED = -1309,

        /// <summary>        
        /// 开始录屏失败，如果在移动设备出现，可能是权限被用户拒绝了
        /// </summary>
        V2TXLIVE_WARNING_SCREEN_CAPTURE_START_FAILED = -1308,

        /// <summary>        
        /// 录屏被系统中断
        /// </summary>
        V2TXLIVE_WARNING_SCREEN_CAPTURE_INTERRUPTED = -7001,

    };
} // namespace ManageLiteAV
/// @}