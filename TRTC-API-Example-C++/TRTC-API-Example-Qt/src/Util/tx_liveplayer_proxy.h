#ifndef TXLIVEPLAYERPROXY_H
#define TXLIVEPLAYERPROXY_H

#include <string>
#include <Live2/V2TXLivePlayer.hpp>
#include <Live2/V2TXLiveDef.hpp>
#include <Live2/V2TXLiveCode.hpp>

enum TXLivePlayerProxy_RenderMode {
  /// Fill full of screen, exceeding part cropped then display incomplete prcture
  V2TXLiveFillModeFill = 0,

  /// Long side fill the screen and short side fill with black for complete picture
  V2TXLiveFillModeFit = 1,
};

class TXLivePlayerProxy {
 public:
  TXLivePlayerProxy();
  ~TXLivePlayerProxy();

 public:
  void startPlay(const std::string& url);
  void stopPlay();
  void pause();
  void resume();
  void setRenderFrame(void* handle);
  void setRenderMode(TXLivePlayerProxy_RenderMode render_mode);

 private:
  liteav::V2TXLivePlayer* live_player_ = nullptr;
};
#endif  // TXLIVEPLAYERPROXY_H
