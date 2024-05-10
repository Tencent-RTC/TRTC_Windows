/**
 * Cross-room call (anchor competition)
 *
 * The "cross-room call" feature allows two anchors in different rooms to co-anchor with each other without the need to exit their own rooms.
 * - Example:  Anchor A in room 001 uses connectOtherRoom() to call anchor B in room 002. After the call is connected, all users in room 001 receive the onUserEnter(B) and onUserVideoAvailable(B,true) callbacks,  and all users in room 002 receive the onUserEnter(A) and onUserVideoAvailable(A,true) callbacks.
 * - For the specific method, please refer to connectOtherRoom()/disconnectOtherRoom().
 */

#ifndef TESTCONNECTOTHERROOM_H
#define TESTCONNECTOTHERROOM_H

#include <QDialog>
#include "trtc_cloud_callback_default_impl.h"
#include "ui_MainWindow.h"

class TestConnectOtherRoom: public QObject, public TrtcCloudCallbackDefaultImpl
{
    Q_OBJECT

public:
    TestConnectOtherRoom();
    ~TestConnectOtherRoom();

    void connectOtherRoom(uint32_t roomId, std::string userId);
    void disconnectOtherRoom();

    //============= ITRTCCloudCallback start ===============//
    void onDisconnectOtherRoom(TXLiteAVError errCode, const char* errMsg) override;
    void onConnectOtherRoom(const char* userId, TXLiteAVError errCode, const char* errMsg) override;
    //============= ITRTCCloudCallback end =================/

signals:
    void onConnectOtherRoomResult(bool success);
    void onExitOtherRoomConnection();
private:
    std::string room_id_;
    std::string user_id_;
};

#endif // TESTCONNECTOTHERROOM_H
