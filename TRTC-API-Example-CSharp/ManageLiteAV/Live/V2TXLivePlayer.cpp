#include "cli/Live/V2TXLivePlayer.h"
#include "V2TXLivePlayerObserverImpl.h"
#include "Utils.h"

using namespace System::Runtime::InteropServices;

namespace ManageLiteAV {
    V2TXLivePlayer::V2TXLivePlayer()
        : m_player(nullptr)
        , m_AudioLoadingDelegate(nullptr)
        , m_AudioPlayingDelegate(nullptr)
        , m_ConnectedDelegate(nullptr)
        , m_ErrorDelegate(nullptr)
        , m_PlayoutVolumeUpdateDelegate(nullptr)
        , m_ReceiveSeiMessageDelegate(nullptr)
        , m_RenderVideoFrameDelegate(nullptr)
        , m_SnapshotCompleteDelegate(nullptr)
        , m_StatisticsUpdateDelegate(nullptr)
        , m_V2TXLivePlayerObserver(nullptr)
        , m_V2TXLivePlayerObserverImpl(nullptr)
        , m_VideoLoadingDelegate(nullptr)
        , m_VideoPlayingDelegate(nullptr)
        , m_VideoResolutionChangedDelegate(nullptr)
        , m_WarningDelegate(nullptr)
        , m_Videoframe(nullptr) {
        m_player = ::createV2TXLivePlayer();
    }

    V2TXLivePlayer::~V2TXLivePlayer() {
        ::releaseV2TXLivePlayer(m_player);
        m_player = nullptr;
        if (m_V2TXLivePlayerObserverImpl)
        {
            delete m_V2TXLivePlayerObserverImpl;
            m_V2TXLivePlayerObserverImpl = nullptr;
        }
    }

    V2TXLivePlayer^ V2TXLivePlayer::createV2TXLivePlayer() {
        V2TXLivePlayer^ sInstance = gcnew V2TXLivePlayer();
        return sInstance;
    }

    void V2TXLivePlayer::releaseV2TXLivePlayer(V2TXLivePlayer^ player) {
        player->~V2TXLivePlayer();
    }

    void V2TXLivePlayer::setObserver(V2TXLivePlayerObserver^ observer) {
        m_V2TXLivePlayerObserver = observer;
        if (m_V2TXLivePlayerObserver && m_player)
        {
            if (m_V2TXLivePlayerObserverImpl == nullptr)
            {
                m_V2TXLivePlayerObserverImpl = new V2TXLivePlayerObserverImpl();
            }

            if (m_V2TXLivePlayerObserverImpl)
            {
                if (!m_ErrorDelegate) {
                    m_ErrorDelegate = gcnew ErrorDelegate(this, &V2TXLivePlayer::onError);
                }
                m_V2TXLivePlayerObserverImpl->setError((PFN_ERROR)Marshal::GetFunctionPointerForDelegate(m_ErrorDelegate).ToPointer());

                if (!m_WarningDelegate) {
                    m_WarningDelegate = gcnew WarningDelegate(this, &V2TXLivePlayer::onWarning);
                }
                m_V2TXLivePlayerObserverImpl->setWarning((PFN_WARNING)Marshal::GetFunctionPointerForDelegate(m_WarningDelegate).ToPointer());

                if (!m_VideoResolutionChangedDelegate) {
                    m_VideoResolutionChangedDelegate = gcnew VideoResolutionChangedDelegate(this, &V2TXLivePlayer::onVideoResolutionChanged);
                }
                m_V2TXLivePlayerObserverImpl->setVideoResolutionChanged((PFN_VIDEORESOLUTIONCHANGED)Marshal::GetFunctionPointerForDelegate(m_VideoResolutionChangedDelegate).ToPointer());

                if (!m_ConnectedDelegate) {
                    m_ConnectedDelegate = gcnew ConnectedDelegate(this, &V2TXLivePlayer::onConnected);
                }
                m_V2TXLivePlayerObserverImpl->setConnected((PFN_CONNECTED)Marshal::GetFunctionPointerForDelegate(m_ConnectedDelegate).ToPointer());

                if (!m_VideoPlayingDelegate) {
                    m_VideoPlayingDelegate = gcnew VideoPlayingDelegate(this, &V2TXLivePlayer::onVideoPlaying);
                }
                m_V2TXLivePlayerObserverImpl->setVideoPlaying((PFN_VIDEOPLAYING)Marshal::GetFunctionPointerForDelegate(m_VideoPlayingDelegate).ToPointer());

                if (!m_AudioPlayingDelegate) {
                    m_AudioPlayingDelegate = gcnew AudioPlayingDelegate(this, &V2TXLivePlayer::onAudioPlaying);
                }
                m_V2TXLivePlayerObserverImpl->setAudioPlaying((PFN_AUDIOPLAYING)Marshal::GetFunctionPointerForDelegate(m_AudioPlayingDelegate).ToPointer());

                if (!m_VideoLoadingDelegate) {
                    m_VideoLoadingDelegate = gcnew VideoLoadingDelegate(this, &V2TXLivePlayer::onVideoLoading);
                }
                m_V2TXLivePlayerObserverImpl->setVideoLoading((PFN_VIDEOLOADING)Marshal::GetFunctionPointerForDelegate(m_VideoLoadingDelegate).ToPointer());

                if (!m_AudioLoadingDelegate) {
                    m_AudioLoadingDelegate = gcnew AudioLoadingDelegate(this, &V2TXLivePlayer::onAudioLoading);
                }
                m_V2TXLivePlayerObserverImpl->setAudioLoading((PFN_ADUIOLOADING)Marshal::GetFunctionPointerForDelegate(m_AudioLoadingDelegate).ToPointer());

                if (!m_PlayoutVolumeUpdateDelegate) {
                    m_PlayoutVolumeUpdateDelegate = gcnew PlayoutVolumeUpdateDelegate(this, &V2TXLivePlayer::onPlayoutVolumeUpdate);
                }
                m_V2TXLivePlayerObserverImpl->setPlayoutVolumeUpdate((PFN_PLAYOUTVOLUMEUPDATE)Marshal::GetFunctionPointerForDelegate(m_PlayoutVolumeUpdateDelegate).ToPointer());

                if (!m_StatisticsUpdateDelegate) {
                    m_StatisticsUpdateDelegate = gcnew StatisticsUpdateDelegate(this, &V2TXLivePlayer::onStatisticsUpdate);
                }
                m_V2TXLivePlayerObserverImpl->setStatisticsUpdate((PFN_STATISTICSUPDATE)Marshal::GetFunctionPointerForDelegate(m_StatisticsUpdateDelegate).ToPointer());

                if (!m_SnapshotCompleteDelegate) {
                    m_SnapshotCompleteDelegate = gcnew SnapshotCompleteDelegate(this, &V2TXLivePlayer::onSnapshotComplete);
                }
                m_V2TXLivePlayerObserverImpl->setSnapshotComplete((PFN_SNAPSHOTCOMPLETE)Marshal::GetFunctionPointerForDelegate(m_SnapshotCompleteDelegate).ToPointer());

                if (!m_RenderVideoFrameDelegate) {
                    m_RenderVideoFrameDelegate = gcnew RenderVideoFrameDelegate(this, &V2TXLivePlayer::onRenderVideoFrame);
                }
                m_V2TXLivePlayerObserverImpl->setRenderVideoFrame((PFN_RENDERVIDEOFRAME)Marshal::GetFunctionPointerForDelegate(m_RenderVideoFrameDelegate).ToPointer());

                if (!m_ReceiveSeiMessageDelegate) {
                    m_ReceiveSeiMessageDelegate = gcnew ReceiveSeiMessageDelegate(this, &V2TXLivePlayer::onReceiveSeiMessage);
                }
                m_V2TXLivePlayerObserverImpl->setReceiveSeiMessage((PFN_RECEIVESEIMESSAGE)Marshal::GetFunctionPointerForDelegate(m_ReceiveSeiMessageDelegate).ToPointer());

                m_player->setObserver(m_V2TXLivePlayerObserverImpl);
            }
        }
        else
        {
            m_player->setObserver(nullptr);
        }
    }

    Int32 V2TXLivePlayer::setRenderView(IntPtr view) {
        if (m_player)
        {
            return m_player->setRenderView(view.ToPointer());
        }
        return -1;
    }

    Int32 V2TXLivePlayer::setRenderRotation(V2TXLiveRotation rotation) {
        if (m_player)
        {
            liteav::V2TXLiveRotation param = static_cast<liteav::V2TXLiveRotation>(rotation);
            return m_player->setRenderRotation(param);
        }
        return -1;
    }

    Int32 V2TXLivePlayer::setRenderFillMode(V2TXLiveFillMode mode) {
        if (m_player)
        {
            liteav::V2TXLiveFillMode param = static_cast<liteav::V2TXLiveFillMode>(mode);
            return m_player->setRenderFillMode(param);
        }
        return -1;
    }

    Int32 V2TXLivePlayer::startPlay(String^ url) {
        if (m_player)
        {
            char* str = Utils::StringToUTF8CharPtr(url);
            int32_t ret = m_player->startPlay(str);
            delete[] str;
            str = nullptr;
            return ret;
        }
        return -1;
    }

    Int32 V2TXLivePlayer::stopPlay() {
        if (m_player)
        {
            return m_player->stopPlay();
        }
        return -1;
    }

    Int32 V2TXLivePlayer::isPlaying() {
        if (m_player)
        {
            return m_player->isPlaying();
        }
        return -1;
    }

    Int32 V2TXLivePlayer::pauseAudio() {
        if (m_player)
        {
            return m_player->pauseAudio();
        }
        return -1;
    }

    Int32 V2TXLivePlayer::resumeAudio() {
        if (m_player)
        {
            return m_player->resumeAudio();
        }
        return -1;
    }

    Int32 V2TXLivePlayer::pauseVideo() {
        if (m_player)
        {
            return m_player->pauseVideo();
        }
        return -1;
    }

    Int32 V2TXLivePlayer::resumeVideo() {
        if (m_player)
        {
            return m_player->resumeVideo();
        }
        return -1;
    }

    Int32 V2TXLivePlayer::setPlayoutVolume(Int32 volume) {
        if (m_player)
        {
            return m_player->setPlayoutVolume(volume);
        }
        return -1;
    }

    Int32 V2TXLivePlayer::setCacheParams(float minTime, float maxTime) {
        if (m_player)
        {
            return m_player->setCacheParams(minTime, maxTime);
        }
        return -1;
    }

    Int32 V2TXLivePlayer::switchStream(String^ newUrl) {
        if (m_player)
        {
            char* str = Utils::StringToUTF8CharPtr(newUrl);
            int32_t ret = m_player->switchStream(str);
            delete[] str;
            str = nullptr;
            return ret;
        }
        return -1;
    }

    Int32 V2TXLivePlayer::enableVolumeEvaluation(Int32 intervalMs) {
        if (m_player)
        {
            return m_player->enableVolumeEvaluation(intervalMs);
        }
        return -1;
    }

    Int32 V2TXLivePlayer::snapshot() {
        if (m_player)
        {
            return m_player->snapshot();
        }
        return -1;
    }

    Int32 V2TXLivePlayer::enableObserveVideoFrame(bool enable, V2TXLivePixelFormat pixelFormat, V2TXLiveBufferType bufferType) {
        if (m_player)
        {
            liteav::V2TXLivePixelFormat format = static_cast<liteav::V2TXLivePixelFormat>(pixelFormat);
            liteav::V2TXLiveBufferType type = static_cast<liteav::V2TXLiveBufferType>(bufferType);
            return m_player->enableObserveVideoFrame(enable, format, type);
        }
        return -1;
    }

    Int32 V2TXLivePlayer::enableReceiveSeiMessage(bool enable, int payloadType) {
        if (m_player)
        {
            return m_player->enableReceiveSeiMessage(enable, payloadType);
        }
        return -1;
    }

    void V2TXLivePlayer::showDebugView(bool isShow) {
        if (m_player)
        {
            m_player->showDebugView(isShow);
        }
    }

    Int32 V2TXLivePlayer::setProperty(String^ key, String^ value) {
        if (m_player)
        {
            char* str_key = Utils::StringToUTF8CharPtr(key);
            char* str_value = Utils::StringToUTF8CharPtr(value);
            int32_t ret = m_player->setProperty(str_key, str_value);
            delete[] str_key;
            delete[] str_value;
            str_key = nullptr;
            str_value = nullptr;
            return ret;
        }
        return -1;
    }


    void V2TXLivePlayer::onError(liteav::V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo) {
        if (m_V2TXLivePlayerObserver)
        {
            String^ message = Utils::CharPtrToString(msg);
            m_V2TXLivePlayerObserver->onError(this, code, message, IntPtr(extraInfo));
        }
    }

    void V2TXLivePlayer::onWarning(liteav::V2TXLivePlayer* player, int32_t code, const char* msg, void* extraInfo) {
        if (m_V2TXLivePlayerObserver)
        {
            String^ message = Utils::CharPtrToString(msg);
            m_V2TXLivePlayerObserver->onWarning(this, code, message, IntPtr(extraInfo));
        }
    }

    void V2TXLivePlayer::onVideoResolutionChanged(liteav::V2TXLivePlayer* player, int width, int height) {
        if (m_V2TXLivePlayerObserver)
        {
            m_V2TXLivePlayerObserver->onVideoResolutionChanged(this, width, height);
        }
    }

    void V2TXLivePlayer::onConnected(liteav::V2TXLivePlayer* player, void* extraInfo) {
        if (m_V2TXLivePlayerObserver)
        {
            m_V2TXLivePlayerObserver->onConnected(this, IntPtr(extraInfo));
        }
    }

    void V2TXLivePlayer::onVideoPlaying(liteav::V2TXLivePlayer* player, bool firstPlay, void* extraInfo) {
        if (m_V2TXLivePlayerObserver)
        {
            m_V2TXLivePlayerObserver->onVideoPlaying(this, firstPlay, IntPtr(extraInfo));
        }
    }

    void V2TXLivePlayer::onAudioPlaying(liteav::V2TXLivePlayer* player, bool firstPlay, void* extraInfo) {
        if (m_V2TXLivePlayerObserver)
        {
            m_V2TXLivePlayerObserver->onAudioPlaying(this, firstPlay, IntPtr(extraInfo));
        }
    }

    void V2TXLivePlayer::onVideoLoading(liteav::V2TXLivePlayer* player, void* extraInfo) {
        if (m_V2TXLivePlayerObserver)
        {
            m_V2TXLivePlayerObserver->onVideoLoading(this, IntPtr(extraInfo));
        }
    }

    void V2TXLivePlayer::onAudioLoading(liteav::V2TXLivePlayer* player, void* extraInfo) {
        if (m_V2TXLivePlayerObserver)
        {
            m_V2TXLivePlayerObserver->onAudioLoading(this, IntPtr(extraInfo));
        }
    }

    void V2TXLivePlayer::onPlayoutVolumeUpdate(liteav::V2TXLivePlayer* player, int32_t volume) {
        if (m_V2TXLivePlayerObserver)
        {
            m_V2TXLivePlayerObserver->onPlayoutVolumeUpdate(this, volume);
        }
    }

    void V2TXLivePlayer::onStatisticsUpdate(liteav::V2TXLivePlayer* player, liteav::V2TXLivePlayerStatistics statistics) {
        if (m_V2TXLivePlayerObserver)
        {
            V2TXLivePlayerStatistics^ param = gcnew V2TXLivePlayerStatistics;
            param->appCpu = statistics.appCpu;
            param->systemCpu = statistics.systemCpu;
            param->width = statistics.width;
            param->height = statistics.height;
            param->fps = statistics.fps;
            param->videoBitrate = statistics.videoBitrate;
            param->audioBitrate = statistics.audioBitrate;
            m_V2TXLivePlayerObserver->onStatisticsUpdate(this, param);
        }
    }

    void V2TXLivePlayer::onSnapshotComplete(liteav::V2TXLivePlayer* player, const char* image, int length, int width, int height, liteav::V2TXLivePixelFormat format) {
        if (m_V2TXLivePlayerObserver)
        {
            array<Byte>^ buffer = gcnew array<Byte>(length);
            Marshal::Copy(IntPtr((void*)image), buffer, 0, length);
            ManageLiteAV::V2TXLivePixelFormat pixel_format = static_cast<ManageLiteAV::V2TXLivePixelFormat>(format);
            m_V2TXLivePlayerObserver->onSnapshotComplete(this, buffer, length, width, height, pixel_format);
        }
    }

    void V2TXLivePlayer::onRenderVideoFrame(liteav::V2TXLivePlayer* player, const liteav::V2TXLiveVideoFrame* videoFrame) {
        if (m_V2TXLivePlayerObserver)
        {
            if (!m_Videoframe)
            {
                m_Videoframe = gcnew V2TXLiveVideoFrame;
            }
            if (!m_Videoframe->data || m_Videoframe->length != videoFrame->length)
            {
                m_Videoframe->data = gcnew array<Byte>(videoFrame->length);
            }

            m_Videoframe->pixelFormat = static_cast<ManageLiteAV::V2TXLivePixelFormat>(videoFrame->pixelFormat);
            m_Videoframe->bufferType = static_cast<ManageLiteAV::V2TXLiveBufferType>(videoFrame->bufferType);
            m_Videoframe->rotation = static_cast<ManageLiteAV::V2TXLiveRotation>(videoFrame->rotation);

            Marshal::Copy(IntPtr((void*)videoFrame->data), m_Videoframe->data, 0, videoFrame->length);
            m_Videoframe->length = videoFrame->length;
            m_Videoframe->width = videoFrame->width;
            m_Videoframe->height = videoFrame->height;
            m_V2TXLivePlayerObserver->onRenderVideoFrame(this, m_Videoframe);
        }
    }

    void V2TXLivePlayer::onReceiveSeiMessage(liteav::V2TXLivePlayer* player, int payloadType, const uint8_t* data, uint32_t dataSize) {
        if (m_V2TXLivePlayerObserver)
        {
            array<Byte>^ buffer = gcnew array<Byte>(dataSize);
            Marshal::Copy(IntPtr((void*)data), buffer, 0, dataSize);
            m_V2TXLivePlayerObserver->onReceiveSeiMessage(this, payloadType, buffer, dataSize);
        }
    }
}