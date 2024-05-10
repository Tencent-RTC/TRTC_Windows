/**
 * Background music and reverb effects
 *
 * In this module, you can use the methods below to set background music and reverb effects for anchors after room entry.
 * - startPlayBgmMusic: start playing background music
 * - stopPlayBgmMusic: stop playing background music
 * - pausePlayBgmMusic: pause background music
 * - resumePlayBgmMusic: resume background music
 * - setVoiceReverbType: set reverb effects (karaoke, room, hall, deep, resonant, etc.)
 * - setMusicPublishVolume: set the volume of the anchor’s background music heard by remote users
 * - setMusicPlayoutVolume: set the volume of background music heard by the anchor
 * - setAllMusicVolume: set the global (local and remote) playback volume of background music
 * - setMusicPitch: change the pitch of background music
 * - setMusicSpeedRate: change the playback speed of background music
 * - setVoiceCaptureVolume: set the volume of audio captured by the mic
 */

#ifndef TESTBGMSETTING_H
#define TESTBGMSETTING_H

#include <QTemporaryDir>

#include "ITRTCCloud.h"
#include "ui_TestBGMSettingDialog.h"
#include "base_dialog.h"

class TestBGMSetting:public BaseDialog,public liteav::ITXMusicPlayObserver
{
    Q_OBJECT
public:
    explicit TestBGMSetting(QWidget* parent = nullptr);
    ~TestBGMSetting();

private:
    void startPlayBgmMusic(std::string& path,int loopCount,bool publish,bool isShortFile);
    void stopPlayBgmMusic();

    void pausePlayBgmMusic();
    void resumePlayBgmMusic();

    void setVoiceReverbType(liteav::TXVoiceReverbType type);
    void setMusicPublishVolume(int volume);
    void setMusicPlayoutVolume(int volume);
    void setAllMusicVolume(int volume);
    void setMusicPitch(float pitch);
    void setMusicSpeedRate(float speedRate);
    void setVoiceCaptureVolume(int volume);

    //============= ITXMusicPlayObserver start ===============//
    void onStart(int id,int errCode) override;
    void onPlayProgress(int id,long curPtsMS,long durationMS) override;
    void onComplete(int id,int errCode) override;
    //=============  ITXMusicPlayObserver end  ===============//


private slots:
    void on_btnStartBgm_clicked();

    void on_btnPauseBgm_clicked();

    void on_btnRestSetting_clicked();

    void on_comboxSetVoiceReverbType_currentIndexChanged(int index);

    void on_sliderSetVoiceCaptureVolum_valueChanged(int value);

    void on_sliderSetLocalVolume_valueChanged(int value);

    void on_sliderSetRemoteVolume_valueChanged(int value);

    void on_sliderSetAllVolume_valueChanged(int value);

    void on_sliderSetPitch_valueChanged(int value);

    void on_sliderSetSpeedRate_valueChanged(int value);

    void handleStart(int id,int errCode);
    void handleComplete(int id,int errCode);
    void handlePlayProgress(int id,long curPtsMS,long durationMS);

signals:
    void start(int, int);
    void complete(int, int);
    void playProgress(int, long, long);

public:
    void closeEvent(QCloseEvent *event) override;

private:
    void changeBgmControlerStatus(bool enable);
    void resetDefaultValue();
    void retranslateUi() override;
    void updateDynamicTextUI() override;
private:
     std::unique_ptr<Ui::TestBGMSettingDialog> ui_bgm_setting_;
     liteav::ITXAudioEffectManager* audio_effect_manager_ = nullptr;
     QTemporaryDir qtemp_dir_;
     int bgm_music_id = -1;
     bool started_bgm_status_ = false;
     bool paused_bgm_status_ = false;
     const QString BGM_FIRST  = "/assets/audio/bgm/test_bgm_music_first.mp3";
     const QString BGM_SECOND = "/assets/audio/bgm/test_bgm_music_second.mp3";
     const QString BGM_THIRD = "/assets/audio/bgm/test_bgm_music_third.mp3";
};

#endif // TESTBGMSETTING_H
