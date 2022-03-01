#include "test_cdn_player.h"

#include <QDebug>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>
#include "ITRTCCloud.h"
#include "defs.h"

TestCdnPlayer::TestCdnPlayer(QWidget* parent)
    : BaseDialog(parent), ui_test_cdn_player_(new Ui::TestCdnPlayerDialog) {
  ui_test_cdn_player_->setupUi(this);
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  ui_test_cdn_player_->lineEditDoMain->setText(
      QString::fromLocal8Bit(DOMAIN_URL));
  domain_url_ = DOMAIN_URL;
  live_player_ = new TXLivePlayerProxy();
  live_player_->setRenderMode(
      TXLivePlayerProxy_RenderMode::V2TXLiveFillModeFit);
}

TestCdnPlayer::~TestCdnPlayer() {
  if (live_player_ != nullptr) {
    delete live_player_;
    live_player_ = nullptr;
  }
}

void TestCdnPlayer::on_btStart_clicked() {
  if (live_player_ == nullptr) {
    return;
  }

  if (started_) {
    live_player_->stopPlay();
  } else {
    if (stream_id_.isEmpty()) {
      QMessageBox::warning(this, "Failed to play CDN streams",
                           "Enter a stream ID.", QMessageBox::Ok);
      return;
    }

    if (domain_url_.isEmpty()) {
      QMessageBox::warning(this, "Failed to play CDN streams",
                           "Enter a playback domain name.", QMessageBox::Ok);
      return;
    }

    std::string play_url = play_url_.toStdString();
    live_player_->setRenderFrame(reinterpret_cast<liteav::TXView>(
        ui_test_cdn_player_->videoPlaceHolder->winId()));
    live_player_->startPlay(play_url.c_str());
  }
  paused_ = false;
  started_ = !started_;
  updateDynamicTextUI();
}

void TestCdnPlayer::on_btPause_clicked() {
  if (!started_) {
    return;
  }
  if (paused_) {
    live_player_->resume();
  } else {
    live_player_->pause();
  }
  paused_ = !paused_;
  updateDynamicTextUI();
}

void TestCdnPlayer::closeEvent(QCloseEvent* event) {
  if (started_) {
    live_player_->stopPlay();
    started_ = false;
  }
  paused_ = false;
  updateDynamicTextUI();
  QDialog::closeEvent(event);
}

void TestCdnPlayer::updateDynamicTextUI() {
  if (started_) {
    ui_test_cdn_player_->btStart->setText(tr("结束").toUtf8());
  } else {
    ui_test_cdn_player_->btStart->setText(tr("开始").toUtf8());
  }
  if (paused_) {
    ui_test_cdn_player_->btPause->setText(tr("恢复").toUtf8());
  } else {
    ui_test_cdn_player_->btPause->setText(tr("暂停").toUtf8());
  }
}

void TestCdnPlayer::on_lineEditDoMain_textChanged(QString text) {
  domain_url_ = ui_test_cdn_player_->lineEditDoMain->text();
  updatePlayerURL();
}

void TestCdnPlayer::on_lineEditStreamId_textChanged(QString text) {
  stream_id_ = ui_test_cdn_player_->lineEditStreamId->text();
  updatePlayerURL();
}

void TestCdnPlayer::updatePlayerURL() {
  play_url_ = QString("http://%1/live/%2.flv").arg(domain_url_).arg(stream_id_);
  ui_test_cdn_player_->play_url_label->setText(play_url_);
}

void TestCdnPlayer::retranslateUi() {
    ui_test_cdn_player_->retranslateUi(this);
}
