#include "TXVodPlayerCallback.h"

ITXVodPlayerDataCallbackImpl::ITXVodPlayerDataCallbackImpl()
	: m_vodAudioFrame(nullptr)
	, m_vodVideoFrame(nullptr)
{
}

ITXVodPlayerDataCallbackImpl::~ITXVodPlayerDataCallbackImpl()
{

}

void ITXVodPlayerDataCallbackImpl::setVodVideoFrame(PFN_VODVIDEOFRAME pointer) {
    m_vodVideoFrame = pointer;
}

void ITXVodPlayerDataCallbackImpl::setVodAudioFrame(PFN_VODAUDIOFRAME pointer) {
    m_vodAudioFrame = pointer;
}

int ITXVodPlayerDataCallbackImpl::onVodVideoFrame(::TRTCVideoFrame& frame) {
	if (m_vodVideoFrame != nullptr)
	{
		return m_vodVideoFrame(frame);
	}

	return 0;
}

int ITXVodPlayerDataCallbackImpl::onVodAudioFrame(::TRTCAudioFrame& frame) {
	if (m_vodAudioFrame != nullptr)
	{
		return m_vodAudioFrame(frame);
	}

	return 0;
}


ITXVodPlayerEventCallbackImpl::ITXVodPlayerEventCallbackImpl() 
	: m_vodPlayerError(nullptr)
	, m_vodPlayerPaused(nullptr)
	, m_vodPlayerProgress(nullptr)
	, m_vodPlayerResumed(nullptr)
	, m_vodPlayerStarted(nullptr)
	, m_vodPlayerStoped(nullptr)
{

}

ITXVodPlayerEventCallbackImpl::~ITXVodPlayerEventCallbackImpl() {

}

void ITXVodPlayerEventCallbackImpl::setVodPlayerStarted(PFN_VODPLAYERSTARTED pointer) {
	m_vodPlayerStarted = pointer;
}

void ITXVodPlayerEventCallbackImpl::setVodPlayerProcess(PFN_VODPLAYERPROCESS pointer) {
	m_vodPlayerProgress = pointer;
}

void ITXVodPlayerEventCallbackImpl::setVodPlayerPaused(PFN_VODPLAYERPAUSED pointer) {
	m_vodPlayerPaused = pointer;
}

void ITXVodPlayerEventCallbackImpl::setVodPlayerResumed(PFN_VODPLAYERRESUMED pointer) {
	m_vodPlayerResumed = pointer;
}

void ITXVodPlayerEventCallbackImpl::setVodPlayerStoped(PFN_VODPLAYERSTOPED pointer) {
	m_vodPlayerStoped = pointer;
}

void ITXVodPlayerEventCallbackImpl::setVodPlayerError(PFN_VODPLAYERERROR pointer) {
	m_vodPlayerError = pointer;
}

void ITXVodPlayerEventCallbackImpl::onVodPlayerStarted(uint64_t msLength) {
	if (m_vodPlayerStarted != nullptr)
	{
		m_vodPlayerStarted(msLength);
	}
}

void ITXVodPlayerEventCallbackImpl::onVodPlayerProgress(uint64_t msPos) {
	if (m_vodPlayerProgress != nullptr)
	{
		m_vodPlayerProgress(msPos);
	}
}

void ITXVodPlayerEventCallbackImpl::onVodPlayerPaused() {
	if (m_vodPlayerPaused != nullptr)
	{
		m_vodPlayerPaused();
	}
}

void ITXVodPlayerEventCallbackImpl::onVodPlayerResumed() {
	if (m_vodPlayerResumed != nullptr)
	{
		m_vodPlayerResumed();
	}
}

void ITXVodPlayerEventCallbackImpl::onVodPlayerStoped(int reason) {
	if (m_vodPlayerStoped != nullptr)
	{
		m_vodPlayerStoped(reason);
	}
}

void ITXVodPlayerEventCallbackImpl::onVodPlayerError(int error) {
	if (m_vodPlayerError != nullptr)
	{
		m_vodPlayerError(error);
	}
}