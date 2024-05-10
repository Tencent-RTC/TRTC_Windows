/**
 * Audio settings
 *
 * - In this module, you can do the following by calling the corresponding APIs.
 * - setRemoteAudioVolume -> set the playback volume of a remote user
 * - setAudioPlayoutVolume -> set the SDK playback volume
 * - setApplicationPlayVolume -> set the volume of the current process in the Windows volume mixer
 * - setApplicationMuteState -> mute/unmute the current process in the Windows volume mixer
 * - setCurrentDeviceVolume -> set the volume of the current device
 * - setCurrentDeviceMute -> mute/unmute the current device
 * - setAudioCaptureVolume -> set the SDK capturing volume
 * - setSystemAudioLoopbackVolume -> set the system audio capturing volume
 * - startSystemAudioLoopback -> enable system audio capturing. After system audio capturing is enabled, the audio played by the entire system (if the parameter "path" is empty) or a specific player (if "path" is not empty) will be captured, mixed into the audio captured by the current mic, and sent to the cloud.
 */

#ifndef TESTAUDIOSETTING_H
#define TESTAUDIOSETTING_H

#include<QVector>

#include "ui_TestAudioSettingDialog.h"
#include "ITRTCCloud.h"
#include "room_info_holder.h"
#include "base_dialog.h"

class TestAudioSetting:
        public BaseDialog
{
    Q_OBJECT
public:
    explicit TestAudioSetting(QWidget *parent = nullptr);
    ~TestAudioSetting();

private slots:
    void on_comboBoxRemoteUsers_currentIndexChanged(const QString &value);

    void on_horizontalSliderRemoteUserVolume_valueChanged(int value);

    void on_horizontalSliderAudioPlayoutVolume_valueChanged(int value);

    void on_horizontalSliderApplicationVolume_valueChanged(int value);

    void on_checkBoxApplicationMute_stateChanged(int state);

    void on_horizontalSliderCurrentDeviceVolume_valueChanged(int value);

    void on_checkBoxCurrentDeviceMute_stateChanged(int state);

    void on_horizontalSliderAudioCaptureVolume_valueChanged(int value);

    void on_horizontalSliderSystemAudioLoopbackVolume_valueChanged(int value);

    void on_checkBoxSytemAudioLoopbak_stateChanged(int state);

public:
    //UI-related
    void resetUI() override;

private:
    //UI-related
    void showEvent(QShowEvent* event) override;
    void initUIStatus();
    void updateRemoteUsersList();
    void retranslateUi() override;

private:
    std::unique_ptr<Ui::TestAudioSettingDialog> ui_audio_setting_;
    liteav::ITRTCCloud *trtccloud_;
    liteav::ITXDeviceManager *tx_device_manager_;
    liteav::ITXAudioEffectManager *tx_audio_effect_manager_;
    QString current_selected_remote_user_id_;

};

#endif // TESTAUDIOSETTING_H
