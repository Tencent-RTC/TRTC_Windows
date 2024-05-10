/**
 * Network speed testing
 *
 * - Start network speed testing, which should be avoided during video calls to ensure call quality
 * - The test result can be used to optimize the SDK's server selection policy, so you are advised to run the test before the first call, which will help the SDK select the best server.  In addition, if the test result is not satisfactory, you can show a UI message asking users to change to a better network.
 *
 * - For the specific method, please refer to:  startSpeedTest()/stopSpeedTest()
 * - Callback: onSpeedTest()
 */

#ifndef TESTNETWORKCHECK_H
#define TESTNETWORKCHECK_H

#include <stdlib.h>

#include "base_dialog.h"
#include "ui_TestNetworkCheckDialog.h"
#include "ui_MainWindow.h"
#include "trtc_cloud_callback_default_impl.h"

class TestNetworkCheck:public BaseDialog, public TrtcCloudCallbackDefaultImpl
{
    Q_OBJECT
public:
    explicit TestNetworkCheck(QWidget *parent = nullptr);
    ~TestNetworkCheck();

private:
    void startSpeedTest(std::string& userId);
    void stopSpeedTest();

private :
    //============= ITRTCCloudCallback start =================//
    void onSpeedTest(const liteav::TRTCSpeedTestResult &currentResult, uint32_t finishedCount, uint32_t totalCount);
    //============= ITRTCCloudCallback end ===================//

private slots:

    void on_startSpeedTest_clicked();

public:
    //UI-related
    void closeEvent(QCloseEvent *event) override;

private:
    void retranslateUi() override;
    void updateDynamicTextUI() override;
    std::unique_ptr<Ui::TestNetworkCheckDialog> ui_test_network_check_;
    bool is_network_checking = false;
};

#endif // TESTNETWORKCHECK_H
