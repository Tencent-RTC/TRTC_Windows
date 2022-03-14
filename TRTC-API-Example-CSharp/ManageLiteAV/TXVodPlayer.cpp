#include "Utils.h"
#include "cli\Vod\ITXVodPlayer.h"
#include "TXVodPlayerCallback.h"
#include <assert.h>
#include <string>
#include <msclr\marshal_cppstd.h>
#include <msclr\lock.h> 
#include <codecvt>

using namespace System::Runtime::InteropServices;

namespace ManageLiteAV
{
    ITXVodPlayer::ITXVodPlayer(String^ mediaFile, bool repeat)
        : m_vodPlayerEventCallbackImpl(new ITXVodPlayerEventCallbackImpl())
        , m_vodPlayerDataCallbackImpl(new ITXVodPlayerDataCallbackImpl())
        , m_cb_locker_event(gcnew Object())
        , m_cb_locker_data(gcnew Object())
    {
        char* mediaFileName = Utils::StringToUTF8CharPtr(mediaFile);
        m_player = ::createTXVodPlayer(mediaFileName, repeat);
        delete[] mediaFileName;
        mediaFileName = nullptr;
    }

    ITXVodPlayer::~ITXVodPlayer()
    {
        if (m_player == nullptr)
        {
            return;
        }
        ::ITXVodPlayer* p = m_player;
        ::destroyTXVodPlayer(&p);
        m_player = nullptr;

        if (nullptr != m_vodPlayerDataCallbackImpl)
        {
            delete m_vodPlayerDataCallbackImpl;
            m_vodPlayerDataCallbackImpl = nullptr;
        }

        if (nullptr != m_vodPlayerEventCallbackImpl)
        {
            delete m_vodPlayerEventCallbackImpl;
            m_vodPlayerEventCallbackImpl = nullptr;
        }
    }

    ITXVodPlayer^ ITXVodPlayer::createTXVodPlayer(String^ mediaFile, bool repeat)
    {
        ITXVodPlayer^ sInstance = gcnew ITXVodPlayer(mediaFile, repeat);
        return sInstance;
    }

    void ITXVodPlayer::destroyTXVodPlayer(ITXVodPlayer^ player)
    {
        player->~ITXVodPlayer();
    }

    // 
    int ITXVodPlayer::onVodVideoFrame(::TRTCVideoFrame& frame)
    {
        ITXVodPlayerDataCallback ^ callback = nullptr;
        {
            msclr::lock l(m_cb_locker_data);
            callback = m_vodPlayerDataCallback;
        }
        if (callback)
        {
            TRTCVideoFrame^ videoFrame = gcnew TRTCVideoFrame();
            videoFrame->videoFormat = static_cast<TRTCVideoPixelFormat>(frame.videoFormat);
            videoFrame->bufferType = static_cast<TRTCVideoBufferType>(frame.bufferType);
            videoFrame->length = frame.length;
            videoFrame->height = frame.height;
            videoFrame->width = frame.width;
            videoFrame->width = frame.width;
            videoFrame->data = gcnew array<Byte>(frame.length);
            Marshal::Copy(IntPtr(frame.data), videoFrame->data, 0, frame.length);
            videoFrame->rotation = static_cast<TRTCVideoRotation>(frame.rotation);
            videoFrame->textureId = frame.textureId;
            videoFrame->timestamp = frame.timestamp;

            return callback->onVodVideoFrame(videoFrame);
        }

        return 0;
    }

    int ITXVodPlayer::onVodAudioFrame(::TRTCAudioFrame& frame)
    {
        ITXVodPlayerDataCallback^ callback = nullptr;
        {
            msclr::lock l(m_cb_locker_data);
            callback = m_vodPlayerDataCallback;
        }
        if (callback != nullptr)
        {
            TRTCAudioFrame^ audioFrame = gcnew TRTCAudioFrame();
            audioFrame->audioFormat = static_cast<TRTCAudioFrameFormat>(frame.audioFormat);
            audioFrame->length = frame.length;
            audioFrame->data = gcnew array<Byte>(frame.length);
            Marshal::Copy(IntPtr(frame.data), audioFrame->data, 0, frame.length);
            audioFrame->channel = frame.channel;
            audioFrame->sampleRate = frame.sampleRate;
            audioFrame->timestamp = frame.timestamp;

            return callback->onVodAudioFrame(audioFrame);
        }

        return 0;
    }

    void ITXVodPlayer::onVodPlayerStarted(uint64_t msLength)
    {
        ITXVodPlayerEventCallback^ callback = nullptr;
        {
            msclr::lock l(m_cb_locker_event);
            callback = m_vodPlayerEventCallback;
        }
        if (callback != nullptr)
        {
            callback->onVodPlayerStarted(msLength);
        }
    }

    void ITXVodPlayer::onVodPlayerProgress(uint64_t msPos)
    {
        ITXVodPlayerEventCallback^ callback = nullptr;
        {
            msclr::lock l(m_cb_locker_event);
            callback = m_vodPlayerEventCallback;
        }
        if (callback != nullptr)
        {
            callback->onVodPlayerProgress(msPos);
        }
    }

    void ITXVodPlayer::onVodPlayerPaused()
    {
        ITXVodPlayerEventCallback^ callback = nullptr;
        {
            msclr::lock l(m_cb_locker_event);
            callback = m_vodPlayerEventCallback;
        }
        if (callback != nullptr)
        {
            callback->onVodPlayerPaused();
        }
    }

    void ITXVodPlayer::onVodPlayerResumed()
    {
        ITXVodPlayerEventCallback^ callback = nullptr;
        {
            msclr::lock l(m_cb_locker_event);
            callback = m_vodPlayerEventCallback;
        }
        if (callback != nullptr)
        {
            callback->onVodPlayerResumed();
        }
    }

    void ITXVodPlayer::onVodPlayerStoped(int reason)
    {
        ITXVodPlayerEventCallback^ callback = nullptr;
        {
            msclr::lock l(m_cb_locker_event);
            callback = m_vodPlayerEventCallback;
        }
        if (callback != nullptr)
        {
            callback->onVodPlayerStoped(reason);
        }
    }

    void ITXVodPlayer::onVodPlayerError(int error)
    {
        ITXVodPlayerEventCallback^ callback = nullptr;
        {
            msclr::lock l(m_cb_locker_event);
            callback = m_vodPlayerEventCallback;
        }
        if (callback != nullptr)
        {
            callback->onVodPlayerError(error);
        }
    }

    void ITXVodPlayer::setEventCallback(ITXVodPlayerEventCallback^ callback)
    {
        if (m_player)
        {
            {
                msclr::lock l(m_cb_locker_event);
                m_vodPlayerEventCallback = callback;
            }
            if (callback)
            {
                if (m_vodPlayerEventCallbackImpl == nullptr)
                {
                    m_vodPlayerEventCallbackImpl = new ITXVodPlayerEventCallbackImpl();
                }

                if (m_vodPlayerEventCallbackImpl != nullptr)
                {
                    if (m_vodPlayerStartedDelegate == nullptr)
                        m_vodPlayerStartedDelegate = gcnew VodPlayerStartedDelegate(this, &ITXVodPlayer::onVodPlayerStarted);
                    m_vodPlayerEventCallbackImpl->setVodPlayerStarted((PFN_VODPLAYERSTARTED)Marshal::GetFunctionPointerForDelegate(m_vodPlayerStartedDelegate).ToPointer());
                    if (m_vodPlayerProcessDelegate == nullptr)
                        m_vodPlayerProcessDelegate = gcnew VodPlayerProgressDelegate(this, &ITXVodPlayer::onVodPlayerProgress);
                    m_vodPlayerEventCallbackImpl->setVodPlayerProcess((PFN_VODPLAYERPROCESS)Marshal::GetFunctionPointerForDelegate(m_vodPlayerProcessDelegate).ToPointer());
                    if (m_vodPlayerPausedDelegate == nullptr)
                        m_vodPlayerPausedDelegate = gcnew VodPlayerPausedDelegate(this, &ITXVodPlayer::onVodPlayerPaused);
                    m_vodPlayerEventCallbackImpl->setVodPlayerPaused((PFN_VODPLAYERPAUSED)Marshal::GetFunctionPointerForDelegate(m_vodPlayerPausedDelegate).ToPointer());
                    if (m_vodPlayerResumedDelegate == nullptr)
                        m_vodPlayerResumedDelegate = gcnew VodPlayerResumedDelegate(this, &ITXVodPlayer::onVodPlayerResumed);
                    m_vodPlayerEventCallbackImpl->setVodPlayerResumed((PFN_VODPLAYERRESUMED)Marshal::GetFunctionPointerForDelegate(m_vodPlayerResumedDelegate).ToPointer());
                    if (m_vodPlayerStopedDelegate == nullptr)
                        m_vodPlayerStopedDelegate = gcnew VodPlayerStopedDelegate(this, &ITXVodPlayer::onVodPlayerStoped);
                    m_vodPlayerEventCallbackImpl->setVodPlayerStoped((PFN_VODPLAYERSTOPED)Marshal::GetFunctionPointerForDelegate(m_vodPlayerStopedDelegate).ToPointer());
                    if (m_vodPlayerErrorDelegate == nullptr)
                        m_vodPlayerErrorDelegate = gcnew VodPlayerErrorDelegate(this, &ITXVodPlayer::onVodPlayerError);
                    m_vodPlayerEventCallbackImpl->setVodPlayerError((PFN_VODPLAYERERROR)Marshal::GetFunctionPointerForDelegate(m_vodPlayerErrorDelegate).ToPointer());
                }
                m_player->setEventCallback(m_vodPlayerEventCallbackImpl);
            }
            else
            {
                m_player->setEventCallback(nullptr);
            }
        }
    }

    void ITXVodPlayer::setDataCallback(ITXVodPlayerDataCallback^ callback)
    {
        if (m_player)
        {
            {
                msclr::lock l(m_cb_locker_data);
                m_vodPlayerDataCallback = callback;
            }
            if (callback)
            {
                if (m_vodPlayerDataCallbackImpl == nullptr)
                {
                    m_vodPlayerDataCallbackImpl = new ITXVodPlayerDataCallbackImpl();
                }
                if (m_vodPlayerDataCallbackImpl != nullptr)
                {
                    if (m_vodVideoFrameDeleate == nullptr)
                        m_vodVideoFrameDeleate = gcnew VodVideoFrameDelegate(this, &ITXVodPlayer::onVodVideoFrame);
                    m_vodPlayerDataCallbackImpl->setVodVideoFrame((PFN_VODVIDEOFRAME)Marshal::GetFunctionPointerForDelegate(m_vodVideoFrameDeleate).ToPointer());
                    if (m_vodAudioFrameDeleate == nullptr)
                        m_vodAudioFrameDeleate = gcnew VodAudioFrameDelegate(this, &ITXVodPlayer::onVodAudioFrame);
                    m_vodPlayerDataCallbackImpl->setVodAudioFrame((PFN_VODAUDIOFRAME)Marshal::GetFunctionPointerForDelegate(m_vodAudioFrameDeleate).ToPointer());
                }

                m_player->setDataCallback(m_vodPlayerDataCallbackImpl);
            }
            else
            {
                m_player->setDataCallback(nullptr);
            }
        }

    }


    void ITXVodPlayer::publishVideo()
    {
        if (m_player != nullptr)
        {
            m_player->publishVideo();
        }
    }

    void ITXVodPlayer::publishAudio()
    {
        if (m_player != nullptr)
        {
            m_player->publishAudio();
        }
    }

    void ITXVodPlayer::unpublishVideo()
    {
        if (m_player != nullptr)
        {
            m_player->unpublishVideo();
        }
    }

    void ITXVodPlayer::unpublishAudio()
    {
        if (m_player != nullptr)
        {
            m_player->unpublishAudio();
        }
    }

    void ITXVodPlayer::start()
    {
        if (m_player != nullptr)
        {
            m_player->start();
        }
    }

    void ITXVodPlayer::pause()
    {
        if (m_player != nullptr)
        {
            m_player->pause();
        }
    }

    void ITXVodPlayer::resume()
    {
        if (m_player != nullptr)
        {
            m_player->resume();
        }
    }

    void ITXVodPlayer::stop()
    {
        if (m_player != nullptr)
        {
            m_player->stop();
        }
    }

    void ITXVodPlayer::seek(UInt64 msPos)
    {
        if (m_player != nullptr)
        {
            m_player->seek(msPos);
        }
    }

    void ITXVodPlayer::setRate(float rate)
    {
        if (m_player != nullptr)
        {
            m_player->setRate(rate);
        }
    }

    void ITXVodPlayer::setView(IntPtr hwnd)
    {
        if (m_player != nullptr)
        {
            m_player->setView(static_cast<HWND>(hwnd.ToPointer()));
        }
    }

    Int32 ITXVodPlayer::getDuration()
    {
        if (m_player != nullptr)
        {
            return m_player->getDuration();
        }

        return 0;
    }

    Int32 ITXVodPlayer::getWidth()
    {
        if (m_player != nullptr)
        {
            return m_player->getWidth();
        }

        return 0;
    }

    Int32 ITXVodPlayer::getHeight()
    {
        if (m_player != nullptr)
        {
            return m_player->getHeight();
        }

        return 0;
    }

    void ITXVodPlayer::setVolume(Int32 volume)
    {
        if (m_player != nullptr)
        {
            m_player->setVolume(volume);
        }
    }

    void ITXVodPlayer::mute(bool mute)
    {
        if (m_player != nullptr)
        {
            m_player->mute(mute);
        }
    }

    void ITXVodPlayer::setRenderRotation(TRTCVideoRotation rotation)
    {
        if (m_player != nullptr)
        {
            m_player->setRenderRotation(static_cast<::TRTCVideoRotation>(rotation));
        }
    }

    void ITXVodPlayer::setFillMode(TRTCVideoFillMode mode)
    {
        if (m_player != nullptr)
        {
            m_player->setFillMode(static_cast<::TRTCVideoFillMode>(mode));
        }
    }

    void ITXVodPlayer::setMirror(bool mirror)
    {
        if (m_player != nullptr)
        {
            m_player->setMirror(mirror);
        }
    }

    void ITXVodPlayer::attachTRTC()
    {
        if (nullptr != m_player) {
            m_player->attachTRTC(::getTRTCShareInstance());
        }
    }

    void ITXVodPlayer::detachTRTC()
    {
        if (nullptr != m_player) {
            m_player->detachTRTC();
        }
    }
}