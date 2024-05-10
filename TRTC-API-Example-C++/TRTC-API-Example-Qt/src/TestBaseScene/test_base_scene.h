/**
 * Basic features, including room entry, room exit, role switching, and video/volume display
 *
 * - There are four scenarios:
 * - Video call (TRTCAppSceneVideoCall), interactive live streaming (TRTCAppSceneLIVE), audio call (TRTCAppSceneAudioCall), and audio chat room (TRTCAppSceneVoiceChatRoom)
 * - If the parameter "scene" is set to "TRTCAppSceneLIVE" or "TRTCAppSceneVoiceChatRoom", you must select a role for the current user by specifying the "role" field in TRTCParams.
 * -
 * - Room entry:  Call enterRoom to enter a room. For details, see: https://liteav.sdk.qcloud.com/doc/api/zh-cn/group__ITRTCCloud__cplusplus.html#ac73c4ad51eda05cd2bcec820c847e84f
 * - Room exit:  Call exitRoom to exit a room. Please note that after you call enterRoom, regardless of whether room entry is successful, you must call exitRoom before calling enterRoom again; otherwise, an unexpected error will occur.
 * -
 * - Role switching:  Call switchRole to switch roles. This feature is applicable only in live streaming scenarios (TRTCAppSceneLIVE and TRTCAppSceneVoiceChatRoom).
 * - There are two roles:
 * - 1. Anchor (TRTCRoleAnchor), who can send video and audio. Up to 50 anchors are allowed to send audio and video at the same time in a room.
 * - 2. Audience (TRTCRoleAudience), who can play but cannot send audio or video. There is no upper limit on the audience size in a room.
 *
 * - For the callbacks and display relevant to room entry and exit, see test_user_video_group.h.
 */

#ifndef TESTBASESCENE_H
#define TESTBAEESCENC_H

#include <QObject>
#include "ITRTCCloud.h"
#include "ui_MainWindow.h"
#include "test_user_video_group.h"
#include "trtc_cloud_callback_default_impl.h"

class TestBaseScene :public QObject,public TrtcCloudCallbackDefaultImpl
{
    Q_OBJECT
public:
    explicit TestBaseScene(std::shared_ptr<TestUserVideoGroup> testUserVideoGroup);
    ~TestBaseScene();

    void enterRoom(uint32_t roomId, std::string userId, liteav::TRTCAppScene appScene, liteav::TRTCRoleType roleType = liteav::TRTCRoleType::TRTCRoleAnchor);
    void exitRoom();
    void switchRole(liteav::TRTCRoleType roleType);

    //============= ITRTCCloudCallback start =================//
    void onEnterRoom(int result);
    void onExitRoom(int reason);
    void onSwitchRole(TXLiteAVError errCode, const char *errMsg);
    //============= ITRTCCloudCallback end ===================//

private:
    std::shared_ptr<TestUserVideoGroup> test_user_video_group_;
    uint32_t room_id_;
    std::string user_id_;
    liteav::TRTCAppScene app_scene_;
    liteav::TRTCRoleType role_type_;
    std::string stream_id_;

};

#endif //TESTBASESCENE_H
