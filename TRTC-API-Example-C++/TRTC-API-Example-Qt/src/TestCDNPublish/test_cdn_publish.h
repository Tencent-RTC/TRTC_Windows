/**
 * CDN publishing
 *
 * In this module, you can use the methods below to set relayed push to Tencent Cloud’s CDN server after room entry.
 * - startPublishing:  start pushing streams to Tencent Cloud’s live streaming CDNs
 * - To use the stream pushing feature, you need to enable relayed push on the "Function Configuration" page of the TRTC console.
 * - If you select "Specified stream for relayed push",  you can call startPublishing to push streams, with the stream ID specified via the corresponding parameter of the API.
 * - If you select "Global auto-relayed push",  streams will be pushed automatically after room entry. There’s no need to call startPublishing.
 * - You can set the stream ID by specifying the "streamId" field of enterRoom or change the stream ID by calling startPublishing.
 * - Please note that you cannot call stopPublishing to stop pushing streams in the global auto-relayed push mode.
 * -
 * - stopPublishing:  stop pushing streams. This API does not work in the global auto-relayed push mode.
 * -
 * - Note:  The callback onStartPublishing() only indicates that the startPublishing API is called successfully. It does not necessarily mean that streams are successfully pushed to the cloud.
 * - Note:  In the global auto-relayed push mode, the error code "-102069" will be returned if stopPublishing is called.
 */

#ifndef TESTCDNPUBLISH_H
#define TESTCDNPUBLISH_H

#include "base_dialog.h"
#include "ui_TestCdnPublishDialog.h"
#include "trtc_cloud_callback_default_impl.h"

class TestCdnPublish:public BaseDialog,public TrtcCloudCallbackDefaultImpl{
    Q_OBJECT
public:
    explicit TestCdnPublish(QWidget* parent = nullptr);
    ~TestCdnPublish();
private:
    void stopPublishing();
    void startPublishing(std::string streamId);
    //============= ITRTCCloudCallback start ===============//
    void onStartPublishing (int err, const char *errMsg) override;
    void onStopPublishing (int err, const char *errMsg) override;
    //============= ITRTCCloudCallback end =================//
    void retranslateUi() override;
    void updateDynamicTextUI() override;
private slots:
    void on_switchPublishStatus_clicked();
    void on_streamIdLineEt_textChanged(const QString &streamId);

public:
    void showEvent(QShowEvent *) override;

private:
    Ui::TestCdnPublishDialog* ui;
    bool is_publishing_ = false;
    bool is_manual_closing_ = false;
};

#endif // TESTCDNPUBLISH_H
