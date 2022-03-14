#include "Utils.h"

// C++ Header
#include "include/TRTC/ITRTCCloud.h"

// C++/CLI Header
#include "cli/ITRTCAudioEffectManager.h"

#include <assert.h>
#include <string>
#include <msclr\marshal_cppstd.h>
#include <msclr\lock.h> 
#include <codecvt>


using namespace System::Runtime::InteropServices;
using namespace System::Text;
typedef void (CALLBACK* PFN_START)(int id, int errCode);
typedef void (CALLBACK* PFN_PLAYPROGRESS)(int id, long curPtsMS, long durationMS);
typedef void (CALLBACK* PFN_COMPLETE)(int id, int errCode);

class TXMusicPlayObserverImpl : public ::ITXMusicPlayObserver
{
public:
    virtual ~TXMusicPlayObserverImpl() {}

    void SetStart(PFN_START pointer)
    {
        start_ = pointer;
    }
    void SetPlayProgrss(PFN_PLAYPROGRESS pointer)
    {
        play_progress_ = pointer;
    }
    void SetComplete(PFN_COMPLETE pointer)
    {
        complete_ = pointer;
    }
    virtual void onStart(int id, int errCode)
    {
        if (nullptr != start_)
        {
            start_(id, errCode);
        }
    }
    virtual void onPlayProgress(int id, long curPtsMS, long durationMS)
    {
        if (nullptr != play_progress_)
        {
            play_progress_(id, curPtsMS, durationMS);
        }
    }
    virtual void onComplete(int id, int errCode)
    {
        if (nullptr != complete_)
        {
            complete_(id, errCode);
        }
    }
private:
    PFN_START        start_;
    PFN_PLAYPROGRESS play_progress_;
    PFN_COMPLETE     complete_;
};

namespace ManageLiteAV
{

    void ITXAudioEffectManager::onStart(int id, int errCode)
    {
        music_play_observer_callback_->onStart(id, errCode);
    }
    void ITXAudioEffectManager::onPlayProgress(int id, long curPtsMS, long durationMS)
    {
        music_play_observer_callback_->onPlayProgress(id, curPtsMS, durationMS);
    }
    void ITXAudioEffectManager::onComplete(int id, int errCode)
    {
        music_play_observer_callback_->onComplete(id, errCode);
    }
    ITXAudioEffectManager::ITXAudioEffectManager()
        : audio_effect_manager_(::getTRTCShareInstance()->getAudioEffectManager())
        , music_play_observer_impl_()
        , music_play_observer_callback_()
    {
        assert(nullptr != audio_effect_manager_);
        music_play_observer_impl_ = new TXMusicPlayObserverImpl;
    }

    ITXAudioEffectManager::~ITXAudioEffectManager()
    {
        this->!ITXAudioEffectManager();
        if (music_play_observer_callback_ != nullptr)
        {
            delete music_play_observer_callback_;
            music_play_observer_callback_ = nullptr;
        }

    }
    ITXAudioEffectManager::!ITXAudioEffectManager()
    {
        audio_effect_manager_ = nullptr;
        if (nullptr != music_play_observer_impl_)
        {
            delete music_play_observer_impl_;
            music_play_observer_impl_ = nullptr;
        }
    }
    void ITXAudioEffectManager::setVoiceReverbType(TXVoiceReverbType type)
    {
        if (nullptr != audio_effect_manager_)
            audio_effect_manager_->setVoiceReverbType(static_cast<liteav::TXVoiceReverbType>(type));
    }

    void ITXAudioEffectManager::setVoiceCaptureVolume(int volume)
    {
        if (nullptr != audio_effect_manager_)
            audio_effect_manager_->setVoiceCaptureVolume(volume);
    }

    void ITXAudioEffectManager::setMusicObserver(int id, ITXMusicPlayObserver ^ observer)
    {
        if (nullptr != audio_effect_manager_)
        {
            if (observer != nullptr)
            {
                music_play_observer_callback_ = observer;
                

                if (startDelegate_ == nullptr)
                    startDelegate_ = gcnew StartDelegate(this, &ITXAudioEffectManager::onStart);
                music_play_observer_impl_->SetStart((PFN_START)Marshal::GetFunctionPointerForDelegate(startDelegate_).ToPointer());

                if(playProgressDelegate_ == nullptr)
                    playProgressDelegate_ = gcnew PlayProgressDelegate(this, &ITXAudioEffectManager::onPlayProgress);
                music_play_observer_impl_->SetPlayProgrss((PFN_PLAYPROGRESS)Marshal::GetFunctionPointerForDelegate(playProgressDelegate_).ToPointer());

                if(completeDelegate_ == nullptr)
                    completeDelegate_ = gcnew CompleteDelegate(this, &ITXAudioEffectManager::onComplete);
                music_play_observer_impl_->SetComplete((PFN_COMPLETE)Marshal::GetFunctionPointerForDelegate(completeDelegate_).ToPointer());

                audio_effect_manager_->setMusicObserver(id,music_play_observer_impl_);
            }
            else
            {
                music_play_observer_callback_ = nullptr;
                startDelegate_ = nullptr;
                playProgressDelegate_ = nullptr;
                completeDelegate_ = nullptr;

                music_play_observer_impl_->SetStart(nullptr);
                music_play_observer_impl_->SetPlayProgrss(nullptr);
                music_play_observer_impl_->SetComplete(nullptr);

                audio_effect_manager_->setMusicObserver(id,nullptr);
            }
        }
    }

    void ITXAudioEffectManager::startPlayMusic(AudioMusicParam ^ musicParam)
    {
        if (nullptr != audio_effect_manager_)
        {
            char * path = Utils::StringToUTF8CharPtr(musicParam->path);
            liteav::AudioMusicParam audio_music_temp(musicParam->id, path);

            audio_music_temp.isShortFile = musicParam->isShortFile;
            audio_music_temp.loopCount = musicParam->loopCount;
            audio_music_temp.publish = musicParam->publish;
            audio_music_temp.startTimeMS = musicParam->startTimeMS;
            audio_music_temp.endTimeMS = musicParam->endTimeMS;
            
            audio_effect_manager_->startPlayMusic(audio_music_temp);

            delete path;
            path = nullptr;

        }
       
    }

    void ITXAudioEffectManager::stopPlayMusic(int id)
    {
        if (nullptr != audio_effect_manager_)
        {
            audio_effect_manager_->stopPlayMusic(id);
        }
    }

    void ITXAudioEffectManager::pausePlayMusic(int id)
    {
        if (nullptr != audio_effect_manager_)
        {
            audio_effect_manager_->pausePlayMusic(id);
        }
    }

    void ITXAudioEffectManager::resumePlayMusic(int id)
    {
        if (nullptr != audio_effect_manager_)
        {
            audio_effect_manager_->resumePlayMusic(id);
        }
    }

    void ITXAudioEffectManager::setMusicPublishVolume(int id, int volume)
    {
        if (nullptr != audio_effect_manager_)
        {
            audio_effect_manager_->setMusicPublishVolume(id,volume);
        }
    }

    void ITXAudioEffectManager::setMusicPlayoutVolume(int id, int volume)
    {
        if (nullptr != audio_effect_manager_)
        {
            audio_effect_manager_->setMusicPlayoutVolume(id, volume);
        }
    }

    void ITXAudioEffectManager::setAllMusicVolume(int volume)
    {
        if (nullptr != audio_effect_manager_)
        {
            audio_effect_manager_->setAllMusicVolume(volume);
        }
    }

    void ITXAudioEffectManager::setMusicPitch(int id, float pitch)
    {
        if (nullptr != audio_effect_manager_)
        {
            audio_effect_manager_->setMusicPitch(id,pitch);
        }
    }

    void ITXAudioEffectManager::setMusicSpeedRate(int id, float speedRate)
    {
        if (nullptr != audio_effect_manager_)
        {
            audio_effect_manager_->setMusicSpeedRate(id, speedRate);
        }
    }

    long ITXAudioEffectManager::getMusicCurrentPosInMS(int id)
    {
        if (nullptr != audio_effect_manager_)
        {
            return audio_effect_manager_->getMusicCurrentPosInMS(id);
        }
        return 0;
    }

    void ITXAudioEffectManager::seekMusicToPosInTime(int id, int pts)
    {
        if (nullptr != audio_effect_manager_)
        {
            audio_effect_manager_->seekMusicToPosInTime(id,pts);
        }
    }

    long ITXAudioEffectManager::getMusicDurationInMS(String ^ path)
    {
        if (nullptr != audio_effect_manager_)
        {
            char * path_temp = Utils::StringToUTF8CharPtr(path);

            long res = audio_effect_manager_->getMusicDurationInMS(path_temp);

            delete path_temp;
            path_temp = nullptr;

            return res;
        }

        return 0;
    }
}

