/**
 * Custom video rendering, i.e., receiving video data and rendering it by yourself instead of using the default rendering capability of the TRTC library
 *
 * - Method:  Call setLocalVideoRenderCallback()/setRemoteVideoRenderCallback() to set callbacks for the receiving of local/remote data so that you can render the data by yourself.
 * -
 * - For the specific method, please refer to:
 * - startLocalVideoRender()/stopLocalVideoRender()
 * - startRemoteVideoRender()/stopRemoteVideoRender()
 * -
 * - For details about the APIs, see: https://liteav.sdk.qcloud.com/doc/api/zh-cn/group__ITRTCCloud__cplusplus.html#ad64031e060146f7985263aad994fc733
 */

#ifndef TESTCUSTOMRENDER_H
#define TESTCUSTOMRENDER_H

#include "base_dialog.h"
#include "ITRTCCloud.h"
#include "gl_yuv_widget.h"
#include "ui_TestCustomRenderDialog.h"

class TestCustomRender:public BaseDialog, public liteav::ITRTCVideoRenderCallback
{
    Q_OBJECT
public:
    explicit TestCustomRender(QWidget * parent = nullptr);
    ~TestCustomRender();

private:
    void startLocalVideoRender();
    void startRemoteVideoRender(std::string& userId);

    void stopLocalVideoRender();
    void stopRemoteVideoRender(std::string& userId);

    //============= ITRTCVideoRenderCallback start ===================//
    void onRenderVideoFrame(const char *userId, liteav::TRTCVideoStreamType streamType, liteav::TRTCVideoFrame *frame) override;
    //=============  ITRTCVideoRenderCallback end  ===================//

private slots:
    void on_btnStartCustomRender_clicked();

signals:
    void renderViewSize(int wdith,int height);

public:
    void closeEvent(QCloseEvent* event) override;
    void showEvent(QShowEvent *) override;

    void stopRender();

    void startRender();

    void refrshUsers();

private:
    void initViews();
    void destroyCustomRender();
    void adapterRenderViewSize(int width,int height);
    void retranslateUi() override;
    void updateDynamicTextUI() override;
private:
    std::unique_ptr<Ui::TestCustomRenderDialog> ui_test_custom_render_;
    GLYuvWidget* gl_yuv_widget_ = nullptr;
    bool started_custom_render = false;
    std::string current_render_user_id;

    bool view_resized_ = false;
};

#endif // TESTCUSTOMRENDER_H
