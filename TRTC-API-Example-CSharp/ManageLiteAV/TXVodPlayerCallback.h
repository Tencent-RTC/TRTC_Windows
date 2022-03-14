#pragma once
// C++ Header
#include "include/Vod/ITXVodPlayer.h"	

// C++/CLI Header
#include "cli/TRTC/TRTCCloudDef.h"

// ITXVodPlayerDataCallbackImpl
typedef int (CALLBACK* PFN_VODVIDEOFRAME)(TRTCVideoFrame& frame);
typedef int (CALLBACK* PFN_VODAUDIOFRAME)(TRTCAudioFrame& frame);
// end ITXVodPlayerDataCallbackImpl

// ITXVodPlayerEventCallbackImpl
typedef void (CALLBACK* PFN_VODPLAYERSTARTED)(uint64_t msLength);
typedef void (CALLBACK* PFN_VODPLAYERPROCESS)(uint64_t msPos);
typedef void (CALLBACK* PFN_VODPLAYERPAUSED)();
typedef void (CALLBACK* PFN_VODPLAYERRESUMED)();
typedef void (CALLBACK* PFN_VODPLAYERSTOPED)(int reason);
typedef void (CALLBACK* PFN_VODPLAYERERROR)(int error);
// end ITXVodPlayerEventCallbackImpl

class ITXVodPlayerDataCallbackImpl
	: public ITXVodPlayerDataCallback
{
public:
	ITXVodPlayerDataCallbackImpl();
	virtual ~ITXVodPlayerDataCallbackImpl();
	void setVodVideoFrame(PFN_VODVIDEOFRAME pointer);

	void setVodAudioFrame(PFN_VODAUDIOFRAME pointer);

	/**
	 * Vod视频帧回调
	 * param frame 音频帧数据
	 */
	virtual int onVodVideoFrame(TRTCVideoFrame& frame);

	/**
	 * Vod音频帧回调
	 * param frame 音频帧数据
	 */
	virtual int onVodAudioFrame(TRTCAudioFrame& frame);
private:
	PFN_VODVIDEOFRAME m_vodVideoFrame;
	PFN_VODAUDIOFRAME m_vodAudioFrame;
};


///播放事件回调
class ITXVodPlayerEventCallbackImpl : public ITXVodPlayerEventCallback
{
public:
	ITXVodPlayerEventCallbackImpl();
	virtual ~ITXVodPlayerEventCallbackImpl();

	void setVodPlayerStarted(PFN_VODPLAYERSTARTED pointer);

	void setVodPlayerProcess(PFN_VODPLAYERPROCESS pointer);

	void setVodPlayerPaused(PFN_VODPLAYERPAUSED pointer);

	void setVodPlayerResumed(PFN_VODPLAYERRESUMED pointer);

	void setVodPlayerStoped(PFN_VODPLAYERSTOPED pointer);

	void setVodPlayerError(PFN_VODPLAYERERROR pointer);

	virtual void onVodPlayerStarted(uint64_t msLength);

	/**
	* 当多媒体文件播放进度改变时，SDK会通过此回调通知
	*
	* param msPos 多媒体文件播放进度，单位毫秒
	*/
	virtual void onVodPlayerProgress(uint64_t msPos);

	/**
	* 当多媒体文件播放暂停时，SDK会通过此回调通知
	*/
	virtual void onVodPlayerPaused();

	/**
	* 当多媒体文件播放恢复时，SDK会通过此回调通知
	*/
	virtual void onVodPlayerResumed();

	/**
	* 当多媒体文件播放停止时，SDK会通过此回调通知
	*
	* param reason 停止原因，0表示用户主动停止，1表示文件播放完，2表示视频断流
	*/
	virtual void onVodPlayerStoped(int reason);

	/**
	* 当多媒体文件播放出错时，SDK会通过此回调通知
	*
	* param error 错误码
	*/
	virtual void onVodPlayerError(int error);
private:

	PFN_VODPLAYERSTARTED m_vodPlayerStarted;
	PFN_VODPLAYERPROCESS m_vodPlayerProgress;
	PFN_VODPLAYERPAUSED  m_vodPlayerPaused;
	PFN_VODPLAYERRESUMED m_vodPlayerResumed;
	PFN_VODPLAYERSTOPED  m_vodPlayerStoped;
	PFN_VODPLAYERERROR   m_vodPlayerError;
};