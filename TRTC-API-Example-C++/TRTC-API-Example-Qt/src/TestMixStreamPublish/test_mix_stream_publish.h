/**
 * CDN stream mixing (2 or more anchors)
 *
 * - To use this feature, you must enable global auto-relayed push on the
 * "Function Configuration" page of the TRTC console, so that each channel of
 * video in the room has a default CDN address.
 * - There may be multiple anchors in a room, each sending their own video and
 * audio, but CDN audience needs only one live stream. Therefore, you need to
 * mix multiple audio/video streams into one standard live stream, which
 * requires mixtranscoding.
 * - When you call the setMixTranscodingConfig() API, the SDK will send a
 * command to the Tencent Cloud transcoding server to combine multiple
 * audio/video streams in the room into one stream. You can use the "mixUsers"
 * parameter to set the position of each channel of image and specify whether to
 * mix only audio. You can also set the encoding parameters of the mixed stream,
 * including "videoWidth", "videoHeight", and "videoBitrate".
 * -
 * - Supported stream mixing methods:
 * - 1. Manual - startManualTemplate()
 * - 2. Preset layout - startPresetLayoutTemplate()
 * - 3. Screen sharing - startScreenSharingTemplate()
 * - 4. Audio only - startPureAudioTemplate()
 *
 * - For details about the APIs, see:
 * https://liteav.sdk.qcloud.com/doc/api/zh-cn/group__ITRTCCloud__cplusplus.html#a8c835f1d49ab0f80a85569e030689850
 * - For more on stream mixing scenarios and instructions, see:
 * https://intl.cloud.tencent.com/document/product/647/34618
 */

#ifndef TESTMIXSTREAMPUBLISH_H
#define TESTMIXSTREAMPUBLISH_H

#include <QButtonGroup>
#include <set>

#include "trtc_cloud_callback_default_impl.h"
#include "base_dialog.h"
#include "ui_TestMixStreamPublishDialog.h"

class TestMixStreamPublish : public BaseDialog,
                             public TrtcCloudCallbackDefaultImpl {
  Q_OBJECT
 public:
  explicit TestMixStreamPublish(QWidget* parent = nullptr);
  ~TestMixStreamPublish();

 private:
  void startManualTemplate();
  void startPresetLayoutTemplate();
  void startScreenSharingTemplate();
  void startPureAudioTemplate();

  //============= ITRTCCloudCallback start ===============//
  void onExitRoom(int reason) override;
  void onSetMixTranscodingConfig(int errCode, const char* errMsg) override;
  void onUserVideoAvailable(const char* userId, bool available) override;
  void onUserAudioAvailable(const char* userId, bool available) override;

  // In the manual mode, if screen sharing is enabled, you need to manually
  // change the user’s stream to mix.
  void onScreenCaptureStarted() override;
  void onScreenCaptureStoped(int reason) override;
  //============= ITRTCCloudCallback end =================//

 private slots:
  void on_streamIdLineEt_textChanged(const QString& arg1);
  void on_startMixStreamPublishBt_clicked();
  void on_config_mode_checked_change();

 public:
  void closeEvent(QCloseEvent* event) override;
  void showEvent(QShowEvent* event) override;

 private:
  void initUI();
  bool isStartMixStreamBtAvailable();
  bool getTranscodingConfig();
  void updateTranscodingConfig();
  void updatePublishButtonStatus();
  void retranslateUi() override;
  void updateDynamicTextUI();

 private:
  struct RemoteUserInfo {
    std::string user_id_;
    bool video_available_ = false;
    bool audio_available_ = false;
  };

  static constexpr const int32_t kAudioSampleRate[]{
      16000,
      32000,
      44100,
      48000,
  };

 private:
  std::unique_ptr<Ui::TestMixStreamPublishDialog> ui_test_mix_stream_publish_;

  liteav::TRTCTranscodingConfig trtc_transcoding_config;
  //============= TRTCTranscodingConfig start ===============//
  liteav::TRTCTranscodingConfigMode mix_config_mode_ =
      liteav::TRTCTranscodingConfigMode_Unknown;
  uint32_t video_width_ = 360;
  uint32_t video_height_ = 640;
  uint32_t video_bitrate_ = 64;
  uint32_t video_framerate_ = 15;
  uint32_t video_gop_ = 2;
  uint32_t background_color_ = 0x000000;
  std::string background_imag_;
  uint32_t audio_samplerate_ = 48000;
  uint32_t audio_bitrate_ = 64;
  uint32_t audio_channels_ = 1;
  //============= TRTCTranscodingConfig end =================//

  // Data lock, which can prevent data inconsistency
  std::vector<RemoteUserInfo*> remote_userinfos_;
  bool screen_shared_started_ = false;

  QButtonGroup config_mode_button_group;
  bool started_transcoding_ = false;
};

#endif  // TESTMIXSTREAMPUBLISH_H
