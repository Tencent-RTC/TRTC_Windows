// ITRTCCloudCallback.h

#pragma once

// C++/CLI Header
#include "cli/ITRTCDeviceManager.h"
#include "cli/TRTC/TRTCCloudDef.h"
#include "cli/TRTC/TRTCStatistics.h"

using namespace System;

namespace ManageLiteAV
{
    /// @defgroup ITRTCCloudCallback_csharp ITRTCCloudCallback
    /// 腾讯云视频通话功能的回调接口类
    /// @{

    /// <summary>
    /// 腾讯云视频通话功能的回调接口类
    /// </summary>
    public interface class ITRTCCloudCallback
    {
        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （一）错误事件和警告事件
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 错误事件和警告事件
        /// @{

        /// <summary>
        /// 1.1 错误回调，SDK 不可恢复的错误，一定要监听，并分情况给用户适当的界面提示。
        /// </summary>
        /// <param name="errCode">错误码</param>
        /// <param name="errMsg">错误信息</param>
        /// <param name="arg"> 扩展信息字段，个别错误码可能会带额外的信息帮助定位问题</param>
        void onError(TXLiteAVError errCode, String^ errMsg, IntPtr arg);

        /// <summary>
        /// 1.2 警告回调：用于告知您一些非严重性问题，例如出现了卡顿或者可恢复的解码失败。
        /// </summary>
        /// <param name="warningCode">错误码</param>
        /// <param name="warningMsg">警告信息</param>
        /// <param name="arg">扩展信息字段，个别警告码可能会带额外的信息帮助定位问题</param>
        void onWarning(TXLiteAVWarning warningCode, String^ warningMsg, IntPtr arg);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （二）房间事件回调
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 房间事件回调
        /// @{

        /// <summary>
        /// 2.1 已加入房间的回调
        /// 
        /// <para>调用 TRTCCloud 中的 enterRoom() 接口执行进房操作后，会收到来自 SDK 的 onEnterRoom(result) 回调：</para>
        /// <para>- 如果加入成功，result 会是一个正数（result &gt; 0），代表加入房间的时间消耗，单位是毫秒（ms）。</para>
        /// <para>- 如果加入失败，result 会是一个负数（result &lt; 0），代表进房失败的错误码。</para>
        /// <para>进房失败的错误码含义请参见 [错误码](https://cloud.tencent.com/document/product/647/32257)。</para>
        /// </summary>
        /// <remarks>
        /// 在 Ver6.6 之前的版本，只有进房成功会抛出 onEnterRoom(result) 回调，进房失败由 onError() 回调抛出。
        /// <para>在 Ver6.6 及之后改为：进房成功返回正的 result，进房失败返回负的 result，同时进房失败也会有 onError() 回调抛出。</para>
        /// </remarks>
        /// <param name="result">result &gt; 0 时为进房耗时（ms），result &lt; 0 时为进房错误码。</param>
        void onEnterRoom(int result);

        /// <summary>
        /// 2.2 离开房间的事件回调
        /// <para>
        /// 调用 TRTCCloud 中的 exitRoom() 接口会执行退出房间的相关逻辑，例如释放音视频设备资源和编解码器资源等。
        /// 待资源释放完毕之后，SDK 会通过 onExitRoom() 回调通知到您。
        /// </para>
        /// <para>
        /// 如果您要再次调用 enterRoom() 或者切换到其他的音视频 SDK，请等待 onExitRoom() 回调到来之后再执行相关操作。
        /// 否则可能会遇到如摄像头、麦克风设备被强占等各种异常问题。
        /// </para>
        /// </summary>
        /// <param name="reason">离开房间原因，0：主动调用 exitRoom 退房；1：被服务器踢出当前房间；2：当前房间整个被解散。</param>
        void onExitRoom(int reason);

        /// <summary>
        /// 2.3 切换角色结果回调
        /// <para>
        /// 调用 TRTCCloud 中的 switchRole() 接口会切换主播和观众的角色，该操作会伴随一个线路切换的过程，
        /// 待 SDK 切换完成后，会抛出 onSwitchRole() 事件回调。
        /// </para>
        /// </summary>
        /// <param name="errCode">错误码，ERR_NULL 代表切换成功，其他请参见 [错误码](https://cloud.tencent.com/document/product/647/32257)。</param>
        /// <param name="errMsg">错误信息</param>
        void onSwitchRole(TXLiteAVError errCode, String^ errMsg);

        /// <summary>
        /// 2.4 切换房间 (switchRoom) 的结果回调
        /// </summary>
        void onSwitchRoom(TXLiteAVError errCode, String ^ errMsg);

        /// <summary>
        /// 2.5 请求跨房通话（主播 PK）的结果回调
        /// 
        /// <para>调用 TRTCCloud 中的 connectOtherRoom() 接口会将两个不同房间中的主播拉通视频通话，也就是所谓的“主播PK”功能。</para>
        /// <para>
        /// 调用者会收到 onConnectOtherRoom() 回调来获知跨房通话是否成功，
        /// 如果成功，两个房间中的所有用户都会收到 PK 主播的 onUserVideoAvailable() 回调。
        /// </para>
        /// </summary>
        /// <param name="userId">要 PK 的目标主播 userId。</param>
        /// <param name="errCode">错误码，ERR_NULL 代表切换成功，其他请参见 [错误码](https://cloud.tencent.com/document/product/647/32257)。</param>
        /// <param name="errMsg">错误信息</param>
        void onConnectOtherRoom(String^ userId, TXLiteAVError errCode, String^ errMsg);

        /// <summary>
        /// 2.6 结束跨房通话（主播 PK）的结果回调
        /// </summary>
        /// <param name="errCode">错误码，ERR_NULL 代表切换成功，其他请参见 [错误码](https://cloud.tencent.com/document/product/647/32257)。</param>
        /// <param name="errMsg">错误信息</param>
        void onDisconnectOtherRoom(TXLiteAVError errCode, String^ errMsg);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （三）成员事件回调
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 成员事件回调
        /// @{

        /// <summary>
        /// 3.1 有用户加入当前房间
        /// 
        /// <para>出于性能方面的考虑，在两种不同的应用场景下，该通知的行为会有差别：</para>
        /// <para>- 通话场景（TRTCAppSceneVideoCall 和 TRTCAppSceneAudioCall）：该场景下用户没有角色的区别，任何用户进入房间都会触发该通知。</para>
        /// <para>- 直播场景（TRTCAppSceneLIVE 和 TRTCAppSceneVoiceChatRoom）：该场景不限制观众的数量，如果任何用户进出都抛出回调会引起很大的性能损耗，所以该场景下只有主播进入房间时才会触发该通知，观众进入房间不会触发该通知。</para>
        /// </summary>
        /// <remarks>
        /// 注意 onRemoteUserEnterRoom 和 onRemoteUserLeaveRoom 只适用于维护当前房间里的“成员列表”，如果需要显示远程画面，建议使用监听 onUserVideoAvailable() 事件回调。
        /// </remarks>
        /// <param name="userId">用户标识</param>
        void onRemoteUserEnterRoom(String^ userId);

        /// <summary>
        /// 3.2 有用户离开当前房间
        /// 
        /// <para>与 onRemoteUserEnterRoom 相对应，在两种不同的应用场景下，该通知的行为会有差别：</para>
        /// <para>- 通话场景（TRTCAppSceneVideoCall 和 TRTCAppSceneAudioCall）：该场景下用户没有角色的区别，任何用户的离开都会触发该通知。</para>
        /// <para>- 直播场景（TRTCAppSceneLIVE 和 TRTCAppSceneVoiceChatRoom）：只有主播离开房间时才会触发该通知，观众离开房间不会触发该通知。</para>
        /// </summary>
        /// <param name="userId">用户标识</param>
        /// <param name="reason">离开原因，0表示用户主动退出房间，1表示用户超时退出，2表示被踢出房间。</param>
        void onRemoteUserLeaveRoom(String^ userId, int reason);

        /// <summary>
        /// 3.3 用户是否开启摄像头视频
        ///
        /// <para>当您收到 onUserVideoAvailable(userId, YES) 通知时，代表该路画面已经有可用的视频数据帧到达。</para>
        /// <para>之后，您需要调用 startRemoteView(userId) 接口加载该用户的远程画面。</para>
        /// <para>再之后，您还会收到名为 onFirstVideoFrame(userId) 的首帧画面渲染回调。</para>
        /// <para>
        /// 当您收到了 onUserVideoAvailable(userId, NO) 通知时，代表该路远程画面已经被关闭，这可能是
        /// 由于该用户调用了 muteLocalVideo() 或 stopLocalPreview() 所致。
        /// </para>
        /// </summary>
        /// <param name="userId">用户标识</param>
        /// <param name="available">画面是否开启</param>
        void onUserVideoAvailable(String^ userId, bool available);

        /// <summary>
        /// 3.4 用户是否开启屏幕分享
        /// </summary>
        /// <param name="userId">用户标识</param>
        /// <param name="available">屏幕分享是否开启</param>
        void onUserSubStreamAvailable(String^ userId, bool available);

        /// <summary>
        /// 3.5 用户是否开启音频上行
        /// </summary>
        /// <param name="userId">用户标识</param>
        /// <param name="available">声音是否开启</param>
        void onUserAudioAvailable(String^ userId, bool available);

        /// <summary>
        /// 3.6 开始渲染本地或远程用户的首帧画面
        /// 
        /// <para>如果 userId 为 null，代表开始渲染本地采集的摄像头画面，需要您先调用 startLocalPreview 触发。</para>
        /// <para>如果 userId 不为 null，代表开始渲染远程用户的首帧画面，需要您先调用 startRemoteView 触发。</para>
        /// </summary>
        /// <remarks>
        /// 只有当您调用 startLocalPreview()、startRemoteView() 或 startRemoteSubStreamView() 之后，才会触发该回调。
        /// </remarks>
        /// <param name="userId">本地或远程用户 ID，如果 userId == null 代表本地，userId != null 代表远程。</param>
        /// <param name="streamType">视频流类型：摄像头或屏幕分享。</param>
        /// <param name="width">画面宽度</param>
        /// <param name="height">画面高度</param>
        void onFirstVideoFrame(String^ userId, TRTCVideoStreamType streamType, int width, int height);

        /// <summary>
        /// 3.7 开始播放远程用户的首帧音频（本地声音暂不通知）
        /// </summary>
        /// <param name="userId">远程用户 ID</param>
        void onFirstAudioFrame(String^ userId);

        /// <summary>
        /// 3.8 首帧本地视频数据已经被送出
        /// 
        /// <para>SDK 会在 enterRoom() 并 startLocalPreview() 成功后开始摄像头采集，并将采集到的画面进行编码。</para>
        /// <para>当 SDK 成功向云端送出第一帧视频数据后，会抛出这个回调事件。</para>
        /// </summary>
        /// <param name="streamType">视频流类型，大画面还是小画面或辅流画面（屏幕分享）</param>
        void onSendFirstLocalVideoFrame(TRTCVideoStreamType streamType);

        /// <summary>
        /// 3.9 首帧本地音频数据已经被送出
        /// 
        /// <para>SDK 会在 enterRoom() 并 startLocalAudio() 成功后开始麦克风采集，并将采集到的声音进行编码。</para>
        /// <para>当 SDK 成功向云端送出第一帧音频数据后，会抛出这个回调事件。</para>
        /// </summary>
        void onSendFirstLocalAudioFrame();

        /// <summary>
        /// 3.10 远端视频状态变化的事件回调
        ///
        /// <para>您可以通过此事件回调获取远端每一路画面的播放状态（包括 Playing、Loading 和 Stopped 三种状态），从而进行相应的 UI 展示。</para> 
        /// </summary>
        /// <param name="userId">用户标识</param>
        /// <param name="streamType">视频流类型：主路（Main）一般用于承载摄像头画面，辅路（Sub）一般用于承载屏幕分享画面。</param>
        /// <param name="status">视频状态：包括 Playing、Loading 和 Stopped 三种状态。</param>
        /// <param name="reason">视频状态改变的原因</param>
        /// <param name="extrainfo">额外信息</param>
        void onRemoteVideoStatusUpdated(String ^ userId, TRTCVideoStreamType streamType, TRTCAVStatusType status, TRTCAVStatusChangeReason reason, IntPtr extrainfo);

        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （四）统计和质量回调
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 统计和质量回调
        /// @{

        /// <summary>
        /// 4.1 网络质量：该回调每2秒触发一次，统计当前网络的上行和下行质量
        /// </summary>
        /// <remarks>
        /// userId == null 代表自己当前的视频质量
        /// </remarks>
        /// <param name="localQuality">上行网络质量</param>
        /// <param name="remoteQuality">下行网络质量的数组</param>
        /// <param name="remoteQualityCount">下行网络质量的数组大小</param>
        void onNetworkQuality(TRTCQualityInfo^ localQuality, array<TRTCQualityInfo^>^ remoteQuality, UInt32 remoteQualityCount);

        /// <summary>
        /// 4.2 技术指标统计回调
        /// 
        /// <para>如果您是熟悉音视频领域相关术语，可以通过这个回调获取 SDK 的所有技术指标。</para>
        /// <para>如果您是首次开发音视频相关项目，可以只关注 onNetworkQuality 回调。</para>
        /// </summary>
        /// <remarks>
        /// 每2秒回调一次
        /// </remarks>
        /// <param name="statis">统计数据，包括本地和远程的</param>
        void onStatistics(TRTCStatistics^ statis);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （五）服务器事件回调
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 服务器事件回调
        /// @{

        /// <summary>
        /// 5.1 SDK 跟服务器的连接断开
        /// </summary>
        void onConnectionLost();

        /// <summary>
        /// 5.2 SDK 尝试重新连接到服务器
        /// </summary>
        void onTryToReconnect();

        /// <summary>
        /// 5.3 SDK 跟服务器的连接恢复
        /// </summary>
        void onConnectionRecovery();

        /// <summary>
        /// 5.4 服务器测速的回调，SDK 对多个服务器 IP 做测速，每个 IP 的测速结果通过这个回调通知
        /// </summary>
        /// <param name="currentResult">当前完成的测速结果</param>
        /// <param name="finishedCount">已完成测速的服务器数量</param>
        /// <param name="totalCount">需要测速的服务器总数量</param>
        void onSpeedTest(TRTCSpeedTestResult^ currentResult, UInt32 finishedCount, UInt32 totalCount);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （六）硬件设备事件回调
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 硬件设备事件回调
        /// @{

        /// <summary>
        /// 6.1 摄像头准备就绪
        /// </summary>
        void onCameraDidReady();

        /// <summary>
        /// 6.2 麦克风准备就绪
        /// </summary>
        void onMicDidReady();

        /// <summary>
        /// 6.3 用于提示音量大小的回调,包括每个 userId 的音量和远端总音量
        ///
        /// <para>您可以通过调用 TRTCCloud 中的 enableAudioVolumeEvaluation 接口来开关这个回调。</para>
        /// <para>需要注意的是，调用 enableAudioVolumeEvaluation 开启音量回调后，无论频道内是否有人说话，都会按设置的时间间隔调用这个回调;</para>
        /// <para>如果没有人说话，则 userVolumes 为空，totalVolume 为0。</para>
        /// </summary>
        /// <remarks>
        /// userId 为 null 时表示自己的音量，userVolumes 内仅包含正在说话（音量不为0）的用户音量信息。
        /// </remarks>
        /// <param name="userVolumes">所有正在说话的房间成员的音量，取值范围0 - 100。</param>
        /// <param name="userVolumesCount">房间成员数量</param>
        /// <param name="totalVolume">所有远端成员的总音量, 取值范围0 - 100。</param>
        void onUserVoiceVolume(array<TRTCVolumeInfo^>^ userVolumes, UInt32 userVolumesCount, UInt32 totalVolume);

        /// <summary> 
        /// 6.4 本地设备通断回调
        /// </summary>
        /// <param name="deviceId">设备 ID</param>
        /// <param name="type">设备类型</param>
        /// <param name="state">事件类型</param>
        void onDeviceChange(String^ deviceId, TRTCDeviceType type, TRTCDeviceState state);

        /// <summary>
        /// 6.5 当前音频采集设备音量变化通知
        /// </summary>
        /// <remarks>使用enableAudioVolumeEvaluation（interval>0）开启，（interval==0）关闭</remarks> 
        /// <param name="volume">音量值，取值范围0 - 100</param> 
        /// <param name="muted">当前采集音频设备是否被静音，true：静音；false：取消静音</param>
        void onAudioDeviceCaptureVolumeChanged(UInt32 volume, bool muted);

        /// <summary>
        /// 6.6 当前音频播放设备音量变化通知
        /// </summary>
        /// <remarks>使用enableAudioVolumeEvaluation（interval>0）开启，（interval==0）关闭</remarks> 
        /// <param name="volume">音量值，取值范围0 - 100</param> 
        /// <param name="muted">当前音频播放设备是否被静音，true：静音；false：取消静音</param>
        void onAudioDevicePlayoutVolumeChanged(UInt32 volume, bool muted);

        /// <summary>
        /// 6.7 麦克风测试音量回调
        ///
        /// <para>麦克风测试接口 startMicDeviceTest 会触发这个回调</para>
        /// </summary>
        /// <param name="volume">音量值，取值范围0 - 100</param>
        void onTestMicVolume(UInt32 volume);

        /// <summary>
        /// 6.8 扬声器测试音量回调
        ///
        /// <para>扬声器测试接口 startSpeakerDeviceTest 会触发这个回调</para>
        /// </summary>
        /// <param name="volume">音量值，取值范围0 - 100</param>
        void onTestSpeakerVolume(UInt32 volume);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （七）自定义消息的接收回调
        //
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 自定义消息的接收回调
        /// @{

        /// <summary>
        /// 7.1 收到自定义消息回调
        ///
        /// <para>当房间中的某个用户使用 sendCustomCmdMsg 发送自定义消息时，房间中的其它用户可以通过 onRecvCustomCmdMsg 接口接收消息</para>
        /// </summary>
        /// <param name="userId">用户标识</param>
        /// <param name="cmdID">命令 ID</param>
        /// <param name="seq">消息序号</param>
        /// <param name="msg">消息数据</param>
        /// <param name="msgSize">消息数据大小</param>
        void onRecvCustomCmdMsg(String^ userId, Int32 cmdID, UInt32 seq, array<Byte>^ msg, UInt32 msgSize);

        /// <summary>
        /// 7.2 自定义消息丢失回调
        ///
        /// <para>TRTC 所使用的传输通道为 UDP 通道，所以即使设置了 reliable，也做不到100%不丢失，只是丢消息概率极低，能满足常规可靠性要求。</para>
        /// <para>在过去的一段时间内（通常为5s），自定义消息在传输途中丢失的消息数量的统计，SDK 都会通过此回调通知出来。</para>
        /// </summary>
        /// <remarks>
        /// 只有在发送端设置了可靠传输(reliable)，接收方才能收到消息的丢失回调
        /// </remarks>
        /// <param name="userId">用户标识</param>
        /// <param name="cmdId">命令 ID</param>
        /// <param name="errCode">错误码，当前版本为-1</param>
        /// <param name="missed">丢失的消息数量</param>
        void onMissCustomCmdMsg(String^ userId, Int32 cmdId, Int32 errCode, Int32 missed);

        /// <summary>
        /// 7.3 收到 SEI 消息的回调
        ///
        /// <para>当房间中的某个用户使用 sendSEIMsg 发送数据时，房间中的其它用户可以通过 onRecvSEIMsg 接口接收数据。</para>
        /// </summary>
        /// <param name="userId">用户标识</param>
        /// <param name="message">数据</param>
        /// <param name="msgSize">数据大小</param>
        void onRecvSEIMsg(String^ userId, array<Byte>^ message, UInt32 msgSize);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （八）CDN 旁路转推回调
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name CDN 旁路转推回调
        /// @{

        /// <summary>
        /// 8.1 开始向腾讯云的直播 CDN 推流的回调，对应于 TRTCCloud 中的 startPublishing() 接口
        /// </summary>
        /// <param name="errCode">0表示成功，其余值表示失败</param>
        /// <param name="errMsg">具体错误原因</param>
        /// 
        void onStartPublishing(int errCode, String^ errMsg);

        /// <summary>
        /// 8.2 停止向腾讯云的直播 CDN 推流的回调，对应于 TRTCCloud 中的 stopPublishing() 接口
        /// </summary>
        /// <param name="errCode">0表示成功，其余值表示失败</param>
        /// <param name="errMsg">具体错误原因</param>
        void onStopPublishing(int errCode, String^ errMsg);

        /// <summary>
        /// 8.3 启动旁路推流到 CDN 完成的回调
        ///
        /// <para>对应于 TRTCCloud 的 startPublishCDNStream() 接口</para>
        /// </summary>
        /// <remarks>
        /// Start 回调如果成功，只能说明转推请求已经成功告知给腾讯云，如果目标 CDN 有异常，还是有可能会转推失败。
        /// </remarks>
        /// <param name="errCode">错误码，参考 TXLiteAVCode.h</param>
        /// <param name="errMsg">错误详细信息</param>
        void onStartPublishCDNStream(int errCode, String^ errMsg);

        /// <summary>
        /// 8.4 停止旁路推流到 CDN 完成的回调
        ///
        /// <para>对应于 TRTCCloud 中的 stopPublishCDNStream() 接口</para>
        /// </summary>
        /// <param name="errCode">错误码，参考 TXLiteAVCode.h</param>
        /// <param name="errMsg">错误详细信息</param>
        void onStopPublishCDNStream(int errCode, String^ errMsg);

        /// <summary>
        /// 8.5 设置云端的混流转码参数的回调，对应于 TRTCCloud 中的 setMixTranscodingConfig() 接口
        ///
        /// <para>对应于 TRTCCloud 中的 setMixTranscodingConfig() 接口</para>
        /// </summary>
        /// <param name="errCode">0表示成功，其余值表示失败</param>
        /// <param name="errMsg">具体错误原因</param>
        void onSetMixTranscodingConfig(int errCode, String^ errMsg);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （九）屏幕分享回调
        //
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 屏幕分享回调
        /// @{
        /// <summary>
        /// 9.1 当屏幕分享开始时，SDK 会通过此回调通知
        /// </summary>
        void onScreenCaptureStarted();

        /// <summary>
        /// 9.2 当屏幕分享暂停时，SDK 会通过此回调通知
        /// </summary>
        /// <param name="reason">停止原因，0：表示用户主动暂停；1：表示设置屏幕分享参数导致的暂停；2：表示屏幕分享窗口被最小化导致的暂停；3：表示屏幕分享窗口被隐藏导致的暂停</param>
        void onScreenCapturePaused(int reason);

        /// <summary>
        /// 9.3 当屏幕分享恢复时，SDK 会通过此回调通知
        /// </summary>
        /// <param name="reason">停止原因，0：表示用户主动恢复，1：表示屏幕分享参数设置完毕后自动恢复；2：表示屏幕分享窗口从最小化被恢复；3：表示屏幕分享窗口从隐藏被恢复</param>
        void onScreenCaptureResumed(int reason);

        /// <summary>
        /// 9.4 当屏幕分享停止时，SDK 会通过此回调通知
        /// </summary>
        /// <param name="reason">停止原因，0：表示用户主动停止；1：表示屏幕分享窗口被关闭</param>
        void onScreenCaptureStoped(int reason);

        /// <summary>
        /// 9.5 当屏幕分享窗口被遮挡无法正常捕获时，SDK 会通过此回调通知，可在此回调里通知用户移开遮挡窗口
        /// </summary>
        void onScreenCaptureCovered();
        /// @}


		/////////////////////////////////////////////////////////////////////////////////
		//
		//                      （十）本地录制回调和截图回调
		//
		/////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 录制任务已经开始
		/// </summary>
		/// <param name="errCode">错误码 0：初始化录制成功；-1：初始化录制失败；-2: 文件后缀名有误</param>
		/// <param name="storagePath">录制文件存储路径</param>
		void onLocalRecordBegin(Int32 errCode, String^ storagePath);

		/// <summary>
		/// 录制任务进行中
		/// </summary>
		/// <param name="duration">已经录制的累计时长，单位毫秒</param>
		/// <param name="storagePath">录制文件存储路径</param>
		void onLocalRecording(Int32 duration, String^storagePath);


		/// <summary>
		/// 录制任务已结束
		/// </summary>
		/// <param name="errCode">错误码 0：录制成功；-1：录制失败；-2：切换分辨率或横竖屏导致录制结束</param>
		/// <param name="storagePath">录制文件存储路径</param>
		void onLocalRecordComplete(Int32 errCode, String^storagePath);

		/// <summary>
		/// 截图完成时回调
		/// </summary>
		/// <param name="userId">用户 ID，空字符串表示截取本地画面</param>
		/// <param name="type">视频流类型</param>
		/// <param name="data">截图数据，为 nullptr 表示截图失败</param>
		/// <param name="length">截图数据长度，对于BGRA32而言，length = width * height * 4</param>
		/// <param name="width">截图画面的宽度</param>
		/// <param name="height">截图画面的高度</param>
		/// <param name="format">截图数据格式，目前只支持 TRTCVideoPixelFormat_BGRA32</param>
		void onSnapshotComplete(String^ userId, TRTCVideoStreamType type, array<Byte>^ data, UInt32 length, UInt32 width, UInt32 height, TRTCVideoPixelFormat format);

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （十）弃用接口回调
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 弃用接口回调
        /// @{

        /// <summary>
        /// 10.1 废弃接口：有主播加入当前房间
        ///
        /// <para>该回调接口可以被看作是 onRemoteUserEnterRoom 的废弃版本，不推荐使用。请使用
        /// onUserVideoAvailable 或 onRemoteUserEnterRoom 进行替代。</para>
        /// </summary>
        /// <remarks>该接口已被废弃，不推荐使用</remarks>
        /// <param name="userId">用户标识</param>
        [Obsolete("Please use the latest method(onRemoteUserEnterRoom).")] 
        void onUserEnter(String ^ userId);

        /// <summary>
        /// 10.2 废弃接口：有用户（主播）离开当前房间
        ///
        /// <para>该回调接口可以被看作是 onRemoteUserLeaveRoom 的废弃版本，不推荐使用。请使用
        /// onUserVideoAvailable 或 onRemoteUserEnterRoom 进行替代。</para>
        /// </summary>
        /// <remarks>该接口已被废弃，不推荐使用</remarks>
        /// <param name="userId">用户标识</param>
        /// <param name="reason">离开原因</param>
        [Obsolete("Please use the latest method(onRemoteUserLeaveRoom).")]
        void onUserExit(String ^ userId, int reason);

        /// <summary>
        /// 10.3 废弃接口：播放音效结束回调
        /// </summary>
        /// <remarks>该接口已被废弃，不推荐使用</remarks>
        /// <param name="effectId">音效id</param>
        /// <param name="code">0表示播放正常结束；其他表示异常结束</param>
        [Obsolete("use ITXAudioEffectManager.startPlayMusic instead")]
        void onAudioEffectFinished(int effectId, int code);

        /// <summary>
        /// 10.4 废弃接口：开始播放背景音乐
        /// </summary>
        /// <remarks>该接口已被废弃，不推荐使用</remarks>
        /// <param name="errCode">错误码</param>
        [Obsolete("use ITXAudioEffectManager.ITXMusicPlayObserver instead")]
        void onPlayBGMBegin(TXLiteAVError errCode);

        /// <summary>
        /// 10.5 废弃接口：播放背景音乐的进度
        /// </summary>
        /// <remarks>该接口已被废弃，不推荐使用</remarks>
        /// <param name="progressMS">已播放时间</param>
        /// <param name="durationMS">总时间</param>
        [Obsolete("use ITXAudioEffectManager.ITXMusicPlayObserver instead")]
        void onPlayBGMProgress(UInt32 progressMS, UInt32 durationMS);

        /// <summary>
        /// 10.6 废弃接口：播放背景音乐结束
        /// </summary>
        /// <remarks>该接口已被废弃，不推荐使用</remarks>
        /// <param name="errCode">错误码</param>
        [Obsolete("use ITXAudioEffectManager.ITXMusicPlayObserver instead")]
        void onPlayBGMComplete(TXLiteAVError errCode);
        /// @}
    };

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （十二）视频相关回调
    //
    /////////////////////////////////////////////////////////////////////////////////

    /// <summary>
    /// 视频数据帧的自定义处理回调
    /// </summary>
    public interface class ITRTCVideoRenderCallback
    {
    public:
        /// <summary>
        /// 12.1 自定义视频渲染回调
        ///
        /// <para>可以通过 setLocalVideoRenderCallback 和 setRemoteVideoRenderCallback 接口设置自定义渲染回调</para>
        /// </summary>
        /// <param name="userId">用户标识</param>
        /// <param name="streamType">流类型：即摄像头还是屏幕分享</param>
        /// <param name="frame">视频帧数据</param>
        void onRenderVideoFrame(String^ userId, TRTCVideoStreamType streamType, TRTCVideoFrame^ frame);
    };

    public interface class ITRTCVideoFrameCallback
    {
    public:
         /// <summary>
         /// 12.2 用于对接第三方美颜组件的视频处理回调
         ///
         /// <para>如果您选购了第三方美颜组件，就需要在 TRTCCloud 中设置第三方美颜回调，之后 TRTC 就会将原本要进行预处理的视频帧通过此回调接口抛送给您。</para>
         /// <para>之后您就可以将 TRTC 抛出的视频帧交给第三方美颜组件进行图像处理，由于抛出的数据是可读且可写的，因此第三方美颜的处理结果也可以同步给 TRTC 进行后续的编码和发送。</para>
         /// </summary>
         /// <param name="srcFrame">用于承载 TRTC 采集到的摄像头画面</param>
         /// <param name="dstFrame">用于接收第三方美颜处理过的视频画面</param>
         /// 
         /// <para>之后您就可以将 TRTC 抛出的视频帧交给第三方美颜组件进行图像处理，由于抛出的数据是可读且可写的，因此第三方美颜的处理结果也可以同步给 TRTC 进行后续的编码和发送。</para>
        int onProcessVideoFrame(TRTCVideoFrame ^ srcFrame, TRTCVideoFrame ^ dstFrame);
    };

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （十三）音频相关回调
    //
    /////////////////////////////////////////////////////////////////////////////////

    /// <summary>
    /// 声音数据帧的自定义处理回调（只读）
    /// </summary>
    /// <remarks>
    /// 回调函数是在 SDK 内部线程同步抛出来的，请不要做耗时操作。
    /// <para>提示：请按需定义相关函数实现，减少不必要的性能损耗。</para>
    /// </remarks>
    public interface class ITRTCAudioFrameCallback
    {
    public:
        /// <summary>
        /// 13.1 本地麦克风采集到的音频数据回调
        /// </summary>
        /// <remarks>
        /// - 请不要在此回调函数中做任何耗时操作，建议直接拷贝到另一线程进行处理，否则会导致各种声音问题。
        /// <para>- 此接口回调出的音频数据支持修改。</para>
        /// <para>- 此接口回调出的音频时间帧长固定为0.02s。</para>
        /// <para>  由时间帧长转化为字节帧长的公式为【采样率 × 时间帧长 × 声道数 × 采样点位宽】。</para>
        /// <para>  以SDK默认的音频录制格式48000采样率、单声道、16采样点位宽为例，字节帧长为【48000 × 0.02s × 1 × 16bit = 15360bit = 1920字节】。</para>
        /// <para>- 此接口回调出的音频数据包含背景音、音效、混响等前处理效果。</para>
        /// </remarks>
        /// <param name="frame">PCM 格式的音频数据帧</param>
        void onCapturedRawAudioFrame(TRTCAudioFrame^ frame);

        /// <summary>
        /// 13.2 本地采集并经过音频模块前处理后的音频数据回调
        /// <para>当您设置完音频数据自定义回调之后，SDK 内部会把刚采集到并经过前处理(ANS、AEC、AGC）之后的数据，以 PCM 格式的形式通过本接口回调给您。</para>
        /// <para>- 此接口回调出的音频时间帧长固定为0.02s，格式为 PCM 格式。</para>
        /// <para>- 由时间帧长转化为字节帧长的公式为【采样率 × 时间帧长 × 声道数 × 采样点位宽】。</para>
        /// <para>- 以 TRTC 默认的音频录制格式48000采样率、单声道、16采样点位宽为例，字节帧长为【48000 × 0.02s × 1 × 16bit = 15360bit = 1920字节】。</para>
        /// <para>特殊说明：</para>
        /// <para>您可以通过设置接口中的 `TRTCAudioFrame.extraData` 字段，达到传输信令的目的。</para>
        /// <para>由于音频帧头部的数据块不能太大，建议您写入 `extraData` 时，尽量将信令控制在几个字节的大小，如果超过 100 个字节，写入的数据不会被发送。</para>
        /// <para>房间内其他用户可以通过 {@link TRTCAudioFrameDelegate} 中的 `onRemoteUserAudioFrame` 中的 `TRTCAudioFrame.extraData` 字段回调接收数据。</para>
        /// </summary>
        /// <param name="frame">PCM 格式的音频数据帧</param>
        void onLocalProcessedAudioFrame(TRTCAudioFrame ^ frame);

        /// <summary>
        /// 13.3 混音前的每一路远程用户的音频数据（例如您要对某一路的语音进行文字转换，必须要使用这里的原始数据，而不是混音之后的数据）
        /// </summary>
        /// <remarks>
        /// - 请不要在此回调函数中做任何耗时操作，建议直接拷贝到另一线程进行处理，否则会导致各种声音问题。
        /// <para>- 此接口回调出的音频数据是只读的，不支持修改。</para>
        /// </remarks>
        /// <param name="frame">PCM 格式的音频数据帧</param>
        /// <param name="userId">用户标识</param>
        void onPlayAudioFrame(TRTCAudioFrame^ frame, String^ userId);

        /// <summary>
        /// 13.4 各路音频数据混合后送入喇叭播放的音频数据
        /// </summary>
        /// <remarks>
        /// - 请不要在此回调函数中做任何耗时操作，建议直接拷贝到另一线程进行处理，否则会导致各种声音问题。
        /// <para>- 此接口回调出的音频数据支持修改。</para>
        /// <para>- 此接口回调出的音频时间帧长固定为0.02s。</para>
        /// <para>  由时间帧长转化为字节帧长的公式为【采样率 × 时间帧长 × 声道数 × 采样点位宽】。</para>
        /// <para>  以SDK默认的音频播放格式48000采样率、双声道、16采样点位宽为例，字节帧长为【48000 × 0.02s × 2 × 16bit = 30720bit = 3840字节】。</para>
        /// <para>- 此接口回调出的音频数据是各路音频播放数据的混合,不包含耳返的音频数据。</para>
        /// </remarks>
        /// <param name="frame">PCM 格式的音频数据帧</param>
        void onMixedPlayAudioFrame(TRTCAudioFrame^ frame);
    };

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （十四）Log 信息回调
    //
    /////////////////////////////////////////////////////////////////////////////////

    /// <summary>
    /// 日志相关回调
    /// </summary>
    public interface class ITRTCLogCallback
    {
    public:
        /// <summary>
        /// 14.1 有日志打印时的回调
        /// </summary>
        /// <param name="log">日志内容</param>
        /// <param name="level">日志等级 参见 TRTCLogLevel</param>
        /// <param name="module">暂无具体意义，目前为固定值 TXLiteAVSDK</param>
        void onLog(String^ log, TRTCLogLevel level, String^ module);
    };
    /// @}
}
