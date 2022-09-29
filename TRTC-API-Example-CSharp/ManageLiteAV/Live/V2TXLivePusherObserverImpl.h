#pragma once

#include "include/Live2/V2TXLivePusherObserver.hpp"

typedef void (CALLBACK * PFN_ERROR)(int32_t code, const char* msg, void* extraInfo);
typedef void (CALLBACK * PFN_WARNING)(int32_t code, const char* msg, void* extraInfo);
typedef void (CALLBACK * PFN_CAPTUREFIRSTADUIOFRAME)();
typedef void (CALLBACK * PFN_CAPTUREFIRSTVIDEOFRAME)();
typedef void (CALLBACK * PFN_MICROPHONEVOLUMEUPDATE)(int32_t volume);
typedef void (CALLBACK * PFN_PUSHSTATUSUPDATE)(liteav::V2TXLivePushStatus state, const char* msg, void* extraInfo);
typedef void (CALLBACK * PFN_STATISTICEUPDATE)(liteav::V2TXLivePusherStatistics statistics);
typedef void (CALLBACK * PFN_SNAPSHOTCOMPLETE)(const char* image, int length, int width, int height, liteav::V2TXLivePixelFormat format);
typedef void (CALLBACK * PFN_RENDERVIDEOFRAME)(const liteav::V2TXLiveVideoFrame* videoFrame);
typedef int  (CALLBACK * PFN_PROCESSVIDEOFRAME)(liteav::V2TXLiveVideoFrame* srcFrame, liteav::V2TXLiveVideoFrame* dstFrame);
typedef void (CALLBACK * PFN_SCREENCAPTURESTARTED)();
typedef void (CALLBACK * PFN_SCREENCAPTURESTOPED)(int reason);


class V2TXLivePusherObserverImpl
	: public liteav::V2TXLivePusherObserver
{
public:
	V2TXLivePusherObserverImpl();
	~V2TXLivePusherObserverImpl();

    void setError(PFN_ERROR pointer);
    void setWarning(PFN_WARNING pointer);
    void setCaptureFirstAudioFrame(PFN_CAPTUREFIRSTADUIOFRAME pointer);
    void setCaptureFirstVideoFrame(PFN_CAPTUREFIRSTVIDEOFRAME pointer);
    void setMicrophoneVolumeUpdate(PFN_MICROPHONEVOLUMEUPDATE pointer);
    void setPushStatusUpdate(PFN_PUSHSTATUSUPDATE pointer);
    void setStatisticsUpdate(PFN_STATISTICEUPDATE pointer);
    void setSnapshotComplete(PFN_SNAPSHOTCOMPLETE pointer);
    void setRenderVideoFrame(PFN_RENDERVIDEOFRAME pointer);
    void  setProcessVideoFrame(PFN_PROCESSVIDEOFRAME pointer);
    void setScreenCaptureStarted(PFN_SCREENCAPTURESTARTED pointer);
    void setScreenCaptureStoped(PFN_SCREENCAPTURESTOPED pointer);
public:
    void onError(int32_t code, const char* msg, void* extraInfo);
    void onWarning(int32_t code, const char* msg, void* extraInfo);
    void onCaptureFirstAudioFrame();
    void onCaptureFirstVideoFrame();
    void onMicrophoneVolumeUpdate(int32_t volume);
    void onPushStatusUpdate(liteav::V2TXLivePushStatus state, const char* msg, void* extraInfo);
    void onStatisticsUpdate(liteav::V2TXLivePusherStatistics statistics);
    void onSnapshotComplete(const char* image, int length, int width, int height, liteav::V2TXLivePixelFormat format);
    void onRenderVideoFrame(const liteav::V2TXLiveVideoFrame* videoFrame);
    int  onProcessVideoFrame(liteav::V2TXLiveVideoFrame* srcFrame, liteav::V2TXLiveVideoFrame* dstFrame);
    void onScreenCaptureStarted();
    void onScreenCaptureStoped(int reason);
private:
    PFN_ERROR m_Error;
    PFN_WARNING m_Warning;
    PFN_CAPTUREFIRSTADUIOFRAME m_CaptureFirstAudioFrame;
    PFN_CAPTUREFIRSTVIDEOFRAME m_CaptureFirstVideoFrame;
    PFN_MICROPHONEVOLUMEUPDATE m_MicrophoneVolumeUpdate;
    PFN_PUSHSTATUSUPDATE m_PushStatusUpdate;
    PFN_STATISTICEUPDATE m_StatisticsUpdate;
    PFN_SNAPSHOTCOMPLETE m_SnapshotComplete;
    PFN_RENDERVIDEOFRAME m_RenderVideoFrame;
    PFN_PROCESSVIDEOFRAME m_ProcessVideoFrame;
    PFN_SCREENCAPTURESTARTED m_ScreenCaptureStarted;
    PFN_SCREENCAPTURESTOPED m_ScreenCaptureStoped;
};
