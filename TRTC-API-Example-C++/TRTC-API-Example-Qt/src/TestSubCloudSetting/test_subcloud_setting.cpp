#include "test_subcloud_setting.h"
#include <QMessageBox>
#include <QObject>
#include <QDebug>
#include "GenerateTestUserSig.h"
#include "defs.h"

TestSubCloudSetting::TestSubCloudSetting(std::shared_ptr<TestUserVideoGroup> testUserVideoGroup) :
    test_user_video_group_(testUserVideoGroup) {
}

TestSubCloudSetting::~TestSubCloudSetting(){
    exitSubCloudRoom();
}

void TestSubCloudSetting::enterSubCloudRoom(uint32_t roomId, std::string userId, liteav::TRTCAppScene appScene) {
    sub_cloud_ = getTRTCShareInstance()->createSubCloud();

    liteav::TRTCParams params;
    params.role = liteav::TRTCRoleAudience;
    room_id_ = roomId;
    params.roomId = room_id_;
    params.sdkAppId = SDKAppID;
    params.userId = userId.c_str();
    /** @note:  Do not use the code below in your commercial application. This is because:
     * The code may be able to calculate UserSig correctly, but it is only for quick testing of the SDK’s basic features, not for commercial applications.
     * SDKSECRETKEY in client code can be easily decompiled and reversed, especially on web.
     * Once your key is disclosed, attackers will be able to steal your Tencent Cloud traffic.
     * The correct method is to deploy the UserSig calculation code and encryption key on your project server so that your application can request from your server a UserSig that is calculated whenever one is needed.
     * Given that it is more difficult to hack a server than a client application, server-end calculation can better protect your key.
     * Documentation:  https://intl.cloud.tencent.com/document/product/647/35166#Server
     */
    params.userSig = GenerateTestUserSig::genTestUserSig(params.userId, SDKAppID, SDKSECRETKEY);

    sub_cloud_->addCallback(this);
    sub_cloud_->enterRoom(params, appScene);
}

void TestSubCloudSetting::exitSubCloudRoom() {
    if (sub_cloud_ != nullptr) {
        sub_cloud_->exitRoom();
    }
}

//============= ITRTCCloudCallback start ===============//
void TestSubCloudSetting::onEnterRoom(int result){
    if(result > 0) {
        QMessageBox::about(NULL, "Successfully", QString("Entered the sub-room successfully.").arg(result));
        sub_cloud_->enableAudioVolumeEvaluation(volume_evaluation_on_ ? 300 : 0);
        emit onEnterSubRoom(true);
    } else {
        QMessageBox::warning(NULL, "Failed to enter the sub-room", QString("errCode = %1").arg(result),QMessageBox::Ok);
    }
}

void TestSubCloudSetting::onExitRoom(int reason){
    test_user_video_group_->onSubRoomExit(room_id_);
    room_id_ = 0;
    user_id_ = "";
    getTRTCShareInstance()->destroySubCloud(sub_cloud_);
    sub_cloud_ = nullptr;
    emit onExitSubRoom();
}

void TestSubCloudSetting::volumeEvaluationStateChanged(bool state) {
    volume_evaluation_on_ = state;
    if (sub_cloud_ != nullptr) {
        sub_cloud_->enableAudioVolumeEvaluation(state ? 300 : 0);
    }
}

void TestSubCloudSetting::onUserVideoAvailable(const char *userId, bool available)
{
    qDebug() << "RoomState: TestSubCloudSetting onUserVideoAvailable(userId:" << userId << ",available:" << available;
    test_user_video_group_->onSubRoomUserVideoAvailable(sub_cloud_, room_id_, userId, available);
}

void TestSubCloudSetting::onUserAudioAvailable(const char *userId, bool available)
{
    qDebug() << "RoomState: TestSubCloudSetting onUserAudioAvailable(userId:" << userId << ",available:" << available;
    test_user_video_group_->onSubRoomUserAudioAvailable(room_id_, userId, available);
}

void TestSubCloudSetting::onRemoteUserEnterRoom(const char *userId){
    qDebug() << "RoomState: TestSubCloudSetting onRemoteUserEnterRoom(userId:" << userId;
    test_user_video_group_->onSubRoomUserEnterRoom(sub_cloud_, room_id_, userId);
}

void TestSubCloudSetting::onRemoteUserLeaveRoom(const char *userId, int reason){
    qDebug() << "RoomState: TestSubCloudSetting onRemoteUserLeaveRoom(userId:" << userId;
    test_user_video_group_->onSubRoomUserLeaveRoom(room_id_, userId);
}

void TestSubCloudSetting::onUserVoiceVolume(liteav::TRTCVolumeInfo* userVolumes, uint32_t userVolumesCount, uint32_t totalVolume) {
    test_user_video_group_->handleUserVolume(userVolumes, userVolumesCount, totalVolume);
}

//============= ITRTCCloudCallback end =================//
