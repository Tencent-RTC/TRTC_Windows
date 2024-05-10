/**
 * Custom message sending
 *
 * - Two methods:
 * - 1.  Broadcast custom data to other users in the room via the audio/video transmission channel.
 * - 2.  Embed small-volume custom data into video frames, in which case the data will be retained even if the video frames are relayed to live streaming CDNs. The most common practice is using sendSEIMsg to insert custom timestamps into video frames, which can ensure that the messages and video images are in sync.
 * -
 * - Sending messages via the audio/video transmission channel:  sendCustomMessage()
 * - Sending messages by inserting data into video frames:  sendSEIMessage()
 * - For details about the APIs, see: https://liteav.sdk.qcloud.com/doc/api/zh-cn/group__ITRTCCloud__cplusplus.html#a858b11d4d32ee0fd69b42d64a1d65389
 */

#ifndef TESTCUSTOMMESSAGE_H
#define TESTCUSTOMMESSAGE_H

#include "base_dialog.h"
#include "ui_TestCustomMessageDialog.h"
#include "ITRTCCloud.h"
#include "trtc_cloud_callback_default_impl.h"

class TestCustomMessage:
        public BaseDialog,
        public TrtcCloudCallbackDefaultImpl
{
    Q_OBJECT
public:
    explicit TestCustomMessage(QWidget *parent = nullptr);
    ~TestCustomMessage();

    //============= ITRTCCloudCallback start ===================//
    void onRecvSEIMsg(const char* userId, const uint8_t* message, uint32_t messageSize) override;
    void onMissCustomCmdMsg(const char* userId, int32_t cmdID, int32_t errCode, int32_t missed) override;
    void onRecvCustomCmdMsg(const char* userId, int32_t cmdID, uint32_t seq, const uint8_t* message, uint32_t messageSize) override;
    //============= ITRTCCloudCallback end ===================//

private:
    void sendCustomMessage();
    void sendSEIMessage();

    void retranslateUi() override;
private slots:
    void on_pushButtonSendCmdMsg_clicked();

    void on_pushButtonSendSEIMsg_clicked();

public:
    //UI-related
    void closeEvent(QCloseEvent *event) override;

private:
    std::unique_ptr<Ui::TestCustomMessageDialog> ui_custom_message_;
    liteav::ITRTCCloud *trtccloud_;
};

#endif // TESTCUSTOMMESSAGE_H
