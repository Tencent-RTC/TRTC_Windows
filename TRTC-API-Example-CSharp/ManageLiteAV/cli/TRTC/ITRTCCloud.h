// ITRTCCloud.h

#pragma once

#include <vector>
// C++ Header
#include "include/TRTC/ITRTCCloud.h"

// C++/CLI Header
#include "cli/TRTC/TRTCCloudDef.h"
#include "cli/TRTC/ITRTCCloudCallback.h"
#include "cli/ITRTCAudioEffectManager.h"
#include "cli/ITRTCDeviceManager.h"


using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

class TRTCCloudCallbackImpl;
class TRTCVideoRenderCallbackImpl;
class ITRTCVideoFrameCallbackImpl;
class TRTCAudioFrameCallbackImpl;
class TRTCLogCallbackImpl;

namespace ManageLiteAV
{
    /// @defgroup ITRTCCloud_csharp ITRTCCloud
    /// 腾讯云视频通话功能的主要接口类
    /// @{
    /// <summary>
    /// Module:   ITRTCCloud @ TXLiteAVSDK
    ///
    /// <para>Function: 腾讯云视频通话功能的主要接口类</para>
    /// </summary>
    /// <example>
    /// 创建/使用/销毁 ITRTCCloud 对象的示例代码：
    /// <code>
    /// ITRTCCloud trtcCloud = ITRTCCloud.getTRTCShareInstance();
    /// if (trtcCloud != null)
    /// {
    ///		string version = trtcCloud->getSDKVersion();
    /// }
    /// </code>
    ///	程序退出或者后续程序流程不需要使用 TRTCCloud 对象时，释放单例对象 ITRTCCloud
    /// <code>
    /// ITRTCCloud.destroyTRTCShareInstance();
    /// trtcCloud = null;
    /// </code>
    /// </example>
    public ref class ITRTCCloud
    {
    public:
        /// <summary>
        /// 析构函数，C# 代码中要求显示调用 Dispose 函数，来触发托管 C++ 类析构函数。
        /// </summary>
        ~ITRTCCloud();

        /// @name 创建与销毁 ITRTCCloud 单例
        /// @{

        /// <summary>
        /// 获取 ITRTCCloud 单例对象。
        /// </summary>
        /// <returns>返回 ITRTCCloud 单例对象</returns>
        static ITRTCCloud^ getTRTCShareInstance();

        /// <summary>
        /// 释放 ITRTCCloud 单例对象。
        /// </summary>
        static void destroyTRTCShareInstance();
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                       设置 TRTCCloudCallback 回调
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 设置 TRTCCloudCallback 回调
        /// @{

        /// <summary>
        /// 设置回调接口 ITRTCCloudCallback
        /// <para>您可以通过 ITRTCCloudCallback 获得来自 SDK 的各种状态通知，详见 ITRTCCloudCallback.h 中的定义</para>
        /// </summary>
        /// <param name="callback">事件回调</param>
        void addCallback(ITRTCCloudCallback^ callback);

        /// <summary>
        /// 移除事件回调
        /// </summary>
        /// <param name="callback">事件回调</param>
        void removeCallback(ITRTCCloudCallback^ callback);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （一）房间相关接口函数
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 房间相关接口函数
        /// @{
        /// <summary>
        /// 1.1 进入房间
        /// 
        /// <para>您会收到来自 TRTCCloudCallback 中的 onEnterRoom(result) 回调:</para>
        /// - 如果加入成功，result 会是一个正数（result &gt; 0），表示加入房间的时间消耗，单位是毫秒（ms）。
        /// - 如果加入失败，result 会是一个负数（result &lt; 0），表示进房失败的错误码。
        /// <para>进房失败的错误码含义请参见[错误码](https://cloud.tencent.com/document/product/647/32257)。</para>
        /// - {@link TRTCAppSceneVideoCall}：<br>
        ///          视频通话场景，支持720P、1080P高清画质，单个房间最多支持300人同时在线，最高支持50人同时发言。<br>
        ///          适合：[1对1视频通话]、[300人视频会议]、[在线问诊]、[视频聊天]、[远程面试]等。<br>
        /// - {@link TRTCAppSceneAudioCall}：<br>
        ///          语音通话场景，支持 48kHz，支持双声道。单个房间最多支持300人同时在线，最高支持50人同时发言。<br>
        ///          适合：[1对1语音通话]、[300人语音会议]、[在线狼人杀]、[语音聊天室]等。<br>
        /// - {@link TRTCAppSceneLIVE}：<br>
        ///          视频互动直播，支持平滑上下麦，切换过程无需等待，主播延时小于300ms；支持十万级别观众同时播放，播放延时低至1000ms。<br>
        ///          适合：[在线互动课堂]、[互动直播]、[视频相亲]、[远程培训]、[超大型会议]等。<br>
        /// - {@link TRTCAppSceneVoiceChatRoom}：<br>
        ///          语音互动直播，支持平滑上下麦，切换过程无需等待，主播延时小于300ms；支持十万级别观众同时播放，播放延时低至1000ms。<br>
        ///         适合：[语聊房]、[K 歌房]、[FM 电台]等。<br>
        /// </summary>
        /// <remarks>
        /// 1. 当 scene 选择为 TRTCAppSceneLIVE 或 TRTCAppSceneVoiceChatRoom 时，您必须通过 TRTCParams 中的 role 字段指定当前用户的角色。
        /// 2. 不管进房是否成功，enterRoom 都必须与 exitRoom 配对使用，在调用 exitRoom 前再次调用 enterRoom 函数会导致不可预期的错误问题。
        /// </remarks>
        /// <param name="params">进房参数，请参考 TRTCParams</param>
        /// <param name="scene">应用场景，目前支持视频通话（VideoCall）、在线直播（Live）、语音通话（AudioCall）、语音聊天室（VoiceChatRoom）四种场景</param>
        void enterRoom(TRTCParams^% params, TRTCAppScene scene);

        /// <summary>
        /// 1.2 离开房间
        ///
        /// <para>调用 exitRoom() 接口会执行退出房间的相关逻辑，例如释放音视频设备资源和编解码器资源等。</para>
        /// <para>待资源释放完毕，SDK 会通过 TRTCCloudCallback 中的 onExitRoom() 回调通知您。</para>
        /// 
        /// <para>如果您要再次调用 enterRoom() 或者切换到其他的音视频 SDK，请等待 onExitRoom() 回调到来后再执行相关操作。</para>
        /// <para>否则可能会遇到如摄像头、麦克风设备被强占等各种异常问题。</para>
        /// </summary>
        void exitRoom();

        /// <summary>
        /// 1.3 切换角色，仅适用于直播场景（TRTCAppSceneLIVE 和 TRTCAppSceneVoiceChatRoom）
        /// 
        /// <para>在直播场景下，一个用户可能需要在“观众”和“主播”之间来回切换。</para>
        /// <para>您可以在进房前通过 TRTCParams 中的 role 字段确定角色，也可以通过 switchRole 在进房后切换角色。</para>
        /// </summary>
        /// <param name="role">目标角色，默认为主播：
        /// - {@link TRTCRoleAnchor} 主播，可以上行视频和音频，一个房间里最多支持50个主播同时上行音视频。
        /// - {@link TRTCRoleAudience} 观众，只能观看，不能上行视频和音频，一个房间里的观众人数没有上限。
        /// </param>
        void switchRole(TRTCRoleType role);

        /// <summary>
        /// 1.4 切换房间
        ///
        /// <para>调用该接口后，用户会先退出原来的房间并快速进入 TRTCSwitchRoomConfig
        /// 中指定的新房间:</para> <para>相比于直接调用 exitRoom + enterRoom 的方式，switchRoom
        /// 接口对主播更加友好，因为 switchRoom 不会停止主播端视频的采集和预览。</para>
        /// <para>接口调用结果会通过 ITRTCCloudCallback 中的 onSwitchRoom(errCode, errMsg)
        /// 回调通知给您。</para>
        /// </summary>
        /// <param name="config">房间切换参数，请参考 {@link TRTCSwitchRoomConfig}</param>
        void switchRoom(TRTCSwitchRoomConfig^% config);

        /// <summary>
        /// 1.5 请求跨房通话（主播 PK）
        /// <para>
        /// TRTC 中两个不同音视频房间中的主播，可以通过“跨房通话”功能拉通连麦通话功能。使用此功能时，
        /// 两个主播无需退出各自原来的直播间即可进行“连麦 PK”。
        /// </para>
        /// <para>
        /// 例如：当房间“001”中的主播 A 通过 connectOtherRoom() 跟房间“002”中的主播 B 拉通跨房通话后，
        /// 房间“001”中的用户都会收到主播 B 的 onUserEnter(B) 回调和 onUserVideoAvailable(B, true) 回调。
        /// 房间“002”中的用户都会收到主播 A 的 onUserEnter(A) 回调和 onUserVideoAvailable(A, true) 回调。
        /// </para>
        /// <para>简言之，跨房通话的本质，就是把两个不同房间中的主播相互分享，让每个房间里的观众都能看到两个主播。</para>
        /// <para>                房间 001                     房间 002      </para>
        /// <para>              -------------               ------------     </para>
        /// <para> 跨房通话前： | 主播 A      |             | 主播 B     |    </para>
        /// <para>             | 观众 U V W  |             | 观众 X Y Z |     </para>
        /// <para>              -------------               ------------     </para>
        /// <para>                                                           </para>
        /// <para>                房间 001                     房间 002       </para>
        /// <para>              -------------               ------------     </para>
        /// <para> 跨房通话后： | 主播 A B    |             | 主播 B A   |     </para>
        /// <para>             | 观众 U V W  |             | 观众 X Y Z |     </para>
        /// <para>              -------------               ------------     </para>
        /// 
        /// <para>跨房通话的参数考虑到后续扩展字段的兼容性问题，暂时采用了 JSON 格式的参数，要求至少包含两个字段：</para>
        /// <para>- roomId：房间“001”中的主播 A 要跟房间“002”中的主播 B 连麦，主播 A 调用 connectOtherRoom() 时 roomId 应指定为“002”。</para>
        /// <para>- userId：房间“001”中的主播 A 要跟房间“002”中的主播 B 连麦，主播 A 调用 connectOtherRoom() 时 userId 应指定为 B 的 userId。</para>
        /// 
        /// <para>跨房通话的请求结果会通过 TRTCCloudCallback 中的 onConnectOtherRoom() 回调通知给您。</para>
        /// 
        /// <para> 此处用到 json 库来格式化json字符串                            </para>
        /// <para> dynamic jsonObj = new JObject();                             </para>
        /// <para> jsonObj["roomId"] = 002;                                     </para>
        /// <para> jsonObj["userId"] = "userB";                                 </para>
        /// <para> string params = JsonConvert.SerializeObject(jsonObj);        </para>
        /// <para> trtc.connectOtherRoom(params);                               </para>
        /// </summary>
        /// <param name="params">JSON 字符串连麦参数，roomId 代表目标房间号，userId 代表目标用户 ID。</param>
        void connectOtherRoom(String^ params);

        /// <summary>
        /// 1.6 关闭跨房连麦
        ///
        /// <para>跨房通话的退出结果会通过 ITRTCCloudCallback 中的 onDisconnectOtherRoom 回调通知给您。</para>
        /// </summary>
        void disconnectOtherRoom();

        /// <summary>
        /// 1.7 设置音视频数据接收模式（需要在进房前设置才能生效）
        /// 
        /// <para>为实现进房秒开的绝佳体验，SDK 默认进房后自动接收音视频。即在您进房成功的同时，您将立刻收到远端所有用户的音视频数据。</para>
        /// <para>若您没有调用 startRemoteView，视频数据将自动超时取消。</para>
        /// <para>若您主要用于语音聊天等没有自动接收视频数据需求的场景，您可以根据实际需求选择接收模式，以免产生预期之外的视频时长费用。</para>
        /// </summary>
        /// <param name="autoRecvAudio">true：自动接收音频数据；false：需要调用 muteRemoteAudio 进行请求或取消。默认值：true</param>
        /// <param name="autoRecvVideo">true：自动接收视频数据；false：需要调用 startRemoteView/stopRemoteView 进行请求或取消。默认值：true</param>
        /// <remarks>需要在进房前设置才能生效。</remarks>
        void setDefaultStreamRecvMode(bool autoRecvAudio, bool autoRecvVideo);

        /// <summary>
        /// 1.8 创建子房间示例（用于多房间并发观看）
        /// <para>TRTCCloud 一开始被设计成单例模式，限制了多房间并发观看的能力。</para>
        /// <para>通过调用该接口，您可以创建出多个 TRTCCloud 实例，以便同时进入多个不同的房间观看音视频流。</para>
        /// <para>但需要注意的是，由于摄像头和麦克风还是只有一份，因此您只能同时在一个 TRTCCloud 实例中以“主播”的身份存在，也就是您只能同时在一个 TRTCCloud 实例中发布自己的音视频流。</para>
        /// <para>该功能主要用于在线教育场景中一种被称为“超级小班课”的业务场景中，用于解决“每个 TRTC 的房间中最多只能有 50 人同时发布自己音视频流”的限制。</para>
        /// <para>示例代码如下：</para>
        /// <pre>
        ///     ITRTCCloud *mainCloud = getTRTCShareInstance();
        ///     mainCloud->enterRoom(params1, TRTCAppSceneLIVE);
        ///     //...
        ///     //Switch the role from "anchor" to "audience" in your own room
        ///     mainCloud->switchRole(TRTCRoleAudience);
        ///     mainCloud->muteLocalVideo(true);
        ///     mainCloud->muteLocalAudio(true);
        ///     //...
        ///     //Use subcloud to enter another room and switch the role from "audience" to "anchor"
        ///     ITRTCCloud *subCloud = mainCloud->createSubCloud();
        ///     subCloud->enterRoom(params2, TRTCAppSceneLIVE);
        ///     subCloud->switchRole(TRTCRoleAnchor);
        ///     subCloud->muteLocalVideo(false);
        ///     subCloud->muteLocalAudio(false);
        ///     //...
        ///     //Exit from new room and release it.
        ///     subCloud->exitRoom();
        ///     mainCloud->destroySubCloud(subCloud);
        /// </pre>
        /// </summary>
        ITRTCCloud^ createSubCloud();

        /// <summary>
        /// 1.9 销毁子房间示例
        /// </summary>
        /// <param name="subCloud">要销毁的子实例</param>
        void destroySubCloud(ITRTCCloud^ subCloud);
        /// 
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （二）CDN 相关接口函数
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name CDN 相关接口函数
        /// @{

        /// <summary>
        /// 2.1 开始向腾讯云的直播 CDN 推流
        /// 
        /// <para>该接口会指定当前用户的音视频流在腾讯云 CDN 所对应的 StreamId，进而可以指定当前用户的 CDN 播放地址。</para>
        /// 
        /// <para>例如：如果我们采用如下代码设置当前用户的主画面 StreamId 为 user_stream_001，那么该用户主画面对应的 CDN 播放地址为：</para>
        /// <para>“http://yourdomain/live/user_stream_001.flv”，其中 yourdomain 为您自己备案的播放域名，</para>
        /// <para>您可以在直播[控制台](https://console.cloud.tencent.com/live) 配置您的播放域名，腾讯云不提供默认的播放域名。</para>
        /// <para>您也可以在设置 enterRoom 的参数 TRTCParams 时指定 streamId, 而且我们更推荐您采用这种方案。</para>
        /// </summary>
        /// <example>
        /// <code>
        /// ITRTCCloud trtcCloud = ITRTCCloud.getTRTCShareInstance();
        /// trtcCloud.enterRoom(params, TRTCAppScene.TRTCAppSceneLIVE);
        /// trtcCloud.startLocalPreview(Intptr);
        /// trtcCloud.startLocalAudio();
        /// trtcCloud.startPublishing("user_stream_001", TRTCVideoStreamType.TRTCVideoStreamTypeBig);
        /// </code>
        /// </example>
        /// <remarks>
        /// 您需要先在实时音视频 [控制台](https://console.cloud.tencent.com/rav/) 中的功能配置页开启“启用旁路推流”才能生效。
        /// - 若您选择“指定流旁路”，则您可以通过该接口将对应音视频流推送到腾讯云 CDN 且指定为填写的流 ID。
        /// - 若您选择“全局自动旁路”，则您可以通过该接口调整默认的流 ID。
        /// </remarks>
        /// <param name="streamId">自定义流 ID</param>
        /// <param name="type">仅支持 TRTCVideoStreamTypeBig 和 TRTCVideoStreamTypeSub。</param>
        void startPublishing(String^ streamId, TRTCVideoStreamType type);

        /// <summary>
        /// 2.2 停止向腾讯云的直播 CDN 推流
        /// </summary>
        void stopPublishing();

        /// <summary>
        /// 2.3 开始向友商云的直播 CDN 转推
        /// 
        /// <para>该接口跟 startPublishing() 类似，但 startPublishCDNStream() 支持向非腾讯云的直播 CDN 转推。</para>
        /// <para>使用 startPublishing() 绑定腾讯云直播 CDN 不收取额外的费用，但使用 startPublishCDNStream() 绑定非腾讯云直播 CDN 需要收取转推费用。</para>
        /// </summary>
        /// <param name="param">param param CDN 转推参数，请参考 TRTCTypeDef.h 中关于 TRTCPublishCDNParam 的介绍。</param>
        void startPublishCDNStream(TRTCPublishCDNParam^% param);

        /// <summary>
        /// 2.4 停止向非腾讯云地址转推
        /// </summary>
        void stopPublishCDNStream();

        /// <summary>
        /// 2.5 设置云端的混流转码参数
        /// 
        /// <para>
        /// 如果您在实时音视频 [控制台](https://console.cloud.tencent.com/trtc/) 中的功能配置页开启了“启用旁路推流”功能，
        /// 房间里的每一路画面都会有一个默认的直播 [CDN 地址](https://cloud.tencent.com/document/product/647/16826)。
        /// 一个直播间中可能有不止一位主播，而且每个主播都有自己的画面和声音，但对于 CDN 观众来说，他们只需要一路直播流，
        /// 所以您需要将多路音视频流混成一路标准的直播流，这就需要混流转码。
        /// </para>
        /// <para>
        /// 当您调用 setMixTranscodingConfig() 接口时，SDK 会向腾讯云的转码服务器发送一条指令，目的是将房间里的多路音视频流混合为一路,
        /// 您可以通过 mixUsers 参数来调整每一路画面的位置，以及是否只混合声音，也可以通过 videoWidth、videoHeight、videoBitrate 等参数控制混合音视频流的编码参数。
        /// </para>
        /// <para>【画面1】=> 解码 ====> \                                       </para>
        /// <para>                         \                                     </para>
        /// <para>【画面2】=> 解码 =>  画面混合 => 编码 => 【混合后的画面】      </para>
        /// <para>                         /                                     </para>
        /// <para>【画面3】=> 解码 ====> /                                       </para>
        /// <para>                                                               </para>
        /// <para>【声音1】=> 解码 ====> \                                       </para>
        /// <para>                         \                                     </para>
        /// <para>【声音2】=> 解码 =>  声音混合 => 编码 => 【混合后的声音        </para>
        /// <para>                        /                                      </para>
        /// <para>【声音3】=> 解码 ====> /                                       </para>
        /// 
        /// <para>参考文档：[云端混流转码](https://cloud.tencent.com/document/product/647/16827)。</para>
        /// </summary>
        /// <remarks>
        /// 关于云端混流的注意事项：
        /// - 云端转码会引入一定的 CDN 观看延时，大概会增加1 - 2秒。
        /// - 调用该函数的用户，会将连麦中的多路画面混合到自己当前这路画面或者 config 中指定的 streamId 上。
        /// - 请注意，若您还在房间中且不再需要混流，请务必传入 nullptr 进行取消，因为当您发起混流后，云端混流模块就会开始工作，不及时取消混流可能会引起不必要的计费损失。
        /// - 请放心，您退房时会自动取消混流状态。
        /// </remarks>
        /// <param name="config">请参考 TRTCCloudDef.h 中关于 TRTCTranscodingConfig 的介绍。如果传入 NULL 取消云端混流转码。</param>
        void setMixTranscodingConfig(TRTCTranscodingConfig^ config);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （三）视频相关接口函数
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name  视频相关接口函数
        /// @{

        /// <summary>
        /// 3.1 开启本地视频的预览画面
        /// </summary>
        /// <remarks>
        /// 这个接口会启动默认的摄像头，可以通过 setCurrentCameraDevice 接口选用其他摄像头
        /// <para>当开始渲染首帧摄像头画面时，您会收到 ITRTCCloudCallback 中的 onFirstVideoFrame(null) 回调。</para>
        /// </remarks>
        /// <param name="rendView">承载预览画面的控件</param>
        void startLocalPreview(IntPtr rendView);

        /// <summary>
        /// 3.2 更新本地视频预览画面的窗口
        /// </summary>
        /// <param name="rendView">承载预览画面的控件</param>
        void updateLocalView(IntPtr rendView);

        /// <summary>
        /// 3.3 停止本地视频采集及预览
        /// </summary>
        void stopLocalPreview();

        /// <summary>
        /// 3.4 暂停/恢复推送本地的视频数据
        /// 
        /// <para>当暂停推送本地视频后，房间里的其它成员将会收到 onUserVideoAvailable(userId, false) 回调通知</para>
        /// <para>当恢复推送本地视频后，房间里的其它成员将会收到 onUserVideoAvailable(userId, true) 回调通知</para>
        /// </summary>
        /// <param name="mute">true：暂停；false：恢复，默认值：false</param>
        void muteLocalVideo(TRTCVideoStreamType streamType, bool mute);

        /// <summary>
        /// 3.5 开始拉取并显示指定用户的远端画面
        /// 
        /// <para>该函数会拉取指定 userid 的视频流显示在您指定的 view 控件上，您可以通过 setRemoteRenderParams 设置显示模式。</para>
        /// <para>- 如果您提前知道房间中某个 userid 正在推流，可以直接调用 startRemoteView 显示该用户的远端画面。</para>
        /// <para>- 如果您不知道房间中有哪些用户开启了视频，可以在 enterRoom 后等待来自 SDK 的 onUserVideoAvailable(userId, true) 回调通知。</para>
        /// <para>调用 startRemoteView 只是启动拉取，此时画面还需要加载，当加载完毕后 ITRTCCloudCallback 会通过 onFirstVideoFrame(userId) 通知您。</para>
        /// </summary>
        /// <param name="userId">指定远端用户的 userId</param>
        /// <param name="streamType">指定要观看 userId 的视频流类型：
        /// <para>- 高清大画面：({@link  TRTCVideoStreamTypeBig})</para>
        /// <para>- 低清小画面：({@link  TRTCVideoStreamTypeSmall})</para>
        /// <para>- 辅流（屏幕分享）：({@link  TRTCVideoStreamTypeSub})</para></param>
        /// <param name="rendView">承载视频画面的控件</param>
        /// <remarks>
        /// 注意几点规则需要您关注：
        /// <para>1. SDK 支持同时观看某 userid 的大画面和辅路，或者小画面和辅路，但不支持同时观看大画面和小画面。</para>
        /// <para>2. 只有当指定的 userid 通过 enableEncSmallVideoStream 开启双路编码后，才能观看该用户的小画面。</para>
        /// <para>3. 如果该用户的小画面不存在，则默认切换到大画面。</para>
        /// </remarks>
        void startRemoteView(String^ userId, TRTCVideoStreamType streamType, IntPtr rendView);

        /// <summary>
        /// 3.6 更新远端视频渲染的窗口
        /// </summary>
        /// <param name="userId">对方的用户标识</param>
        /// <param name="streamType">要设置预览窗口的流类型(TRTCVideoStreamTypeBig、TRTCVideoStreamTypeSub)</param>
        /// <param name="rendView">承载预览画面的控件</param>
        void updateRemoteView(String^ userId, TRTCVideoStreamType streamType, IntPtr rendView);

        /// <summary>
        /// 3.7 停止显示远端视频画面，同时不再拉取远端用户的视频数据流
        /// 
        /// <para>调用此接口后，SDK 会停止接收该用户的远程视频流，同时会清理相关的视频显示资源。</para>
        /// </summary>
        /// <param name="userId">指定远端用户的 userId</param>
        /// <param name="streamType">指定要停止观看的 userId 的视频流类型</param>
        void stopRemoteView(String^ userId, TRTCVideoStreamType streamType);

        /// <summary>
        /// 3.8 停止显示所有远端视频画面，同时不再拉取远端用户的视频数据流
        /// </summary>
        /// <remarks>
        /// 如果有屏幕分享的画面在显示，则屏幕分享的画面也会一并被关闭
        /// </remarks>
        void stopAllRemoteView();

        /// <summary>
        /// 3.9 暂停/恢复接收指定的远端视频流
        ///
        /// <para>该接口仅暂停/恢复接收指定的远端用户的视频流，但并不释放显示资源，所以如果暂停，视频画面会冻屏在 mute 前的最后一帧。</para>
        /// </summary>
        /// <param name="userId">对方的用户标识</param>
        /// <param name="streamType">要暂停/恢复的视频流类型（仅支持 {@link TRTCVideoStreamTypeBig} 和 {@link TRTCVideoStreamTypeSub}）</param>
        /// <param name="mute">是否暂停接收</param>
        /// <remarks>
        /// 该接口支持您在进入房间（enterRoom）前调用，暂停状态会在退出房间（exitRoom）在之后会被重置。
        /// </remarks>
        void muteRemoteVideoStream(String^ userId, TRTCVideoStreamType streamType, bool mute);

        /// <summary>
        /// 3.10 暂停/恢复接收所有远端视频流
        /// </summary>
        /// <para>该接口仅暂停/恢复接收所有远端用户的视频流，但并不释放显示资源，所以如果暂停，视频画面会冻屏在 mute 前的最后一帧。</para>
        /// <param name="mute">是否停止接收</param>
        void muteAllRemoteVideoStreams(bool mute);

        /// <summary>
        /// 3.11 设置视频编码器相关参数
        ///
        /// <para>该设置决定了远端用户看到的画面质量（同时也是云端录制出的视频文件的画面质量）</para>
        /// </summary>
        /// <param name="params">视频编码参数，详情请参考 TRTCCloudDef.h 中 TRTCVideoEncParam 的定义</param>
        void setVideoEncoderParam(TRTCVideoEncParam^% params);

        /// <summary>
        /// 3.12 设置网络流控相关参数
        /// 
        /// <para>该设置决定了 SDK 在各种网络环境下的调控策略（例如弱网下是“保清晰”还是“保流畅”）</para>
        /// </summary>
        /// <param name="params">网络流控参数，详情请参考 TRTCCloudDef.h 中 TRTCNetworkQosParam 的定义</param>
        void setNetworkQosParam(TRTCNetworkQosParam^% params);

        /// <summary>
        /// 3.13 设置本地图像（主流）的渲染参数
        /// </summary>
        /// <param name="params">本地图像的参数，详情请参考 TRTCTypeDef.h 中的 TRTCRenderParams 定义</param>
        void setLocalRenderParams(TRTCRenderParams^% params);

        /// <summary>
        /// 3.14 设置远端图像的渲染模式
        /// </summary>
        /// <param name="userId">对应的远端视频流用户ID</param>
        /// <param name="streamType">远端图像的视频流类型，详见 TRTCVideoStreamType 定义</param>
        /// <param name="params">远端图像的参数，详情请参考 TRTCTypeDef.h 中的 TRTCRenderParams 定义</param>
        void setRemoteRenderParams(String^ userId, TRTCVideoStreamType streamType, TRTCRenderParams^% params);

        /// <summary>
        /// 3.15 设置视频编码输出的画面方向，即设置远端用户观看到的和服务器录制的画面方向
        /// </summary>
        /// <param name="rotation">目前支持 TRTCVideoRotation0 和 TRTCVideoRotation180 旋转角度，默认值：TRTCVideoRotation0</param>
        void setVideoEncoderRotation(TRTCVideoRotation rotation);

        /// <summary>
        /// 3.16 设置编码器输出的画面镜像模式
        ///
        /// <para>该接口不改变本地摄像头的预览画面，但会改变另一端用户看到的（以及服务器录制的）画面效果。</para>
        /// </summary>
        /// <param name="mirror">是否开启远端镜像, true：远端画面镜像；false：远端画面非镜像。默认值：false</param>
        void setVideoEncoderMirror(bool mirror);

        /// <summary>
        /// 3.17 开启大小画面双路编码模式
        /// 
        /// <para>如果当前用户是房间中的主要角色（例如主播、老师、主持人等），并且使用 PC 或者 Mac 环境，可以开启该模式。</para>
        /// <para>开启该模式后，当前用户会同时输出【高清】和【低清】两路视频流（但只有一路音频流）。</para>
        /// <para>对于开启该模式的当前用户，会占用更多的网络带宽，并且会更加消耗 CPU 计算资源。</para>
        ///
        /// <para>对于同一房间的远程观众而言：</para>
        /// <para>- 如果用户的下行网络很好，可以选择观看【高清】画面</para>
        /// <para>- 如果用户的下行网络较差，可以选择观看【低清】画面</para>
        /// </summary>
        /// <param name="enable">是否开启小画面编码，默认值：false</param>
        /// <param name="smallVideoParam">小流的视频参数</param>
        void enableSmallVideoStream(bool enable, TRTCVideoEncParam^% smallVideoParam);

        /// <summary>
        /// 3.18 选定观看指定 userId 的大画面还是小画面
        /// <para>此功能需要该 userId 通过 enableEncSmallVideoStream 提前开启双路编码模式。</para>
        /// <para>如果该 userId 没有开启双路编码模式，则此操作无效。</para>
        /// </summary>
        /// <param name="userId">用户 ID</param>
        /// <param name="type">视频流类型，即选择看大画面还是小画面，默认为
        /// TRTCVideoStreamTypeBig</param>
        void setRemoteVideoStreamType(String^ userId, TRTCVideoStreamType type);

        /// <summary>
        /// 3.19 视频画面截图
        ///
        /// <para>截取本地主路、本地辅路、远程主路和远端辅流的视频画面，并通过 ITRTCCloudCallback 的 onSnapshotComplete 回调返回截图画面的数据给您。</para>
        /// </summary>
        /// <param name="userId">用户 ID，空字符串表示截取本地画面</param>
        /// <param name="streamType">视频流类型，支持摄像头画面（{@link TRTCVideoStreamTypeBig}）和屏幕分享画面（{@link TRTCVideoStreamTypeSub}）</param>
        /// <param name="sourceType">视频截图来源类型，Windows 端只支持 {@link TRTCSnapshotSourceTypeStream}</param>
        void snapshotVideo(String^ userId, TRTCVideoStreamType streamType, TRTCSnapshotSourceType sourceType);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （四）音频相关接口函数
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 音频相关接口函数
        /// @{

        /// <summary>
        /// 4.1 开启本地音频的采集和上行
        /// 
        /// <para>该函数会启动麦克风采集，并将音频数据传输给房间里的其他用户。</para>
        /// <para>SDK 并不会默认开启本地的音频上行，也就说，如果您不调用这个函数，房间里的其他用户就听不到您的声音。</para>
        /// </summary>
        /// <remarks>
        /// TRTC SDK 并不会默认打开本地的麦克风采集。
        /// </remarks>
        /// <param name="quality">声音质量，参见 TRTCAudioQuality</param>
        void startLocalAudio(TRTCAudioQuality quality);

        /// <summary>
        /// 4.2 关闭本地音频的采集和上行
        ///
        /// <para>当关闭本地音频的采集和上行，房间里的其它成员会收到 onUserAudioAvailable(false) 回调通知。</para>
        /// </summary>
        void stopLocalAudio();

        /// <summary>
        /// 4.3 静音/取消静音本地的音频
        ///
        /// <para>当静音本地音频后，房间里的其它成员会收到 onUserAudioAvailable(userId, false) 回调通知。</para>
        /// <para>当取消静音本地音频后，房间里的其它成员会收到 onUserAudioAvailable(userId, true) 回调通知。</para>
        /// <para>与 stopLocalAudio 不同之处在于，muteLocalAudio(true) 并不会停止发送音视频数据，而是继续发送码率极低的静音包。</para>
        /// <para>由于 MP4 等视频文件格式，对于音频的连续性是要求很高的，使用 stopLocalAudio 会导致录制出的 MP4 不易播放。</para>
        /// <para>因此在对录制质量要求很高的场景中，建议选择 muteLocalAudio，从而录制出兼容性更好的 MP4 文件。</para>
        /// </summary>
        /// <param name="mute">true：静音；false：取消静音，默认值：false</param>
        void muteLocalAudio(bool mute);

        /// <summary>
        /// 4.4 静音/取消静音指定的远端用户的声音
        /// </summary>
        /// <param name="userId">用户 ID</param>
        /// <param name="mute">true：静音；false：取消静音</param>
        /// <remarks>
        /// 静音时会停止接收该用户的远端音频流并停止播放，取消静音时会自动拉取该用户的远端音频流并进行播放。
        /// </remarks>
        void muteRemoteAudio(String^ userId, bool mute);

        /// <summary>
        /// 4.5 静音/取消静音所有用户的声音
        /// </summary>
        /// <param name="mute">true：静音；false：取消静音</param>
        /// <remarks>
        /// 静音时会停止接收所有用户的远端音频流并停止播放，取消静音时会自动拉取所有用户的远端音频流并进行播放。
        /// </remarks>
        void muteAllRemoteAudio(bool mute);

        /// <summary>
        /// 4.6 设置某个远程用户的播放音量
        /// </summary>
        /// <param name="userId">远程用户 ID</param>
        /// <param name="volume">音量大小，100为原始音量，范围是：[0 ~ 150]，默认值为100 </param>
        void setRemoteAudioVolume(String^ userId, int volume);

        /// <summary>
        /// 4.7 设置 SDK 采集音量
        /// </summary>
        /// <param name="volume">音量大小，取值0 - 100，默认值为100</param>
        void setAudioCaptureVolume(int volume);

        /// <summary>
        /// 4.8 获取 SDK 采集音量
        /// </summary>
        int getAudioCaptureVolume();

        /// <summary>
        /// 4.9 设置 SDK 播放音量。
        /// </summary>
        /// <remarks>
        /// 该函数会控制最终交给系统播放的声音音量
        /// <para>会影响本地录制的音量大小。</para>
        /// <para>不会影响耳返的音量。</para>
        /// </remarks>
        /// <param name="volume">音量大小，取值0 - 100，默认值为100</param>
        void setAudioPlayoutVolume(int volume);

        /// <summary>
        /// 4.10 获取 SDK 播放音量
        /// </summary>
        int getAudioPlayoutVolume();

        /// <summary>
        /// 4.11 启用或关闭音量大小提示
        ///
        /// <para>开启此功能后，SDK 会在 onUserVoiceVolume() 中反馈对每一路声音音量大小值的评估。</para>
        /// <para>我们在 Demo 中有一个音量大小的提示条，就是基于这个接口实现的。</para>
        /// <para>如希望打开此功能，请在 startLocalAudio() 之前调用。</para>
        /// </summary>
        /// <param name="interval">设置 onUserVoiceVolume 回调的触发间隔，单位为ms，最小间隔为100ms，如果小于等于0则会关闭回调，建议设置为300ms</param>
        void enableAudioVolumeEvaluation(UInt32 interval);

        /// <summary>
        /// 4.12 开始录音
        /// 
        /// <para>该方法调用后，SDK 会将通话过程中的所有音频(包括本地音频，远端音频，BGM等)录制到一个文件里。</para>
        /// <para>无论是否进房，调用该接口都生效。</para>
        /// <para>如果调用 exitRoom 时还在录音，录音会自动停止。</para>
        /// </summary>
        /// <param name="audioRecordingParams">录音参数，请参考 TRTCAudioRecordingParams</param>
        /// <returns>0：成功；-1：录音已开始；-2：文件或目录创建失败；-3：后缀指定的音频格式不支持</returns>
        int startAudioRecording(TRTCAudioRecordingParams^% audioRecordingParams);

        /// <summary>
        /// 4.13 停止录音
        /// <para>如果调用 exitRoom 时还在录音，录音会自动停止。</para>
        /// </summary>
        void stopAudioRecording();

        /// <summary>
        /// 4.14 开启本地录制
        /// </summary>
        /// <param name="volume">录制参数</param>
        void startLocalRecording(TRTCLocalRecordingParams^% params);

        /// <summary>
        /// 4.15 停止本地录制
        /// </summary>
        void stopLocalRecording();
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （五）设备相关接口函数
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 设备相关接口函数
        /// @{

        /// <summary>
        /// 5.1 获取设备管理模块
        /// </summary>
        /// <returns>ITXDeviceManager 设备管理类</returns>
        ITXDeviceManager^ getDeviceManager();
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （六）美颜特效和图像水印
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 美颜特效和图像水印
        /// @{

        /// <summary>
        /// 6.1 设置美颜、美白、红润效果级别
        ///
        /// <para>SDK 内部集成了两套风格不同的磨皮算法，一套我们取名叫“光滑”，适用于美女秀场，效果比较明显。</para>
        /// <para>另一套我们取名“自然”，磨皮算法更多地保留了面部细节，主观感受上会更加自然。</para>
        /// </summary>
        /// <param name="style">美颜风格，光滑或者自然，光滑风格磨皮更加明显，适合娱乐场景。</param>
        /// <param name="beauty">美颜级别，取值范围0 - 9，0表示关闭，1 - 9值越大，效果越明显</param>
        /// <param name="white">美白级别，取值范围0 - 9，0表示关闭，1 - 9值越大，效果越明显</param>
        /// <param name="ruddiness">红润级别，取值范围0 - 9，0表示关闭，1 - 9值越大，效果越明显，该参数暂未生效</param>
        void setBeautyStyle(TRTCBeautyStyle style, UInt32 beauty, UInt32 white, UInt32 ruddiness);


        /// <summary>
        /// 6.2 设置水印
        ///
        /// <para>水印的位置是通过 xOffset, yOffset, fWidthRatio 来指定的。</para>
        /// <para>-xOffset：水印的坐标，取值范围为0 - 1的浮点数。</para>
        /// <para>-yOffset：水印的坐标，取值范围为0 - 1的浮点数。</para>
        /// <para>-fWidthRatio：水印的大小比例，取值范围为0 - 1的浮点数。</para>
        /// </summary>
        /// <remarks>
        /// 只支持主路视频流
        /// </remarks>
        /// <param name="streamType">要设置水印的流类型(TRTCVideoStreamTypeBig、TRTCVideoStreamTypeSub)</param>
        /// <param name="srcData">水印图片源数据（传 NULL 表示去掉水印）</param>
        /// <param name="srcType">水印图片源数据类型（传 NULL 时忽略该参数）</param>
        /// <param name="nWidth">水印图片像素宽度（源数据为文件路径时忽略该参数）</param>
        /// <param name="nHeight">水印图片像素高度（源数据为文件路径时忽略该参数）</param>
        /// <param name="xOffset">水印显示的左上角 x 轴偏移</param>
        /// <param name="yOffset">水印显示的左上角 y 轴偏移</param>
        /// <param name="fWidthRatio">水印显示的宽度占画面宽度比例（水印按该参数等比例缩放显示）</param>
        void setWaterMark(TRTCVideoStreamType streamType, String^ srcData, TRTCWaterMarkSrcType srcType, UInt32 nWidth, UInt32 nHeight, float xOffset, float yOffset, float fWidthRatio);

        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （七）音乐特效和人声特效
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 音乐特效和人声特效
        /// @{

        /// <summary>
        /// 7.1 获取音效管理类 ITXAudioEffectManager
        /// </summary>
        /// <para>该模块是整个 SDK 的音效管理模块，支持如下功能：</para>
        /// <para>- 耳机耳返：麦克风捕捉的声音实时通过耳机播放。</para>
        /// <para>- 混响效果：KTV、小房间、大会堂、低沉、洪亮...</para>
        /// <para>- 变声特效：萝莉、大叔、重金属、外国人...</para>
        /// <para>- 背景音乐：支持在线音乐和本地音乐，支持变速、变调等特效、支持原生和伴奏并播放和循环播放。</para>
        /// <para>- 短音效：鼓掌声、欢笑声等简短的音效文件，对于小于10秒的文件，请将 isShortFile 参数设置为 YES。</para>
        ITXAudioEffectManager^ getAudioEffectManager();

        /// <summary>
        /// 7.2 打开系统声音采集
        /// </summary>
        /// <para>该接口会从电脑的声卡中采集音频数据，并将其混入到 SDK 当前的音频数据流中，从而使房间中的其他用户也能听到主播的电脑所播放出的声音。</para>
        /// <para>在线教育场景中，老师可以使用此功能让 SDK 采集教学影片中的声音，并广播给同房间中的学生。</para>
        /// <para>音乐直播场景中，主播可以使用此功能让 SDK 采集音乐播放器中的音乐，从而为自己的直播间增加背景音乐。</para> 
        /// <param name="deviceName"> 
        /// <para> deviceName 您可以指定该参数为空置（nullptr），代表让 SDK 采集整个系统的声音。</para> 
        /// <para> 在 Windows 平台下，您也可以将参数 deviceName设置为某个应用程序的可执行文件（如 QQMuisc.exe）的绝对路径，此时 SDK只会采集该应用程序的声音（仅支持 32 位版本的 SDK）。</para>
        /// <para> 您也可以指定该参数为某个扬声器设备的名称或PID来采集特定扬声器声音（通过接口 {@linkXDeviceManager} 中的 getDevicesList 接口，可以获取类型为 {@linkTXMediaDeviceTypeSpeaker} 的扬声器设备）</para>
        /// </param>
        void startSystemAudioLoopback(String^ deviceName);

        /// <summary>
        /// 7.3 关闭系统声音采集。
        /// </summary>
        void stopSystemAudioLoopback();

        /// <summary>
        /// 7.4 设置系统声音采集的音量。
        /// </summary>
        /// <param name="volume">音量大小，取值范围为0 - 100。</param>
        void setSystemAudioLoopbackVolume(UInt32 volume);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （八）屏幕分享相关接口函数
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 屏幕分享相关接口函数
        /// @{

        /// <summary>
        /// 8.1 启动屏幕分享
        /// </summary>
        /// <remarks>
        /// 一个用户同时最多只能上传一条主路（TRTCVideoStreamTypeBig）画面和一条辅路（TRTCVideoStreamTypeSub）画面，
        /// 默认情况下，屏幕分享使用辅路画面，如果使用主路画面，建议您提前停止摄像头采集（stopLocalPreview）避免相互冲突。
        ///</remarks>
        /// <param name="rendView">承载预览画面的 HWND，可以设置为 nullptr，表示不显示屏幕分享的预览效果。</param>
        /// <param name="type">屏幕分享使用的线路，可以设置为主路（TRTCVideoStreamTypeBig）或者辅路（TRTCVideoStreamTypeSub），默认使用辅路。</param>
        /// <param name="params">屏幕分享的画面编码参数，可以设置为 nullptr，表示让 SDK 选择最佳的编码参数（分辨率、码率等）。</param>
        void startScreenCapture(IntPtr rendView, TRTCVideoStreamType type, TRTCVideoEncParam^ params);

        /// <summary>
        /// 8.2 停止屏幕采集
        /// </summary>
        void stopScreenCapture();

        /// <summary>
        /// 8.3 暂停屏幕分享
        /// </summary>
        void pauseScreenCapture();

        /// <summary>
        /// 8.4 恢复屏幕分享
        /// </summary>
        void resumeScreenCapture();

        /// <summary>
        /// 8.5 枚举可分享的屏幕窗口，建议在 startScreenCapture 之前调用
        /// 
        /// <para>如果您要给您的 App 增加屏幕分享功能，一般需要先显示一个窗口选择界面，这样用户可以选择希望分享的窗口。</para>
        /// <para>通过如下函数，您可以获得可分享窗口的 ID、类型、窗口名称以及缩略图。</para>
        /// <para>拿到这些信息后，您就可以实现一个窗口选择界面，当然，您也可以使用我们在 Demo 源码中已经实现好的一个界面。</para>
        /// </summary>
        /// <remarks>
        /// 返回的列表中包括屏幕和应用窗口，屏幕会在列表的前面几个元素中。
        /// <para>SDK 维护 ITRTCScreenCaptureSourceList 对象的生命周期。</para>
        /// </remarks>
        /// <param name="thumbSize">指定要获取的窗口缩略图大小，缩略图可用于绘制在窗口选择界面上</param>
        /// <param name="iconSize">指定要获取的窗口图标大小</param>
        /// <returns>窗口列表包括屏幕</returns>
        ITRTCScreenCaptureSourceList^ getScreenCaptureSources(SIZE^% thumbSize, SIZE^% iconSize);

        /// <summary>
        /// 8.6 设置屏幕共享参数，该方法在屏幕共享过程中也可以调用
        ///
        /// <para>如果您期望在屏幕分享的过程中，切换想要分享的窗口，可以再次调用这个函数而不需要重新开启屏幕分享。</para>
        ///
        /// <para>支持如下四种情况：</para>
        /// <para>-共享整个屏幕：sourceInfoList 中 type 为 Screen 的 source，captureRect 设为{ 0, 0, 0, 0 }</para>
        /// <para>-共享指定区域：sourceInfoList 中 type 为 Screen 的 source，captureRect 设为非 NULL，例如{ 100, 100, 300, 300 }</para>
        /// <para>-共享整个窗口：sourceInfoList 中 type 为 Window 的 source，captureRect 设为{ 0, 0, 0, 0 }</para>
        /// <para>-共享窗口区域：sourceInfoList 中 type 为 Window 的 source，captureRect 设为非 NULL，例如{ 100, 100, 300, 300 }</para>
        /// </summary>
        /// <param name="source">指定分享源</param>
        /// <param name="captureRect">指定捕获的区域</param>
        /// <param name="property">指定屏幕分享目标的属性，包括捕获鼠标，高亮捕获窗口等，详情参考 TRTCScreenCaptureProperty 定义</param>
        void selectScreenCaptureTarget(TRTCScreenCaptureSourceInfo^% source, RECT^% captureRect, TRTCScreenCaptureProperty^% property);

        /// <summary>
        /// 8.7 设置屏幕分享的编码器参数
        /// </summary>
        /// <para>- setVideoEncoderParam() 用于设置远端主路画面（TRTCVideoStreamTypeBig，一般用于摄像头）的编码参数。</para>
        /// <para>- setSubStreamEncoderParam() 用于设置远端辅路画面（TRTCVideoStreamTypeSub，一般用于屏幕分享）的编码参数。</para>
        /// <para>该设置决定远端用户看到的画面质量，同时也是云端录制出的视频文件的画面质量。</para>
        /// <remarks>即使使用主路传输屏幕分享的数据（在调用 startScreenCapture 时设置 type=TRTCVideoStreamTypeBig），依然要使用此接口更新屏幕分享的编码参数。</remarks>
        /// <param name="params">辅流编码参数，详情请参考 TRTCCloudDef.h 中的 TRTCVideoEncParam 定义</param>
        void setSubStreamEncoderParam(TRTCVideoEncParam^% params);

        /// <summary>
        /// 8.8 设置屏幕分享的混音音量大小
        /// </summary>
        /// <para>这个数值越高，屏幕分享音量的占比就越高，麦克风音量占比就越小，所以不推荐设置得太大，否则麦克风的声音就被压制了。</para>
        /// <param name="volume">设置的混音音量大小，范围0 - 100</param>
        void setSubStreamMixVolume(UInt32 volume);

        /// <summary>
        /// 8.9 将指定窗口加入屏幕分享的排除列表中，加入排除列表中的窗口不会被分享出去
        /// 
        /// <para>支持启动屏幕分享前设置过滤窗口，也支持屏幕分享过程中动态添加过滤窗口。</para>
        /// </summary>
        /// <param name="window">不希望分享出去的窗口</param>
        void addExcludedShareWindow(IntPtr window);

        /// <summary>
        /// 8.10 将指定窗口从屏幕分享的排除列表中移除
        /// </summary>
        /// <param name="window">不希望分享出去的窗口</param>
        void removeExcludedShareWindow(IntPtr window);

        /// <summary>
        /// 8.11 将所有窗口从屏幕分享的排除列表中移除
        /// </summary>
        void removeAllExcludedShareWindow();


        /// <summary>
        /// 8.12 将指定窗口加入屏幕分享的包含列表中，加入包含列表中的窗口如果在采集窗口区域内会被分享出去
        /// <para>支持启动屏幕分享前设置包含的窗口，也支持屏幕分享过程中动态添加包含的窗口</para>
        /// <para>该方法只有在 TRTCScreenCaptureSourceInfo 中的 type 指定为 TRTCScreenCaptureSourceTypeWindow 时生效，即分享窗口时生效</para>
        /// <para>该方法添加的窗口列表会在退房后清除</para>
        /// </summary>
        /// <param name="window">窗口句柄</param>
        void addIncludedShareWindow(IntPtr window);

        /// <summary>
        /// 8.13 将指定窗口从屏幕分享的包含列表中移除
        /// <para>该方法只有在 TRTCScreenCaptureSourceInfo 中的 type 指定为 TRTCScreenCaptureSourceTypeWindow 时生效，即分享窗口时生效</para>
        /// </summary>
        /// <param name="window">窗口句柄</param>
        void removeIncludedShareWindow(IntPtr window);

        /// <summary>
        /// 8.14 将所有窗口从屏幕分享的包含列表中移除
        /// <para>该方法只有在 TRTCScreenCaptureSourceInfo 中的 type 指定为 TRTCScreenCaptureSourceTypeWindow 时生效，即分享窗口时生效</para>
        /// </summary>
        void removeAllIncludedShareWindow();
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （九）自定义采集和渲染
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        /// 9.1 启用视频自定义采集模式
        ///
        /// <para>开启该模式后，SDK 不再运行原有的视频采集流程，只保留编码和发送能力。</para>
        /// <para>您需要用 sendCustomVideoData() 不断地向 SDK 塞入自己采集的视频画面。</para>
        /// </summary>
        /// <param name="type">指定视频流类型</param>
        /// <param name="enable">是否启用，默认值：false</param>
        void enableCustomVideoCapture(TRTCVideoStreamType type, bool enable);

        /// <summary>
        /// 9.2 向 SDK 投送自己采集的视频数据
        ///
        /// <para>TRTCVideoFrame 推荐如下填写方式（其他字段不需要填写）：</para>
        /// <para>-pixelFormat：仅支持 LiteAVVideoPixelFormat_I420。</para>
        /// <para>-bufferType：仅支持 LiteAVVideoBufferType_Buffer。</para>
        /// <para>-data：视频帧 buffer。</para>
        /// <para>-length：视频帧数据长度，I420 格式下，其值等于：width × height × 3 / 2。</para>
        /// <para>-width：视频图像长度。</para>
        /// <para>-height：视频图像宽度。</para>
        /// <para>-timestamp：如果 timestamp 间隔不均匀，会严重影响音画同步和录制出的 MP4 质量。</para>
        ///
        /// <para>参考文档：[自定义采集和渲染](https://cloud.tencent.com/document/product/647/34066)。</para>
        /// </summary>
        /// <remarks>
        /// - SDK 内部有帧率控制逻辑，目标帧率以您在 setVideoEncoderParam 中设置的为准，太快会自动丢帧，太慢则会自动补帧。
        /// <para>- 可以设置 frame 中的 timestamp 为 0，相当于让 SDK 自己设置时间戳，但请“均匀”地控制 sendCustomVideoData 的调用间隔，否则会导致视频帧率不稳定。</para>
        /// </remarks>
        /// <param name="type">视频流类型</param>
        /// <param name="frame">视频数据，支持 I420 格式数据。</param>
        void sendCustomVideoData(TRTCVideoStreamType type, TRTCVideoFrame^ frame);

        /// <summary>
        /// 9.3 启用音频自定义采集模式
        ///
        /// <para>开启该模式后，SDK 停止运行原有的音频采集流程，只保留编码和发送能力。</para>
        /// <para>您需要用 sendCustomAudioData() 不断地向 SDK 塞入自己采集的音频数据。</para>
        /// </summary>
        /// <param name="enable">是否启用，默认值：false</param>
        void enableCustomAudioCapture(bool enable);

        /// <summary>
        /// 9.4 向 SDK 投送自己采集的音频数据
        /// <para>TRTCAudioFrame 推荐如下填写方式：</para>
        /// <para>-audioFormat：音频数据格式，仅支持 TRTCAudioFrameFormatPCM。</para>
        /// <para>-data：音频帧 buffer。</para>
        /// <para>-length：音频帧数据长度，支持[5ms ~ 100ms]帧长，推荐使用20 ms帧长，【48000采样率、单声道的帧长度：48000 × 0.02s × 1 × 16bit = 15360bit = 1920字节】。</para>
        /// <para>-sampleRate：采样率，支持：16000、24000、32000、44100、48000。</para>
        /// <para>-channel：声道数（如果是立体声，数据是交叉的），单声道：1； 双声道：2。</para>
        /// <para>-timestamp：时间戳，单位毫秒（ms），可以设置 timestamp 为0，相当于让 SDK 自己设置时间戳。</para>
        ///
        /// <para>参考文档：[自定义采集和渲染](https://cloud.tencent.com/document/product/647/34066)。</para>
        /// </summary>
        /// <remarks>
        /// 需精准的按每帧时长的间隔调用 sendCustomAudioData，否则会容易触发声音卡顿。
        /// </remarks>
        /// <param name="frame">音频数据</param>
        void sendCustomAudioData(TRTCAudioFrame^ frame);

        /// <summary>
        /// 9.5 控制外部音频是否要混入推流和混入播放
        /// <para>通过 mixExternalAudioFrame()
        /// 增加一路音频混合到推流的音频流，同时可以支持本地播放</para> <para>enablePublish =
        /// false， enablePlayout = false时，表示完全关闭这个额外的音频流，即不推流，也不播放</para>
        /// </summary>
        /// <param name="enablePublish">是否混入推流
        /// true：混入推流；false：不混入推流，默认值：false</param> <param
        /// name="enablePlayout">是否混入播放
        /// true：混入播放；false：不混入播放，默认值：false</param>
        void enableMixExternalAudioFrame(bool enablePublish, bool enablePlayout);

        /// <summary>
        /// 9.6 向 SDK 发送自定义辅流音频数据
        /// <para>RTCAudioFrame 推荐如下填写方式（其他字段不需要填写）：</para>
        /// <para>- audioFormat：仅支持 TRTCAudioFrameFormatPCM.</para>
        /// <para>- data：音频帧 buffer。</para>
        /// <para>-
        /// length：音频帧数据长度，推荐每帧20ms采样数。【PCM格式、48000采样率、单声道的帧长度：48000
        /// ×</para> <para>0.02s × 1 × 16bit = 15360bit = 1920字节】。</para> <para>-
        /// sampleRate：采样率，仅支持48000。</para> <para>-
        /// channel：频道数量（如果是立体声，数据是交叉的），单声道：1； 双声道：2。</para>
        /// <para>可以设置 frame 中的 timestamp 为 0，相当于让 SDK
        /// 自己设置时间戳，但请“均匀”地控制</para> <para>mixExternalAudioFrame
        /// 的调用间隔，否则会导致声音断断续续。</para>
        /// </summary>
        /// <param name="frame">音频帧，仅支持
        /// TRTCAudioFrameFormatPCM格式。目前只支持单声道，仅支持48K采样率，TRTCAudioFrameFormatPCM
        /// 格式< / param>
        int mixExternalAudioFrame(TRTCAudioFrame^ frame);

        /// <summary>
        /// 9.7 设置推流时混入外部音频的推流音量和播放音量
        /// </summary>
        /// <param name="publishVolume"> 设置的推流音量大小，范围0 - 100, -1表示不改变</param>
        /// <param name="playoutVolume"> 设置的播放音量大小，范围0 - 100, -1表示不改变</param>
        void setMixExternalAudioVolume(int publishVolume, int playoutVolume);

        /// <summary>
        /// 9.8 生成自定义采集时间戳
        /// <para>此函数仅适合自定义视频采集时使用，当您的 App 自己或由第三方美颜 SDK 调用摄像头 API
        /// 采集视频时，由于可能引入一些耗时的外部操作（比</para>
        /// <para>为避免发生音画不同步的问题，请按照如下步骤正确使用该接口：</para>
        /// <para>1. 在调用系统相机 API 采集到一帧视频时，额外调用一次 generateCustomPTS() 获得 pts
        /// 时间戳。</para> <para>2. 在调用 {@link sendCustomVideoData()} 时，将该帧采集时记录的 pts
        /// 时间戳赋值给入参 TRTCVideoFrame 中的 timestamp 字段。</para>
        /// </summary>
        /// <returns>时间戳（单位：ms）</returns>
        UInt64 generateCustomPTS();

        /// <summary>
        /// 9.9 设置第三方美颜的视频数据回调
        /// </summary>
        /// <remarks>
        /// <para>设置该回调之后，SDK 会把采集到的视频帧通过您设置的 callback 回调出来，用于第三方美颜组件进行二次处理，之后 SDK 会将处理后的视频帧进行编码和发送。</para>
        /// </remarks>
        /// <param name="pixelFormat">指定回调的像素格式</param>
        /// <param name="bufferType">指定视频数据结构类型</param>
        /// <param name="callback">自定义美颜回调</param>
        /// <returns>0：成功；&lt;0：错误</returns>
        int setLocalVideoProcessCallback(TRTCVideoPixelFormat pixelFormat, TRTCVideoBufferType bufferType, ITRTCVideoFrameCallback^ callback);

        /// <summary>
        /// 9.10 设置本地视频自定义渲染
        /// </summary>
        /// <remarks>
        /// 设置此方法，SDK 内部会把采集到的数据回调出来，SDK 跳过 HWND 渲染逻辑
        /// <para>调用 setLocalVideoRenderCallback(TRTCVideoPixelFormat_Unknown, TRTCVideoBufferType_Unknown, nullptr) 停止回调</para>
        /// </remarks>
        /// <param name="pixelFormat">指定回调的像素格式</param>
        /// <param name="bufferType">指定视频数据结构类型</param>
        /// <param name="callback">自定义渲染回调</param>
        /// <returns>0：成功；&lt;0：错误</returns>
        int setLocalVideoRenderCallback(TRTCVideoPixelFormat pixelFormat, TRTCVideoBufferType bufferType, ITRTCVideoRenderCallback^ callback);

        /// <summary>
        /// 9.11 设置远端视频自定义渲染
        ///
        /// <para>此方法同 setLocalVideoRenderDelegate，区别在于一个是本地画面的渲染回调， 一个是远程画面的渲染回调。</para>
        /// </summary>
        /// <remarks>
        /// 设置此方法，SDK 内部会把远端的数据解码后回调出来，SDK 跳过 HWND 渲染逻辑
        /// <para>调用 setRemoteVideoRenderCallback(userId, TRTCVideoPixelFormat_Unknown,  TRTCVideoBufferType_Unknown, nullptr) 停止回调。</para>
        /// </remarks>
        /// <param name="userId">用户标识</param>
        /// <param name="pixelFormat">指定回调的像素格式</param>
        /// <param name="bufferType">指定视频数据结构类型</param>
        /// <param name="callback">自定义渲染回调</param>
        /// <returns>0：成功；&lt;0：错误</returns>
        int setRemoteVideoRenderCallback(String^ userId, TRTCVideoPixelFormat pixelFormat, TRTCVideoBufferType bufferType, ITRTCVideoRenderCallback^ callback);

        /// <summary>
        /// 9.12 设置音频数据回调
        /// <para>设置此方法，SDK 内部会把声音模块的数据（PCM 格式）回调出来，包括：</para>
        /// <para>-onCapturedAudioFrame：本机麦克风采集到的音频数据</para>
        /// <para>-onPlayAudioFrame：混音前的每一路远程用户的音频数据</para>
        /// <para>-onMixedPlayAudioFrame：各路音频数据混合后送入扬声器播放的音频数据</para>
        /// </summary>
        /// <param name="callback">声音帧数据（PCM 格式）的回调，callback = nullptr 则停止回调数据</param>
        /// <returns>0：成功；&lt;0：错误</returns>
        int setAudioFrameCallback(ITRTCAudioFrameCallback^ callback);

        /// <summary>
        /// 9.13 设置本地麦克风采集出的原始音频帧回调格式
        /// <para>本接口用于设置 {@link onCapturedRawAudioFrame} 回调出来的 AudioFrame 的格式:</para>
        /// <para>- sampleRate：采样率，支持：16000、32000、44100、48000。</para>
        /// <para>- channel：声道数（如果是立体声，数据是交叉的），单声道：1； 双声道：2。</para>
        /// <para>- samplesPerCall：采样点数，定义回调数据帧长。帧长必须为 10ms 的整数倍。</para>
        /// </summary>
        /// <remarks>
        /// <para>如果希望用毫秒数计算回调帧长，则将毫秒数转换成采样点数的公式为：采样点数 = 毫秒数 * 采样率 / 1000；</para>
        /// <para>举例：48000 采样率希望回调 20ms 帧长的数据，则采样点数应该填: 960 = 20 * 48000 / 1000；</para>
        /// <para>注意，最终回调的帧长度是以字节为单位，采样点数转换成字节数的计算公式为：字节数 = 采样点数 * channel * 2（位宽）</para>
        /// <para>举例：48000 采样率，双声道，20ms 帧长，采样点数为 960，字节数为 3840 = 960 * 2 * 2</para>
        /// </remarks>
        /// <param name="format">音频数据回调格式</param> 
        /// <returns>0：成功；&lt;0：错误</returns>
        int setCapturedRawAudioFrameCallbackFormat(TRTCAudioFrameCallbackFormat^ format);

        /// <summary>
        /// 9.14 设置经过前处理后的本地音频帧回调格式
        /// <para>本接口用于设置 {@link onLocalProcessedAudioFrame} 回调出来的 AudioFrame 的格式:</para> 
        /// <para>- sampleRate：采样率，支持：16000、32000、44100、48000。</para>
        /// <para>- channel：声道数（如果是立体声，数据是交叉的），单声道：1； 双声道：2。</para>
        /// <para>- samplesPerCall：采样点数，定义回调数据帧长。帧长必须为 10ms 的整数倍。</para>
        /// </summary>
        /// <remarks>
        /// <para>如果希望用毫秒数计算回调帧长，则将毫秒数转换成采样点数的公式为：采样点数 = 毫秒数* 采样率 / 1000；</para> 
        /// <para>举例：48000 采样率希望回调 20ms帧长的数据，则采样点数应该填: 960 = 20 * 48000 / 1000；</para>
        /// <para>注意，最终回调的帧长度是以字节为单位，采样点数转换成字节数的计算公式为：字节数 =采样点数 * channel * 2（位宽）</para> 
        /// <para>举例：48000 采样率，双声道，20ms帧长，采样点数为 960，字节数为 3840 = 960 * 2 * 2</para>
        /// </remarks>
        /// <param name="format">音频数据回调格式</param>
        /// <returns>0：成功；&lt;0：错误</returns>
        int setLocalProcessedAudioFrameCallbackFormat(TRTCAudioFrameCallbackFormat^ format);

        /// <summary>
        /// 9.15 设置最终要由系统播放出的音频帧回调格式
        /// <para>本接口用于设置 {@link onMixedPlayAudioFrame} 回调出来的 AudioFrame 的格式:</para> 
        /// <para>- sampleRate：采样率，支持：16000、32000、44100、48000。</para>
        /// <para>- channel：声道数（如果是立体声，数据是交叉的），单声道：1； 双声道：2。</para>
        /// <para>- samplesPerCall：采样点数，定义回调数据帧长。帧长必须为 10ms 的整数倍。</para>
        /// </summary>
        /// <remarks>
        /// <para>如果希望用毫秒数计算回调帧长，则将毫秒数转换成采样点数的公式为：采样点数 = 毫秒数* 采样率 / 1000；</para> 
        /// <para>举例：48000 采样率希望回调 20ms帧长的数据，则采样点数应该填: 960 = 20 * 48000 / 1000；</para>
        /// <para>注意，最终回调的帧长度是以字节为单位，采样点数转换成字节数的计算公式为：字节数 =采样点数 * channel * 2（位宽）</para> 
        /// <para>举例：48000 采样率，双声道，20ms帧长，采样点数为 960，字节数为 3840 = 960 * 2 * 2</para>
        /// </remarks>
        /// <param name="format">音频数据回调格式</param>
        /// <returns>0：成功；&lt;0：错误</returns>
        int setMixedPlayAudioFrameCallbackFormat(TRTCAudioFrameCallbackFormat^ format);

        /// <summary>
        /// 9.16 设置推流时混入外部音频的推流音量和播放音量
        /// <para>如果您需要外接一些特定的音频设备，或者希望自己掌控音频的播放逻辑，您可以通过该接口启用音频自定义播放。</para>
        /// <para>启用音频自定义播放后，SDK 将不再调用系统的音频接口播放数据，您需要通过 {@link getCustomAudioRenderingFrame} 获取 SDK 要播放的音频帧并自行播放。</para>
        /// </summary>
        /// <param name="enable"> 是否启用音频自定义播放，默认为关闭状态。</param>
        /// <remarks>
        /// 需要您在进入房间前设置才能生效，暂不支持进入房间后再设置。
        /// </remarks>
        void enableCustomAudioRendering(bool enable);

        /// <summary>
        /// 9.17 获取可播放的音频数据
        /// <para>调用该接口之前，您需要先通过 {@link enableCustomAudioRendering} 开启音频自定义播放。</para> 
        /// <para> 参数 {@link TRTCAudioFrame} 推荐下列填写方式（其他字段不需要填写）：</para>
        /// <para>    - sampleRate：采样率，必填，支持 16000、24000、32000、44100、48000。</para>
        /// <para>    - channel：声道数，必填，单声道请填1，双声道请填2，双声道时数据是交叉的。</para>
        /// <para>    - data 于获取音频数据的 buffer。需要您根据一阵音频帧的帧长度分配好 date 的内存大小。</para>
        /// <para>         获取的 PCM 数据支持 10ms 或 20ms 两种帧长，推荐使用 20ms 的帧长。</para>
        /// <para>         计算公式为：48000采样率、单声道、且播放时长为 20ms 的一帧音频帧的 buffer</para>
        /// <para>         大小为 48000 × 0.02s × 1 × 16bit = 15360bit = 1920字节。</para>
        /// </summary>
        /// <param name="audioFrame"> 音频数据帧</param>
        /// <remarks>
        /// <para> 1. 参数 audioFrame 中的 sampleRate、channel 需提前设置好，同时分配好所需读取帧长的 data 空间。</para>
        /// <para> 2. SDK 内部会根据 sampleRate 和 channel 自动填充 data 数据。</para>
        /// <para> 3. 建议由系统的音频播放线程直接驱动该函数的调用，在播放完一帧音频之后，即调用该接口获取下一帧可播放的音频数据。</para>
        /// </remarks>
        void getCustomAudioRenderingFrame(TRTCAudioFrame^ audioFrame);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （十）自定义消息发送
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 自定义消息发送
        /// @{

        /// <summary>
        /// 10.1 发送自定义消息给房间内所有用户
        /// <para>
        /// 该接口可以借助音视频数据通道向当前房间里的其他用户广播您自定义的数据，但因为复用了音视频数据通道，
        /// 请务必严格控制自定义消息的发送频率和消息体的大小，否则会影响音视频数据的质量控制逻辑，造成不确定性的问题。
        /// </para>
        /// </summary>
        /// <remarks>
        /// 本接口有以下限制：
        /// <para>    -发送消息到房间内所有用户（暂时不支持 Web/小程序端），每秒最多能发送30条消息。</para>
        /// <para>    -每个包最大为1KB，超过则很有可能会被中间路由器或者服务器丢弃。</para>
        /// <para>    -每个客户端每秒最多能发送总计8KB数据。</para>
        /// <para>    -将 reliable 和 ordered 同时设置为 true 或 false，暂不支持交叉设置。</para>
        /// <para>    -强烈建议不同类型的消息使用不同的 cmdID，这样可以在要求有序的情况下减小消息时延。</para>
        /// </remarks>
        /// <param name="cmdId">消息 ID，取值范围为1 - 10</param>
        /// <param name="data">待发送的消息，最大支持1KB（1000字节）的数据大小</param>
        /// <param name="dataSize">待发送的数据大小</param>
        /// <param name="reliable">是否可靠发送，可靠发送的代价是会引入一定的延时，因为接收端要暂存一段时间的数据来等待重传</param>
        /// <param name="ordered">是否要求有序，即是否要求接收端接收的数据顺序和发送端发送的顺序一致，这会带来一定的接收延时，因为在接收端需要暂存并排序这些消息</param>
        /// <returns>true：消息已经发出；false：消息发送失败</returns>
        bool sendCustomCmdMsg(UInt32 cmdId, array<Byte>^ data, UInt32 dataSize, bool reliable, bool ordered);

        /// <summary>
        /// 10.2 将小数据量的自定义数据嵌入视频帧中
        /// <para>
        /// 跟 sendCustomCmdMsg 的原理不同，sendSEIMsg 是将数据直接塞入视频数据头中。因此，即使视频帧被旁路到了直播 CDN 上，
        /// 这些数据也会一直存在。但是由于要把数据嵌入视频帧中，所以数据本身不能太大，推荐几个字节就好。
        /// </para>
        /// <para>最常见的用法是把自定义的时间戳（timstamp）用 sendSEIMsg 嵌入视频帧中，这种方案的最大好处就是可以实现消息和画面的完美对齐。</para>
        /// </summary>
        /// <remarks>
        /// 本接口有以下限制：
        /// <para>    -数据在接口调用完后不会被即时发送出去，而是从下一帧视频帧开始带在视频帧中发送。</para>
        /// <para>    -发送消息到房间内所有用户，每秒最多能发送30条消息（与 sendCustomCmdMsg 共享限制）。</para>
        /// <para>    -每个包最大为1KB，若发送大量数据，会导致视频码率增大，可能导致视频画质下降甚至卡顿（与 sendCustomCmdMsg 共享限制）。</para>
        /// <para>    -每个客户端每秒最多能发送总计8KB数据（与 sendCustomCmdMsg 共享限制）。</para>
        /// <para>    -若指定多次发送（repeatCount > 1），则数据会被带在后续的连续 repeatCount 个视频帧中发送出去，同样会导致视频码率增大。</para>
        /// <para>    -如果 repeatCount > 1，多次发送，接收消息 onRecvSEIMsg 回调也可能会收到多次相同的消息，需要去重。</para>
        /// </remarks>
        /// <param name="data">待发送的数据，最大支持1kb（1000字节）的数据大小</param>
        /// <param name="dataSize">待发送的数据大小</param>
        /// <param name="repeatCount">发送数据次数</param>
        /// <returns>true：消息已通过限制，等待后续视频帧发送；false:消息被限制发送</returns>
        bool sendSEIMsg(array<Byte>^ data, Int32 dataSize, Int32 repeatCount);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （十一）网络测试
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 网络测试
        /// @{

        /// <summary>
        /// 11.1 开始进行网络测速（视频通话期间请勿测试，以免影响通话质量）
        ///
        /// <para>测速结果将会用于优化 SDK 接下来的服务器选择策略，因此推荐您在用户首次通话前先进行一次测速，这将有助于我们选择最佳的服务器。</para>
        /// <para>同时，如果测试结果非常不理想，您可以通过醒目的 UI 提示用户选择更好的网络。</para>
        /// </summary>
        /// <remarks>
        /// 测速本身会消耗一定的流量，所以也会产生少量额外的流量费用。
        /// </remarks>
        /// <param name="sdkAppId">应用标识</param>
        /// <param name="userId">用户标识</param>
        /// <param name="userSig">用户签名</param>
        int startSpeedTest(TRTCSpeedTestParams^ params);

        /// <summary>
        /// 11.2 停止网络测速
        /// </summary>
        void stopSpeedTest();
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （十二）LOG 相关接口函数
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name LOG 相关接口函数
        /// @{

        /// <summary>
        /// 12.1 获取 SDK 版本信息
        /// </summary>
        /// <returns>UTF-8 编码的版本号。</returns>
        String^ getSDKVersion();

        /// <summary>
        /// 12.2 设置 Log 输出级别
        /// </summary>
        /// <param name="level">level 参见 TRTCLogLevel，默认值：TRTCLogLevelNone</param>
        void setLogLevel(TRTCLogLevel level);

        /// <summary>
        /// 12.3 启用或禁用控制台日志打印
        /// </summary>
        /// <param name="enabled">指定是否启用，默认为禁止状态</param>
        void setConsoleEnabled(bool enabled);

        /// <summary>
        /// 12.4 启用或禁用 Log 的本地压缩
        ///
        /// <para>开启压缩后，Log 存储体积明显减小，但需要腾讯云提供的 Python 脚本解压后才能阅读。</para>
        /// <para>禁用压缩后，Log 采用明文存储，可以直接用记事本打开阅读，但占用空间较大。</para>
        /// </summary>
        /// <param name="enabled">指定是否启用，默认为禁止状态</param>
        void setLogCompressEnabled(bool enabled);

        /// <summary>
        /// 12.5 设置日志保存路径
        /// </summary>
        /// <remarks>
        /// 日志文件默认保存在 C:/Users/[系统用户名]/AppData/Roaming/Tencent/liteav/log，即 %appdata%/Tencent/liteav/log 下，如需修改，必须在所有方法前调用。
        /// </remarks>
        /// <param name="path">存储日志的文件夹，例如 "D:\\Log"，UTF-8 编码</param>
        void setLogDirPath(String^ path);

        /// <summary>
        /// 12.6 设置日志回调
        /// </summary>
        /// <param name="callback">日志回调</param>
        void setLogCallback(ITRTCLogCallback^ callback);

        /// <summary>
        /// 12.7 显示仪表盘
        /// </summary>
        /// <remarks>
        /// 仪表盘是状态统计和事件消息浮层 view，方便调试。
        /// </remarks>
        /// <param name="showType">0：不显示；1：显示精简版；2：显示全量版，默认为不显示</param>
        void showDebugView(int showType);

        /// <summary>
        /// 12.8 调用实验性 API 接口
        /// </summary>
        /// <remarks>
        /// 该接口用于调用一些实验性功能
        /// </remarks>
        /// <param name="jsonStr">接口及参数描述的 JSON 字符串</param>
        void callExperimentalAPI(String^ jsonStr);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （十三） 弃用接口函数
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 弃用接口函数
        /// @{

        /// <summary>
        /// 设置麦克风的音量大小
        /// </summary>
        /// <remarks>从 v6.9 版本开始废弃，请使用 setAudioCaptureVolume 接口替代。 </remarks>
        [Obsolete("warning :setMicVolumeOnMixing is deprecated, use setAudioCaptureVolume instead.")]
        void setMicVolumeOnMixing(UInt32 volume);

        /// <summary>
        /// 启动屏幕分享
        /// </summary>
        /// <remarks>从 v7.2 版本开始废弃，请使用 startScreenCapture(HWND rendHwnd, TRTCVideoStreamType type, TRTCVideoEncParam* params) 接口替代。</remarks>
        [Obsolete("use startScreenCapture(HWND rendHwnd, TRTCVideoStreamType type, TRTCVideoEncParam* params) instead.")]
        void startScreenCapture(IntPtr rendHwnd);

        /// <summary>
        /// 启动播放背景音乐
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager startPlayMusic 接口，支持并发播放多个 BGM</remarks>
        /// <param name="path">音乐文件路径，支持的文件格式：aac, mp3。</param>
        [Obsolete("use TXAudioEffectManager startPlayMusic instead")]
        void playBGM(String^ path);

        /// <summary>
        /// 停止播放背景音乐
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager stopPlayMusic 接口</remarks>
        [Obsolete("use TXAudioEffectManager stopPlayMusic instead")]
        void stopBGM();

        /// <summary>
        /// 暂停播放背景音乐
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager pausePlayMusic 接口</remarks>
        [Obsolete("use TXAudioEffectManager pausePlayMusic instead")]
        void pauseBGM();

        /// <summary>
        /// 继续播放背景音乐
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager resumePlayMusic 接口</remarks>
        [Obsolete("use TXAudioEffectManager resumePlayMusic instead")]
        void resumeBGM();

        /// <summary>
        /// 获取音乐文件总时长，单位毫秒
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager getMusicDurationInMS 接口</remarks>
        /// <param name="path">音乐文件路径，如果 path 为空，那么返回当前正在播放的 music 时长</param>
        /// <returns>成功返回时长，失败返回-1</returns>
        [Obsolete("use TXAudioEffectManager getMusicDurationInMS instead")]
        UInt32 getBGMDuration(String^ path);

        /// <summary>
        /// 设置 BGM 播放进度
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager seekMusicToPosInMS 接口</remarks>
        /// <param name="pos">单位毫秒</param>
        [Obsolete("use TXAudioEffectManager seekMusicToPosInMS instead")]
        void setBGMPosition(UInt32 pos);

        /// <summary>
        /// 设置背景音乐播放音量的大小
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager setMusicPublishVolume / setMusicPlayoutVolume 接口</remarks>
        /// <para>播放背景音乐混音时使用，用来控制背景音乐播放音量的大小，</para>
        /// <para>该接口会同时控制远端播放音量的大小和本地播放音量的大小，</para>
        /// <para>因此调用该接口后，setBGMPlayoutVolume 和 setBGMPublishVolume 设置的音量值会被覆盖</para>
        /// <param name="volume">音量大小，100为正常音量，取值范围为0 - 100；默认值：100</param>
        [Obsolete("use TXAudioEffectManager setMusicPublishVolume / setMusicPlayoutVolume instead")]
        void setBGMVolume(UInt32 volume);

        /// <summary>
        /// 设置背景音乐本地播放音量的大小
        /// </summary>
        /// <para>v7.3 版本弃用，请使用 TXAudioEffectManager setMusicPlayoutVolume 接口</para>
        /// <para>播放背景音乐混音时使用，用来控制背景音乐在本地播放时的音量大小。</para>
        /// <param name="volume"> 音量大小，100为正常音量，取值范围为0 - 100；默认值：100</param>
        [Obsolete("use TXAudioEffectManager setMusicPlayoutVolume instead")]
        void setBGMPlayoutVolume(UInt32 volume);

        /// <summary>
        /// 设置背景音乐远端播放音量的大小
        /// </summary>
        /// <para>v7.3 版本弃用，请使用 TXAudioEffectManager setMusicPublishVolume 接口</para>
        /// <para>播放背景音乐混音时使用，用来控制背景音乐在远端播放时的音量大小。</para>
        /// <param name="volume"> 音量大小，100为正常音量，取值范围为0 - 100；默认值：100</param>
        [Obsolete("use TXAudioEffectManager setMusicPublishVolume instead")]
        void setBGMPublishVolume(UInt32 volume);

        /// <summary>
        /// 播放音效
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager startPlayMusic 接口</remarks>
        /// <para>每个音效都需要您指定具体的 ID，您可以通过该 ID 对音效的开始、停止、音量等进行设置。</para>
        /// <para>支持的文件格式：aac, mp3。</para>
        /// <remarks>若您想同时播放多个音效，请分配不同的 ID 进行播放。因为使用同一个 ID 播放不同音效，SDK 将会停止上一个 ID 对应的音效播放，再启动新的音效播放。</remarks>
        /// <param name="effect">音效</param>
        [Obsolete("use TXAudioEffectManager startPlayMusic instead")]
        void playAudioEffect(TRTCAudioEffectParam^ effect);

        /// <summary>
        /// 设置音效音量
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager setMusicPublishVolume / setMusicPlayoutVolume 接口</remarks>
        /// <remarks>会覆盖通过 setAllAudioEffectsVolume 指定的整体音效音量。</remarks>
        /// <param name="effectId">音效 ID</param>
        /// <param name="volume">音量大小，取值范围为0 - 100；默认值：100</param>
        [Obsolete("use TXAudioEffectManager setMusicPublishVolume / setMusicPlayoutVolume instead")]
        void setAudioEffectVolume(int effectId, int volume);

        /// <summary>
        /// 停止音效
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager stopPlayMusic 接口</remarks>
        /// <param name="effectId">音效 ID</param>
        [Obsolete("use TXAudioEffectManager stopPlayMusic instead")]
        void stopAudioEffect(int effectId);

        /// <summary>
        /// 停止所有音效
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager stopPlayMusic 接口</remarks>
        [Obsolete("use TXAudioEffectManager stopPlayMusic instead")]
        void stopAllAudioEffects();

        /// <summary>
        /// 设置所有音效的音量
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager setMusicPublishVolume / setMusicPlayoutVolume 接口</remarks>
        /// <remarks>该操作会覆盖通过 setAudioEffectVolume 指定的单独音效音量。</remarks>
        /// <param name="volume">音量大小，取值范围为0 - 100；默认值：100</param>
        [Obsolete("use TXAudioEffectManager setMusicPublishVolume / setMusicPlayoutVolume instead")]
        void setAllAudioEffectsVolume(int volume);

        /// <summary>
        /// 暂停音效
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager pausePlayMusic 接口</remarks>
        /// <param name="effectId">音效 Id</param>
        [Obsolete("use TXAudioEffectManager pausePlayMusic instead")]
        void pauseAudioEffect(int effectId);

        /// <summary>
        /// 恢复音效
        /// </summary>
        /// <remarks>v7.3 版本弃用，请使用 TXAudioEffectManager resumePlayMusic 接口</remarks>
        /// <param name="effectId">音效 Id</param>
        [Obsolete("use TXAudioEffectManager resumePlayMusic instead")]
        void resumeAudioEffect(int effectId);

        /// <summary>
        /// 设置屏幕共享参数
        /// </summary>
        /// <remarks>v7.9 版本弃用，请使用 selectScreenCaptureTarget(TRTCScreenCaptureSourceInfo,RECT,TRTCScreenCaptureProperty) 接口</remarks>
        /// <param name="source">指定分享源</param>
        /// <param name="captureRect">指定捕获的区域</param>
        /// <param name="captureMouse">指定是否捕获鼠标指针</param>
        /// <param name="highlightWindow">指定是否高亮正在共享的窗口，以及当捕获图像被遮挡时高亮遮挡窗口提示用户移走遮挡</param>
        [Obsolete("use selectScreenCaptureTarget(TRTCScreenCaptureSourceInfo,RECT,TRTCScreenCaptureProperty) instead")]
        void selectScreenCaptureTarget(TRTCScreenCaptureSourceInfo^% source, RECT^% captureRect, bool captureMouse, bool highlightWindow);

        /// <summary>
        /// 开始显示远端视频画面
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 startRemoteView(userId,streamType,rendView) 接口</remarks>
        /// <param name="userId">对方的用户标识</param>
        /// <param name="rendHwnd">承载预览画面的窗口句柄</param>
        [Obsolete("use startRemoteView(userId,streamType,rendView) instead.")]
        void startRemoteView(String^ userId, IntPtr rendHwnd);

        /// <summary>
        /// 停止显示远端视频画面，同时不再拉取远端用户的视频数据流
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 stopRemoteView(userId,streamType) 接口</remarks>
        /// <remarks>调用此接口后，SDK 会停止接收该用户的远程视频流，同时会清理相关的视频显示资源。</remarks>
        /// <param name="userId">对方的用户标识</param>
        [Obsolete("use stopRemoteView(userId,streamType) instead.")]
        void stopRemoteView(String^ userId);

        /// <summary>
        /// 设置本地图像的渲染模式
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 setLocalRenderParams(TRTCRenderParams) 接口</remarks>
        /// <param name="mode">填充（画面可能会被拉伸裁剪）或适应（画面可能会有黑边），默认值：TRTCVideoFillMode_Fit</param>
        [Obsolete("use setLocalRenderParams(TRTCRenderParams) instead.")]
        void setLocalViewFillMode(TRTCVideoFillMode mode);

        /// <summary>
        /// 设置远端图像的渲染模式
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 setRemoteRenderParams(userId,streamType,param) 接口</remarks>
        /// <param name="userId">用户 ID</param>
        /// <param name="mode">填充（画面可能会被拉伸裁剪）或适应（画面可能会有黑边），默认值：TRTCVideoFillMode_Fit</param>
        [Obsolete("use setRemoteRenderParams(userId,streamType,param) instead.")]
        void setRemoteViewFillMode(String^ userId, TRTCVideoFillMode mode);

        /// <summary>
        /// 设置本地图像的顺时针旋转角度
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 setLocalRenderParams(TRTCRenderParams) 接口</remarks>
        /// <param name="rotation">支持 TRTCVideoRotation90 、 TRTCVideoRotation180 以及 TRTCVideoRotation270 旋转角度，默认值：TRTCVideoRotation0</param>
        [Obsolete("use setLocalRenderParams(TRTCRenderParams) instead.")]
        void setLocalViewRotation(TRTCVideoRotation rotation);

        /// <summary>
        /// 设置远端图像的顺时针旋转角度
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 setRemoteRenderParams(userId,streamType,param) 接口</remarks>
        /// <param name="userId">用户 ID</param>
        /// <param name="rotation">支持 TRTCVideoRotation90 、 TRTCVideoRotation180 以及
        /// TRTCVideoRotation270 旋转角度，默认值：TRTCVideoRotation0</param>
        [Obsolete("use setRemoteRenderParams(userId,streamType,param) instead.")]
        void setRemoteViewRotation(String^ userId, TRTCVideoRotation rotation);

        /// <summary>
        /// 设置本地摄像头预览画面的镜像模式
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 setLocalRenderParams(TRTCRenderParams) 接口</remarks>
        /// <param name="mirror">镜像模式，默认值：false（非镜像模式）</param>
        [Obsolete("use setLocalRenderParams(TRTCRenderParams) instead.")]
        void setLocalViewMirror(bool mirror);

        /// <summary>
        /// 开始显示远端用户的辅路画面（TRTCVideoStreamTypeSub，一般用于屏幕分享）
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 startRemoteView(userId,streamType,rendView) 接口</remarks>
        /// <para>- startRemoteView() 用于显示主路画面（TRTCVideoStreamTypeBig，一般用于摄像头）。</para>
        /// <para>- startRemoteSubStreamView() 用于显示辅路画面（TRTCVideoStreamTypeSub，一般用于屏幕分享）。</para>
        /// <remarks>请在 onUserSubStreamAvailable 回调后再调用这个接口。</remarks>
        /// <param name="userId">对方的用户标识</param>
        /// <param name="rendHwnd">渲染画面的 HWND</param>
        [Obsolete("use startRemoteView(userId,streamType,rendView) instead.")]
        void startRemoteSubStreamView(String^ userId, IntPtr rendHwnd);

        /// <summary>
        /// 停止显示远端用户的辅路画面（TRTCVideoStreamTypeSub，一般用于屏幕分享）。
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 stopRemoteView(userId,streamType) 接口</remarks>
        /// <param name="userId">对方的用户标识</param>
        [Obsolete("use stopRemoteView(userId,streamType) instead.")]
        void stopRemoteSubStreamView(String^ userId);

        /// <summary>
        /// 设置辅路画面（TRTCVideoStreamTypeSub，一般用于屏幕分享）的显示模式
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 setRemoteRenderParams(userId,streamType,param) 接口</remarks>
        /// <para>- setRemoteViewFillMode() 用于设置远端主路画面（TRTCVideoStreamTypeBig，一般用于摄像头）的显示模式。</para>
        /// <para>- setRemoteSubStreamViewFillMode() 用于设置远端辅路画面（TRTCVideoStreamTypeSub，一般用于屏幕分享）的显示模式。</para>
        /// <param name="userId">用户的 ID</param>
        /// <param name="mode">填充（画面可能会被拉伸裁剪）或适应（画面可能会有黑边），默认值：TRTCVideoFillMode_Fit</param>
        [Obsolete("use setRemoteRenderParams(userId,streamType,param) instead.")]
        void setRemoteSubStreamViewFillMode(String^ userId, TRTCVideoFillMode mode);

        /// <summary>
        /// 设置辅路画面（TRTCVideoStreamTypeSub，一般用于屏幕分享）的顺时针旋转角度
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 setRemoteRenderParams(userId,streamType,param) 接口</remarks>
        /// <para>- setRemoteViewRotation() 用于设置远端主路画面（TRTCVideoStreamTypeBig，一般用于摄像头）的旋转角度。</para>
        /// <para>- setRemoteSubStreamViewRotation() 用于设置远端辅路画面（TRTCVideoStreamTypeSub，一般用于屏幕分享）的旋转角度。</para>
        /// <param name="userId">用户的 ID</param>
        /// <param name="rotation">支持90、180、270旋转角度</param>
        [Obsolete("use setRemoteRenderParams(userId,streamType,param) instead.")]
        void setRemoteSubStreamViewRotation(String^ userId, TRTCVideoRotation rotation);

        /// <summary>
        /// 设定观看方优先选择的视频质量
        ///
        /// <para>低端设备推荐优先选择低清晰度的小画面。</para>
        /// <para>如果对方没有开启双路视频模式，则此操作无效。</para>
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 startRemoteView(userId,streamType,rendView) 接口</remarks>
        /// <param name="type">默认观看大画面还是小画面，默认为 TRTCVideoStreamTypeBig</param>
        [Obsolete("use startRemoteView(userId,streamType,rendView) instead.")]
        void setPriorRemoteVideoStreamType(TRTCVideoStreamType type);

        /// <summary>
        /// 设置音频质量
        ///
        /// <para>主播端的音质越高，观众端的听感越好，但传输所依赖的带宽也就越高，在带宽有限的场景下也更容易出现卡顿。</para>
        /// <para>TRTCCloudDef#TRTCAudioQualitySpeech}，流畅：采样率：16k；单声道；音频裸码率：16kbps；适合语音通话为主的场景，比如在线会议，语音通话。</para>
        /// <para>TRTCCloudDef#TRTCAudioQualityDefault}，默认：采样率：48k；单声道；音频裸码率：50kbps；SDK 默认的音频质量，如无特殊需求推荐选择之。</para>
        /// <para>TRTCCloudDef#TRTCAudioQualityMusic}，高音质：采样率：48k；双声道 + 全频带；音频裸码率：128kbps；适合需要高保真传输音乐的场景，比如K歌、音乐直播等。</para>
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 startLocalAudio(TRTCAudioQuality) 接口</remarks>
        /// <remarks>
        /// 该方法需要在 startLocalAudio 之前进行设置，否则不会生效。
        /// </remarks>
        [Obsolete("use startLocalAudio(TRTCAudioQuality) instead.")]
        void setAudioQuality(TRTCAudioQuality quality);

        /// <summary>
        /// 开启本地音频的采集和上行
        ///
        /// <para>该函数会启动麦克风采集，并将音频数据传输给房间里的其他用户。</para>
        /// <para>SDK 不会默认开启本地音频采集和上行，您需要调用该函数开启，否则房间里的其他用户将无法听到您的声音。</para>
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 startLocalAudio(TRTCAudioQuality) 接口</remarks>
        /// <remarks>
        /// TRTC SDK 并不会默认打开本地的麦克风采集。
        /// </remarks>
        [Obsolete("use startLocalAudio(TRTCAudioQuality) instead.")]
        void startLocalAudio();

        /// <summary>
        /// 获取摄像头设备列表
        /// </summary>
        /// <example>
        /// 示例代码：
        /// <code>
        /// ITRTCDeviceCollection pDevice = m_pCloud.getCameraDevicesList();
        /// for (int i = 0; i &lt; pDevice.getCount(); i++)
        /// {
        ///     string name = pDevice.getDeviceName(i));
        /// }
        /// pDevice.release();
        /// pDevice = null;
        /// </code>
        /// </example>
        /// <remarks>
        /// SDK 维护 ITRTCDeviceCollection 对象的生命周期，使用完毕后请调用 release 方法释放资源。
        /// </remarks>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::getDevicesList 接口</remarks>
        /// <returns>摄像头管理器对象 ITRTCDeviceCollection</returns>
        [Obsolete("use ITXDeviceManager::getDevicesList instead.")]
        ITRTCDeviceCollection^ getCameraDevicesList();

        /// <summary>
        /// 设置要使用的摄像头
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::setCurrentDevice 接口</remarks>
        /// <param name="deviceId">从 getCameraDevicesList 中得到的设备 ID</param>
        [Obsolete("use ITXDeviceManager::setCurrentDevice instead.")]
        void setCurrentCameraDevice(String^ deviceId);

        /// <summary>
        /// 获取当前使用的摄像头
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::getCurrentDevice 接口</remarks>
        /// <returns> ITRTCDeviceInfo 设备信息，能获取设备 ID 和设备名称</returns>
        [Obsolete("use ITXDeviceManager::getCurrentDevice instead.")]
        ITRTCDeviceInfo^ getCurrentCameraDevice();

        /// <summary>
        /// 获取麦克风设备列表
        /// </summary>
        /// <example>
        /// 示例代码：
        /// <code>
        /// ITRTCDeviceCollection pDevice = m_pCloud.getMicDevicesList();
        /// for (int i = 0; i &lt; pDevice.getCount(); i++)
        /// {
        ///     string name = pDevice.getDeviceName(i));
        /// }
        /// pDevice.release();
        /// pDevice = null;
        /// </code>
        /// </example>
        /// <remarks>
        /// SDK 维护 ITRTCDeviceCollection 对象的生命周期，使用完毕后请调用 release 方法释放资源。
        /// </remarks>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::getDevicesList 接口</remarks>
        /// <returns>麦克风管理器对象 ITRTCDeviceCollection</returns>
        [Obsolete("use ITXDeviceManager::getDevicesList instead.")]
        ITRTCDeviceCollection^ getMicDevicesList();

        /// <summary>
        /// 获取当前选择的麦克风
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::getCurrentDevice 接口</remarks>
        /// <returns>ITRTCDeviceInfo 设备信息，能获取设备 ID 和设备名称</returns>
        [Obsolete("use ITXDeviceManager::getCurrentDevice instead.")]
        ITRTCDeviceInfo^ getCurrentMicDevice();

        /// <summary>
        /// 设置要使用的麦克风
        ///
        /// <para>选择指定的麦克风作为录音设备，不调用该接口时，默认选择索引为0的麦克风</para>
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::setCurrentDevice 接口</remarks>
        /// <param name="micId">从 getMicDevicesList 中得到的设备 ID</param>
        [Obsolete("use ITXDeviceManager::setCurrentDevice instead.")]
        void setCurrentMicDevice(String^ micId);

        /// <summary>
        /// 获取系统当前麦克风设备音量
        /// </summary>
        /// <remarks>查询的是系统硬件音量大小。</remarks>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::getCurrentDeviceVolume 接口</remarks>
        /// <returns>音量值，范围是[0 - 100]</returns>
        [Obsolete("use ITXDeviceManager::getCurrentDeviceVolume instead.")]
        UInt32 getCurrentMicDeviceVolume();

        /// <summary>
        /// 设置系统当前麦克风设备的音量
        /// </summary>
        /// <remarks>该接口的功能是调节系统采集音量，如果用户直接调节 WIN 系统设置的采集音量时，该接口的设置结果会被用户的操作所覆盖。</remarks>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::setCurrentDeviceVolume 接口</remarks>
        /// <param name="volume">麦克风音量值，范围[0 - 100]</param>
        [Obsolete("use ITXDeviceManager::setCurrentDeviceVolume instead.")]
        void setCurrentMicDeviceVolume(UInt32 volume);

        /// <summary>
        /// 获取扬声器设备列表
        /// </summary>
        /// <example>
        /// 示例代码：
        /// <code>
        /// ITRTCDeviceCollection pDevice = m_pCloud.getSpeakerDevicesList();
        /// for (int i = 0; i &lt; pDevice.getCount(); i++)
        /// {
        ///     string name = pDevice.getDeviceName(i));
        /// }
        /// pDevice.release();
        /// </code>
        /// </example>
        /// <remarks>
        /// SDK 维护 ITRTCDeviceCollection 对象的生命周期，使用完毕后请调用 release 方法释放资源。
        /// </remarks>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::getDevicesList 接口</remarks>
        /// <returns>扬声器管理器对象 ITRTCDeviceCollection</returns>
        [Obsolete("use ITXDeviceManager::getDevicesList instead.")]
        ITRTCDeviceCollection^ getSpeakerDevicesList();

        /// <summary>
        /// 获取当前的扬声器设备
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::getCurrentDevice 接口</remarks>
        /// <returns>ITRTCDeviceInfo 设备信息，能获取设备 ID 和设备名称</returns>
        [Obsolete("use ITXDeviceManager::getCurrentDevice instead.")]
        ITRTCDeviceInfo^ getCurrentSpeakerDevice();

        /// <summary>
        /// 设置要使用的扬声器
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::setCurrentDevice 接口</remarks>
        /// <param name="speakerId">从 getDevicesList 中得到的设备 ID</param>
        [Obsolete("use ITXDeviceManager::setCurrentDevice instead.")]
        void setCurrentSpeakerDevice(String^ speakerId);

        /// <summary>
        /// 获取系统当前扬声器设备音量
        /// </summary>
        /// <remarks>查询的是系统硬件音量大小。</remarks>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::getCurrentVolume 接口</remarks>
        /// <returns>扬声器音量，范围0 - 100</returns>
        [Obsolete("use ITXDeviceManager::getCurrentVolume instead.")]
        UInt32 getCurrentSpeakerVolume();

        /// <summary>
        /// 设置系统当前扬声器设备音量
        /// </summary>
        /// <para>v8.0 版本弃用，请使用 ITXDeviceManager::setCurrentVolume 接口</para>
        /// <remarks>该接口的功能是调节系统播放音量，如果用户直接调节 WIN 系统设置的播放音量时，该接口的设置结果会被用户的操作所覆盖。</remarks>
        /// <param name="volume">设置的扬声器音量，范围0 - 100</param>
        [Obsolete("use ITXDeviceManager::setCurrentVolume instead.")]
        void setCurrentSpeakerVolume(UInt32 volume);

        /// <summary>
        /// 开始进行摄像头测试
        ///
        /// <para>会触发 onFirstVideoFrame 回调接口</para>
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::startCameraDeviceTest 接口</remarks>
        /// <remarks>
        /// 在测试过程中可以使用 setCurrentCameraDevice 接口切换摄像头。
        /// </remarks>
        /// <param name="rendHwnd">承载预览画面的窗口句柄</param>
        [Obsolete("use ITXDeviceManager::startCameraDeviceTest instead.")]
        void startCameraDeviceTest(IntPtr rendHwnd);

        /// <summary>
        /// 停止摄像头测试
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::stopCameraDeviceTest 接口</remarks>
        [Obsolete("use ITXDeviceManager::stopCameraDeviceTest instead.")]
        void stopCameraDeviceTest();

        /// <summary>
        /// 开启麦克风测试
        /// <para>回调接口 onTestMicVolume 获取测试数据</para>
        /// <para>该方法测试麦克风是否能正常工作，volume 的取值范围为0 - 100。</para>
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::startMicDeviceTest 接口</remarks>
        /// <param name="interval">反馈音量提示的时间间隔（ms），建议设置到大于 200 毫秒</param>
        [Obsolete("use ITXDeviceManager::startMicDeviceTest instead.")]
        void startMicDeviceTest(UInt32 interval);

        /// <summary>
        /// 停止麦克风测试
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::stopMicDeviceTest 接口</remarks>
        [Obsolete("use ITXDeviceManager::stopMicDeviceTest instead.")]
        void stopMicDeviceTest();

        /// <summary>
        /// 开启扬声器测试
        /// <para>回调接口 onTestSpeakerVolume 获取测试数据</para>
        /// <para>该方法播放指定的音频文件测试播放设备是否能正常工作。如果能听到声音，说明播放设备能正常工作。</para>
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::startSpeakerDeviceTest 接口</remarks>
        /// <param name="testAudioFilePath">音频文件的绝对路径，路径字符串使用 UTF-8
        /// 编码格式，支持文件格式：WAV、MP3</param>
        [Obsolete("use ITXDeviceManager::startSpeakerDeviceTest instead.")]
        void startSpeakerDeviceTest(String^ testAudioFilePath);

        /// <summary>
        /// 停止扬声器测试
        /// </summary>
        /// <remarks>v8.0 版本弃用，请使用 ITXDeviceManager::stopSpeakerDeviceTest 接口</remarks>
        [Obsolete("use ITXDeviceManager::stopSpeakerDeviceTest instead.")]
        void stopSpeakerDeviceTest();

        /// @name 自定义采集和渲染
        /// @{
        /// /// <summary>
        /// 9.1 启用视频自定义采集模式(已废弃)
        ///
        /// <para>开启该模式后，SDK 不再运行原有的视频采集流程，只保留编码和发送能力。</para>
        /// <para>您需要用 sendCustomVideoData() 不断地向 SDK 塞入自己采集的视频画面。</para>
        /// </summary>
        /// <param name="enable">是否启用，默认值：false</param>
        [Obsolete("use enableCustomVideoCapture(streamType, frame) instead.")]
        void enableCustomVideoCapture(bool enable);

        /// <summary>
        /// 暂停/恢复接收指定的远端视频流
        /// </summary>
        /// <remarks>v9.0 版本弃用，请使用 muteRemoteVideoStream(userId, streamType, mute)
        /// 接口</remarks>
        [Obsolete("use muteRemoteVideoStream(userId, streamType, mute) instead.")] void
            muteRemoteVideoStream(String^ userId, bool mute);

        /// <summary>
        /// 向 SDK 投送自己采集的视频数据(已废弃)
        /// </summary>
        /// <remarks>
        /// v9.0 版本弃用，请使用 sendCustomVideoData(streamType, frame) 接口
        /// </remarks>
        [Obsolete("use sendCustomVideoData(streamType, frame) instead.")]
        void sendCustomVideoData(TRTCVideoFrame^ frame);

        /// <summary>
        /// 暂停/恢复推送本地的视频数据(已废弃)
        /// </summary>
        /// <remarks>
        /// v9.0 版本弃用，请使用 muteLocalVideo(streamType, mute) 接口
        /// </remarks>
        [Obsolete("use muteLocalVideo(streamType, mute) instead.")]
        void muteLocalVideo(bool mute);

        /// <summary>
        /// 开始进行网络测速（视频通话期间请勿测试，以免影响通话质量）
        /// </summary>
        /// <remarks>
        /// v9.3 版本弃用，请使用 startSpeedTest(TRTCSpeedTestParams) 接口
        /// </remarks>
        [Obsolete("use startSpeedTest(TRTCSpeedTestParams) instead.")]
        void startSpeedTest(UInt32 sdkAppId, String^ userId, String^ userSig);
        /// @}
    private:
        !ITRTCCloud();

        // ITRTCCloudCallback
        delegate void ErrorDelegate(::TXLiteAVError errCode, const char* errMsg, void* arg);
        delegate void WarningDelegate(::TXLiteAVWarning warningCode, const char* warningMsg, void* arg);
        delegate void EnterRoomDelegate(int reason);
        delegate void ExitRoomDelegate(int reason);
        delegate void UserEnterDelegate(const char* userId);
        delegate void UserExitDelegate(const char* userId, int reason);
        delegate void RemoteUserEnterRoomDelegate(const char* userId);
        delegate void RemoteUserLeaveRoomDelegate(const char* userId, int reason);
        delegate void SwitchRoleDelegate(::TXLiteAVError errCode, const char* errMsg);
        delegate void SwitchRoomDelegate(::TXLiteAVError errCode, const char* errMsg);
        delegate void ConnectOtherRoomDelegate(const char* userId, ::TXLiteAVError errCode, const char* errMsg);
        delegate void DisconnectOtherRoomDelegate(::TXLiteAVError errCode, const char* errMsg);
        delegate void UserVideoAvailableDelegate(const char* userId, bool available);
        delegate void UserSubStreamAvailableDelegate(const char* userId, bool available);
        delegate void UserAudioAvailableDelegate(const char* userId, bool available);
        delegate void UserVoiceVolumeDelegate(liteav::TRTCVolumeInfo* userVolumes, unsigned int userVolumesCount, unsigned int totalVolume);
        delegate void NetworkQualityDelegate(liteav::TRTCQualityInfo localQuality, ::TRTCQualityInfo* remoteQuality, unsigned int remoteQualityCount);
        delegate void StatisticsDelegate(const liteav::TRTCStatistics& statis);
        delegate void FirstVideoFrameDelegate(const char* userId, liteav::TRTCVideoStreamType streamType, const int width, const int height);
        delegate void FirstAudioFrameDelegate(const char* userId);
        delegate void SendFirstLocalVideoFrameDelegate(const liteav::TRTCVideoStreamType streamType);
        delegate void SendFirstLocalAudioFrameDelegate();
        delegate void PlayBGMBeginDelegate(::TXLiteAVError errCode);
        delegate void PlayBGMProgressDelegate(unsigned int progressMS, unsigned int durationMS);
        delegate void PlayBGMCompleteDelegate(::TXLiteAVError errCode);
        delegate void ConnectionLostDelegate();
        delegate void TryToReconnectDelegate();
        delegate void ConnectionRecoveryDelegate();
        delegate void SpeedTestDelegate(const liteav::TRTCSpeedTestResult& currentResult, unsigned int finishedCount, unsigned int totalCount);
        delegate void CameraDidReadyDelegate();
        delegate void MicDidReadyDelegate();
        delegate void DeviceChangeDelegate(const char* deviceId, liteav::TRTCDeviceType type, liteav::TRTCDeviceState state);
        delegate void TestMicVolumeDelegate(unsigned int volume);
        delegate void TestSpeakerVolumeDelegate(unsigned int volume);
        delegate void AudioDeviceCaptureVolumeChangedDelegate(unsigned int volume, bool muted);
        delegate void AudioDevicePlayoutVolumeChangedDelegate(unsigned int volume, bool muted);
        delegate void RecvCustomCmdMsgDelegate(const char* userId, int cmdId, unsigned int seq, const unsigned char* msg, unsigned int msgSize);
        delegate void MissCustomCmdMsgDelegate(const char* userId, int  cmdId, int errCode, int missed);
        delegate void RecvSEIMsgDelegate(const char* userId, const unsigned char* message, unsigned int msgSize);
        delegate void StartPublishingDelegate(int errCode, const char* errMsg);
        delegate void StopPublishingDelegate(int errCode, const char* errMsg);
        delegate void StartPublishCDNStreamDelegate(int errCode, const char* errMsg);
        delegate void StopPublishCDNStreamDelegate(int errCode, const char* errMsg);
        delegate void SetMixTranscodingConfigDelegate(int errCode, const char* errMsg);
        delegate void AudioEffectFinishedDelegate(int effectId, int code);
        delegate void ScreenCaptureCoveredDelegate();
        delegate void ScreenCaptureStartedDelegate();
        delegate void ScreenCapturePausedDelegate(int reason);
        delegate void ScreenCaptureResumedDelegate(int reason);
        delegate void ScreenCaptureStopedDelegate(int reason);
        delegate void SnapshotCompleteDelegate(const char* userId, liteav::TRTCVideoStreamType type, char* data, uint32_t length, uint32_t width, uint32_t height, liteav::TRTCVideoPixelFormat format);
        delegate void LocalRecordBeginDelegate(int errCode, const char* storagePath);
        delegate void LocalRecordingDelegate(long duration, const char* storagePath);
        delegate void LocalRecordCompleteDelegate(int errCode, const char* storagePath);
        delegate void RemoteVideoStatusUpdatedDelegate(const char* userId, liteav::TRTCVideoStreamType streamType, liteav::TRTCAVStatusType status, liteav::TRTCAVStatusChangeReason reason, void* extrainfo);

        void onError(::TXLiteAVError errCode, const char* errMsg, void* arg);
        void onWarning(::TXLiteAVWarning warningCode, const char* warningMsg, void* arg);
        void onEnterRoom(int reason);
        void onExitRoom(int reason);
        void onSwitchRole(::TXLiteAVError errCode, const char* errMsg);
        void onConnectOtherRoom(const char* userId, ::TXLiteAVError errCode, const char* errMsg);
        void onDisconnectOtherRoom(::TXLiteAVError errCode, const char* errMsg);
        void onSwitchRoom(::TXLiteAVError errCode, const char* errMsg);
        void onUserEnter(const char* userId);
        void onUserExit(const char* userId, int reason);
        void onRemoteUserEnterRoom(const char* userId);
        void onRemoteUserLeaveRoom(const char* userId, int reason);
        void onUserVideoAvailable(const char* userId, bool available);
        void onUserSubStreamAvailable(const char* userId, bool available);
        void onUserAudioAvailable(const char* userId, bool available);
        void onUserVoiceVolume(liteav::TRTCVolumeInfo* userVolumes, unsigned int userVolumesCount, unsigned int totalVolume);
        void onNetworkQuality(liteav::TRTCQualityInfo localQuality, liteav::TRTCQualityInfo* remoteQuality, unsigned int remoteQualityCount);
        void onStatistics(const liteav::TRTCStatistics& statis);
        void onFirstVideoFrame(const char* userId, const liteav::TRTCVideoStreamType streamType, const int width, const int height);
        void onFirstAudioFrame(const char* userId);
        void onSendFirstLocalVideoFrame(const liteav::TRTCVideoStreamType streamType);
        void onSendFirstLocalAudioFrame();
        void onPlayBGMBegin(::TXLiteAVError errCode);
        void onPlayBGMProgress(unsigned int progressMS, unsigned int durationMS);
        void onPlayBGMComplete(::TXLiteAVError errCode);
        void onConnectionLost();
        void onTryToReconnect();
        void onConnectionRecovery();
        void onSpeedTest(const liteav::TRTCSpeedTestResult& currentResult, unsigned int finishedCount, unsigned int totalCount);
        void onCameraDidReady();
        void onMicDidReady();
        void onDeviceChange(const char* deviceId, liteav::TRTCDeviceType type, liteav::TRTCDeviceState state);
        void onTestMicVolume(unsigned int volume);
        void onTestSpeakerVolume(unsigned int volume);
        void onAudioDeviceCaptureVolumeChanged(unsigned int volume, bool muted);
        void onAudioDevicePlayoutVolumeChanged(unsigned int volume, bool muted);
        void onRecvCustomCmdMsg(const char* userId, int cmdId, unsigned int seq, const unsigned char* msg, unsigned int msgSize);
        void onMissCustomCmdMsg(const char* userId, int cmdId, int errCode, int missed);
        void onRecvSEIMsg(const char* userId, const unsigned char* message, unsigned int msgSize);
        void onStartPublishing(int errCode, const char* errMsg);
        void onStopPublishing(int errCode, const char* errMsg);
        void onStartPublishCDNStream(int errCode, const char* errMsg);
        void onStopPublishCDNStream(int errCode, const char* errMsg);
        void onSetMixTranscodingConfig(int errCode, const char* errMsg);
        void onAudioEffectFinished(int effectId, int code);
        void onScreenCaptureCovered();
        void onScreenCaptureStarted();
        void onScreenCapturePaused(int reason);
        void onScreenCaptureResumed(int reason);
        void onScreenCaptureStoped(int reason);
        void onSnapshotComplete(const char* userId, liteav::TRTCVideoStreamType type, char* data, uint32_t length, uint32_t width, uint32_t height, liteav::TRTCVideoPixelFormat format);
        void onRemoteVideoStatusUpdated(const char* userId, liteav::TRTCVideoStreamType streamType, liteav::TRTCAVStatusType status, liteav::TRTCAVStatusChangeReason reason, void* extrainfo);

        void onLocalRecordBegin(int errCode, const char* storagePath);
        void onLocalRecording(long duration, const char* storagePath);
        void onLocalRecordComplete(int errCode, const char* storagePath);

        // ITRTCVideoRenderCallback
        delegate void RenderVideoFrameDelegate(const char* userId, liteav::TRTCVideoStreamType streamType, liteav::TRTCVideoFrame* frame);
        void onRenderVideoFrame(const char* userId, liteav::TRTCVideoStreamType streamType, liteav::TRTCVideoFrame* frame);
        // ITRTCVideoFrameCallback
        delegate int ProcessVideoFrameDelegate(liteav::TRTCVideoFrame* srcFrame, liteav::TRTCVideoFrame* dstFrame);
        int onProcessVideoFrame(liteav::TRTCVideoFrame* srcFrame, liteav::TRTCVideoFrame* dstFrame);
        // ITRTCAudioFrameCallback
        delegate void CapturedAudioFrameDelegate(liteav::TRTCAudioFrame* frame);
        delegate void PlayAudioFrameDelegate(liteav::TRTCAudioFrame* frame, const char* userId);
        delegate void MixedPlayAudioFrameDelegate(liteav::TRTCAudioFrame* frame);
        delegate void LocalProcessedAudioFrame(liteav::TRTCAudioFrame* frame);

        void onCapturedRawAudioFrame(liteav::TRTCAudioFrame* frame);
        void onPlayAudioFrame(liteav::TRTCAudioFrame* frame, const char* userId);
        void onMixedPlayAudioFrame(liteav::TRTCAudioFrame* frame);
        void onLocalProcessedAudioFrame(liteav::TRTCAudioFrame* frame);
        // ITRTCLogCallback
        delegate void LogDelegate(const char* log, liteav::TRTCLogLevel level, const char* module);

        void onLog(const char* log, liteav::TRTCLogLevel level, const char* module);
    private:
        ITRTCCloud();

        ITRTCCloud(bool useless);

        List<ITRTCCloudCallback^>^ copyTRTCCallbackList();
    private:
        static ITRTCCloud^ sInstance;
        static Object^ slocker = gcnew Object();

    private:
        liteav::ITRTCCloud* m_impl;
        bool is_sub_cloud;

        TRTCCloudCallbackImpl* m_callbackImpl;

        TRTCVideoRenderCallbackImpl* m_videoRenderCallbackImpl;
        ITRTCVideoFrameCallbackImpl* m_videoFrameCallbackImpl;

        TRTCAudioFrameCallbackImpl* m_audioFrameCallbackImpl;
        TRTCLogCallbackImpl* m_logCallbackImpl;

        Object^ m_cb_locker = gcnew Object();
        List<ITRTCCloudCallback^>^ m_callbacks;
        Dictionary<String^, ITRTCVideoRenderCallback^>^ m_videoRenderCallbacks;
        ITRTCVideoFrameCallback^ m_videoFrameCallback;
        ITRTCAudioFrameCallback^ m_audioFrameCallback;
        ITRTCLogCallback^ m_logCallback;

        ITXAudioEffectManager^ m_audioEffectManager;
        ITXDeviceManager^ m_deviceManager;

        // ITRTCCloudCallback
        ErrorDelegate^ m_errorDelegate;
        WarningDelegate^ m_warningDelegate;
        EnterRoomDelegate^ m_enterRoomDelegate;
        ExitRoomDelegate^ m_exitRoomDelegate;
        UserEnterDelegate^ m_userEnterDelegate;
        UserExitDelegate^ m_userExitDelegate;
        RemoteUserEnterRoomDelegate^ m_remoteUserEnterRoomDelegate;
        RemoteUserLeaveRoomDelegate^ m_remoteUserLeaveRoomDelegate;
        SwitchRoleDelegate^ m_switchRoleDelegate;
        SwitchRoomDelegate^ m_switchRoomDelegate;
        ConnectOtherRoomDelegate^ m_connectOtherRoomDelegate;
        DisconnectOtherRoomDelegate^ m_disconnectOtherRoomDelegate;
        UserVideoAvailableDelegate^ m_userVideoAvailableDelegate;
        UserSubStreamAvailableDelegate^ m_userSubStreamAvailableDelegate;
        UserAudioAvailableDelegate^ m_userAudioAvailableDelegate;
        UserVoiceVolumeDelegate^ m_userVoiceVolumeDelegate;
        NetworkQualityDelegate^ m_networkQualityDelegate;
        StatisticsDelegate^ m_statisticsDelegate;
        FirstVideoFrameDelegate^ m_firstVideoFrameDelegate;
        FirstAudioFrameDelegate^ m_firstAudioFrameDelegate;
        SendFirstLocalVideoFrameDelegate^ m_sendFirstLocalVideoFrameDelegate;
        SendFirstLocalAudioFrameDelegate^ m_sendFirstLocalAudioFrameDelegate;
        PlayBGMBeginDelegate^ m_playBGMBeginDelegate;
        PlayBGMProgressDelegate^ m_playBGMProgressDelegate;
        PlayBGMCompleteDelegate^ m_playBGMCompleteDelegate;
        ConnectionLostDelegate^ m_connectionLostDelegate;
        TryToReconnectDelegate^ m_tryToReconnectDelegate;
        ConnectionRecoveryDelegate^ m_connectionRecoveryDelegate;
        SpeedTestDelegate^ m_speedTestDelegate;
        CameraDidReadyDelegate^ m_cameraDidReadyDelegate;
        MicDidReadyDelegate^ m_micDidReadyDelegate;
        DeviceChangeDelegate^ m_deviceChangeDelegate;
        TestMicVolumeDelegate^ m_testMicVolumeDelegate;
        TestSpeakerVolumeDelegate^ m_testSpeakerVolumeDelegate;
        AudioDeviceCaptureVolumeChangedDelegate^ m_audioDeviceCaptureVolumeChangedDelegate;
        AudioDevicePlayoutVolumeChangedDelegate^ m_audioDevicePlayoutVolumeChangedDelegate;
        RecvCustomCmdMsgDelegate^ m_recvCustomCmdMsgDelegate;
        MissCustomCmdMsgDelegate^ m_missCustomCmdMsgDelegate;
        RecvSEIMsgDelegate^ m_recvSEIMsgDelegate;
        StartPublishingDelegate^ m_startPublishingDelegate;
        StopPublishingDelegate^ m_stopPublishingDelegate;
        StartPublishCDNStreamDelegate^ m_startPublishCDNStreamDelegate;
        StopPublishCDNStreamDelegate^ m_stopPublishCDNStreamDelegate;
        SetMixTranscodingConfigDelegate^ m_setMixTranscodingConfigDelegate;
        AudioEffectFinishedDelegate^ m_audioEffectFinishedDelegate;
        ScreenCaptureCoveredDelegate^ m_screenCaptureCoveredDelegate;
        ScreenCaptureStartedDelegate^ m_screenCaptureStartedDelegate;
        ScreenCapturePausedDelegate^ m_screenCapturePausedDelegate;
        ScreenCaptureResumedDelegate^ m_screenCaptureResumedDelegate;
        ScreenCaptureStopedDelegate^ m_screenCaptureStopedDelegate;
        SnapshotCompleteDelegate^ m_snapshotCompleteDelegate;
        LocalRecordBeginDelegate^ m_localRecordBeginDelegate;
        LocalRecordingDelegate^ m_localRecordingDelegate;
        LocalRecordCompleteDelegate^ m_localRecordCompleteDelegate;
        RemoteVideoStatusUpdatedDelegate^ m_remoteVideoStatusUpdatedDelegate;

        // ITRTCVideoRenderCallback
        RenderVideoFrameDelegate^ m_renderVideoFramedelegate;
        ProcessVideoFrameDelegate^ m_processVideoFramedelegate;
        // ITRTCAudioFrameCallback
        CapturedAudioFrameDelegate^ m_capturedAudioFrameDelegate;
        PlayAudioFrameDelegate^ m_playAudioFrameDelegate;
        MixedPlayAudioFrameDelegate^ m_mixedPlayAudioFrameDelegate;
        CapturedAudioFrameDelegate^ m_localProcessedAudioFrameDelegate;
        // ITRTCLogCallback
        LogDelegate^ m_logDeleagte;
    private:
        Dictionary<String^, TRTCVideoFrame^>^ m_video_frame_cache;
        Dictionary<String^, TRTCAudioFrame^>^ m_audio_frame_cache;
    };
    /// @}
}
