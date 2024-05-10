#include "test_base_scene.h"

#include <QMessageBox>
#include <QObject>
#include <sstream>

#include "defs.h"
#include "GenerateTestUserSig.h"

#include "room_info_holder.h"

TestBaseScene::TestBaseScene(std::shared_ptr<TestUserVideoGroup> testUserVideoGroup)
    : test_user_video_group_(testUserVideoGroup){
    getTRTCShareInstance()->addCallback(this);
}

TestBaseScene::~TestBaseScene() {
    getTRTCShareInstance()->removeCallback(this);
}

void TestBaseScene::enterRoom(uint32_t roomId, std::string userId, liteav::TRTCAppScene appScene, liteav::TRTCRoleType roleType) {

    room_id_ = roomId;
    user_id_ = userId;
    app_scene_ = appScene;
    role_type_ = roleType;

    // Stream ID for CDN live streaming, which is optional and cannot be longer than 64 bytes. 
    // To avoid application conflicts, we recommend that you set it to "sdkappid_roomid_userid_main".
    std::ostringstream streamid_os;
    streamid_os << SDKAppID << "_" << room_id_ << "_" << user_id_ << "_" << "main";
    stream_id_ = streamid_os.str();

    liteav::TRTCParams params;
    params.sdkAppId = SDKAppID;
    params.userId = user_id_.c_str();
    /** @note:  Do not use the code below in your commercial application. This is because:
     * The code may be able to calculate UserSig correctly, but it is only for quick testing of the SDK’s basic features, not for commercial applications.
     * SDKSECRETKEY in client code can be easily decompiled and reversed, especially on web.
     * Once your key is disclosed, attackers will be able to steal your Tencent Cloud traffic.
     * The correct method is to deploy the UserSig calculation code and encryption key on your project server so that your application can request from your server a UserSig that is calculated whenever one is needed.
     * Given that it is more difficult to hack a server than a client application, server-end calculation can better protect your key.
     * Documentation:  https://intl.cloud.tencent.com/document/product/647/35166#Server
     */
    params.userSig = GenerateTestUserSig::genTestUserSig(params.userId, SDKAppID, SDKSECRETKEY);
    params.role = role_type_;
    params.roomId = room_id_;
    params.streamId = stream_id_.c_str();

    getTRTCShareInstance()->enterRoom(params, appScene);
}

void TestBaseScene::exitRoom() {
    getTRTCShareInstance()->exitRoom();
}

void TestBaseScene::switchRole(liteav::TRTCRoleType roleType){
    getTRTCShareInstance()->switchRole(roleType);
}

//============= ITRTCCloudCallback start===================//

void TestBaseScene::onEnterRoom(int result) {
    if (result > 0) {
        test_user_video_group_->setMainRoomId(room_id_);
        test_user_video_group_->show();

        // Enable audio
        getTRTCShareInstance()->enableAudioVolumeEvaluation(300); // Effective before the calling of startLocalAudio
        getTRTCShareInstance()->startLocalAudio(liteav::TRTCAudioQualityDefault);

        // Enable video
        if(app_scene_ == liteav::TRTCAppScene::TRTCAppSceneVideoCall || app_scene_ == liteav::TRTCAppScene::TRTCAppSceneLIVE){
            getTRTCShareInstance()->setBeautyStyle(liteav::TRTCBeautyStyleSmooth, 6, 6, 6);
            getTRTCShareInstance()->startLocalPreview(test_user_video_group_->getLocalVideoTxView());
        }

        RoomInfoHolder::GetInstance().setMainRoomId(room_id_);
        RoomInfoHolder::GetInstance().setUserId(user_id_);
        RoomInfoHolder::GetInstance().setCDNPublishStreamId(stream_id_);
        RoomInfoHolder::GetInstance().setMixTranscodingStreamId(stream_id_);
    } else {
        getTRTCShareInstance()->exitRoom();
    }
}

void TestBaseScene::onExitRoom(int reason) {
    test_user_video_group_->close();
    RoomInfoHolder::GetInstance().resetData();
}

void TestBaseScene::onSwitchRole(TXLiteAVError errCode, const char * errMsg){
    if (errCode == TXLiteAVError::ERR_NULL){
        QMessageBox::about(NULL, "Successfully", "Role changed successfully.");
    }else{
        QMessageBox::warning(NULL, "Failed to change the role",errMsg);
    }
}
//============= ITRTCCloudCallback end===================//

