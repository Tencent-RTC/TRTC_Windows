#pragma once

#include "cli/Live/V2TXLiveDef.h"
using namespace System;

namespace ManageLiteAV
{
    /// @defgroup ITRTCCloudCallback_csharp ITRTCCloudCallback
    /// 腾讯云直播推流的回调通知
    /// @{
    /// <summary>
    /// V2TXLivePusher 的一些回调事件，包括推流器状态，推流音量，统计信息，警告以及错误信息。
    /// </summary>
    public interface class V2TXLivePusherObserver {
        /////////////////////////////////////////////////////////////////////////////////
        //
        //                   直播推流器事件回调
        //
        /////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 直播推流器错误通知，推流器出现错误时，会回调该通知
        /// </summary>
        /// <param name="code">错误码 {@link V2TXLiveCode}</param>
        /// <param name="msg">错误信息</param>
        /// <param name="extraInfo">扩展信息</param>
        void onError(Int32 code, String^ msg, IntPtr extraInfo);

        /// <summary>
        /// 直播推流器警告通知
        /// </summary>
        /// <param name="code">警告码 {@link V2TXLiveCode}</param>
        /// <param name="msg">警告信息</param>
        /// <param name="extraInfo">扩展信息</param>
        void onWarning(Int32 code, String^ msg, IntPtr extraInfo);

        /// <summary>
        /// 首帧音频采集完成的回调通知
        /// </summary>
        void onCaptureFirstAudioFrame();

        /// <summary>
        /// 首帧视频采集完成的回调通知
        /// </summary>
        void onCaptureFirstVideoFrame();

        /// <summary>
        /// 麦克风采集音量值回调
        /// </summary>
        /// <param name="volume">音量大小</param>
        void onMicrophoneVolumeUpdate(Int32 volume);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="state"></param>
        /// <param name="msg"></param>
        /// <param name="extraInfo"></param>
        void onPushStatusUpdate(V2TXLivePushStatus state, String^ msg, IntPtr extraInfo);

        /// <summary>
        /// 直播推流器统计数据回调
        /// </summary>
        /// <param name="statistics">推流器统计数据 {@link V2TXLivePusherStatistics}</param>
        void onStatisticsUpdate(V2TXLivePusherStatistics^ statistics);

        /// <summary>
        /// 截图回调
        /// </summary>
        /// <param name="image">已截取的视频画面</param>
        /// <param name="length">截图数据长度，对于BGRA32而言，length = width * height * 4</param>
        /// <param name="width">截图画面的宽度</param>
        /// <param name="height">截图画面的高度</param>
        /// <param name="format">截图数据格式，目前只支持 V2TXLivePixelFormatBGRA32</param>
        void onSnapshotComplete(String^ image, int length, int width, int height, V2TXLivePixelFormat format);

        /// <summary>
        /// 自定义视频渲染回调
        /// </summary>
        /// <param name="videoFrame">videoFrame 视频帧数据 {@link V2TXLiveVideoFrame}</param>
        void onRenderVideoFrame(V2TXLiveVideoFrame^ videoFrame);

        /// <summary>
        /// 自定义视频预处理数据回调
        /// </summary>
        /// <param name="srcFrame">处理前的视频帧</param>
        /// <param name="dstFrame">处理后的视频帧</param>
        /// <returns>- 0：成功,其他： 错误</returns>
        int onProcessVideoFrame(V2TXLiveVideoFrame^ srcFrame, V2TXLiveVideoFrame^ dstFrame);

        /// <summary>
        /// 当屏幕分享开始时，SDK 会通过此回调通知
        /// </summary>
        void onScreenCaptureStarted();

        /// <summary>
        /// 当屏幕分享停止时，SDK 会通过此回调通知
        /// </summary>
        /// <param name="reason">
        /// 停止原因
        /// - 0：表示用户主动停止；
        /// - 1：表示屏幕分享窗口被关闭；
        /// - 2：表示屏幕分享的显示屏状态变更（如接口被拔出、投影模式变更等）
        /// </param>
        void onScreenCaptureStoped(int reason);
    };
}