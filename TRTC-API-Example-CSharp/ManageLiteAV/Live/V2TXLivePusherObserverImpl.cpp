#include "V2TXLivePusherObserverImpl.h"

V2TXLivePusherObserverImpl::V2TXLivePusherObserverImpl()
{
}

V2TXLivePusherObserverImpl::~V2TXLivePusherObserverImpl()
{
}

void V2TXLivePusherObserverImpl::setError(PFN_ERROR pointer) {
    m_Error = pointer;
}

void V2TXLivePusherObserverImpl::setWarning(PFN_WARNING pointer) {
    m_Warning = pointer;
}

void V2TXLivePusherObserverImpl::setCaptureFirstAudioFrame(PFN_CAPTUREFIRSTADUIOFRAME pointer) {
    m_CaptureFirstAudioFrame = pointer;
}

void V2TXLivePusherObserverImpl::setCaptureFirstVideoFrame(PFN_CAPTUREFIRSTVIDEOFRAME pointer) {
    m_CaptureFirstVideoFrame = pointer;
}

void V2TXLivePusherObserverImpl::setMicrophoneVolumeUpdate(PFN_MICROPHONEVOLUMEUPDATE pointer) {
    m_MicrophoneVolumeUpdate = pointer;
}

void V2TXLivePusherObserverImpl::setPushStatusUpdate(PFN_PUSHSTATUSUPDATE pointer) {
    m_PushStatusUpdate = pointer;
}

void V2TXLivePusherObserverImpl::setStatisticsUpdate(PFN_STATISTICEUPDATE pointer) {
    m_StatisticsUpdate = pointer;
}

void V2TXLivePusherObserverImpl::setSnapshotComplete(PFN_SNAPSHOTCOMPLETE pointer) {
    m_SnapshotComplete = pointer;
}

void V2TXLivePusherObserverImpl::setRenderVideoFrame(PFN_RENDERVIDEOFRAME pointer) {
    m_RenderVideoFrame = pointer;
}

void  V2TXLivePusherObserverImpl::setProcessVideoFrame(PFN_PROCESSVIDEOFRAME pointer) {
    m_ProcessVideoFrame = pointer;
}

void V2TXLivePusherObserverImpl::setScreenCaptureStarted(PFN_SCREENCAPTURESTARTED pointer) {
    m_ScreenCaptureStarted = pointer;
}

void V2TXLivePusherObserverImpl::setScreenCaptureStoped(PFN_SCREENCAPTURESTOPED pointer) {
    m_ScreenCaptureStoped = pointer;
}

void V2TXLivePusherObserverImpl::onError(int32_t code, const char* msg, void* extraInfo) {
    if (m_Error)
    {
        m_Error(code, msg, extraInfo);
    }
}

void V2TXLivePusherObserverImpl::onWarning(int32_t code, const char* msg, void* extraInfo) {
    if (m_Warning)
    {
        m_Warning(code, msg, extraInfo);
    }
}

void V2TXLivePusherObserverImpl::onCaptureFirstAudioFrame() {
    if (m_CaptureFirstAudioFrame)
    {
        m_CaptureFirstAudioFrame();
    }
}

void V2TXLivePusherObserverImpl::onCaptureFirstVideoFrame() {
    if (m_CaptureFirstVideoFrame)
    {
        m_CaptureFirstVideoFrame();
    }
}

void V2TXLivePusherObserverImpl::onMicrophoneVolumeUpdate(int32_t volume) {
    if (m_MicrophoneVolumeUpdate)
    {
        m_MicrophoneVolumeUpdate(volume);
    }
}

void V2TXLivePusherObserverImpl::onPushStatusUpdate(liteav::V2TXLivePushStatus state, const char* msg, void* extraInfo) {
    if (m_PushStatusUpdate)
    {
        m_PushStatusUpdate(state, msg, extraInfo);
    }
}

void V2TXLivePusherObserverImpl::onStatisticsUpdate(liteav::V2TXLivePusherStatistics statistics) {
    if (m_StatisticsUpdate)
    {
        m_StatisticsUpdate(statistics);
    }
}

void V2TXLivePusherObserverImpl::onSnapshotComplete(const char* image, int length, int width, int height, liteav::V2TXLivePixelFormat format) {
    if (m_SnapshotComplete)
    {
        m_SnapshotComplete(image, length, width, height, format);
    }
}

void V2TXLivePusherObserverImpl::onRenderVideoFrame(const liteav::V2TXLiveVideoFrame* videoFrame) {
    if (m_RenderVideoFrame)
    {
        m_RenderVideoFrame(videoFrame);
    }
}

int  V2TXLivePusherObserverImpl::onProcessVideoFrame(liteav::V2TXLiveVideoFrame* srcFrame, liteav::V2TXLiveVideoFrame* dstFrame) {
    if (m_ProcessVideoFrame)
    {
        return m_ProcessVideoFrame(srcFrame, dstFrame);
    }
    return 0;
}

void V2TXLivePusherObserverImpl::onScreenCaptureStarted() {
    if (m_ScreenCaptureStarted)
    {
        m_ScreenCaptureStarted();
    }
}

void V2TXLivePusherObserverImpl::onScreenCaptureStoped(int reason) {
    if (m_ScreenCaptureStoped)
    {
        m_ScreenCaptureStoped(reason);
    }
}
