#include "tx_liveplayer_proxy.h"

TXLivePlayerProxy::TXLivePlayerProxy() {
  live_player_ = createV2TXLivePlayer();
}

TXLivePlayerProxy::~TXLivePlayerProxy() {
  if (live_player_ != nullptr) {
    releaseV2TXLivePlayer(live_player_);
    live_player_ = nullptr;
  }
}

void TXLivePlayerProxy::startPlay(const std::string& url) {
  live_player_->startPlay(url.c_str());
}

void TXLivePlayerProxy::stopPlay() {
  live_player_->stopPlay();
}

void TXLivePlayerProxy::pause() {
  live_player_->pauseAudio();
  live_player_->pauseVideo();
}

void TXLivePlayerProxy::resume() {
  live_player_->resumeAudio();
  live_player_->resumeVideo();
}

void TXLivePlayerProxy::setRenderFrame(void* handle) {
  live_player_->setRenderView(handle);
}

void TXLivePlayerProxy::setRenderMode(
    TXLivePlayerProxy_RenderMode render_mode) {
  live_player_->setRenderFillMode((liteav::V2TXLiveFillMode)render_mode);
}
