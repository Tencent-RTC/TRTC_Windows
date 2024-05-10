/**
 * Sub-room
 *
 * - Implementation logic:  Create a TRTCCloud sub-instance and call enterRoom to enter a different room and play the audio/video of anchors in the room. Other users in the room cannot receive your audio/video as you are in the role of audience.
 * -
 * - For details on the application scenarios and APIs used, see:  https://liteav.sdk.qcloud.com/doc/api/zh-cn/group__ITRTCCloud__cplusplus.html#a5ce8b3f393ad3e46a3af39b045c1c5a2
 * - For the specific method, please refer to:  enterSubCloudRoom()/exitSubCloudRoom()
 * -
 * - Relevant callback APIs (which correspond to the callback APIs for main rooms):
 * - onEnterRoom(int result): callback of room entry
 * - onExitRoom(int reason): callback of room exit
 * - onUserVideoAvailable: callback of whether a user has playable video in a sub-room
 * - onUserAudioAvailable: callback of whether a user has playable audio in a sub-room
 * - onRemoteUserEnterRoom: callback of the entry of a user in a sub-room
 * - onRemoteUserLeaveRoom: callback of the exit of a user in a sub-room
 */

#ifndef TESTSUBCLOUDSETTING_H
#define TESTSUBCLOUDSETTING_H

#include <QDialog>

#include "test_user_video_group.h"
#include "trtc_cloud_callback_default_impl.h"

class TestSubCloudSetting:public QObject,public TrtcCloudCallbackDefaultImpl
{
    Q_OBJECT
public:
    explicit TestSubCloudSetting(std::shared_ptr<TestUserVideoGroup> testUserVideoGroup);
    ~TestSubCloudSetting();

    void enterSubCloudRoom(uint32_t roomId, std::string userId, liteav::TRTCAppScene appScene);
    void exitSubCloudRoom();

public slots:
    void volumeEvaluationStateChanged(bool state);

private:
    //============= ITRTCCloudCallback start ===============//
    void onEnterRoom(int result) override;
    void onExitRoom(int reason) override;
    void onUserVideoAvailable(const char* userId, bool available) override;
    void onUserAudioAvailable(const char* userId, bool available) override;
    void onRemoteUserEnterRoom (const char *userId) override;
    void onRemoteUserLeaveRoom (const char *userId, int reason) override;
    void onUserVoiceVolume(liteav::TRTCVolumeInfo* userVolumes, uint32_t userVolumesCount, uint32_t totalVolume) override;
    //============= ITRTCCloudCallback end =================//
signals:
    void onEnterSubRoom(bool result);
    void onExitSubRoom();

private:
    liteav::ITRTCCloud *sub_cloud_ = nullptr;
    std::shared_ptr<TestUserVideoGroup> test_user_video_group_;

    uint32_t room_id_;
    std::string user_id_;
    bool volume_evaluation_on_ = true;
};

#endif // TESTSUBCLOUDSETTING_H
