/**
 * Parent control for user display and status management in TRTC rooms
 *
 * - Users' entry and exit, audio/video availability, unified room management
 * -
 * - Unified room management:
 * - setNetworkQosParam:  set QoS parameters
 * - muteAllRemoteVideoStreams:  pause/resume receiving all remote video streams
 * - muteAllRemoteAudio:   mute/unmute all users
 * - showDebugView:  display the dashboard, including the floating views of status statistics and event messages, which are used for debugging
 * -
 * - Callbacks for user status:
 * - onRemoteUserEnterRoom:  callback of the entry of a remote user
 * - onRemoteUserLeaveRoom:  callback of the exit of a remote user
 * - onUserVideoAvailable:  callback of whether a remote user has playable video. You can call startRemoteView to display the user’s video.
 * - onUserAudioAvailable:  callback of whether a remote user has playable audio
 * - onUserSubStreamAvailable:  callback of whether a remote user has enabled screen sharing. You can call startRemoteView to display the shared screen.
 * - onUserVoiceVolume:  callback of the volume of each remote user and total remote volume. You can enable this callback by calling enableAudioVolumeEvaluation.
 */

#ifndef TESTVIDEOGROUP_H
#define TESTVIDEOGROUP_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <map>

#include "ITRTCCloud.h"

#include "ui_TestUserVideoGroup.h"
#include "trtc_cloud_callback_default_impl.h"
#include "test_user_video_item.h"
#include "test_user_screen_share_view.h"

class TestUserVideoGroup:public QWidget,public TrtcCloudCallbackDefaultImpl
{
    Q_OBJECT
public:
    explicit TestUserVideoGroup(QWidget* parent = nullptr);
    ~TestUserVideoGroup();

private:
    void setNetworkQosParam(liteav::TRTCVideoQosPreference preference, liteav::TRTCQosControlMode controlMode);
    void muteAllRemoteVideoStreams(bool mute);
    void muteAllRemoteAudio(bool mute);
    void showDebugView(bool show);

    //============= ITRTCCloudCallback start ===============//
    void onRemoteUserEnterRoom (const char *userId) override;
    void onRemoteUserLeaveRoom (const char *userId, int reason) override;
    void onUserVideoAvailable(const char* userId, bool available) override;
    void onUserAudioAvailable(const char* userId, bool available) override;
    void onUserSubStreamAvailable(const char *userId, bool available) override;
    void onUserVoiceVolume(liteav::TRTCVolumeInfo* userVolumes, uint32_t userVolumesCount, uint32_t totalVolume) override;
    //============= ITRTCCloudCallback end =================//

signals:
    void onVolumeEvaluationStateChanged(bool state);

private slots:
    void on_networkModeCb_currentIndexChanged(int index);

    void on_muteAllRemoteAudioCb_clicked(bool checked);

    void on_muteAllRemoteVideoCb_clicked(bool checked);

    void on_openDashBoardCb_clicked(bool checked);

    void on_pushButtonShowRemoteScreenShare_clicked();

    void on_checkBoxVolumeEvaluation_stateChanged(int state);

public:
    void closeEvent(QCloseEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void changeEvent(QEvent* event);

    void setMainRoomId(int mainRoomId);
    void addUserVideoItem(liteav::ITRTCCloud* cloud,int roomId,const char *userId,const TEST_VIDEO_ITEM::ViewItemType type);
    void onSubRoomUserEnterRoom(liteav::ITRTCCloud* subCloud,int roomId,std::string userId);
    void onSubRoomUserLeaveRoom(int roomId,std::string userId);
    void onSubRoomUserVideoAvailable(liteav::ITRTCCloud*, int roomId, std::string userId, bool available);
    void onSubRoomUserAudioAvailable(int roomId, std::string userId, bool available);
    void onSubRoomExit(int roomId);
    void handleUserVolume(liteav::TRTCVolumeInfo* userVolumes, uint32_t userVolumesCount, uint32_t totalVolume);

private:
    void removeUserVideoItem(int roomId,const char *userId);
    void removeAllUsers();
    void initView();
    void handleUserVideoAvailable(liteav::ITRTCCloud* cloud, int roomId, const char* userId, bool available);
    void handleUserAudioAvailable(int roomId, const char* userId, bool available);
    void updateRemoteViewsMuteStatus(bool status, TEST_VIDEO_ITEM::MuteAllType muteType);

public:
    liteav::TXView getLocalVideoTxView();
    static const int ROW_NUM = 3;

private:
    std::unique_ptr<Ui::TestUserVideoGroup> ui_video_group_;
    int main_room_id_ = 0;

    std::vector<TestUserVideoItem*> visible_user_video_items_;
    std::unique_ptr<TestUserScreenShareView> user_screen_share_view_;
    QString current_screen_sharing_user_id_;
};



#endif // TESTVIDEOGROUP_H
