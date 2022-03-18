#ifndef TXLIVEPLAYERPROXY_H
#define TXLIVEPLAYERPROXY_H

#include <string>
#include <Live2/V2TXLivePlayer.hpp>
#include <Live2/V2TXLiveDef.hpp>
#include <Live2/V2TXLiveCode.hpp>

enum TXLivePlayerProxy_RenderMode {
  /// 图像铺满屏幕，超出显示视窗的视频部分将被裁剪，画面显示可能不完整
  V2TXLiveFillModeFill = 0,

  /// 图像长边填满屏幕，短边区域会被填充黑色，画面的内容完整
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
