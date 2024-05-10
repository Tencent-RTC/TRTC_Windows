/**
 * Audio recording
 *
 * - The audio recording feature records all audio during a call, including local audio, remote audio, and background music, into a file.  This API works regardless of whether you have entered the room.  When exitRoom is called, audio recording will stop automatically.
 * -
 * - For the specific method, please refer to:
 * - startAudioRecording()
 * - stopAudioRecording()
 * - handleWithRecordingResult(int result)
 */

#ifndef TESTAUDIORECORD_H
#define TESTAUDIORECORD_H

#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>
#include <QDesktopServices>
#include <QProcess>

#include "ITRTCCloud.h"
#include "ui_TestAudioRecordDialog.h"
#include "trtc_cloud_callback_default_impl.h"
#include "base_dialog.h"

class TestAudioRecord:
        public BaseDialog,
        public TrtcCloudCallbackDefaultImpl
{
    Q_OBJECT
public:
    explicit TestAudioRecord(QWidget *parent = nullptr);
    ~TestAudioRecord();

    void startAudioRecording();
    void stopAudioRecording();
    void handleWithRecordingResult(int result);

private slots:

    void on_pushButtonStartStopRecord_clicked();

    void onRecordingTimeUpdate();

    void on_pushButtonRecordPathChoose_clicked();

    void on_pushButtonOpenPath_clicked();

public:
    //UI-related
    void closeEvent(QCloseEvent *event) override;

private:
    //UI-related
    QString formatTimeString(qint64 timeSeconds);
    void showPathInGraphicalShell(QWidget *parent, const QString &path);
    void retranslateUi() override;
    void updateDynamicTextUI() override;
private:
    bool is_recording_ = false;
    uint64_t current_recording_time_seconds_ = 0;
    uint64_t current_playback_position_seconds_ = 0;
    uint64_t current_playback_duration_seconds_ = 0;
    QString last_record_file_path;

    std::unique_ptr<Ui::TestAudioRecordDialog> ui_audio_record_;
    liteav::ITRTCCloud *trtccloud_;
    QTimer *qtimer_ = new QTimer();

};

#endif // TESTAUDIORECORD_H
