#pragma once
// C++
#include "include/Live2/V2TXLivePlayer.hpp"


typedef void (CALLBACK* PFN_ERROR)(liteav::V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo);
typedef void (CALLBACK* PFN_WARNING)(liteav::V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo);
typedef void (CALLBACK* PFN_VIDEORESOLUTIONCHANGED)(liteav::V2TXLivePlayer* player, int width, int height);
typedef void (CALLBACK* PFN_CONNECTED)(liteav::V2TXLivePlayer* player, void* extraInfo);
typedef void (CALLBACK* PFN_VIDEOPLAYING)(liteav::V2TXLivePlayer* player, bool firstPlay, void* extraInfo);
typedef void (CALLBACK* PFN_AUDIOPLAYING)(liteav::V2TXLivePlayer* player, bool firstPlay, void* extraInfo);
typedef void (CALLBACK* PFN_VIDEOLOADING)(liteav::V2TXLivePlayer* player, void* extraInfo);
typedef void (CALLBACK* PFN_ADUIOLOADING)(liteav::V2TXLivePlayer* player, void* extraInfo);
typedef void (CALLBACK* PFN_PLAYOUTVOLUMEUPDATE)(liteav::V2TXLivePlayer* player, int32_t volume);
typedef void (CALLBACK* PFN_STATISTICSUPDATE)(liteav::V2TXLivePlayer* player, liteav::V2TXLivePlayerStatistics statistics);
typedef void (CALLBACK* PFN_SNAPSHOTCOMPLETE)(liteav::V2TXLivePlayer* player, const char* image, int length, int width, int height, liteav::V2TXLivePixelFormat format);
typedef void (CALLBACK* PFN_RENDERVIDEOFRAME)(liteav::V2TXLivePlayer* player, const liteav::V2TXLiveVideoFrame* videoFrame);
typedef void (CALLBACK* PFN_RECEIVESEIMESSAGE)(liteav::V2TXLivePlayer* player, int payloadType, const uint8_t* data, uint32_t dataSize);

class V2TXLivePlayerObserverImpl
	: public liteav::V2TXLivePlayerObserver
{
public:
	V2TXLivePlayerObserverImpl();
	~V2TXLivePlayerObserverImpl();
	
    void setError(PFN_ERROR pointer);

    void setWarning(PFN_WARNING pointer);

    void setVideoResolutionChanged(PFN_VIDEORESOLUTIONCHANGED pointer);

    void setConnected(PFN_CONNECTED  pointer);

    void setVideoPlaying(PFN_VIDEOPLAYING  pointer);

    void setAudioPlaying(PFN_AUDIOPLAYING  pointer);

    void setVideoLoading(PFN_VIDEOLOADING  pointer);

    void setAudioLoading(PFN_ADUIOLOADING  pointer);

    void setPlayoutVolumeUpdate(PFN_PLAYOUTVOLUMEUPDATE  pointer);

    void setStatisticsUpdate(PFN_STATISTICSUPDATE  pointer);

    void setSnapshotComplete(PFN_SNAPSHOTCOMPLETE  pointer);

    void setRenderVideoFrame(PFN_RENDERVIDEOFRAME  pointer);

    void setReceiveSeiMessage(PFN_RECEIVESEIMESSAGE  pointer);
public:

    void onError(liteav::V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo);

    void onWarning(liteav::V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo);

    void onVideoResolutionChanged(liteav::V2TXLivePlayer* player, int width, int height);

    void onConnected(liteav::V2TXLivePlayer* player, void* extraInfo);

    void onVideoPlaying(liteav::V2TXLivePlayer* player, bool firstPlay, void* extraInfo);

    void onAudioPlaying(liteav::V2TXLivePlayer* player, bool firstPlay, void* extraInfo);

    void onVideoLoading(liteav::V2TXLivePlayer* player, void* extraInfo);

    void onAudioLoading(liteav::V2TXLivePlayer* player, void* extraInfo);

    void onPlayoutVolumeUpdate(liteav::V2TXLivePlayer* player, int32_t volume);

    void onStatisticsUpdate(liteav::V2TXLivePlayer* player, liteav::V2TXLivePlayerStatistics statistics);

    void onSnapshotComplete(liteav::V2TXLivePlayer* player, const char* image, int length, int width, int height, liteav::V2TXLivePixelFormat format);

    void onRenderVideoFrame(liteav::V2TXLivePlayer* player, const liteav::V2TXLiveVideoFrame* videoFrame);

    void onReceiveSeiMessage(liteav::V2TXLivePlayer* player, int payloadType, const uint8_t* data, uint32_t dataSize);
private:
    PFN_ERROR                  m_Error;
    PFN_WARNING                m_Warning;
    PFN_VIDEORESOLUTIONCHANGED m_VideoResolutionChanged;
    PFN_CONNECTED              m_Connected;
    PFN_VIDEOPLAYING           m_VideoPlaying;
    PFN_AUDIOPLAYING           m_AudioPlaying;
    PFN_VIDEOLOADING           m_VideoLoading;
    PFN_ADUIOLOADING           m_AudioLoading;
    PFN_PLAYOUTVOLUMEUPDATE    m_PlayoutVolumeUpdate;
    PFN_STATISTICSUPDATE       m_StatisticsUpdate;
    PFN_SNAPSHOTCOMPLETE       m_SnapshotComplete;
    PFN_RENDERVIDEOFRAME       m_RenderVideoFrame;
    PFN_RECEIVESEIMESSAGE      m_ReceiveSeiMessage;
};
