// This is the main DLL file.
#include "Utils.h"
// C++/CLI Header
#include "cli/TXLiteAVCode.h"
#include "cli/TRTC/ITRTCCloud.h"
#include "cli/ITRTCDeviceManager.h"
#include "TRTCCloudCallback.h"

#include <assert.h>
#include <string>
#include <msclr\marshal_cppstd.h>
#include <msclr\lock.h> 
#include <codecvt>

using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace ManageLiteAV
{
    ref class TRTCDeviceCollectionImpl : public ITRTCDeviceCollection
    {
    public:
        ~TRTCDeviceCollectionImpl() {}

        void setDeviceCollection(liteav::ITRTCDeviceCollection* pointer) {
            device_collection_ = pointer;
        }

        virtual UInt32 getCount()
        {
            if (nullptr != device_collection_)
                return device_collection_->getCount();
            else
                return 0;
        }
        virtual String^ getDeviceName(UInt32 index)
        {
            if (nullptr != device_collection_)
                return Utils::CharPtrToString(device_collection_->getDeviceName(index));
            else
                return nullptr;
        }
        virtual String^ getDevicePID(UInt32 index)
        {
            if (nullptr != device_collection_)
                return Utils::CharPtrToString(device_collection_->getDevicePID(index));
            else
                return nullptr;
        }
        virtual void release()
        {
            if (nullptr != device_collection_)
                device_collection_->release();
        }
    private:
        liteav::ITRTCDeviceCollection* device_collection_;
    };

    ref class TRTCDeviceInfoImpl : ITRTCDeviceInfo
    {
    public:
        ~TRTCDeviceInfoImpl() {}

        void setDeviceInfo(liteav::ITRTCDeviceInfo* pointer)
        {
            device_info_ = pointer;
        }

        virtual String^ getDeviceName()
        {
            if (nullptr != device_info_)
                return Utils::CharPtrToString(device_info_->getDeviceName());
            else
                return nullptr;
        }
        virtual String^ getDevicePID()
        {
            if (nullptr != device_info_)
                return Utils::CharPtrToString(device_info_->getDevicePID());
            else
                return nullptr;
        }
        virtual void release()
        {
            if (nullptr != device_info_)
                device_info_->release();
        }
    private:
        liteav::ITRTCDeviceInfo* device_info_;
    };

    ref class TRTCScreenCaptureSourceListImpl : ITRTCScreenCaptureSourceList
    {
    public:
        ~TRTCScreenCaptureSourceListImpl() {}

        void setScreenCaptureSourceList(liteav::ITRTCScreenCaptureSourceList* pointer)
        {
            m_screenCaptureSourceList = pointer;
        }

        virtual UInt32 getCount()
        {
            if (nullptr != m_screenCaptureSourceList)
                return m_screenCaptureSourceList->getCount();
            else
                return 0;
        }

        virtual TRTCScreenCaptureSourceInfo^ getSourceInfo(UInt32 index)
        {
            if (nullptr != m_screenCaptureSourceList)
            {
                liteav::TRTCScreenCaptureSourceInfo info = m_screenCaptureSourceList->getSourceInfo(index);
                TRTCScreenCaptureSourceInfo^ temp = gcnew TRTCScreenCaptureSourceInfo();
                temp->type = static_cast<TRTCScreenCaptureSourceType>(info.type);
                temp->sourceId = IntPtr(info.sourceId);
                temp->sourceName = Utils::CharPtrToString(info.sourceName);
                temp->isMainScreen = info.isMainScreen;
                temp->isMinmizeWindow = info.isMinimizeWindow;
                TRTCImageBuffer^ thumbTemp = gcnew TRTCImageBuffer();
                array<Byte>^ thumbArray;
                if (info.thumbBGRA.length <= 0)
                    thumbArray = gcnew array<Byte>(0);
                thumbArray = gcnew array<Byte>(info.thumbBGRA.length);
                for (unsigned int i = 0; i < info.thumbBGRA.length; i++)
                {
                    thumbArray[i] = info.thumbBGRA.buffer[i];
                }
                thumbTemp->buffer = thumbArray;
                thumbTemp->height = info.thumbBGRA.height;
                thumbTemp->length = info.thumbBGRA.length;
                thumbTemp->width = info.thumbBGRA.width;
                TRTCImageBuffer^ iconTemp = gcnew TRTCImageBuffer();
                array<Byte>^ iconArray;
                if (info.iconBGRA.length <= 0)
                    iconArray = gcnew array<Byte>(0);
                iconArray = gcnew array<Byte>(info.iconBGRA.length);
                for (unsigned int i = 0; i < info.iconBGRA.length; i++)
                {
                    iconArray[i] = info.iconBGRA.buffer[i];
                }
                iconTemp->buffer = iconArray;
                iconTemp->height = info.iconBGRA.height;
                iconTemp->length = info.iconBGRA.length;
                iconTemp->width = info.iconBGRA.width;
                temp->thumbBGRA = thumbTemp;
                temp->iconBGRA = iconTemp;
                return temp;
            }
            else
            {
                return nullptr;
            }
        }

        virtual void release()
        {
            if (nullptr != m_screenCaptureSourceList)
                m_screenCaptureSourceList->release();
        }
    private:
        liteav::ITRTCScreenCaptureSourceList* m_screenCaptureSourceList;
    };

    ITRTCCloud::ITRTCCloud()
        : m_impl(::getTRTCShareInstance())
        , is_sub_cloud(false)
        , m_callbackImpl()
        , m_videoRenderCallbackImpl(new TRTCVideoRenderCallbackImpl())
        , m_videoFrameCallbackImpl(new ITRTCVideoFrameCallbackImpl())
        , m_audioFrameCallbackImpl(new TRTCAudioFrameCallbackImpl())
        , m_logCallbackImpl(new TRTCLogCallbackImpl())
        , m_callbacks(gcnew List<ITRTCCloudCallback^>())
        , m_audioFrameCallback(nullptr)
        , m_videoFrameCallback(nullptr)
        , m_videoRenderCallbacks(gcnew Dictionary<String^, ITRTCVideoRenderCallback^>())
        , m_logCallback()
        , m_video_frame_cache(gcnew Dictionary<String^, TRTCVideoFrame^>())
        , m_audio_frame_cache(gcnew Dictionary<String^, TRTCAudioFrame^>())
        , m_audioEffectManager(gcnew ITXAudioEffectManager())
        , m_deviceManager(gcnew ITXDeviceManager())
    {
        assert(nullptr != m_impl);
        assert(nullptr != m_audioFrameCallbackImpl);
        assert(nullptr != m_videoRenderCallbackImpl);
        assert(nullptr != m_videoFrameCallbackImpl);
        assert(nullptr != m_logCallbackImpl);
        char str[] = R"({"api":"setFramework","params":{"framework":13}})";
        m_impl->callExperimentalAPI(str);
    }

    ITRTCCloud::ITRTCCloud(bool useless)
        : is_sub_cloud(useless)
        , m_callbackImpl()
        , m_videoRenderCallbackImpl(new TRTCVideoRenderCallbackImpl())
        , m_videoFrameCallbackImpl(new ITRTCVideoFrameCallbackImpl())
        , m_audioFrameCallbackImpl(new TRTCAudioFrameCallbackImpl())
        , m_logCallbackImpl(new TRTCLogCallbackImpl())
        , m_callbacks(gcnew List<ITRTCCloudCallback^>())
        , m_audioFrameCallback(nullptr)
        , m_videoFrameCallback(nullptr)
        , m_videoRenderCallbacks(gcnew Dictionary<String^, ITRTCVideoRenderCallback^>())
        , m_logCallback()
        , m_video_frame_cache(gcnew Dictionary<String^, TRTCVideoFrame^>())
        , m_audio_frame_cache(gcnew Dictionary<String^, TRTCAudioFrame^>())
        , m_audioEffectManager(gcnew ITXAudioEffectManager())
        , m_deviceManager(gcnew ITXDeviceManager())
    {
        if (is_sub_cloud)
        {
            m_impl = ::getTRTCShareInstance()->createSubCloud();
        }
        assert(nullptr != m_impl);
        assert(nullptr != m_audioFrameCallbackImpl);
        assert(nullptr != m_videoRenderCallbackImpl);
        assert(nullptr != m_videoFrameCallbackImpl);
        assert(nullptr != m_logCallbackImpl);
    }

    ITRTCCloud^ ITRTCCloud::getTRTCShareInstance()
    {
        if (sInstance == nullptr)
        {
            msclr::lock l(slocker);
            if (sInstance == nullptr)
            {
                sInstance = gcnew ITRTCCloud();
            }
        }
        return sInstance;
    }

    void ITRTCCloud::destroyTRTCShareInstance()
    {
        msclr::lock l(slocker);
        if (sInstance != nullptr)
        {
            sInstance->~ITRTCCloud();
            sInstance = nullptr;
        }
    }

    ITRTCCloud::~ITRTCCloud()
    {
        this->!ITRTCCloud();
        if (m_callbacks != nullptr)
        {
            msclr::lock l(m_cb_locker);
            m_callbacks->Clear();
            delete m_callbacks;
            m_callbacks = nullptr;
        }
        if (m_video_frame_cache != nullptr)
        {
            msclr::lock l(m_video_frame_cache);
            m_video_frame_cache->Clear();
            delete m_video_frame_cache;
            m_video_frame_cache = nullptr;
        }
        if (m_audio_frame_cache != nullptr)
        {
            msclr::lock l(m_audio_frame_cache);
            m_audio_frame_cache->Clear();
            delete m_audio_frame_cache;
            m_audio_frame_cache = nullptr;
        }

        if (m_videoRenderCallbacks != nullptr)
        {
            msclr::lock l(m_videoRenderCallbacks);
            m_videoRenderCallbacks->Clear();
            delete m_videoRenderCallbacks;
            m_videoRenderCallbacks = nullptr;
        }
    }

    ITRTCCloud::!ITRTCCloud()
    {
        if (is_sub_cloud) {
            ::getTRTCShareInstance()->destroySubCloud(m_impl);
        }
        else {
            ::destroyTRTCShareInstance();
        }
        m_impl = nullptr;

        if (nullptr != m_callbackImpl)
        {
            delete m_callbackImpl;
            m_callbackImpl = nullptr;
        }
        if (nullptr != m_audioFrameCallbackImpl)
        {
            delete m_audioFrameCallbackImpl;
            m_audioFrameCallbackImpl = nullptr;
        }
        if (nullptr != m_videoFrameCallbackImpl) {
            delete m_videoFrameCallbackImpl;
            m_videoFrameCallbackImpl = nullptr;
        }
        if (nullptr != m_videoRenderCallbackImpl)
        {
            delete m_videoRenderCallbackImpl;
            m_videoRenderCallbackImpl = nullptr;
        }
        if (nullptr != m_logCallbackImpl)
        {
            delete m_logCallbackImpl;
            m_logCallbackImpl = nullptr;
        }
    }

    void ITRTCCloud::addCallback(ITRTCCloudCallback^ callback)
    {
        if (m_impl && callback)
        {
            msclr::lock l(m_cb_locker);
            m_callbacks->Add(callback);

            if (m_callbackImpl == nullptr)
            {
                m_callbackImpl = new TRTCCloudCallbackImpl();
                if (m_errorDelegate == nullptr)
                    m_errorDelegate = gcnew ErrorDelegate(this, &ITRTCCloud::onError);
                m_callbackImpl->setError((PFN_ERROR)Marshal::GetFunctionPointerForDelegate(m_errorDelegate).ToPointer());
                if (m_warningDelegate == nullptr)
                    m_warningDelegate = gcnew WarningDelegate(this, &ITRTCCloud::onWarning);
                m_callbackImpl->setWarning((PFN_WARNING)Marshal::GetFunctionPointerForDelegate(m_warningDelegate).ToPointer());
                if (m_enterRoomDelegate == nullptr)
                    m_enterRoomDelegate = gcnew EnterRoomDelegate(this, &ITRTCCloud::onEnterRoom);
                m_callbackImpl->setEnterRoom((PFN_ENTERROOM)Marshal::GetFunctionPointerForDelegate(m_enterRoomDelegate).ToPointer());
                if (m_exitRoomDelegate == nullptr)
                    m_exitRoomDelegate = gcnew ExitRoomDelegate(this, &ITRTCCloud::onExitRoom);
                m_callbackImpl->setExitRoom((PFN_EXITROOM)Marshal::GetFunctionPointerForDelegate(m_exitRoomDelegate).ToPointer());
                if (m_userEnterDelegate == nullptr)
                    m_userEnterDelegate = gcnew UserEnterDelegate(this, &ITRTCCloud::onUserEnter);
                m_callbackImpl->setUserEnter((PFN_USERENTER)Marshal::GetFunctionPointerForDelegate(m_userEnterDelegate).ToPointer());
                if (m_remoteUserEnterRoomDelegate == nullptr)
                    m_remoteUserEnterRoomDelegate = gcnew RemoteUserEnterRoomDelegate(this, &ITRTCCloud::onRemoteUserEnterRoom);
                m_callbackImpl->setRemoteUserEnterRoom((PFN_REMOTEUSERENTERROOM)Marshal::GetFunctionPointerForDelegate(m_remoteUserEnterRoomDelegate).ToPointer());
                if (m_remoteUserLeaveRoomDelegate == nullptr)
                    m_remoteUserLeaveRoomDelegate = gcnew RemoteUserLeaveRoomDelegate(this, &ITRTCCloud::onRemoteUserLeaveRoom);
                m_callbackImpl->setRemoteUserLeaveRoom((PFN_REMOTEUSERLEAVEROOM)Marshal::GetFunctionPointerForDelegate(m_remoteUserLeaveRoomDelegate).ToPointer());
                if (m_userExitDelegate == nullptr)
                    m_userExitDelegate = gcnew UserExitDelegate(this, &ITRTCCloud::onUserExit);
                m_callbackImpl->setUserExit((PFN_USEREXIT)Marshal::GetFunctionPointerForDelegate(m_userExitDelegate).ToPointer());
                if (m_switchRoleDelegate == nullptr)
                    m_switchRoleDelegate = gcnew SwitchRoleDelegate(this, &ITRTCCloud::onSwitchRole);
                m_callbackImpl->setSwitchRole((PFN_SWITCHROLE)Marshal::GetFunctionPointerForDelegate(m_switchRoleDelegate).ToPointer());
                if (m_switchRoomDelegate == nullptr)
                    m_switchRoomDelegate = gcnew SwitchRoomDelegate(this, &ITRTCCloud::onSwitchRoom);
                m_callbackImpl->setSwitchRoom((PFN_SWITCHROOM)Marshal::GetFunctionPointerForDelegate(m_switchRoomDelegate).ToPointer());
                if (m_connectOtherRoomDelegate == nullptr)
                    m_connectOtherRoomDelegate = gcnew ConnectOtherRoomDelegate(this, &ITRTCCloud::onConnectOtherRoom);
                m_callbackImpl->setConnectOtherRoom((PFN_CONNECTOTHERROOM)Marshal::GetFunctionPointerForDelegate(m_connectOtherRoomDelegate).ToPointer());
                if (m_disconnectOtherRoomDelegate == nullptr)
                    m_disconnectOtherRoomDelegate = gcnew DisconnectOtherRoomDelegate(this, &ITRTCCloud::onDisconnectOtherRoom);
                m_callbackImpl->setDisconnectOtherRoom((PFN_DISCONNECTOTHERROOM)Marshal::GetFunctionPointerForDelegate(m_disconnectOtherRoomDelegate).ToPointer());
                if (m_userVideoAvailableDelegate == nullptr)
                    m_userVideoAvailableDelegate = gcnew UserVideoAvailableDelegate(this, &ITRTCCloud::onUserVideoAvailable);
                m_callbackImpl->setUserVideoAvailable((PFN_USERVIDEOAVAILABLE)Marshal::GetFunctionPointerForDelegate(m_userVideoAvailableDelegate).ToPointer());
                if (m_userSubStreamAvailableDelegate == nullptr)
                    m_userSubStreamAvailableDelegate = gcnew UserSubStreamAvailableDelegate(this, &ITRTCCloud::onUserSubStreamAvailable);
                m_callbackImpl->setUserSubStreamAvailable((PFN_USERSUBSTREAMAVAILABLE)Marshal::GetFunctionPointerForDelegate(m_userSubStreamAvailableDelegate).ToPointer());
                if (m_userAudioAvailableDelegate == nullptr)
                    m_userAudioAvailableDelegate = gcnew UserAudioAvailableDelegate(this, &ITRTCCloud::onUserAudioAvailable);
                m_callbackImpl->setUserAudioAvailable((PFN_USERAUDIOAVAILABLE)Marshal::GetFunctionPointerForDelegate(m_userAudioAvailableDelegate).ToPointer());
                if (m_userVoiceVolumeDelegate == nullptr)
                    m_userVoiceVolumeDelegate = gcnew UserVoiceVolumeDelegate(this, &ITRTCCloud::onUserVoiceVolume);
                m_callbackImpl->setUserVoiceVolume((PFN_USERVOICEVOLUME)Marshal::GetFunctionPointerForDelegate(m_userVoiceVolumeDelegate).ToPointer());
                if (m_networkQualityDelegate == nullptr)
                    m_networkQualityDelegate = gcnew NetworkQualityDelegate(this, &ITRTCCloud::onNetworkQuality);
                m_callbackImpl->setNetworkQuality((PFN_NETWORKQUALITY)Marshal::GetFunctionPointerForDelegate(m_networkQualityDelegate).ToPointer());
                if (m_statisticsDelegate == nullptr)
                    m_statisticsDelegate = gcnew StatisticsDelegate(this, &ITRTCCloud::onStatistics);
                m_callbackImpl->setStatistics((PFN_STATISTICS)Marshal::GetFunctionPointerForDelegate(m_statisticsDelegate).ToPointer());
                if (m_firstVideoFrameDelegate == nullptr)
                    m_firstVideoFrameDelegate = gcnew FirstVideoFrameDelegate(this, &ITRTCCloud::onFirstVideoFrame);
                m_callbackImpl->setFirstVideoFrame((PFN_FIRSTVIDEOFRAME)Marshal::GetFunctionPointerForDelegate(m_firstVideoFrameDelegate).ToPointer());
                if (m_firstAudioFrameDelegate == nullptr)
                    m_firstAudioFrameDelegate = gcnew FirstAudioFrameDelegate(this, &ITRTCCloud::onFirstAudioFrame);
                m_callbackImpl->setFirstAudioFrame((PFN_FIRSTAUDIOFRAME)Marshal::GetFunctionPointerForDelegate(m_firstAudioFrameDelegate).ToPointer());
                if (m_sendFirstLocalVideoFrameDelegate == nullptr)
                    m_sendFirstLocalVideoFrameDelegate = gcnew SendFirstLocalVideoFrameDelegate(this, &ITRTCCloud::onSendFirstLocalVideoFrame);
                m_callbackImpl->setSendFirstLocalVideoFrame((PFN_SENDFIRSTLOCALVIDEOFRAME)Marshal::GetFunctionPointerForDelegate(m_sendFirstLocalVideoFrameDelegate).ToPointer());
                if (m_sendFirstLocalAudioFrameDelegate == nullptr)
                    m_sendFirstLocalAudioFrameDelegate = gcnew SendFirstLocalAudioFrameDelegate(this, &ITRTCCloud::onSendFirstLocalAudioFrame);
                m_callbackImpl->setSendFirstLocalAudioFrame((PFN_SENDFIRSTLOCALAUDIOFRAME)Marshal::GetFunctionPointerForDelegate(m_sendFirstLocalAudioFrameDelegate).ToPointer());
                if (m_playBGMBeginDelegate == nullptr)
                    m_playBGMBeginDelegate = gcnew PlayBGMBeginDelegate(this, &ITRTCCloud::onPlayBGMBegin);
                m_callbackImpl->setPlayBGMBegin((PFN_PLAYBGMBEGIN)Marshal::GetFunctionPointerForDelegate(m_playBGMBeginDelegate).ToPointer());
                if (m_playBGMProgressDelegate == nullptr)
                    m_playBGMProgressDelegate = gcnew PlayBGMProgressDelegate(this, &ITRTCCloud::onPlayBGMProgress);
                m_callbackImpl->setPlayBGMProgress((PFN_PLAYBGMPROGRESS)Marshal::GetFunctionPointerForDelegate(m_playBGMProgressDelegate).ToPointer());
                if (m_playBGMCompleteDelegate == nullptr)
                    m_playBGMCompleteDelegate = gcnew PlayBGMCompleteDelegate(this, &ITRTCCloud::onPlayBGMComplete);
                m_callbackImpl->setPlayBGMComplete((PFN_PLAYBGMCOMPLETE)Marshal::GetFunctionPointerForDelegate(m_playBGMCompleteDelegate).ToPointer());
                if (m_connectionLostDelegate == nullptr)
                    m_connectionLostDelegate = gcnew ConnectionLostDelegate(this, &ITRTCCloud::onConnectionLost);
                m_callbackImpl->setConnectionLost((PFN_CONNECTIONLOST)Marshal::GetFunctionPointerForDelegate(m_connectionLostDelegate).ToPointer());
                if (m_tryToReconnectDelegate == nullptr)
                    m_tryToReconnectDelegate = gcnew TryToReconnectDelegate(this, &ITRTCCloud::onTryToReconnect);
                m_callbackImpl->setTryToReconnect((PFN_TRYTORECONNECT)Marshal::GetFunctionPointerForDelegate(m_tryToReconnectDelegate).ToPointer());
                if (m_connectionRecoveryDelegate == nullptr)
                    m_connectionRecoveryDelegate = gcnew ConnectionRecoveryDelegate(this, &ITRTCCloud::onConnectionRecovery);
                m_callbackImpl->setConnectionRecovery((PFN_CONNECTIONRECOVERY)Marshal::GetFunctionPointerForDelegate(m_connectionRecoveryDelegate).ToPointer());
                if (m_speedTestDelegate == nullptr)
                    m_speedTestDelegate = gcnew SpeedTestDelegate(this, &ITRTCCloud::onSpeedTest);
                m_callbackImpl->setSpeedTest((PFN_SPEEDTEST)Marshal::GetFunctionPointerForDelegate(m_speedTestDelegate).ToPointer());
                if (m_cameraDidReadyDelegate == nullptr)
                    m_cameraDidReadyDelegate = gcnew CameraDidReadyDelegate(this, &ITRTCCloud::onCameraDidReady);
                m_callbackImpl->setCameraDidReady((PFN_CAMERADIDREADY)Marshal::GetFunctionPointerForDelegate(m_cameraDidReadyDelegate).ToPointer());
                if (m_micDidReadyDelegate == nullptr)
                    m_micDidReadyDelegate = gcnew MicDidReadyDelegate(this, &ITRTCCloud::onMicDidReady);
                m_callbackImpl->setMicDidReady((PFN_MICDIDREADY)Marshal::GetFunctionPointerForDelegate(m_micDidReadyDelegate).ToPointer());
                if (m_deviceChangeDelegate == nullptr)
                    m_deviceChangeDelegate = gcnew DeviceChangeDelegate(this, &ITRTCCloud::onDeviceChange);
                m_callbackImpl->setDeviceChange((PFN_DEVICECHANGE)Marshal::GetFunctionPointerForDelegate(m_deviceChangeDelegate).ToPointer());
                if (m_testMicVolumeDelegate == nullptr)
                    m_testMicVolumeDelegate = gcnew TestMicVolumeDelegate(this, &ITRTCCloud::onTestMicVolume);
                m_callbackImpl->setTestMicVolume((PFN_TESTMICVOLUME)Marshal::GetFunctionPointerForDelegate(m_testMicVolumeDelegate).ToPointer());
                if (m_testSpeakerVolumeDelegate == nullptr)
                    m_testSpeakerVolumeDelegate = gcnew TestSpeakerVolumeDelegate(this, &ITRTCCloud::onTestSpeakerVolume);
                m_callbackImpl->setTestSpeakerVolume((PFN_TESTSPEAKERVOLUME)Marshal::GetFunctionPointerForDelegate(m_testSpeakerVolumeDelegate).ToPointer());
                if (m_audioDeviceCaptureVolumeChangedDelegate == nullptr)
                    m_audioDeviceCaptureVolumeChangedDelegate = gcnew AudioDeviceCaptureVolumeChangedDelegate(this, &ITRTCCloud::onAudioDeviceCaptureVolumeChanged);
                m_callbackImpl->setAudioDeviceCaptureVolumeChanged((PFN_AUDIODEVICECAPTUREVOLUMECHANGED)Marshal::GetFunctionPointerForDelegate(
                    m_audioDeviceCaptureVolumeChangedDelegate).ToPointer());
                if (m_audioDevicePlayoutVolumeChangedDelegate == nullptr)
                    m_audioDevicePlayoutVolumeChangedDelegate = gcnew AudioDevicePlayoutVolumeChangedDelegate(this, &ITRTCCloud::onAudioDevicePlayoutVolumeChanged);
                m_callbackImpl->setAudioDevicePlayoutVolumeChanged((PFN_AUDIODEVICEPLAYOUTVOLUMECHANGED)Marshal::GetFunctionPointerForDelegate(
                    m_audioDevicePlayoutVolumeChangedDelegate).ToPointer());
                if (m_recvCustomCmdMsgDelegate == nullptr)
                    m_recvCustomCmdMsgDelegate = gcnew RecvCustomCmdMsgDelegate(this, &ITRTCCloud::onRecvCustomCmdMsg);
                m_callbackImpl->setRecvCustomCmdMsg((PFN_RECVCUSTOMCMDMSG)Marshal::GetFunctionPointerForDelegate(m_recvCustomCmdMsgDelegate).ToPointer());
                if (m_missCustomCmdMsgDelegate == nullptr)
                    m_missCustomCmdMsgDelegate = gcnew MissCustomCmdMsgDelegate(this, &ITRTCCloud::onMissCustomCmdMsg);
                m_callbackImpl->setMissCustomCmdMsg((PFN_MISSCUSTOMCMDMSG)Marshal::GetFunctionPointerForDelegate(m_missCustomCmdMsgDelegate).ToPointer());
                if (m_recvSEIMsgDelegate == nullptr)
                    m_recvSEIMsgDelegate = gcnew RecvSEIMsgDelegate(this, &ITRTCCloud::onRecvSEIMsg);
                m_callbackImpl->setRecvSEIMsg((PFN_RECVSEIMSG)Marshal::GetFunctionPointerForDelegate(m_recvSEIMsgDelegate).ToPointer());
                if (m_startPublishingDelegate == nullptr)
                    m_startPublishingDelegate = gcnew StartPublishingDelegate(this, &ITRTCCloud::onStartPublishing);
                m_callbackImpl->setStartPublishing((PFN_STARTPUBLISHING)Marshal::GetFunctionPointerForDelegate(m_startPublishingDelegate).ToPointer());
                if (m_stopPublishingDelegate == nullptr)
                    m_stopPublishingDelegate = gcnew StopPublishingDelegate(this, &ITRTCCloud::onStopPublishing);
                m_callbackImpl->setStopPublishing((PFN_STOPPUBLISHING)Marshal::GetFunctionPointerForDelegate(m_stopPublishingDelegate).ToPointer());
                if (m_startPublishCDNStreamDelegate == nullptr)
                    m_startPublishCDNStreamDelegate = gcnew StartPublishCDNStreamDelegate(this, &ITRTCCloud::onStartPublishCDNStream);
                m_callbackImpl->setStartPublishCDNStream((PFN_STARTPUBLISHCDNSTREAM)Marshal::GetFunctionPointerForDelegate(m_startPublishCDNStreamDelegate).ToPointer());
                if (m_stopPublishCDNStreamDelegate == nullptr)
                    m_stopPublishCDNStreamDelegate = gcnew StopPublishCDNStreamDelegate(this, &ITRTCCloud::onStopPublishCDNStream);
                m_callbackImpl->setStopPublishCDNStream((PFN_STOPPUBLISHCDNSTREAM)Marshal::GetFunctionPointerForDelegate(m_stopPublishCDNStreamDelegate).ToPointer());
                if (m_setMixTranscodingConfigDelegate == nullptr)
                    m_setMixTranscodingConfigDelegate = gcnew SetMixTranscodingConfigDelegate(this, &ITRTCCloud::onSetMixTranscodingConfig);
                m_callbackImpl->setSetMixTranscodingConfig((PFN_SETMIXTRANSCODINGCONFIG)Marshal::GetFunctionPointerForDelegate(m_setMixTranscodingConfigDelegate).ToPointer());
                if (m_audioEffectFinishedDelegate == nullptr)
                    m_audioEffectFinishedDelegate = gcnew AudioEffectFinishedDelegate(this, &ITRTCCloud::onAudioEffectFinished);
                m_callbackImpl->setAudioEffectFinished((PFN_AUDIOEFFECTFINISHED)Marshal::GetFunctionPointerForDelegate(m_audioEffectFinishedDelegate).ToPointer());
                if (m_screenCaptureCoveredDelegate == nullptr)
                    m_screenCaptureCoveredDelegate = gcnew ScreenCaptureCoveredDelegate(this, &ITRTCCloud::onScreenCaptureCovered);
                m_callbackImpl->setScreenCaptureCovered((PFN_SCREENCAPTURECOVERED)Marshal::GetFunctionPointerForDelegate(m_screenCaptureCoveredDelegate).ToPointer());
                if (m_screenCaptureStartedDelegate == nullptr)
                    m_screenCaptureStartedDelegate = gcnew ScreenCaptureStartedDelegate(this, &ITRTCCloud::onScreenCaptureStarted);
                m_callbackImpl->setScreenCaptureStarted((PFN_SCREENCAPTURESTARTED)Marshal::GetFunctionPointerForDelegate(m_screenCaptureStartedDelegate).ToPointer());
                if (m_screenCapturePausedDelegate == nullptr)
                    m_screenCapturePausedDelegate = gcnew ScreenCapturePausedDelegate(this, &ITRTCCloud::onScreenCapturePaused);
                m_callbackImpl->setScreenCapturePaused((PFN_SCREENCAPTUREPAUSED)Marshal::GetFunctionPointerForDelegate(m_screenCapturePausedDelegate).ToPointer());
                if (m_screenCaptureResumedDelegate == nullptr)
                    m_screenCaptureResumedDelegate = gcnew ScreenCaptureResumedDelegate(this, &ITRTCCloud::onScreenCaptureResumed);
                m_callbackImpl->setScreenCaptureResumed((PFN_SCREENCAPTURERESUMED)Marshal::GetFunctionPointerForDelegate(m_screenCaptureResumedDelegate).ToPointer());
                if (m_screenCaptureStopedDelegate == nullptr)
                    m_screenCaptureStopedDelegate = gcnew ScreenCaptureStopedDelegate(this, &ITRTCCloud::onScreenCaptureStoped);
                m_callbackImpl->setScreenCaptureStoped((PFN_SCREENCAPTURESTOPED)Marshal::GetFunctionPointerForDelegate(m_screenCaptureStopedDelegate).ToPointer());
                if (m_snapshotCompleteDelegate == nullptr)
                    m_snapshotCompleteDelegate = gcnew SnapshotCompleteDelegate(this, &ITRTCCloud::onSnapshotComplete);
                m_callbackImpl->setSnapshotComplete((PFN_SNAPSHOTCOMPLETE)Marshal::GetFunctionPointerForDelegate(m_snapshotCompleteDelegate).ToPointer());
                if (m_localRecordBeginDelegate == nullptr)
                    m_localRecordBeginDelegate = gcnew LocalRecordBeginDelegate(this, &ITRTCCloud::onLocalRecordBegin);
                m_callbackImpl->setLocalRecordBegin((PFN_LOCALRECORDBEGIN)Marshal::GetFunctionPointerForDelegate(m_localRecordBeginDelegate).ToPointer());
                if (m_localRecordingDelegate == nullptr)
                    m_localRecordingDelegate = gcnew LocalRecordingDelegate(this, &ITRTCCloud::onLocalRecording);
                m_callbackImpl->setLocalRecording((PFN_LOCALRECORDING)Marshal::GetFunctionPointerForDelegate(m_localRecordingDelegate).ToPointer());
                if (m_localRecordCompleteDelegate == nullptr)
                    m_localRecordCompleteDelegate = gcnew LocalRecordCompleteDelegate(this, &ITRTCCloud::onLocalRecordComplete);
                m_callbackImpl->setLocalRecordComplete((PFN_LOCALRECORDCOMPLETE)Marshal::GetFunctionPointerForDelegate(m_localRecordCompleteDelegate).ToPointer());

                if (nullptr == m_remoteVideoStatusUpdatedDelegate)
                {
                    m_remoteVideoStatusUpdatedDelegate = gcnew RemoteVideoStatusUpdatedDelegate(this, &ITRTCCloud::onRemoteVideoStatusUpdated);
                }
                m_callbackImpl->setRemoteVideoStatusUpdated(
                    (PFN_REMOTEVIDEOSTATUSUPDATED)Marshal::GetFunctionPointerForDelegate(m_remoteVideoStatusUpdatedDelegate).ToPointer()
                );

                m_impl->addCallback(m_callbackImpl);
            }
        }
    }

    void ITRTCCloud::removeCallback(ITRTCCloudCallback^ callback)
    {
        if (m_impl && callback)
        {
            msclr::lock l(m_cb_locker);
            int i = m_callbacks->IndexOf(callback);
            if (i < 0) return;

            m_callbacks->RemoveAt(i);

            if (m_callbacks->Count <= 0)
            {
                m_impl->removeCallback(m_callbackImpl);
                delete m_callbackImpl;
                m_callbackImpl = nullptr;
            }
        }
    }

    void ITRTCCloud::enterRoom(TRTCParams^% params, TRTCAppScene scene)
    {
        if (nullptr != m_impl)
        {
            ::TRTCParams temp;
            temp.sdkAppId = params->sdkAppId;
            char* userId = Utils::StringToUTF8CharPtr(params->userId);
            temp.userId = userId;
            temp.roomId = params->roomId;
            char* str_room_id = Utils::StringToUTF8CharPtr(params->strRoomId);
            temp.strRoomId = str_room_id;
            char* userSig = Utils::StringToUTF8CharPtr(params->userSig);
            temp.userSig = userSig;
            temp.role = static_cast<::TRTCRoleType>(params->role);
            char* privateMapKey = Utils::StringToUTF8CharPtr(params->privateMapKey);
            temp.privateMapKey = privateMapKey;
            char* businessInfo = Utils::StringToUTF8CharPtr(params->businessInfo);
            temp.businessInfo = businessInfo;
            char* streamId = Utils::StringToUTF8CharPtr(params->streamId);
            temp.streamId = streamId;
            char* userDefineRecordId = Utils::StringToUTF8CharPtr(params->userDefineRecordId);
            temp.userDefineRecordId = userDefineRecordId;
            m_impl->enterRoom(temp, static_cast<::TRTCAppScene>(scene));
            delete[] userId;
            userId = nullptr;
            delete[] userSig;
            userSig = nullptr;
            delete[] privateMapKey;
            privateMapKey = nullptr;
            delete[] businessInfo;
            businessInfo = nullptr;
            delete[] streamId;
            streamId = nullptr;
            delete[] userDefineRecordId;
            userDefineRecordId = nullptr;
            delete[] str_room_id;
            str_room_id = nullptr;
        }
    }

    void ITRTCCloud::exitRoom()
    {
        if (nullptr != m_impl)
            m_impl->exitRoom();
    }

    void ITRTCCloud::switchRole(TRTCRoleType role)
    {
        if (nullptr != m_impl)
            m_impl->switchRole(static_cast<::TRTCRoleType>(role));
    }

    void ITRTCCloud::connectOtherRoom(String^ params)
    {
        if (nullptr != m_impl)
        {
            char* paramsStr = Utils::StringToUTF8CharPtr(params);
            m_impl->connectOtherRoom(paramsStr);
            delete[] paramsStr;
            paramsStr = NULL;

        }
    }

    void ITRTCCloud::disconnectOtherRoom()
    {
        if (nullptr != m_impl)
            m_impl->disconnectOtherRoom();
    }

    void ITRTCCloud::setDefaultStreamRecvMode(bool autoRecvAudio, bool autoRecvVideo)
    {
        if (nullptr != m_impl)
            m_impl->setDefaultStreamRecvMode(autoRecvAudio, autoRecvVideo);
    }

    ITRTCCloud^ ITRTCCloud::createSubCloud() {
        ITRTCCloud^ sub_cloud = gcnew ITRTCCloud(true);
        return sub_cloud;
    }

    void ITRTCCloud::destroySubCloud(ITRTCCloud^ subCloud) {
        if (subCloud != nullptr && subCloud->is_sub_cloud) {
            subCloud->~ITRTCCloud();
        }
    }

    void ITRTCCloud::switchRoom(TRTCSwitchRoomConfig^% config) {
        if (nullptr != m_impl) {
            ::TRTCSwitchRoomConfig switch_room_config;
            switch_room_config.roomId = config->roomId;
            char* str_room_id = Utils::StringToUTF8CharPtr(config->strRoomId);
            switch_room_config.strRoomId = str_room_id;
            char* private_map_key = Utils::StringToUTF8CharPtr(config->privateMapKey);
            switch_room_config.privateMapKey = private_map_key;
            char* user_sig = Utils::StringToUTF8CharPtr(config->userSig);
            switch_room_config.userSig = user_sig;
            m_impl->switchRoom(switch_room_config);
            delete[] str_room_id;
            str_room_id = nullptr;
            delete[] private_map_key;
            private_map_key = nullptr;
            delete[] user_sig;
            user_sig = nullptr;
        }
    }

    void ITRTCCloud::startLocalPreview(IntPtr rendView)
    {
        if (nullptr != m_impl)
            m_impl->startLocalPreview((HWND)rendView.ToPointer());
    }

    void ITRTCCloud::updateLocalView(IntPtr rendView) {
        if (nullptr != m_impl)
            m_impl->updateLocalView((HWND)rendView.ToPointer());
    }

    void ITRTCCloud::stopLocalPreview()
    {
        if (nullptr != m_impl)
            m_impl->stopLocalPreview();
    }

    void ITRTCCloud::startRemoteView(String^ userId, TRTCVideoStreamType streamType,
        IntPtr rendView) {
        if (nullptr != m_impl) {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->startRemoteView(uid, static_cast<::TRTCVideoStreamType>(streamType), (HWND)rendView.ToPointer());
            delete[] uid;
            uid = nullptr;
        }
    }

    void ITRTCCloud::updateRemoteView(String^ userId, TRTCVideoStreamType streamType,
        IntPtr rendView) {
        if (nullptr != m_impl) {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->updateRemoteView(uid, static_cast<::TRTCVideoStreamType>(streamType),
                (HWND)rendView.ToPointer());
            delete[] uid;
            uid = nullptr;
        }
    }

    void ITRTCCloud::stopRemoteView(String^ userId, TRTCVideoStreamType streamType) {
        if (nullptr != m_impl) {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->stopRemoteView(uid, static_cast<::TRTCVideoStreamType>(streamType));
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::startRemoteView(String^ userId, IntPtr rendHwnd) {
        if (nullptr != m_impl) {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->startRemoteView(uid, (HWND)rendHwnd.ToPointer());
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::stopRemoteView(String^ userId) {
        if (nullptr != m_impl) {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->stopRemoteView(uid);
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::stopAllRemoteView()
    {
        if (nullptr != m_impl)
            m_impl->stopAllRemoteView();
    }

    void ITRTCCloud::muteLocalVideo(bool mute)
    {
        if (nullptr != m_impl)
            m_impl->muteLocalVideo(mute);
    }

    void ITRTCCloud::muteLocalVideo(TRTCVideoStreamType streamType, bool mute) {
        if (nullptr != m_impl) {
            m_impl->muteLocalVideo(static_cast<::TRTCVideoStreamType>(streamType), mute);
        }
    }

    void ITRTCCloud::muteRemoteVideoStream(String^ userId, TRTCVideoStreamType streamType, bool mute) {
        if (nullptr != m_impl) {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->muteRemoteVideoStream(uid, static_cast<::TRTCVideoStreamType>(streamType), mute);
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::muteRemoteVideoStream(String^ userId, bool mute)
    {
        if (nullptr != m_impl)
        {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->muteRemoteVideoStream(uid, mute);
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::muteAllRemoteVideoStreams(bool mute)
    {
        if (nullptr != m_impl)
        {
            m_impl->muteAllRemoteVideoStreams(mute);
        }
    }

    void ITRTCCloud::setVideoEncoderParam(TRTCVideoEncParam^% params)
    {
        if (nullptr != m_impl)
        {
            ::TRTCVideoEncParam temp;
            temp.resMode = static_cast<::TRTCVideoResolutionMode>(params->resMode);
            temp.videoBitrate = params->videoBitrate;
            temp.minVideoBitrate = params->minVideoBitrate;
            temp.videoFps = params->videoFps;
            temp.videoResolution = static_cast<::TRTCVideoResolution>(params->videoResolution);
            temp.enableAdjustRes = params->enableAdjustRes;
            m_impl->setVideoEncoderParam(temp);
        }
    }

    void ITRTCCloud::setNetworkQosParam(TRTCNetworkQosParam^% params)
    {
        if (nullptr != m_impl)
        {
            ::TRTCNetworkQosParam temp;
            temp.controlMode = static_cast<::TRTCQosControlMode>(params->controlMode);
            temp.preference = static_cast<::TRTCVideoQosPreference>(params->preference);
            m_impl->setNetworkQosParam(temp);
        }
    }

    void ITRTCCloud::setLocalRenderParams(TRTCRenderParams^% params) {
        if (nullptr != m_impl) {
            ::TRTCRenderParams render_param;
            render_param.fillMode = static_cast<::TRTCVideoFillMode>(params->fillMode);
            render_param.mirrorType = static_cast<::TRTCVideoMirrorType>(params->mirrorType);
            render_param.rotation = static_cast<::TRTCVideoRotation>(params->rotation);
            m_impl->setLocalRenderParams(render_param);
        }
    }

    void ITRTCCloud::setLocalViewFillMode(TRTCVideoFillMode mode)
    {
        if (nullptr != m_impl)
            m_impl->setLocalViewFillMode(static_cast<::TRTCVideoFillMode>(mode));
    }

    void ITRTCCloud::setRemoteViewFillMode(String^ userId, TRTCVideoFillMode mode)
    {
        if (nullptr != m_impl)
        {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->setRemoteViewFillMode(uid, static_cast<::TRTCVideoFillMode>(mode));
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::setLocalViewRotation(TRTCVideoRotation rotation)
    {
        if (nullptr != m_impl)
            m_impl->setLocalViewRotation(static_cast<::TRTCVideoRotation>(rotation));
    }

    void ITRTCCloud::setRemoteViewRotation(String^ userId, TRTCVideoRotation rotation)
    {
        if (nullptr != m_impl)
        {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->setRemoteViewRotation(uid, static_cast<::TRTCVideoRotation>(rotation));
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::setVideoEncoderRotation(TRTCVideoRotation rotation)
    {
        if (nullptr != m_impl)
            m_impl->setVideoEncoderRotation(static_cast<::TRTCVideoRotation>(rotation));
    }

    void ITRTCCloud::setLocalViewMirror(bool mirror)
    {
        if (nullptr != m_impl)
            m_impl->setLocalViewMirror(mirror);
    }

    void ITRTCCloud::setVideoEncoderMirror(bool mirror)
    {
        if (nullptr != m_impl)
            m_impl->setVideoEncoderMirror(mirror);
    }

    void ITRTCCloud::setRemoteRenderParams(String^ userId, TRTCVideoStreamType streamType,
        TRTCRenderParams^% params) {
        if (nullptr != m_impl) {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            ::TRTCRenderParams render_param;
            render_param.fillMode = static_cast<::TRTCVideoFillMode>(params->fillMode);
            render_param.mirrorType = static_cast<::TRTCVideoMirrorType>(params->mirrorType);
            render_param.rotation = static_cast<::TRTCVideoRotation>(params->rotation);
            m_impl->setRemoteRenderParams(uid, static_cast<::TRTCVideoStreamType>(streamType), render_param);
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::enableSmallVideoStream(bool enable, TRTCVideoEncParam^% smallVideoParam)
    {
        if (nullptr != m_impl)
        {
            ::TRTCVideoEncParam temp;
            temp.resMode = static_cast<::TRTCVideoResolutionMode>(smallVideoParam->resMode);
            temp.videoBitrate = smallVideoParam->videoBitrate;
            temp.videoFps = smallVideoParam->videoFps;
            temp.videoResolution = static_cast<::TRTCVideoResolution>(smallVideoParam->videoResolution);
            m_impl->enableSmallVideoStream(enable, temp);
        }
    }

    void ITRTCCloud::setRemoteVideoStreamType(String^ userId, TRTCVideoStreamType type)
    {
        if (nullptr != m_impl)
        {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->setRemoteVideoStreamType(uid, static_cast<::TRTCVideoStreamType>(type));
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::snapshotVideo(String^ userId, TRTCVideoStreamType streamType, TRTCSnapshotSourceType sourceType)
    {
        if (nullptr != m_impl)
        {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->snapshotVideo(uid, static_cast<::TRTCVideoStreamType>(streamType),
                static_cast<::TRTCSnapshotSourceType>(sourceType));
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::setPriorRemoteVideoStreamType(TRTCVideoStreamType type)
    {
        if (nullptr != m_impl)
            m_impl->setPriorRemoteVideoStreamType(static_cast<::TRTCVideoStreamType>(type));
    }

    void ITRTCCloud::setAudioQuality(TRTCAudioQuality quality)
    {
        if (nullptr != m_impl)
            m_impl->setAudioQuality(static_cast<::TRTCAudioQuality>(quality));
    }

    void ITRTCCloud::startLocalAudio()
    {
        if (nullptr != m_impl)
            m_impl->startLocalAudio();
    }

    void ITRTCCloud::startLocalAudio(TRTCAudioQuality quality) {
        if (nullptr != m_impl) {
            m_impl->startLocalAudio(static_cast<::TRTCAudioQuality>(quality));
        }
    }

    void ITRTCCloud::stopLocalAudio()
    {
        if (nullptr != m_impl)
            m_impl->stopLocalAudio();
    }

    void ITRTCCloud::muteLocalAudio(bool mute)
    {
        if (nullptr != m_impl)
            m_impl->muteLocalAudio(mute);
    }

    void ITRTCCloud::muteRemoteAudio(String^ userId, bool mute)
    {
        if (nullptr != m_impl)
        {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->muteRemoteAudio(uid, mute);
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::muteAllRemoteAudio(bool mute)
    {
        if (nullptr != m_impl)
            m_impl->muteAllRemoteAudio(mute);
    }

    void ITRTCCloud::setAudioCaptureVolume(int volume)
    {
        if (nullptr != m_impl)
            m_impl->setAudioCaptureVolume(volume);
    }

    int ITRTCCloud::getAudioCaptureVolume()
    {
        if (nullptr != m_impl)
            return m_impl->getAudioCaptureVolume();
        return 0;
    }

    void ITRTCCloud::setAudioPlayoutVolume(int volume)
    {
        if (nullptr != m_impl)
            m_impl->setAudioPlayoutVolume(volume);
    }

    int ITRTCCloud::getAudioPlayoutVolume()
    {
        if (nullptr != m_impl)
            return m_impl->getAudioPlayoutVolume();
        return 0;
    }

    void ITRTCCloud::enableAudioVolumeEvaluation(UInt32 interval)
    {
        if (nullptr != m_impl)
            m_impl->enableAudioVolumeEvaluation(interval);
    }

    int ITRTCCloud::startAudioRecording(TRTCAudioRecordingParams^% audioRecordingParams)
    {
        if (nullptr != m_impl)
        {
            ::TRTCAudioRecordingParams param;
            char* filePath = Utils::StringToUTF8CharPtr(audioRecordingParams->filePath);
            param.filePath = filePath;
            param.recordingContent = static_cast<::TRTCAudioRecordingContent>(audioRecordingParams->recordingContent);
            int result = m_impl->startAudioRecording(param);

            delete[] filePath;
            filePath = NULL;
            return result;
        }
        else
        {
            return -2;
        }
    }

    void ITRTCCloud::stopAudioRecording()
    {
        if (nullptr != m_impl)
            m_impl->stopAudioRecording();
    }

    void ITRTCCloud::setRemoteAudioVolume(String^ userId, int volume)
    {
        if (nullptr != m_impl) {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->setRemoteAudioVolume(uid, volume);
            delete[] uid;
        }
    }

    void ITRTCCloud::startLocalRecording(TRTCLocalRecordingParams^% params)
    {
        if (nullptr != m_impl) {
            ::TRTCLocalRecordingParams param;
            char* filePath = Utils::StringToUTF8CharPtr(params->filePath);
            param.filePath = filePath;
            param.recordType = static_cast<::TRTCLocalRecordType>(params->recordType);
            params->interval = params->interval;
            m_impl->startLocalRecording(param);

            delete[] filePath;
            filePath = NULL;
        }
    }

    void ITRTCCloud::stopLocalRecording()
    {
        if (nullptr != m_impl)
        {
            m_impl->stopLocalRecording();
        }
    }

    ITXDeviceManager^ ITRTCCloud::getDeviceManager() {
        return m_deviceManager;
    }

    ITRTCDeviceCollection^ ITRTCCloud::getCameraDevicesList()
    {
        if (nullptr != m_impl)
        {
            ::ITRTCDeviceCollection* collection = m_impl->getCameraDevicesList();
            if (nullptr != collection)
            {
                TRTCDeviceCollectionImpl^ impl = gcnew TRTCDeviceCollectionImpl();
                impl->setDeviceCollection(collection);
                return impl;
            }
        }
        return nullptr;
    }

    void ITRTCCloud::setCurrentCameraDevice(String^ deviceId)
    {
        if (nullptr != m_impl)
        {
            char* did = Utils::StringToUTF8CharPtr(deviceId);
            m_impl->setCurrentCameraDevice(did);
            delete[] did;
            did = NULL;
        }
    }

    ITRTCDeviceInfo^ ITRTCCloud::getCurrentCameraDevice()
    {
        if (nullptr != m_impl)
        {
            ::ITRTCDeviceInfo* info = m_impl->getCurrentCameraDevice();
            if (nullptr != info)
            {
                TRTCDeviceInfoImpl^ impl = gcnew TRTCDeviceInfoImpl();
                impl->setDeviceInfo(info);
                return impl;
            }
        }
        return nullptr;
    }

    ITRTCDeviceCollection^ ITRTCCloud::getMicDevicesList()
    {
        if (nullptr != m_impl) {
            ::ITRTCDeviceCollection* collection = m_impl->getMicDevicesList();
            if (nullptr != collection)
            {
                TRTCDeviceCollectionImpl^ impl = gcnew TRTCDeviceCollectionImpl();
                impl->setDeviceCollection(collection);
                return impl;
            }
        }
        return nullptr;
    }

    void ITRTCCloud::setCurrentMicDevice(String^ micId)
    {
        if (nullptr != m_impl)
        {
            char* mid = Utils::StringToUTF8CharPtr(micId);
            m_impl->setCurrentMicDevice(mid);
            delete[] mid;
            mid = NULL;
        }
    }

    ITRTCDeviceInfo^ ITRTCCloud::getCurrentMicDevice()
    {
        if (nullptr != m_impl)
        {
            ::ITRTCDeviceInfo* info = m_impl->getCurrentMicDevice();
            if (nullptr != info)
            {
                TRTCDeviceInfoImpl^ impl = gcnew TRTCDeviceInfoImpl();
                impl->setDeviceInfo(info);
                return impl;
            }
        }
        return nullptr;
    }

    UInt32 ITRTCCloud::getCurrentMicDeviceVolume()
    {
        if (nullptr != m_impl)
            return m_impl->getCurrentMicDeviceVolume();
        else
            return 0;
    }

    void ITRTCCloud::setCurrentMicDeviceVolume(UInt32 volume)
    {
        if (nullptr != m_impl)
            m_impl->setCurrentMicDeviceVolume(volume);
    }

    ITRTCDeviceCollection^ ITRTCCloud::getSpeakerDevicesList()
    {
        if (nullptr != m_impl) {
            ::ITRTCDeviceCollection* collection = m_impl->getSpeakerDevicesList();
            if (nullptr != collection)
            {
                TRTCDeviceCollectionImpl^ impl = gcnew TRTCDeviceCollectionImpl();
                impl->setDeviceCollection(collection);
                return impl;
            }
        }
        return nullptr;
    }

    void ITRTCCloud::setCurrentSpeakerDevice(String^ speakerId)
    {
        if (nullptr != m_impl)
        {
            char* sid = Utils::StringToUTF8CharPtr(speakerId);
            m_impl->setCurrentSpeakerDevice(sid);
            delete[] sid;
            sid = NULL;
        }
    }

    ITRTCDeviceInfo^ ITRTCCloud::getCurrentSpeakerDevice()
    {
        if (nullptr != m_impl)
        {
            ::ITRTCDeviceInfo* info = m_impl->getCurrentSpeakerDevice();
            if (nullptr != info)
            {
                TRTCDeviceInfoImpl^ impl = gcnew TRTCDeviceInfoImpl();
                impl->setDeviceInfo(info);
                return impl;
            }
        }
        return nullptr;
    }

    UInt32 ITRTCCloud::getCurrentSpeakerVolume()
    {
        if (nullptr != m_impl)
            return m_impl->getCurrentSpeakerVolume();
        else
            return 0;
    }

    void ITRTCCloud::setCurrentSpeakerVolume(UInt32 volume)
    {
        if (nullptr != m_impl)
            m_impl->setCurrentSpeakerVolume(volume);
    }

    void ITRTCCloud::setBeautyStyle(TRTCBeautyStyle style, UInt32 beauty, UInt32 white, UInt32 ruddiness)
    {
        if (nullptr != m_impl)
            m_impl->setBeautyStyle(static_cast<::TRTCBeautyStyle>(style), beauty, white, ruddiness);
    }

    void ITRTCCloud::setWaterMark(TRTCVideoStreamType streamType, String^ srcData, TRTCWaterMarkSrcType srcType, UInt32 nWidth, UInt32 nHeight, float xOffset, float yOffset, float fWidthRatio)
    {
        if (nullptr != m_impl)
        {
            char* srcDataStr = Utils::StringToUTF8CharPtr(srcData);
            m_impl->setWaterMark(static_cast<::TRTCVideoStreamType>(streamType), srcDataStr, static_cast<::TRTCWaterMarkSrcType>(srcType), nWidth, nHeight, xOffset, yOffset, fWidthRatio);
            delete[] srcDataStr;
            srcDataStr = NULL;
        }
    }

    void ITRTCCloud::selectScreenCaptureTarget(TRTCScreenCaptureSourceInfo^% source, RECT^% captureRect, TRTCScreenCaptureProperty^% property)
    {
        if (nullptr != m_impl)
        {
            ::TRTCScreenCaptureSourceInfo infoTemp;
            infoTemp.type = static_cast<::TRTCScreenCaptureSourceType>(source->type);
            infoTemp.sourceId = (HWND)source->sourceId.ToPointer();
            char* sourceName = Utils::StringToUTF8CharPtr(source->sourceName);
            infoTemp.sourceName = sourceName;
            ::TRTCImageBuffer thumbTemp, iconTemp;
            if (source->thumbBGRA != nullptr)
            {
                thumbTemp.width = source->thumbBGRA->width;
                thumbTemp.height = source->thumbBGRA->height;
                thumbTemp.length = source->thumbBGRA->length;
                pin_ptr<Byte> p = nullptr;
                if (source->thumbBGRA->length > 0)
                    pin_ptr<Byte> p = &source->thumbBGRA->buffer[0];
                thumbTemp.buffer = (const char*)p;
                infoTemp.thumbBGRA = thumbTemp;
            }
            if (source->iconBGRA != nullptr)
            {
                iconTemp.width = source->iconBGRA->width;
                iconTemp.height = source->iconBGRA->height;
                iconTemp.length = source->iconBGRA->length;
                pin_ptr<Byte> p = nullptr;
                if (source->iconBGRA->length > 0)
                    pin_ptr<Byte> p = &source->iconBGRA->buffer[0];
                iconTemp.buffer = (const char*)p;
                infoTemp.iconBGRA = iconTemp;
            }
            ::RECT rectTemp;
            if (captureRect == nullptr)
            {
                rectTemp.top = 0;
                rectTemp.left = 0;
                rectTemp.bottom = 0;
                rectTemp.right = 0;
            }
            else
            {
                rectTemp.top = captureRect->top;
                rectTemp.left = captureRect->left;
                rectTemp.bottom = captureRect->bottom;
                rectTemp.right = captureRect->right;
            }
            ::TRTCScreenCaptureProperty temp_property;
            temp_property.enableCaptureMouse = property->enableCaptureMouse;
            temp_property.enableHighPerformance = property->enableHighPerformance;
            temp_property.enableHighLight = property->enableHighLight;
            temp_property.highLightColor = property->highLightColor;
            temp_property.highLightWidth = property->highLightWidth;
            temp_property.enableCaptureChildWindow = property->enableCaptureChildWindow;
            m_impl->selectScreenCaptureTarget(infoTemp, rectTemp, temp_property);
            delete[] sourceName;
            sourceName = NULL;
        }
    }

    void ITRTCCloud::startRemoteSubStreamView(String^ userId, IntPtr rendHwnd)
    {
        if (nullptr != m_impl)
        {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->startRemoteSubStreamView(uid, (HWND)rendHwnd.ToPointer());
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::stopRemoteSubStreamView(String^ userId)
    {
        if (nullptr != m_impl)
        {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->stopRemoteSubStreamView(uid);
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::setRemoteSubStreamViewFillMode(String^ userId, TRTCVideoFillMode mode)
    {
        if (nullptr != m_impl)
        {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->setRemoteSubStreamViewFillMode(uid, static_cast<::TRTCVideoFillMode>(mode));
            delete[] uid;
            uid = NULL;
        }
    }

    void ITRTCCloud::setRemoteSubStreamViewRotation(String^ userId, TRTCVideoRotation rotation)
    {
        if (nullptr != m_impl)
        {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            m_impl->setRemoteSubStreamViewRotation(uid, static_cast<::TRTCVideoRotation>(rotation));
            delete[] uid;
            uid = NULL;
        }
    }

    ITRTCScreenCaptureSourceList^ ITRTCCloud::getScreenCaptureSources(SIZE^% thumbSize, SIZE^% iconSize)
    {
        if (nullptr != m_impl)
        {
            ::SIZE thumbSizeTemp, iconSizeTemp;
            if (nullptr != thumbSize)
            {
                thumbSizeTemp.cx = thumbSize->cx;
                thumbSizeTemp.cy = thumbSize->cy;
            }
            if (nullptr != iconSize)
            {
                iconSizeTemp.cx = iconSize->cx;
                iconSizeTemp.cy = iconSize->cy;
            }
            ::ITRTCScreenCaptureSourceList* info = m_impl->getScreenCaptureSources(thumbSizeTemp, iconSizeTemp);
            if (nullptr != info)
            {
                TRTCScreenCaptureSourceListImpl^ impl = gcnew TRTCScreenCaptureSourceListImpl();
                impl->setScreenCaptureSourceList(info);
                return impl;
            }
        }
        return nullptr;
    }

    void ITRTCCloud::selectScreenCaptureTarget(TRTCScreenCaptureSourceInfo^% source, RECT^% captureRect, bool captureMouse, bool highlightWindow)
    {

    }

    void ITRTCCloud::startScreenCapture(IntPtr rendHwnd)
    {
        if (nullptr != m_impl)
            m_impl->startScreenCapture((HWND)rendHwnd.ToPointer());
    }

    void ITRTCCloud::pauseScreenCapture()
    {
        if (nullptr != m_impl)
            m_impl->pauseScreenCapture();
    }

    void ITRTCCloud::resumeScreenCapture()
    {
        if (nullptr != m_impl)
            m_impl->resumeScreenCapture();
    }

    void ITRTCCloud::startScreenCapture(IntPtr rendView, TRTCVideoStreamType type, TRTCVideoEncParam^ params)
    {
        if (nullptr != m_impl) {

            if (params != nullptr)
            {
                ::TRTCVideoEncParam videoEncParamTemp;
                videoEncParamTemp.enableAdjustRes = params->enableAdjustRes;
                videoEncParamTemp.minVideoBitrate = params->minVideoBitrate;
                videoEncParamTemp.resMode = static_cast<::TRTCVideoResolutionMode>(params->resMode);
                videoEncParamTemp.videoBitrate = params->videoBitrate;
                videoEncParamTemp.videoFps = params->videoFps;
                videoEncParamTemp.videoResolution = static_cast<::TRTCVideoResolution>(params->videoResolution);

                m_impl->startScreenCapture((HWND)rendView.ToPointer(), static_cast<::TRTCVideoStreamType>(type), &videoEncParamTemp);
            }
            else
            {
                m_impl->startScreenCapture((HWND)rendView.ToPointer(), static_cast<::TRTCVideoStreamType>(type), nullptr);
            }

        }

    }

    void ITRTCCloud::stopScreenCapture()
    {
        if (nullptr != m_impl)
            m_impl->stopScreenCapture();
    }

    void ITRTCCloud::setSubStreamEncoderParam(TRTCVideoEncParam^% params)
    {
        if (nullptr != m_impl)
        {
            ::TRTCVideoEncParam temp;
            temp.resMode = static_cast<::TRTCVideoResolutionMode>(params->resMode);
            temp.videoBitrate = params->videoBitrate;
            temp.videoFps = params->videoFps;
            temp.videoResolution = static_cast<::TRTCVideoResolution>(params->videoResolution);
            m_impl->setSubStreamEncoderParam(temp);
        }
    }

    void ITRTCCloud::setSubStreamMixVolume(UInt32 volume)
    {
        if (nullptr != m_impl)
            m_impl->setSubStreamMixVolume(volume);
    }

    void ITRTCCloud::addExcludedShareWindow(IntPtr window) {
        if (nullptr != m_impl) {
            m_impl->addExcludedShareWindow((HWND)window.ToPointer());
        }
    }

    void ITRTCCloud::removeExcludedShareWindow(IntPtr window) {
        if (nullptr != m_impl) {
            m_impl->removeExcludedShareWindow((HWND)window.ToPointer());
        }
    }

    void ITRTCCloud::removeAllExcludedShareWindow() {
        if (nullptr != m_impl) {
            m_impl->removeAllExcludedShareWindow();
        }
    }

    void ITRTCCloud::addIncludedShareWindow(IntPtr window)
    {
        if (nullptr != m_impl)
        {
            m_impl->addIncludedShareWindow(static_cast<HWND>(window.ToPointer()));
        }
    }

    void ITRTCCloud::removeIncludedShareWindow(IntPtr window)
    {
        if (nullptr != m_impl)
        {
            m_impl->removeIncludedShareWindow(static_cast<HWND>(window.ToPointer()));
        }
    }

    void ITRTCCloud::removeAllIncludedShareWindow()
    {
        if (nullptr != m_impl)
        {
            m_impl->removeAllIncludedShareWindow();
        }
    }

    void ITRTCCloud::enableCustomVideoCapture(bool enable)
    {
        if (nullptr != m_impl)
            m_impl->enableCustomVideoCapture(enable);
    }

    void ITRTCCloud::enableCustomVideoCapture(TRTCVideoStreamType type, bool enable)
    {
        if (nullptr != m_impl)
            m_impl->enableCustomVideoCapture(static_cast<::TRTCVideoStreamType>(type), enable);
    }

    void ITRTCCloud::sendCustomVideoData(TRTCVideoFrame^ frame)
    {
        if (nullptr != m_impl)
        {
            ::TRTCVideoFrame* temp = new ::TRTCVideoFrame();
            temp->bufferType = static_cast<::TRTCVideoBufferType>(frame->bufferType);
            pin_ptr<System::Byte> p = &frame->data[0];
            unsigned char* pby = p;
            temp->data = reinterpret_cast<char*>(pby);
            temp->height = frame->height;
            temp->length = frame->length;
            temp->rotation = static_cast<::TRTCVideoRotation>(frame->rotation);
            temp->textureId = frame->textureId;
            temp->timestamp = frame->timestamp;
            temp->videoFormat = static_cast<::TRTCVideoPixelFormat>(frame->videoFormat);
            temp->width = frame->width;
            m_impl->sendCustomVideoData(temp);
            delete temp;
            temp = nullptr;
        }
    }

    void ITRTCCloud::sendCustomVideoData(TRTCVideoStreamType type, TRTCVideoFrame^ frame)
    {
        if (nullptr != m_impl)
        {
            ::TRTCVideoFrame* temp = new ::TRTCVideoFrame();
            temp->bufferType = static_cast<::TRTCVideoBufferType>(frame->bufferType);
            pin_ptr<System::Byte> p = &frame->data[0];
            unsigned char* pby = p;
            temp->data = reinterpret_cast<char*>(pby);
            temp->height = frame->height;
            temp->length = frame->length;
            temp->rotation = static_cast<::TRTCVideoRotation>(frame->rotation);
            temp->textureId = frame->textureId;
            temp->timestamp = frame->timestamp;
            temp->videoFormat = static_cast<::TRTCVideoPixelFormat>(frame->videoFormat);
            temp->width = frame->width;
            m_impl->sendCustomVideoData(static_cast<::TRTCVideoStreamType>(type), temp);
            delete temp;
            temp = nullptr;
        }
    }

    void ITRTCCloud::enableCustomAudioCapture(bool enable)
    {
        if (nullptr != m_impl)
            m_impl->enableCustomAudioCapture(enable);
    }

    void ITRTCCloud::sendCustomAudioData(TRTCAudioFrame^ frame)
    {
        if (nullptr != m_impl)
        {
            ::TRTCAudioFrame* temp = new ::TRTCAudioFrame();
            temp->audioFormat = static_cast<::TRTCAudioFrameFormat>(frame->audioFormat);
            temp->channel = frame->channel;
            pin_ptr<System::Byte> p = &frame->data[0];
            unsigned char* pby = p;
            temp->data = reinterpret_cast<char*>(pby);
            temp->length = frame->length;
            temp->sampleRate = frame->sampleRate;
            temp->timestamp = frame->timestamp;
            m_impl->sendCustomAudioData(temp);
            delete temp;
            temp = nullptr;
        }
    }

    int ITRTCCloud::setLocalVideoRenderCallback(TRTCVideoPixelFormat pixelFormat, TRTCVideoBufferType bufferType, ITRTCVideoRenderCallback^ callback)
    {
        if (nullptr != m_impl)
        {
            if (callback == nullptr)
            {
                msclr::lock l(m_videoRenderCallbacks);
                m_videoRenderCallbacks->Remove(String::Empty);
            }
            else
            {
                msclr::lock l(m_videoRenderCallbacks);
                if (!m_videoRenderCallbacks->ContainsKey(String::Empty))
                    m_videoRenderCallbacks->Add(String::Empty, callback);
            }

            if (nullptr != callback)
            {
                if (m_renderVideoFramedelegate == nullptr)
                    m_renderVideoFramedelegate = gcnew RenderVideoFrameDelegate(this, &ITRTCCloud::onRenderVideoFrame);
                m_videoRenderCallbackImpl->setRenderVideoFrame((PFN_RENDERVIDEOFRAME)Marshal::GetFunctionPointerForDelegate(m_renderVideoFramedelegate).ToPointer());

                return m_impl->setLocalVideoRenderCallback(static_cast<::TRTCVideoPixelFormat>(pixelFormat), static_cast<::TRTCVideoBufferType>(bufferType), m_videoRenderCallbackImpl);
            }
            else
            {
                return m_impl->setLocalVideoRenderCallback(static_cast<::TRTCVideoPixelFormat>(pixelFormat), static_cast<::TRTCVideoBufferType>(bufferType), nullptr);
            }
        }
        else
        {
            return -1;
        }
    }

    int ITRTCCloud::setLocalVideoProcessCallback(TRTCVideoPixelFormat pixelFormat, TRTCVideoBufferType bufferType, ITRTCVideoFrameCallback^ callback) {
        if (nullptr != m_impl) {
            m_videoFrameCallback = callback;
            if (nullptr == callback) {
                return m_impl->setLocalVideoProcessCallback(
                    static_cast<::TRTCVideoPixelFormat>(pixelFormat),
                    static_cast<::TRTCVideoBufferType>(bufferType),
                    nullptr
                );
            }

            if (nullptr == m_processVideoFramedelegate) {
                m_processVideoFramedelegate = gcnew ProcessVideoFrameDelegate(this, &ITRTCCloud::onProcessVideoFrame);
            }

            m_videoFrameCallbackImpl->setProcessVideoFrame(
                (PFN_PROCESSVIDEOFRAME)Marshal::GetFunctionPointerForDelegate(m_processVideoFramedelegate).ToPointer()
            );

            return m_impl->setLocalVideoProcessCallback(
                static_cast<::TRTCVideoPixelFormat>(pixelFormat),
                static_cast<::TRTCVideoBufferType>(bufferType),
                m_videoFrameCallbackImpl
            );
        }
        else
        {
            return -1;
        }
    }

    int ITRTCCloud::setRemoteVideoRenderCallback(String^ userId, TRTCVideoPixelFormat pixelFormat, TRTCVideoBufferType bufferType, ITRTCVideoRenderCallback^ callback)
    {
        if (nullptr != m_impl)
        {
            if (callback == nullptr)
            {
                msclr::lock l(m_videoRenderCallbacks);
                m_videoRenderCallbacks->Remove(userId);
            }
            else
            {
                msclr::lock l(m_videoRenderCallbacks);
                if (!m_videoRenderCallbacks->ContainsKey(userId))
                    m_videoRenderCallbacks->Add(userId, callback);
            }
            char* uid = Utils::StringToUTF8CharPtr(userId);
            int result = 0;
            if (nullptr != callback)
            {
                if (m_renderVideoFramedelegate == nullptr)
                    m_renderVideoFramedelegate = gcnew RenderVideoFrameDelegate(this, &ITRTCCloud::onRenderVideoFrame);
                m_videoRenderCallbackImpl->setRenderVideoFrame((PFN_RENDERVIDEOFRAME)Marshal::GetFunctionPointerForDelegate(m_renderVideoFramedelegate).ToPointer());

                result = m_impl->setRemoteVideoRenderCallback(uid, static_cast<::TRTCVideoPixelFormat>(pixelFormat), static_cast<::TRTCVideoBufferType>(bufferType), m_videoRenderCallbackImpl);
            }
            else
            {
                result = m_impl->setRemoteVideoRenderCallback(uid, static_cast<::TRTCVideoPixelFormat>(pixelFormat), static_cast<::TRTCVideoBufferType>(bufferType), nullptr);
            }
            delete[] uid;
            uid = NULL;
            return result;
        }
        else
        {
            return -1;
        }
    }

    int ITRTCCloud::setAudioFrameCallback(ITRTCAudioFrameCallback^ callback)
    {
        if (m_impl)
        {
            m_audioFrameCallback = callback;
            if (callback == nullptr)
            {
                return m_impl->setAudioFrameCallback(nullptr);
            }
            if (m_capturedAudioFrameDelegate == nullptr)
                m_capturedAudioFrameDelegate = gcnew CapturedAudioFrameDelegate(this, &ITRTCCloud::onCapturedRawAudioFrame);
            m_audioFrameCallbackImpl->setCapturedAudioFrame((PFN_CAPTUREDAUDIOFRAME)Marshal::GetFunctionPointerForDelegate(m_capturedAudioFrameDelegate).ToPointer());
            if (m_playAudioFrameDelegate == nullptr)
                m_playAudioFrameDelegate = gcnew PlayAudioFrameDelegate(this, &ITRTCCloud::onPlayAudioFrame);
            m_audioFrameCallbackImpl->setPlayAudioFrame((PFN_PLAYAUDIOFRAME)Marshal::GetFunctionPointerForDelegate(m_playAudioFrameDelegate).ToPointer());
            if (m_mixedPlayAudioFrameDelegate == nullptr)
                m_mixedPlayAudioFrameDelegate = gcnew MixedPlayAudioFrameDelegate(this, &ITRTCCloud::onMixedPlayAudioFrame);
            m_audioFrameCallbackImpl->setMixedPlayAudioFrame((PFN_MIXEDPLAYAUDIOFRAME)Marshal::GetFunctionPointerForDelegate(m_mixedPlayAudioFrameDelegate).ToPointer());

            if (nullptr == m_localProcessedAudioFrameDelegate) {
                m_localProcessedAudioFrameDelegate = gcnew CapturedAudioFrameDelegate(this, &ITRTCCloud::onLocalProcessedAudioFrame);
            }
            m_audioFrameCallbackImpl->setLocalProcessedAudioFrame(
                (PFN_CAPTUREDAUDIOFRAME)Marshal::GetFunctionPointerForDelegate(m_localProcessedAudioFrameDelegate).ToPointer()
            );

            return m_impl->setAudioFrameCallback(m_audioFrameCallbackImpl);
        }
        else
        {
            return -1;
        }
    }

    int ITRTCCloud::setCapturedRawAudioFrameCallbackFormat(TRTCAudioFrameCallbackFormat^ format)
    {
        if (m_impl)
        {
            ::TRTCAudioFrameCallbackFormat temp;
            temp.channel = format->channel;
            temp.sampleRate = format->sampleRate;
            temp.samplesPerCall = format->samplesPerCall;
            return m_impl->setCapturedRawAudioFrameCallbackFormat(&temp);
        }
        else
        {
            return -1;
        }
    }

    int ITRTCCloud::setLocalProcessedAudioFrameCallbackFormat(TRTCAudioFrameCallbackFormat^ format)
    {
        if (m_impl) {
            ::TRTCAudioFrameCallbackFormat temp;
            temp.channel = format->channel;
            temp.sampleRate = format->sampleRate;
            temp.samplesPerCall = format->samplesPerCall;
            return m_impl->setLocalProcessedAudioFrameCallbackFormat(&temp);
        }
        else
        {
            return -1;
        }
    }

    int ITRTCCloud::setMixedPlayAudioFrameCallbackFormat(TRTCAudioFrameCallbackFormat^ format)
    {
        if (m_impl) {
            ::TRTCAudioFrameCallbackFormat temp;
            temp.channel = format->channel;
            temp.sampleRate = format->sampleRate;
            temp.samplesPerCall = format->samplesPerCall;
            return m_impl->setMixedPlayAudioFrameCallbackFormat(&temp);
        }
        else
        {
            return -1;
        }
    }

    void ITRTCCloud::enableMixExternalAudioFrame(bool enablePublish, bool enablePlayout)
    {
        if (m_impl)
        {
            m_impl->enableMixExternalAudioFrame(enablePublish, enablePlayout);
        }
    }

    int ITRTCCloud::mixExternalAudioFrame(TRTCAudioFrame^ frame)
    {
        if (m_impl)
        {
            ::TRTCAudioFrame* temp = new ::TRTCAudioFrame();
            temp->audioFormat = static_cast<::TRTCAudioFrameFormat>(frame->audioFormat);
            temp->channel = frame->channel;
            pin_ptr<System::Byte> p = &frame->data[0];
            unsigned char* pby = p;
            temp->data = reinterpret_cast<char*>(pby);
            temp->length = frame->length;
            temp->sampleRate = frame->sampleRate;
            temp->timestamp = frame->timestamp;
            int ret = m_impl->mixExternalAudioFrame(temp);
            delete temp;
            temp = nullptr;
            return ret;
        }
        else
        {
            return -1;
        }
    }

    void ITRTCCloud::setMixExternalAudioVolume(int publishVolume, int playoutVolume) {
        if (m_impl) {
            m_impl->setMixExternalAudioVolume(publishVolume, playoutVolume);
        }
    }


    UInt64 ITRTCCloud::generateCustomPTS()
    {
        if (m_impl != nullptr)
        {
            return m_impl->generateCustomPTS();
        }
        return -1;
    }

    void ITRTCCloud::enableCustomAudioRendering(bool enable) {
        if (m_impl != nullptr) {
            m_impl->enableCustomAudioRendering(enable);
        }
    }

    void ITRTCCloud::getCustomAudioRenderingFrame(TRTCAudioFrame^ audioFrame) {
        if (nullptr != m_impl) {
            ::TRTCAudioFrame frame;
            frame.channel = audioFrame->channel;
            frame.length = audioFrame->length;
            pin_ptr<unsigned char> p = &audioFrame->data[0];
            frame.data = (char*)p;
            m_impl->getCustomAudioRenderingFrame(&frame);
            audioFrame->audioFormat = static_cast<TRTCAudioFrameFormat>(frame.audioFormat);
            audioFrame->channel = frame.channel;
            audioFrame->length = frame.length;
            audioFrame->sampleRate = frame.sampleRate;
            audioFrame->timestamp = frame.timestamp;
        }
    }

    bool ITRTCCloud::sendCustomCmdMsg(UInt32 cmdId, array<Byte>^ data, UInt32 dataSize, bool reliable, bool ordered)
    {
        if (nullptr != m_impl)
        {
            pin_ptr<Byte> p = &data[0];
            return m_impl->sendCustomCmdMsg(cmdId, p, dataSize, reliable, ordered);
        }
        else
        {
            return false;
        }
    }

    bool ITRTCCloud::sendSEIMsg(array<Byte>^ data, Int32 dataSize, Int32 repeatCount)
    {
        if (nullptr != m_impl)
        {
            pin_ptr<Byte> p = &data[0];
            return m_impl->sendSEIMsg(p, dataSize, repeatCount);
        }
        else
        {
            return false;
        }
    }

    void ITRTCCloud::playBGM(String^ path)
    {
        if (nullptr != m_impl)
        {
            char* pathStr = Utils::StringToUTF8CharPtr(path);
            m_impl->playBGM(pathStr);
            delete[] pathStr;
            pathStr = NULL;
        }
    }

    void ITRTCCloud::stopBGM()
    {
        if (nullptr != m_impl)
            m_impl->stopBGM();
    }

    void ITRTCCloud::pauseBGM()
    {
        if (nullptr != m_impl)
            m_impl->pauseBGM();
    }

    void ITRTCCloud::resumeBGM()
    {
        if (nullptr != m_impl)
            m_impl->resumeBGM();
    }

    UInt32 ITRTCCloud::getBGMDuration(String^ path)
    {
        if (nullptr != m_impl)
        {
            char* pathStr = Utils::StringToUTF8CharPtr(path);
            UInt32 duration = m_impl->getBGMDuration(pathStr);
            delete[] pathStr;
            pathStr = NULL;
            return duration;
        }
        else
        {
            return -1;
        }
    }

    void ITRTCCloud::setBGMPosition(UInt32 pos)
    {
        if (nullptr != m_impl)
            m_impl->setBGMPosition(pos);
    }

    void ITRTCCloud::setMicVolumeOnMixing(UInt32 volume)
    {
        if (nullptr != m_impl)
            m_impl->setMicVolumeOnMixing(volume);
    }

    void ITRTCCloud::setBGMVolume(UInt32 volume)
    {
        if (nullptr != m_impl)
            m_impl->setBGMVolume(volume);
    }

    void ITRTCCloud::setBGMPlayoutVolume(UInt32 volume)
    {
        if (nullptr != m_impl)
            m_impl->setBGMPlayoutVolume(volume);
    }

    void ITRTCCloud::setBGMPublishVolume(UInt32 volume)
    {
        if (nullptr != m_impl)
            m_impl->setBGMPublishVolume(volume);
    }

    ITXAudioEffectManager^ ITRTCCloud::getAudioEffectManager()
    {
        return m_audioEffectManager;
    }

    void ITRTCCloud::startSystemAudioLoopback(String^ path)
    {
        if (nullptr != m_impl)
        {
            char* pathStr = Utils::StringToUTF8CharPtr(path);
            m_impl->startSystemAudioLoopback(pathStr);
            delete[] pathStr;
            pathStr = NULL;
        }
    }

    void ITRTCCloud::stopSystemAudioLoopback()
    {
        if (nullptr != m_impl)
            m_impl->stopSystemAudioLoopback();
    }

    void ITRTCCloud::setSystemAudioLoopbackVolume(UInt32 volume)
    {
        if (nullptr != m_impl)
            m_impl->setSystemAudioLoopbackVolume(volume);
    }

    void ITRTCCloud::playAudioEffect(TRTCAudioEffectParam^ effect)
    {
        if (nullptr != m_impl)
        {
            if (effect == nullptr) return;
            char* path = Utils::StringToUTF8CharPtr(effect->path);
            ::TRTCAudioEffectParam* param = new ::TRTCAudioEffectParam(effect->effectId, path);
            param->loopCount = effect->loopCount;
            param->publish = effect->publish;
            param->volume = effect->volume;
            m_impl->playAudioEffect(param);
            delete[] path;
            path = NULL;
        }
    }

    void ITRTCCloud::setAudioEffectVolume(int effectId, int volume)
    {
        if (nullptr != m_impl)
        {
            m_impl->setAudioEffectVolume(effectId, volume);
        }
    }

    void ITRTCCloud::stopAudioEffect(int effectId)
    {
        if (nullptr != m_impl)
        {
            m_impl->stopAudioEffect(effectId);
        }
    }

    void ITRTCCloud::stopAllAudioEffects()
    {
        if (nullptr != m_impl)
        {
            m_impl->stopAllAudioEffects();
        }
    }

    void ITRTCCloud::setAllAudioEffectsVolume(int volume)
    {
        if (nullptr != m_impl)
        {
            m_impl->setAllAudioEffectsVolume(volume);
        }
    }

    void ITRTCCloud::pauseAudioEffect(int effectId)
    {
        if (nullptr != m_impl)
        {
            m_impl->pauseAudioEffect(effectId);
        }
    }

    void ITRTCCloud::resumeAudioEffect(int effectId)
    {
        if (nullptr != m_impl)
        {
            m_impl->resumeAudioEffect(effectId);
        }
    }

    void ITRTCCloud::startSpeedTest(UInt32 sdkAppId, String^ userId, String^ userSig)
    {
        if (nullptr != m_impl)
        {
            char* uid = Utils::StringToUTF8CharPtr(userId);
            char* uSig = Utils::StringToUTF8CharPtr(userSig);
            m_impl->startSpeedTest(sdkAppId, uid, uSig);
            delete[] uid;
            uid = NULL;
            delete[] uSig;
            uSig = NULL;
        }
    }

    int ITRTCCloud::startSpeedTest(TRTCSpeedTestParams^ params) {
        if (nullptr != m_impl) {
            char* uid = Utils::StringToUTF8CharPtr(params->userId);
            char* uSig = Utils::StringToUTF8CharPtr(params->userSig);

            ::TRTCSpeedTestParams tmp;
            tmp.sdkAppId = params->sdkAppId;
            tmp.userId = uid;
            tmp.userSig = uSig;
            tmp.expectedUpBandwidth = params->expectedUpBandwidth;
            tmp.expectedDownBandwidth = params->expectedDownBandwidth;
            int ret = m_impl->startSpeedTest(tmp);
            delete[] uid;
            delete[] uSig;
            uid = NULL;
            uSig = NULL;
            return ret;
        }
        return -1;
    }


    void ITRTCCloud::stopSpeedTest()
    {
        if (nullptr != m_impl)
            m_impl->stopSpeedTest();
    }

    void ITRTCCloud::startCameraDeviceTest(IntPtr rendHwnd)
    {
        if (nullptr != m_impl)
            m_impl->startCameraDeviceTest((HWND)rendHwnd.ToPointer());
    }

    void ITRTCCloud::stopCameraDeviceTest()
    {
        if (nullptr != m_impl)
            m_impl->stopCameraDeviceTest();
    }

    void ITRTCCloud::startMicDeviceTest(UInt32 interval)
    {
        if (nullptr != m_impl)
            m_impl->startMicDeviceTest(interval);
    }

    void ITRTCCloud::stopMicDeviceTest()
    {
        if (nullptr != m_impl)
            m_impl->stopMicDeviceTest();
    }

    void ITRTCCloud::startSpeakerDeviceTest(String^ testAudioFilePath)
    {
        if (nullptr != m_impl)
        {
            char* pathStr = Utils::StringToUTF8CharPtr(testAudioFilePath);
            m_impl->startSpeakerDeviceTest(pathStr);
            delete[] pathStr;
            pathStr = NULL;
        }
    }

    void ITRTCCloud::stopSpeakerDeviceTest()
    {
        if (nullptr != m_impl)
            m_impl->stopSpeakerDeviceTest();
    }

    void ITRTCCloud::setMixTranscodingConfig(TRTCTranscodingConfig^ config)
    {
        if (nullptr != m_impl)
        {
            if (config == nullptr)
            {
                m_impl->setMixTranscodingConfig(nullptr);
                return;
            }
            ::TRTCTranscodingConfig* temp = new ::TRTCTranscodingConfig();
            temp->appId = config->appId;
            temp->audioBitrate = config->audioBitrate;
            temp->audioChannels = config->audioChannels;
            temp->audioSampleRate = config->audioSampleRate;
            temp->bizId = config->bizId;
            temp->mode = static_cast<::TRTCTranscodingConfigMode>(config->mode);
            temp->videoBitrate = config->videoBitrate;
            temp->videoFramerate = config->videoFramerate;
            temp->videoGOP = config->videoGOP;
            temp->videoHeight = config->videoHeight;
            temp->videoWidth = config->videoWidth;
            temp->backgroundColor = config->backgroundColor;
            char* bg_image = Utils::StringToUTF8CharPtr(config->backgroundImage);
            char* streamId = Utils::StringToUTF8CharPtr(config->streamId);
            temp->backgroundImage = bg_image;
            temp->streamId = streamId;
            temp->mixUsersArraySize = config->mixUsersArraySize;
            ::TRTCMixUser* arr = new ::TRTCMixUser[config->mixUsersArraySize];
            array<TRTCMixUser^>^ array = config->mixUsersArray;
            for (unsigned int i = 0; i < config->mixUsersArraySize; i++) {
                arr[i].userId = Utils::StringToUTF8CharPtr(array[i]->userId);
                arr[i].roomId = Utils::StringToUTF8CharPtr(array[i]->roomId);
                ::RECT rectTemp;
                rectTemp.bottom = array[i]->rect->bottom;
                rectTemp.right = array[i]->rect->right;
                rectTemp.left = array[i]->rect->left;
                rectTemp.top = array[i]->rect->top;
                arr[i].rect = rectTemp;
                arr[i].zOrder = array[i]->zOrder;
                arr[i].pureAudio = array[i]->pureAudio;
                arr[i].streamType = static_cast<::TRTCVideoStreamType>(array[i]->streamType);
            }
            temp->mixUsersArray = arr;
            m_impl->setMixTranscodingConfig(temp);
            for (unsigned int i = 0; i < config->mixUsersArraySize; i++) {
                delete[] arr[i].userId;
                arr[i].userId = nullptr;
                delete[] arr[i].roomId;
                arr[i].roomId = nullptr;
            }
            delete[] arr;
            arr = nullptr;
            delete temp;
            temp = nullptr;
            delete[] bg_image;
            bg_image = nullptr;
            delete[] streamId;
            streamId = nullptr;
        }
    }

    void ITRTCCloud::startPublishing(String^ streamId, TRTCVideoStreamType type)
    {
        if (nullptr != m_impl)
        {
            char* sid = Utils::StringToUTF8CharPtr(streamId);
            m_impl->startPublishing(sid, static_cast<::TRTCVideoStreamType>(type));
            delete[] sid;
            sid = NULL;
        }
    }

    void ITRTCCloud::stopPublishing()
    {
        if (nullptr != m_impl)
            m_impl->stopPublishing();
    }

    void ITRTCCloud::startPublishCDNStream(TRTCPublishCDNParam^% param)
    {
        if (nullptr != m_impl)
        {
            ::TRTCPublishCDNParam temp;
            temp.appId = param->appId;
            temp.bizId = param->bizId;
            char* url = Utils::StringToUTF8CharPtr(param->url);
            temp.url = url;
            m_impl->startPublishCDNStream(temp);
            delete[] url;
            url = NULL;
        }
    }

    void ITRTCCloud::stopPublishCDNStream()
    {
        if (nullptr != m_impl)
            m_impl->stopPublishCDNStream();
    }

    String^ ITRTCCloud::getSDKVersion()
    {
        if (nullptr != m_impl)
            return Utils::CharPtrToString(m_impl->getSDKVersion());
        else
            return nullptr;
    }

    void ITRTCCloud::setLogLevel(TRTCLogLevel level)
    {
        if (nullptr != m_impl)
            m_impl->setLogLevel(static_cast<::TRTCLogLevel>(level));
    }

    void ITRTCCloud::setConsoleEnabled(bool enabled)
    {
        if (nullptr != m_impl)
            m_impl->setConsoleEnabled(enabled);
    }

    void ITRTCCloud::setLogCompressEnabled(bool enabled)
    {
        if (nullptr != m_impl)
            m_impl->setLogCompressEnabled(enabled);
    }

    void ITRTCCloud::setLogDirPath(String^ path)
    {
        if (nullptr != m_impl)
        {
            char* pathStr = Utils::StringToUTF8CharPtr(path);
            m_impl->setLogDirPath(pathStr);
            delete[] pathStr;
            pathStr = nullptr;
        }
    }

    void ITRTCCloud::setLogCallback(ITRTCLogCallback^ callback)
    {
        if (nullptr != m_impl)
        {
            if (callback != nullptr)
            {
                m_logCallback = callback;
                if (m_logDeleagte == nullptr)
                    m_logDeleagte = gcnew LogDelegate(this, &ITRTCCloud::onLog);
                m_logCallbackImpl->setLog((PFN_LOG)Marshal::GetFunctionPointerForDelegate(m_logDeleagte).ToPointer());
                m_impl->setLogCallback(m_logCallbackImpl);
            }
            else
            {
                m_logCallback = nullptr;
                m_logDeleagte = nullptr;
                m_logCallbackImpl->setLog(nullptr);
                m_impl->setLogCallback(nullptr);
            }
        }
    }

    void ITRTCCloud::showDebugView(int showType)
    {
        if (nullptr != m_impl)
            m_impl->showDebugView(showType);
    }

    void ITRTCCloud::callExperimentalAPI(String^ jsonStr)
    {
        if (nullptr != m_impl)
        {
            char* json = Utils::StringToUTF8CharPtr(jsonStr);
            m_impl->callExperimentalAPI(json);
            delete[] json;
            json = NULL;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      TRTCCloudCallback 接口转换
    //
    /////////////////////////////////////////////////////////////////////////////////

    void ITRTCCloud::onError(::TXLiteAVError errCode, const char* errMsg, void* arg)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;

        TXLiteAVError err_code = static_cast<TXLiteAVError>(errCode);

        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onError(err_code, Utils::CharPtrToString(errMsg), IntPtr(arg));
        }
    }

    void ITRTCCloud::onWarning(::TXLiteAVWarning warningCode, const char* warningMsg, void* arg)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        TXLiteAVWarning warning_code = static_cast<TXLiteAVWarning>(warningCode);

        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onWarning(warning_code, Utils::CharPtrToString(warningMsg), IntPtr(arg));
        }
    }

    void ITRTCCloud::onEnterRoom(int result)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onEnterRoom(result);
        }
    }

    void ITRTCCloud::onExitRoom(int reason)
    {
        if (m_video_frame_cache != nullptr)
        {
            msclr::lock l(m_video_frame_cache);
            m_video_frame_cache->Clear();
        }
        if (m_audio_frame_cache != nullptr)
        {
            msclr::lock l(m_audio_frame_cache);
            m_audio_frame_cache->Clear();
        }
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onExitRoom(reason);
        }
    }

    void ITRTCCloud::onSwitchRole(::TXLiteAVError errCode, const char* errMsg)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        TXLiteAVError err_code = static_cast<TXLiteAVError>(errCode);

        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onSwitchRole(err_code, Utils::CharPtrToString(errMsg));
        }
    }

    void ITRTCCloud::onConnectOtherRoom(const char* userId, ::TXLiteAVError errCode, const char* errMsg)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        TXLiteAVError err_code = static_cast<TXLiteAVError>(errCode);

        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onConnectOtherRoom(Utils::CharPtrToString(userId), err_code, Utils::CharPtrToString(errMsg));
        }
    }

    void ITRTCCloud::onDisconnectOtherRoom(::TXLiteAVError errCode, const char* errMsg)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        TXLiteAVError err_code = static_cast<TXLiteAVError>(errCode);

        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onDisconnectOtherRoom(err_code, Utils::CharPtrToString(errMsg));
        }
    }

    void ITRTCCloud::onSwitchRoom(::TXLiteAVError errCode, const char* errMsg) {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        TXLiteAVError err_code = static_cast<TXLiteAVError>(errCode);

        for (int i = 0; i < count; i++) {
            if (list[i] != nullptr)
                list[i]->onSwitchRoom(err_code, Utils::CharPtrToString(errMsg));
        }
    }

    void ITRTCCloud::onUserEnter(const char* userId)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onUserEnter(Utils::CharPtrToString(userId));
        }
    }

    void ITRTCCloud::onUserExit(const char* userId, int reason)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onUserExit(Utils::CharPtrToString(userId), reason);
        }
    }

    void ITRTCCloud::onRemoteUserEnterRoom(const char* userId)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onRemoteUserEnterRoom(Utils::CharPtrToString(userId));
        }
    }

    void ITRTCCloud::onRemoteUserLeaveRoom(const char* userId, int reason)
    {
        String^ key = Utils::CharPtrToString(userId);
        if (m_video_frame_cache != nullptr)
        {
            msclr::lock l(m_video_frame_cache);
            m_video_frame_cache->Remove(key + TRTCVideoStreamType::TRTCVideoStreamTypeBig.ToString());
            m_video_frame_cache->Remove(key + TRTCVideoStreamType::TRTCVideoStreamTypeSub.ToString());
        }
        if (m_audio_frame_cache != nullptr)
        {
            msclr::lock l(m_audio_frame_cache);
            m_audio_frame_cache->Remove(key);
        }
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onRemoteUserLeaveRoom(Utils::CharPtrToString(userId), reason);
        }
    }

    void ITRTCCloud::onUserVideoAvailable(const char* userId, bool available)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onUserVideoAvailable(Utils::CharPtrToString(userId), available);
        }
    }

    void ITRTCCloud::onUserSubStreamAvailable(const char* userId, bool available)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onUserSubStreamAvailable(Utils::CharPtrToString(userId), available);
        }
    }

    void ITRTCCloud::onUserAudioAvailable(const char* userId, bool available)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onUserAudioAvailable(Utils::CharPtrToString(userId), available);
        }
    }

    void ITRTCCloud::onUserVoiceVolume(liteav::TRTCVolumeInfo* userVolumes, unsigned int userVolumesCount, unsigned int totalVolume)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        array<TRTCVolumeInfo^>^ arr;
        if (userVolumesCount <= 0)
        {
            arr = gcnew array<TRTCVolumeInfo^>(0);
        }
        else
        {
            arr = gcnew array<TRTCVolumeInfo^>(userVolumesCount);
            for (unsigned int i = 0; i < userVolumesCount; i++)
            {
                TRTCVolumeInfo^ temp = gcnew TRTCVolumeInfo();
                temp->userId = Utils::CharPtrToString(userVolumes[i].userId);
                temp->volume = userVolumes[i].volume;
                arr[i] = temp;
            }
        }
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onUserVoiceVolume(arr, userVolumesCount, totalVolume);
        }
    }

    void ITRTCCloud::onNetworkQuality(liteav::TRTCQualityInfo localQuality, ::TRTCQualityInfo* remoteQuality, unsigned int remoteQualityCount)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        TRTCQualityInfo^ temp = gcnew TRTCQualityInfo();
        temp->userId = Utils::CharPtrToString(localQuality.userId);
        array<TRTCQualityInfo^>^ arr;
        temp->quality = static_cast<TRTCQuality>(localQuality.quality);
        if (remoteQualityCount <= 0)
        {
            arr = gcnew array<TRTCQualityInfo^>(0);
        }
        else
        {
            arr = gcnew array<TRTCQualityInfo^>(remoteQualityCount);
            for (unsigned int i = 0; i < remoteQualityCount; i++)
            {
                TRTCQualityInfo^ info = gcnew TRTCQualityInfo();
                info->userId = Utils::CharPtrToString(remoteQuality[i].userId);
                info->quality = static_cast<TRTCQuality>(remoteQuality[i].quality);
                arr[i] = info;
            }
        }
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onNetworkQuality(temp, arr, remoteQualityCount);
        }
    }

    void ITRTCCloud::onStatistics(const liteav::TRTCStatistics& statis)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        TRTCStatistics^ temp = gcnew TRTCStatistics();
        temp->upLoss = statis.upLoss;
        temp->downLoss = statis.downLoss;
        temp->appCpu = statis.appCpu;
        temp->systemCpu = statis.systemCpu;
        temp->rtt = statis.rtt;
        temp->receivedBytes = statis.receivedBytes;
        temp->sentBytes = statis.sentBytes;
        temp->localStatisticsArraySize = statis.localStatisticsArraySize;
        array<TRTCLocalStatistics^>^ localArr;
        if (statis.localStatisticsArraySize <= 0)
        {
            localArr = gcnew array<TRTCLocalStatistics^>(0);
        }
        else
        {
            localArr = gcnew array<TRTCLocalStatistics^>(statis.localStatisticsArraySize);
            ::TRTCLocalStatistics* localPtr = statis.localStatisticsArray;
            for (unsigned int i = 0; i < statis.localStatisticsArraySize; i++)
            {
                TRTCLocalStatistics^ t = gcnew TRTCLocalStatistics();
                t->audioBitrate = localPtr[i].audioBitrate;
                t->audioSampleRate = localPtr[i].audioSampleRate;
                t->frameRate = localPtr[i].frameRate;
                t->height = localPtr[i].height;
                t->streamType = static_cast<TRTCVideoStreamType>(localPtr[i].streamType);
                t->videoBitrate = localPtr[i].videoBitrate;
                t->width = localPtr[i].width;
                localArr[i] = t;
            }
        }
        temp->localStatisticsArray = localArr;
        temp->remoteStatisticsArraySize = statis.remoteStatisticsArraySize;
        array<TRTCRemoteStatistics^>^ remoteArr;
        if (statis.remoteStatisticsArraySize <= 0)
        {
            remoteArr = gcnew array<TRTCRemoteStatistics^>(0);
        }
        else
        {
            remoteArr = gcnew array<TRTCRemoteStatistics^>(statis.remoteStatisticsArraySize);
            ::TRTCRemoteStatistics* remotePtr = statis.remoteStatisticsArray;
            for (unsigned int i = 0; i < statis.remoteStatisticsArraySize; i++)
            {
                TRTCRemoteStatistics^ t = gcnew TRTCRemoteStatistics();
                t->userId = Utils::CharPtrToString(remotePtr[i].userId);
                t->finalLoss = remotePtr[i].finalLoss;
                t->audioBitrate = remotePtr[i].audioBitrate;
                t->audioSampleRate = remotePtr[i].audioSampleRate;
                t->frameRate = remotePtr[i].frameRate;
                t->height = remotePtr[i].height;
                t->streamType = static_cast<TRTCVideoStreamType>(remotePtr[i].streamType);
                t->videoBitrate = remotePtr[i].videoBitrate;
                t->width = remotePtr[i].width;
                remoteArr[i] = t;
            }
        }
        temp->remoteStatisticsArray = remoteArr;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onStatistics(temp);
        }
    }

    void ITRTCCloud::onFirstVideoFrame(const char* userId, const liteav::TRTCVideoStreamType streamType, const int width, const int height)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        TRTCVideoStreamType stream_type = static_cast<TRTCVideoStreamType>(streamType);

        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onFirstVideoFrame(Utils::CharPtrToString(userId), stream_type, width, height);
        }
    }

    void ITRTCCloud::onFirstAudioFrame(const char* userId)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onFirstAudioFrame(Utils::CharPtrToString(userId));
        }
    }

    void ITRTCCloud::onSendFirstLocalVideoFrame(const liteav::TRTCVideoStreamType streamType)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        TRTCVideoStreamType stream_type = static_cast<TRTCVideoStreamType>(streamType);

        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onSendFirstLocalVideoFrame(stream_type);
        }
    }

    void ITRTCCloud::onSendFirstLocalAudioFrame()
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onSendFirstLocalAudioFrame();
        }
    }

    void ITRTCCloud::onPlayBGMBegin(::TXLiteAVError errCode)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        TXLiteAVError err_code = static_cast<TXLiteAVError>(errCode);

        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onPlayBGMBegin(err_code);
        }
    }

    void ITRTCCloud::onPlayBGMProgress(unsigned int progressMS, unsigned int durationMS)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onPlayBGMProgress(progressMS, durationMS);
        }
    }

    void ITRTCCloud::onPlayBGMComplete(::TXLiteAVError errCode)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        TXLiteAVError err_code = static_cast<TXLiteAVError>(errCode);

        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onPlayBGMComplete(err_code);
        }
    }

    void ITRTCCloud::onConnectionLost()
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onConnectionLost();
        }
    }

    void ITRTCCloud::onTryToReconnect()
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onTryToReconnect();
        }
    }

    void ITRTCCloud::onConnectionRecovery()
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onConnectionRecovery();
        }
    }

    void ITRTCCloud::onSpeedTest(const liteav::TRTCSpeedTestResult& currentResult, unsigned int finishedCount, unsigned int totalCount)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        if (m_callbacks->Count == 0) return;
        TRTCSpeedTestResult^ temp = gcnew TRTCSpeedTestResult();
        temp->downLostRate = currentResult.downLostRate;
        temp->ip = Utils::CharPtrToString(currentResult.ip);
        temp->quality = static_cast<TRTCQuality>(currentResult.quality);
        temp->rtt = currentResult.rtt;
        temp->upLostRate = currentResult.upLostRate;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onSpeedTest(temp, finishedCount, totalCount);
        }
    }

    void ITRTCCloud::onCameraDidReady()
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onCameraDidReady();
        }
    }

    void ITRTCCloud::onMicDidReady()
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onMicDidReady();
        }
    }

    void ITRTCCloud::onDeviceChange(const char* deviceId, liteav::TRTCDeviceType type, liteav::TRTCDeviceState state)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        TRTCDeviceType device_type = static_cast<TRTCDeviceType>(type);
        TRTCDeviceState device_state = static_cast<TRTCDeviceState>(state);

        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onDeviceChange(Utils::CharPtrToString(deviceId), device_type, device_state);
        }
    }

    void ITRTCCloud::onTestMicVolume(unsigned int volume)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onTestMicVolume(volume);
        }
    }

    void ITRTCCloud::onTestSpeakerVolume(unsigned int volume)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onTestSpeakerVolume(volume);
        }
    }

    void ITRTCCloud::onAudioDeviceCaptureVolumeChanged(unsigned int volume, bool muted) {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++) {
            if (list[i] != nullptr)
                list[i]->onAudioDeviceCaptureVolumeChanged(volume, muted);
        }
    }

    void ITRTCCloud::onAudioDevicePlayoutVolumeChanged(unsigned int volume, bool muted) {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++) {
            if (list[i] != nullptr)
                list[i]->onAudioDevicePlayoutVolumeChanged(volume, muted);
        }
    }

    void ITRTCCloud::onRecvCustomCmdMsg(const char* userId, int cmdId, unsigned int seq, const unsigned char* msg, unsigned int msgSize)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        array<Byte>^ arr;
        if (msgSize <= 0)
            arr = gcnew array<Byte>(0);
        arr = gcnew array<Byte>(msgSize);
        for (unsigned int i = 0; i < msgSize; i++)
        {
            arr[i] = msg[i];
        }
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onRecvCustomCmdMsg(Utils::CharPtrToString(userId), cmdId, seq, arr, msgSize);
        }
    }

    void ITRTCCloud::onMissCustomCmdMsg(const char* userId, int cmdId, int errCode, int missed)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onMissCustomCmdMsg(Utils::CharPtrToString(userId), cmdId, errCode, missed);
        }
    }

    void ITRTCCloud::onRecvSEIMsg(const char* userId, const unsigned char* message, unsigned int msgSize)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        array<Byte>^ arr;
        if (msgSize <= 0)
            arr = gcnew array<Byte>(0);
        arr = gcnew array<Byte>(msgSize);
        for (unsigned int i = 0; i < msgSize; i++)
        {
            arr[i] = message[i];
        }
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onRecvSEIMsg(Utils::CharPtrToString(userId), arr, msgSize);
        }
    }

    void ITRTCCloud::onStartPublishing(int errCode, const char* errMsg)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onStartPublishing(errCode, Utils::CharPtrToString(errMsg));
        }
    }

    void ITRTCCloud::onStopPublishing(int errCode, const char* errMsg)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onStopPublishing(errCode, Utils::CharPtrToString(errMsg));
        }
    }

    void ITRTCCloud::onStartPublishCDNStream(int errCode, const char* errMsg)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onStartPublishCDNStream(errCode, Utils::CharPtrToString(errMsg));
        }
    }

    void ITRTCCloud::onStopPublishCDNStream(int errCode, const char* errMsg)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onStopPublishCDNStream(errCode, Utils::CharPtrToString(errMsg));
        }
    }

    void ITRTCCloud::onSetMixTranscodingConfig(int errCode, const char* errMsg)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onSetMixTranscodingConfig(errCode, Utils::CharPtrToString(errMsg));
        }
    }

    void ITRTCCloud::onAudioEffectFinished(int effectId, int code)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onAudioEffectFinished(effectId, code);
        }
    }

    void ITRTCCloud::onScreenCaptureCovered()
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onScreenCaptureCovered();
        }
    }

    void ITRTCCloud::onScreenCaptureStarted()
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onScreenCaptureStarted();
        }
    }

    void ITRTCCloud::onScreenCapturePaused(int reason)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onScreenCapturePaused(reason);
        }
    }

    void ITRTCCloud::onScreenCaptureResumed(int reason)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onScreenCaptureResumed(reason);
        }
    }

    void ITRTCCloud::onScreenCaptureStoped(int reason)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onScreenCaptureStoped(reason);
        }
    }

    void ITRTCCloud::onSnapshotComplete(const char* userId, liteav::TRTCVideoStreamType type, char* data, uint32_t length, uint32_t width, uint32_t height, liteav::TRTCVideoPixelFormat format)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        String^ uid = Utils::CharPtrToString(userId);
        TRTCVideoStreamType stream_type = static_cast<TRTCVideoStreamType>(type);
        TRTCVideoPixelFormat pixel_format = static_cast<TRTCVideoPixelFormat>(format);

        array<Byte>^ arr = gcnew array<Byte>(length);
        Marshal::Copy(IntPtr(data), arr, 0, length);

        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onSnapshotComplete(uid, stream_type, arr, length, width, height, pixel_format);
        }
    }

    void ITRTCCloud::onLocalRecordBegin(int errCode, const char* storagePath)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        String^ path = Utils::CharPtrToString(storagePath);
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onLocalRecordBegin(errCode, path);
        }
    }

    void ITRTCCloud::onLocalRecording(long duration, const char* storagePath)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        String^ path = Utils::CharPtrToString(storagePath);
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onLocalRecording(duration, path);
        }
    }

    void ITRTCCloud::onLocalRecordComplete(int errCode, const char* storagePath)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        String^ path = Utils::CharPtrToString(storagePath);
        int count = list->Count;
        for (int i = 0; i < count; i++)
        {
            if (list[i] != nullptr)
                list[i]->onLocalRecordComplete(errCode, path);
        }
    }

    void ITRTCCloud::onRemoteVideoStatusUpdated(const char* userId, liteav::TRTCVideoStreamType streamType, liteav::TRTCAVStatusType status, liteav::TRTCAVStatusChangeReason reason, void* extrainfo)
    {
        List<ITRTCCloudCallback^>^ list = copyTRTCCallbackList();
        if (list == nullptr) return;
        String^ uid = Utils::CharPtrToString(userId);
        TRTCVideoStreamType stream_type = static_cast<TRTCVideoStreamType>(streamType);
        TRTCAVStatusType status_type = static_cast<TRTCAVStatusType>(status);
        TRTCAVStatusChangeReason change_reason = static_cast<TRTCAVStatusChangeReason>(reason);

        int count = list->Count;
        for (int i = 0; i < count; i++) {
            if (nullptr != list[i]) {
                list[i]->onRemoteVideoStatusUpdated(uid, stream_type, status_type, change_reason, IntPtr(extrainfo));
            }
        }
    }

    void ITRTCCloud::onRenderVideoFrame(const char* userId, liteav::TRTCVideoStreamType streamType, ::TRTCVideoFrame* frame)
    {
        ITRTCVideoRenderCallback^ videoRenderCallback = nullptr;
        bool hasCallback = false;
        {
            msclr::lock l(m_videoRenderCallbacks);
            String^ uid = Utils::CharPtrToString(userId);
            hasCallback = m_videoRenderCallbacks->TryGetValue(uid, videoRenderCallback);
        }

        TRTCVideoStreamType stream_type = static_cast<TRTCVideoStreamType>(streamType);

        if (hasCallback && videoRenderCallback != nullptr) {
            TRTCVideoFrame^ videoFrame = nullptr;
            String^ key = Utils::CharPtrToString(userId) + stream_type.ToString();
            if (m_video_frame_cache != nullptr)
            {
                msclr::lock l(m_video_frame_cache);
                if (m_video_frame_cache->TryGetValue(key, videoFrame))
                {
                    if (videoFrame->data == nullptr || videoFrame->width != frame->width ||
                        videoFrame->height != frame->height || videoFrame->length != frame->length)
                    {
                        videoFrame->width = frame->width;
                        videoFrame->height = frame->height;
                        videoFrame->length = frame->length;
                        videoFrame->data = gcnew array<Byte>(frame->length);
                    }
                }
                else
                {
                    videoFrame = gcnew TRTCVideoFrame();
                    videoFrame->width = frame->width;
                    videoFrame->height = frame->height;
                    videoFrame->length = frame->length;
                    videoFrame->data = gcnew array<Byte>(frame->length);
                    m_video_frame_cache->Add(key, videoFrame);
                }
            }

            Marshal::Copy(IntPtr(frame->data), videoFrame->data, 0, frame->length);
            videoFrame->bufferType = static_cast<TRTCVideoBufferType>(frame->bufferType);
            videoFrame->rotation = static_cast<TRTCVideoRotation>(frame->rotation);
            videoFrame->textureId = frame->textureId;
            videoFrame->timestamp = frame->timestamp;
            videoFrame->videoFormat = static_cast<TRTCVideoPixelFormat>(frame->videoFormat);
            videoRenderCallback->onRenderVideoFrame(Utils::CharPtrToString(userId), stream_type, videoFrame);
        }
    }

    int ITRTCCloud::onProcessVideoFrame(liteav::TRTCVideoFrame* srcFrame, liteav::TRTCVideoFrame* dstFrame)
    {
        if (nullptr != m_videoFrameCallback) {
            TRTCVideoFrame^ srcVideoFrame, ^ dstVideoFrame;

            String^ key = "process";
            {
                msclr::lock l(m_video_frame_cache);
                if (m_video_frame_cache->TryGetValue(key, srcVideoFrame)) {
                    if (srcVideoFrame->length != srcFrame->length || srcVideoFrame->width != srcFrame->width || srcVideoFrame->height != srcFrame->height) {
                        srcVideoFrame->width = srcFrame->width;
                        srcVideoFrame->height = srcFrame->height;
                        srcVideoFrame->length = srcFrame->length;
                        srcVideoFrame->data = gcnew array<Byte>(srcFrame->length);
                    }
                }
                else
                {
                    srcVideoFrame = gcnew TRTCVideoFrame();
                    srcVideoFrame->width = srcFrame->width;
                    srcVideoFrame->height = srcFrame->height;
                    srcVideoFrame->length = srcFrame->length;
                    srcVideoFrame->data = gcnew array<Byte>(srcFrame->length);
                    m_video_frame_cache->Add(key, srcVideoFrame);
                }
            }
            srcVideoFrame->bufferType = static_cast<TRTCVideoBufferType>(srcFrame->bufferType);
            srcVideoFrame->rotation = static_cast<TRTCVideoRotation>(srcFrame->rotation);
            srcVideoFrame->videoFormat = static_cast<TRTCVideoPixelFormat>(srcFrame->videoFormat);
            srcVideoFrame->textureId = srcFrame->textureId;
            srcVideoFrame->timestamp = srcFrame->timestamp;
            Marshal::Copy(IntPtr(srcFrame->data), srcVideoFrame->data, 0, srcFrame->length);

            key = "dstProcess";
            {
                msclr::lock l(m_video_frame_cache);
                if (m_video_frame_cache->TryGetValue(key, dstVideoFrame)) {
                    if (dstVideoFrame->length != dstFrame->length || dstVideoFrame->width != dstFrame->width || dstVideoFrame->height != dstFrame->height) {
                        dstVideoFrame->width = dstFrame->width;
                        dstVideoFrame->height = dstFrame->height;
                        dstVideoFrame->length = dstFrame->length;
                        dstVideoFrame->data = gcnew array<Byte>(dstFrame->length);
                    }
                }
                else {
                    dstVideoFrame = gcnew TRTCVideoFrame();
                    dstVideoFrame->width = dstFrame->width;
                    dstVideoFrame->height = dstFrame->height;
                    dstVideoFrame->length = dstFrame->length;
                    dstVideoFrame->data = gcnew array<Byte>(dstFrame->length);
                    m_video_frame_cache->Add(key, dstVideoFrame);
                }
            }
            dstVideoFrame->timestamp = dstFrame->timestamp;
            int ret = m_videoFrameCallback->onProcessVideoFrame(srcVideoFrame, dstVideoFrame);

            dstFrame->bufferType = static_cast<::TRTCVideoBufferType>(dstVideoFrame->bufferType);
            dstFrame->rotation = static_cast<::TRTCVideoRotation>(dstVideoFrame->rotation);
            dstFrame->videoFormat = static_cast<::TRTCVideoPixelFormat>(dstVideoFrame->videoFormat);
            dstFrame->width = dstVideoFrame->width;
            dstFrame->height = dstVideoFrame->height;
            dstFrame->length = dstVideoFrame->length;
            dstFrame->textureId = dstVideoFrame->textureId;
            dstFrame->timestamp = dstVideoFrame->timestamp;
            pin_ptr<Byte> pByte = &dstVideoFrame->data[0];
            memcpy(dstFrame->data, pByte, dstVideoFrame->data->Length);
            return ret;
        }
    }

    void ITRTCCloud::onCapturedRawAudioFrame(liteav::TRTCAudioFrame* frame)
    {
        if (nullptr != m_audioFrameCallback)
        {
            TRTCAudioFrame^ audioFrame;
            String^ key = "local";
            if (m_audio_frame_cache != nullptr)
            {
                msclr::lock l(m_audio_frame_cache);
                if (m_audio_frame_cache->TryGetValue(key, audioFrame))
                {
                    if (audioFrame->data == nullptr || audioFrame->channel != frame->channel ||
                        audioFrame->length != frame->length || audioFrame->sampleRate != frame->sampleRate)
                    {
                        audioFrame->channel = frame->channel;
                        audioFrame->length = frame->length;
                        audioFrame->sampleRate = frame->sampleRate;
                        audioFrame->data = gcnew array<Byte>(frame->length);
                    }
                }
                else
                {
                    audioFrame = gcnew TRTCAudioFrame();
                    audioFrame->channel = frame->channel;
                    audioFrame->length = frame->length;
                    audioFrame->sampleRate = frame->sampleRate;
                    audioFrame->data = gcnew array<Byte>(frame->length);
                    m_audio_frame_cache->Add(key, audioFrame);
                }
            }
            Marshal::Copy(IntPtr(frame->data), audioFrame->data, 0, frame->length);
            audioFrame->audioFormat = static_cast<TRTCAudioFrameFormat>(frame->audioFormat);
            audioFrame->timestamp = frame->timestamp;
            m_audioFrameCallback->onCapturedRawAudioFrame(audioFrame);
        }
    }

    void ITRTCCloud::onLocalProcessedAudioFrame(liteav::TRTCAudioFrame* frame)
    {
        if (nullptr != m_audioFrameCallback) {
            TRTCAudioFrame^ audioFrame;
            String^ key = "processed";
            if (m_audio_frame_cache != nullptr) {
                msclr::lock l(m_audio_frame_cache);
                if (m_audio_frame_cache->TryGetValue(key, audioFrame)) {
                    if (audioFrame->data == nullptr || audioFrame->channel != frame->channel ||
                        audioFrame->length != frame->length ||
                        audioFrame->sampleRate != frame->sampleRate) {
                        audioFrame->channel = frame->channel;
                        audioFrame->length = frame->length;
                        audioFrame->sampleRate = frame->sampleRate;
                        audioFrame->data = gcnew array<Byte>(frame->length);
                    }
                }
                else {
                    audioFrame = gcnew TRTCAudioFrame();
                    audioFrame->channel = frame->channel;
                    audioFrame->length = frame->length;
                    audioFrame->sampleRate = frame->sampleRate;
                    audioFrame->data = gcnew array<Byte>(frame->length);
                    m_audio_frame_cache->Add(key, audioFrame);
                }
            }
            Marshal::Copy(IntPtr(frame->data), audioFrame->data, 0, frame->length);
            audioFrame->audioFormat = static_cast<TRTCAudioFrameFormat>(frame->audioFormat);
            audioFrame->timestamp = frame->timestamp;
            m_audioFrameCallback->onLocalProcessedAudioFrame(audioFrame);
        }
    }

    void ITRTCCloud::onPlayAudioFrame(::TRTCAudioFrame* frame, const char* userId)
    {
        if (nullptr != m_audioFrameCallback)
        {
            TRTCAudioFrame^ audioFrame;
            String^ key = Utils::CharPtrToString(userId);
            if (m_audio_frame_cache != nullptr)
            {
                msclr::lock l(m_audio_frame_cache);
                if (m_audio_frame_cache->TryGetValue(key, audioFrame))
                {
                    if (audioFrame->data == nullptr || audioFrame->channel != frame->channel ||
                        audioFrame->length != frame->length || audioFrame->sampleRate != frame->sampleRate)
                    {
                        audioFrame->channel = frame->channel;
                        audioFrame->length = frame->length;
                        audioFrame->sampleRate = frame->sampleRate;
                        audioFrame->data = gcnew array<Byte>(frame->length);
                    }
                }
                else
                {
                    audioFrame = gcnew TRTCAudioFrame();
                    audioFrame->channel = frame->channel;
                    audioFrame->length = frame->length;
                    audioFrame->sampleRate = frame->sampleRate;
                    audioFrame->data = gcnew array<Byte>(frame->length);
                    m_audio_frame_cache->Add(key, audioFrame);
                }
            }
            Marshal::Copy(IntPtr(frame->data), audioFrame->data, 0, frame->length);
            audioFrame->audioFormat = static_cast<TRTCAudioFrameFormat>(frame->audioFormat);
            audioFrame->timestamp = frame->timestamp;
            m_audioFrameCallback->onPlayAudioFrame(audioFrame, key);
        }
    }

    void ITRTCCloud::onMixedPlayAudioFrame(::TRTCAudioFrame* frame)
    {
        if (nullptr != m_audioFrameCallback)
        {
            TRTCAudioFrame^ audioFrame;
            String^ key = "mix";
            if (m_audio_frame_cache != nullptr)
            {
                msclr::lock l(m_audio_frame_cache);
                if (m_audio_frame_cache->TryGetValue(key, audioFrame))
                {
                    if (audioFrame->data == nullptr || audioFrame->channel != frame->channel ||
                        audioFrame->length != frame->length || audioFrame->sampleRate != frame->sampleRate)
                    {
                        audioFrame->channel = frame->channel;
                        audioFrame->length = frame->length;
                        audioFrame->sampleRate = frame->sampleRate;
                        audioFrame->data = gcnew array<Byte>(frame->length);
                    }
                }
                else
                {
                    audioFrame = gcnew TRTCAudioFrame();
                    audioFrame->channel = frame->channel;
                    audioFrame->length = frame->length;
                    audioFrame->sampleRate = frame->sampleRate;
                    audioFrame->data = gcnew array<Byte>(frame->length);
                    m_audio_frame_cache->Add(key, audioFrame);
                }
            }
            Marshal::Copy(IntPtr(frame->data), audioFrame->data, 0, frame->length);
            audioFrame->audioFormat = static_cast<TRTCAudioFrameFormat>(frame->audioFormat);
            audioFrame->timestamp = frame->timestamp;
            m_audioFrameCallback->onMixedPlayAudioFrame(audioFrame);
        }
    }

    void ITRTCCloud::onLog(const char* log, liteav::TRTCLogLevel level, const char* module)
    {
        if (nullptr != m_logCallback) {
            TRTCLogLevel log_level = static_cast<TRTCLogLevel>(level);
            m_logCallback->onLog(Utils::CharPtrToString(log), log_level, Utils::CharPtrToString(module));
        }
    }

    List<ITRTCCloudCallback^>^ ITRTCCloud::copyTRTCCallbackList()
    {
        List<ITRTCCloudCallback^>^ list;
        {
            msclr::lock l(m_cb_locker);
            if (!m_callbacks || m_callbacks->Count == 0) return nullptr;
            list = gcnew List<ITRTCCloudCallback^>();
            for each (ITRTCCloudCallback ^ callback in m_callbacks)
                list->Add(callback);
        }
        return list;
    }
}