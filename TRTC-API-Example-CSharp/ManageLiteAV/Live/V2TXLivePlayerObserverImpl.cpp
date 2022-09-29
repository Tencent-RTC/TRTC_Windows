#include "V2TXLivePlayerObserverImpl.h"

V2TXLivePlayerObserverImpl::V2TXLivePlayerObserverImpl()
{

}

V2TXLivePlayerObserverImpl::~V2TXLivePlayerObserverImpl()
{
}

void V2TXLivePlayerObserverImpl::setError(PFN_ERROR pointer) {
    m_Error = pointer;
}

void V2TXLivePlayerObserverImpl::setWarning(PFN_WARNING pointer) {
    m_Warning = pointer;
}

void V2TXLivePlayerObserverImpl::setVideoResolutionChanged(PFN_VIDEORESOLUTIONCHANGED pointer) {
    m_VideoResolutionChanged = pointer;
}

void V2TXLivePlayerObserverImpl::setConnected(PFN_CONNECTED  pointer) {
    m_Connected = pointer;
}

void V2TXLivePlayerObserverImpl::setVideoPlaying(PFN_VIDEOPLAYING  pointer) {
    m_VideoPlaying = pointer;
}

void V2TXLivePlayerObserverImpl::setAudioPlaying(PFN_AUDIOPLAYING  pointer) {
    m_AudioPlaying = pointer;
}

void V2TXLivePlayerObserverImpl::setVideoLoading(PFN_VIDEOLOADING  pointer) {
    m_VideoLoading = pointer;
};

void V2TXLivePlayerObserverImpl::setAudioLoading(PFN_ADUIOLOADING  pointer) {
    m_AudioLoading = pointer;
}

void V2TXLivePlayerObserverImpl::setPlayoutVolumeUpdate(PFN_PLAYOUTVOLUMEUPDATE  pointer) {
    m_PlayoutVolumeUpdate = pointer;
}

void V2TXLivePlayerObserverImpl::setStatisticsUpdate(PFN_STATISTICSUPDATE  pointer) {
    m_StatisticsUpdate = pointer;
}

void V2TXLivePlayerObserverImpl::setSnapshotComplete(PFN_SNAPSHOTCOMPLETE  pointer) {
    m_SnapshotComplete = pointer;
}

void V2TXLivePlayerObserverImpl::setRenderVideoFrame(PFN_RENDERVIDEOFRAME  pointer) {
    m_RenderVideoFrame = pointer;
}

void V2TXLivePlayerObserverImpl::setReceiveSeiMessage(PFN_RECEIVESEIMESSAGE  pointer) {
    m_ReceiveSeiMessage = pointer;
}

void V2TXLivePlayerObserverImpl::onError(liteav::V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo) {
    if (m_Error)
    {
        m_Error(player, code, msg, extraInfo);
    }
}

void V2TXLivePlayerObserverImpl::onWarning(liteav::V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo) {
    if (m_Warning)
    {
        m_Warning(player, code, msg, extraInfo);
    }
}

void V2TXLivePlayerObserverImpl::onVideoResolutionChanged(liteav::V2TXLivePlayer* player, int width, int height) {
    if (m_VideoResolutionChanged)
    {
        m_VideoResolutionChanged(player, width, height);
    }
}

void V2TXLivePlayerObserverImpl::onConnected(liteav::V2TXLivePlayer* player, void* extraInfo) {
    if (m_Connected)
    {
        m_Connected(player, extraInfo);
    }
}

void V2TXLivePlayerObserverImpl::onVideoPlaying(liteav::V2TXLivePlayer* player, bool firstPlay, void* extraInfo) {
    if (m_VideoPlaying)
    {
        m_VideoPlaying(player, firstPlay, extraInfo);
    }
}

void V2TXLivePlayerObserverImpl::onAudioPlaying(liteav::V2TXLivePlayer* player, bool firstPlay, void* extraInfo) {
    if (m_AudioPlaying)
    {
        m_AudioPlaying(player, firstPlay, extraInfo);
    }
}

void V2TXLivePlayerObserverImpl::onVideoLoading(liteav::V2TXLivePlayer* player, void* extraInfo) {
    if (m_VideoLoading)
    {
        m_VideoLoading(player, extraInfo);
    }
}

void V2TXLivePlayerObserverImpl::onAudioLoading(liteav::V2TXLivePlayer* player, void* extraInfo) {
    if (m_AudioLoading)
    {
        m_AudioLoading(player, extraInfo);
    }
}

void V2TXLivePlayerObserverImpl::onPlayoutVolumeUpdate(liteav::V2TXLivePlayer* player, int32_t volume) {
    if (m_PlayoutVolumeUpdate)
    {
        m_PlayoutVolumeUpdate(player, volume);
    }
}

void V2TXLivePlayerObserverImpl::onStatisticsUpdate(liteav::V2TXLivePlayer* player, liteav::V2TXLivePlayerStatistics statistics) {
    if (m_PlayoutVolumeUpdate)
    {
        m_StatisticsUpdate(player, statistics);
    }
}

void V2TXLivePlayerObserverImpl::onSnapshotComplete(liteav::V2TXLivePlayer* player, const char* image, int length, int width, int height, liteav::V2TXLivePixelFormat format) {
    if (m_SnapshotComplete)
    {
        m_SnapshotComplete(player, image, length, width, height, format);
    }
}

void V2TXLivePlayerObserverImpl::onRenderVideoFrame(liteav::V2TXLivePlayer* player, const liteav::V2TXLiveVideoFrame* videoFrame) {
    if (m_RenderVideoFrame)
    {
        m_RenderVideoFrame(player, videoFrame);
    }
}

void V2TXLivePlayerObserverImpl::onReceiveSeiMessage(liteav::V2TXLivePlayer* player, int payloadType, const uint8_t* data, uint32_t dataSize) {
    if (m_ReceiveSeiMessage)
    {
        m_ReceiveSeiMessage(player, payloadType, data, dataSize);
    }
}