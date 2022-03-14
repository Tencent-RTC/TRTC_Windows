#pragma once

// C++ Header
#include "include/TRTC/TRTCCloudCallback.h"

typedef void (CALLBACK* PFN_ERROR)(TXLiteAVError errCode, const char* errMsg, void* arg);
typedef void (CALLBACK* PFN_WARNING)(TXLiteAVWarning warningCode, const char* warningMsg, void* arg);
typedef void (CALLBACK* PFN_ENTERROOM)(int reason);
typedef void (CALLBACK* PFN_EXITROOM)(int reason);
typedef void (CALLBACK* PFN_USERENTER)(const char* userId);
typedef void (CALLBACK* PFN_USEREXIT)(const char* userId, int reason);
typedef void (CALLBACK* PFN_REMOTEUSERENTERROOM)(const char* userId);
typedef void (CALLBACK* PFN_REMOTEUSERLEAVEROOM)(const char* userId, int reason);
typedef void (CALLBACK* PFN_SWITCHROLE)(TXLiteAVError errCode, const char* errMsg);
typedef void (CALLBACK* PFN_SWITCHROOM)(TXLiteAVError errCode, const char* errMsg);
typedef void (CALLBACK* PFN_CONNECTOTHERROOM)(const char* userId, TXLiteAVError errCode, const char* errMsg);
typedef void (CALLBACK* PFN_DISCONNECTOTHERROOM)(TXLiteAVError errCode, const char* errMsg);
typedef void (CALLBACK* PFN_USERVIDEOAVAILABLE)(const char* userId, bool available);
typedef void (CALLBACK* PFN_USERSUBSTREAMAVAILABLE)(const char* userId, bool available);
typedef void (CALLBACK* PFN_USERAUDIOAVAILABLE)(const char* userId, bool available);
typedef void (CALLBACK* PFN_USERVOICEVOLUME)(TRTCVolumeInfo* userVolumes, unsigned int userVolumesCount, unsigned int totalVolume);
typedef void (CALLBACK* PFN_NETWORKQUALITY)(TRTCQualityInfo localQuality, TRTCQualityInfo* remoteQuality, unsigned int remoteQualityCount);
typedef void (CALLBACK* PFN_STATISTICS)(const TRTCStatistics& statis);
typedef void (CALLBACK* PFN_FIRSTVIDEOFRAME)(const char* userId, const  TRTCVideoStreamType streamType, const int width, const int height);
typedef void (CALLBACK* PFN_FIRSTAUDIOFRAME)(const char* userId);
typedef void (CALLBACK* PFN_SENDFIRSTLOCALVIDEOFRAME)(const TRTCVideoStreamType streamType);
typedef void (CALLBACK* PFN_SENDFIRSTLOCALAUDIOFRAME)();
typedef void (CALLBACK* PFN_PLAYBGMBEGIN)(TXLiteAVError errCode);
typedef void (CALLBACK* PFN_PLAYBGMPROGRESS)(unsigned int progressMS, unsigned int durationMS);
typedef void (CALLBACK* PFN_PLAYBGMCOMPLETE)(TXLiteAVError errCode);
typedef void (CALLBACK* PFN_CONNECTIONLOST)();
typedef void (CALLBACK* PFN_TRYTORECONNECT)();
typedef void (CALLBACK* PFN_CONNECTIONRECOVERY)();
typedef void (CALLBACK* PFN_SPEEDTEST)(const TRTCSpeedTestResult& currentResult, unsigned int finishedCount, unsigned int totalCount);
typedef void (CALLBACK* PFN_CAMERADIDREADY)();
typedef void (CALLBACK* PFN_MICDIDREADY)();
typedef void (CALLBACK* PFN_DEVICECHANGE)(const char* deviceId, TRTCDeviceType type, TRTCDeviceState state);
typedef void (CALLBACK* PFN_TESTMICVOLUME)(unsigned int volume);
typedef void (CALLBACK* PFN_TESTSPEAKERVOLUME)(unsigned int volume);
typedef void (CALLBACK* PFN_AUDIODEVICECAPTUREVOLUMECHANGED)(unsigned int volume, bool muted);
typedef void (CALLBACK* PFN_AUDIODEVICEPLAYOUTVOLUMECHANGED)(unsigned int volume, bool muted);
typedef void (CALLBACK* PFN_RECVCUSTOMCMDMSG)(const char* userId, int cmdId, unsigned int seq, const unsigned char* msg, unsigned int msgSize);
typedef void (CALLBACK* PFN_MISSCUSTOMCMDMSG)(const char* userId, int  cmdId, int errCode, int missed);
typedef void (CALLBACK* PFN_RECVSEIMSG)(const char* userId, const unsigned char* message, unsigned int msgSize);
typedef void (CALLBACK* PFN_STARTPUBLISHING)(int errCode, const char* errMsg);
typedef void (CALLBACK* PFN_STOPPUBLISHING)(int errCode, const char* errMsg);
typedef void (CALLBACK* PFN_STARTPUBLISHCDNSTREAM)(int errCode, const char* errMsg);
typedef void (CALLBACK* PFN_STOPPUBLISHCDNSTREAM)(int errCode, const char* errMsg);
typedef void (CALLBACK* PFN_SETMIXTRANSCODINGCONFIG)(int errCode, const char* errMsg);
typedef void (CALLBACK* PFN_AUDIOEFFECTFINISHED)(int effectId, int code);
typedef void (CALLBACK* PFN_SCREENCAPTURECOVERED)();
typedef void (CALLBACK* PFN_SCREENCAPTURESTARTED)();
typedef void (CALLBACK* PFN_SCREENCAPTUREPAUSED)(int reason);
typedef void (CALLBACK* PFN_SCREENCAPTURERESUMED)(int reason);
typedef void (CALLBACK* PFN_SCREENCAPTURESTOPED)(int reason);

typedef void (CALLBACK* PFN_SNAPSHOTCOMPLETE)(const char* userId, TRTCVideoStreamType type, char* data, uint32_t length, uint32_t width, uint32_t height, TRTCVideoPixelFormat format);
typedef void (CALLBACK* PFN_LOCALRECORDBEGIN)(int errCode, const char* storagePath);
typedef void (CALLBACK* PFN_LOCALRECORDING)(long duration, const char* storagePath);
typedef void (CALLBACK* PFN_LOCALRECORDCOMPLETE)(int errCode, const char* storagePath);
typedef void (CALLBACK* PFN_REMOTEVIDEOSTATUSUPDATED)(const char* userId, TRTCVideoStreamType streamType, TRTCAVStatusType status, TRTCAVStatusChangeReason reason, void* extrainfo);

typedef void (CALLBACK* PFN_RENDERVIDEOFRAME)(const char* userId, TRTCVideoStreamType streamType, TRTCVideoFrame* frame);
typedef int (CALLBACK* PFN_PROCESSVIDEOFRAME)(TRTCVideoFrame* srcFrame, TRTCVideoFrame* dstFrame);

typedef void (CALLBACK* PFN_CAPTUREDAUDIOFRAME)(TRTCAudioFrame* frame);
typedef void (CALLBACK* PFN_PLAYAUDIOFRAME)(TRTCAudioFrame* frame, const char* userId);
typedef void (CALLBACK* PFN_MIXEDPLAYAUDIOFRAME)(TRTCAudioFrame* frame);

typedef void (CALLBACK* PFN_LOG)(const char* log, TRTCLogLevel level, const char* module);

/// TRTCLogCallbackImpl
class TRTCLogCallbackImpl : public ITRTCLogCallback
{
public:
    virtual ~TRTCLogCallbackImpl();

    void setLog(PFN_LOG log);

    virtual void onLog(const char* log, TRTCLogLevel level, const char* module);
private:
    PFN_LOG m_log;
};
/// end TRTCLogCallbackImpl

/// TRTCVideoRenderCallbackImpl
class TRTCVideoRenderCallbackImpl : public liteav::ITRTCVideoRenderCallback
{
public:
    virtual ~TRTCVideoRenderCallbackImpl();

    void setRenderVideoFrame(PFN_RENDERVIDEOFRAME frame);

    virtual void onRenderVideoFrame(const char* userId, TRTCVideoStreamType streamType, TRTCVideoFrame* frame);
private:
    PFN_RENDERVIDEOFRAME m_renderVideoFrame;
};
/// end TRTCVideoRenderCallbackImpl

/// ITRTCVideoFrameCallbackImpl
class ITRTCVideoFrameCallbackImpl :
    public liteav::ITRTCVideoFrameCallback
{
public:
    virtual ~ITRTCVideoFrameCallbackImpl();

    void setProcessVideoFrame(PFN_PROCESSVIDEOFRAME frame);

    virtual int onProcessVideoFrame(TRTCVideoFrame* srcFrame, TRTCVideoFrame* dstFrame);

private:
    PFN_PROCESSVIDEOFRAME m_processVideoFrame;
};
/// end ITRTCVideoFrameCallbackImpl

/// TRTCAudioFrameCallbackImpl
class TRTCAudioFrameCallbackImpl : public ::ITRTCAudioFrameCallback
{
public:
    virtual ~TRTCAudioFrameCallbackImpl();

    void setCapturedAudioFrame(PFN_CAPTUREDAUDIOFRAME frame);

    void setPlayAudioFrame(PFN_PLAYAUDIOFRAME frame);

    void setMixedPlayAudioFrame(PFN_MIXEDPLAYAUDIOFRAME frame);

    void setLocalProcessedAudioFrame(PFN_CAPTUREDAUDIOFRAME frame);

    virtual void onCapturedRawAudioFrame(TRTCAudioFrame* frame);

    virtual void onPlayAudioFrame(TRTCAudioFrame* frame, const char* userId);

    virtual void onMixedPlayAudioFrame(TRTCAudioFrame* frame);

    virtual void onLocalProcessedAudioFrame(TRTCAudioFrame* frame);

private:
    PFN_CAPTUREDAUDIOFRAME m_capturedAudioFrame;
    PFN_CAPTUREDAUDIOFRAME m_localProcessedAudioFrame;
    PFN_PLAYAUDIOFRAME m_playAudioFrame;
    PFN_MIXEDPLAYAUDIOFRAME m_mixedPlayAudioFrame;
};
/// end TRTCAudioFrameCallbackImpl

// 非托管类无法直接引用托管类，故此通过函数指针做桥接
class TRTCCloudCallbackImpl : public ::ITRTCCloudCallback
{

public:
    virtual ~TRTCCloudCallbackImpl();

    void setError(PFN_ERROR pointer);

    void setWarning(PFN_WARNING pointer);

    void setEnterRoom(PFN_ENTERROOM pointer);

    void setExitRoom(PFN_EXITROOM pointer);

    void setUserEnter(PFN_USERENTER pointer);

    void setUserExit(PFN_USEREXIT pointer);

    void setRemoteUserEnterRoom(PFN_REMOTEUSERENTERROOM pointer);

    void setRemoteUserLeaveRoom(PFN_REMOTEUSERLEAVEROOM pointer);

    void setSwitchRole(PFN_SWITCHROLE pointer);

    void setSwitchRoom(PFN_SWITCHROOM pointer);

    void setConnectOtherRoom(PFN_CONNECTOTHERROOM pointer);

    void setDisconnectOtherRoom(PFN_DISCONNECTOTHERROOM pointer);

    void setUserVideoAvailable(PFN_USERVIDEOAVAILABLE pointer);

    void setUserSubStreamAvailable(PFN_USERSUBSTREAMAVAILABLE pointer);

    void setUserAudioAvailable(PFN_USERAUDIOAVAILABLE pointer);

    void setUserVoiceVolume(PFN_USERVOICEVOLUME pointer);

    void setNetworkQuality(PFN_NETWORKQUALITY pointer);

    void setStatistics(PFN_STATISTICS pointer);

    void setFirstVideoFrame(PFN_FIRSTVIDEOFRAME pointer);

    void setFirstAudioFrame(PFN_FIRSTAUDIOFRAME pointer);

    void setSendFirstLocalVideoFrame(PFN_SENDFIRSTLOCALVIDEOFRAME pointer);

    void setSendFirstLocalAudioFrame(PFN_SENDFIRSTLOCALAUDIOFRAME pointer);

    void setPlayBGMBegin(PFN_PLAYBGMBEGIN pointer);

    void setPlayBGMProgress(PFN_PLAYBGMPROGRESS pointer);

    void setPlayBGMComplete(PFN_PLAYBGMCOMPLETE pointer);

    void setConnectionLost(PFN_CONNECTIONLOST pointer);

    void setTryToReconnect(PFN_TRYTORECONNECT pointer);

    void setConnectionRecovery(PFN_CONNECTIONRECOVERY pointer);

    void setSpeedTest(PFN_SPEEDTEST pointer);

    void setCameraDidReady(PFN_CAMERADIDREADY pointer);

    void setMicDidReady(PFN_MICDIDREADY pointer);

    void setDeviceChange(PFN_DEVICECHANGE pointer);

    void setTestMicVolume(PFN_TESTMICVOLUME pointer);

    void setTestSpeakerVolume(PFN_TESTSPEAKERVOLUME pointer);

    void setAudioDeviceCaptureVolumeChanged(PFN_AUDIODEVICECAPTUREVOLUMECHANGED pointer);

    void setAudioDevicePlayoutVolumeChanged(PFN_AUDIODEVICEPLAYOUTVOLUMECHANGED pointer);

    void setRecvCustomCmdMsg(PFN_RECVCUSTOMCMDMSG pointer);

    void setMissCustomCmdMsg(PFN_MISSCUSTOMCMDMSG pointer);

    void setRecvSEIMsg(PFN_RECVSEIMSG pointer);

    void setStartPublishing(PFN_STARTPUBLISHING pointer);

    void setStopPublishing(PFN_STOPPUBLISHING pointer);

    void setStartPublishCDNStream(PFN_STARTPUBLISHCDNSTREAM pointer);

    void setStopPublishCDNStream(PFN_STOPPUBLISHCDNSTREAM pointer);

    void setSetMixTranscodingConfig(PFN_SETMIXTRANSCODINGCONFIG pointer);

    void setAudioEffectFinished(PFN_AUDIOEFFECTFINISHED pointer);

    void setScreenCaptureCovered(PFN_SCREENCAPTURECOVERED pointer);

    void setScreenCaptureStarted(PFN_SCREENCAPTURESTARTED pointer);

    void setScreenCapturePaused(PFN_SCREENCAPTUREPAUSED pointer);

    void setScreenCaptureResumed(PFN_SCREENCAPTURERESUMED pointer);

    void setScreenCaptureStoped(PFN_SCREENCAPTURESTOPED pointer);

    void setSnapshotComplete(PFN_SNAPSHOTCOMPLETE pointer);

    void setLocalRecordBegin(PFN_LOCALRECORDBEGIN pointer);

    void setLocalRecording(PFN_LOCALRECORDING pointer);

    void setLocalRecordComplete(PFN_LOCALRECORDCOMPLETE pointer);

    void setRemoteVideoStatusUpdated(PFN_REMOTEVIDEOSTATUSUPDATED pointer);

    virtual void onError(TXLiteAVError errCode, const char* errMsg, void* arg);

    virtual void onWarning(TXLiteAVWarning warningCode, const char* warningMsg, void* arg);

    virtual void onEnterRoom(int result);

    virtual void onExitRoom(int reason);

    virtual void onUserEnter(const char* userId);

    virtual void onUserExit(const char* userId, int reason);

    virtual void onRemoteUserEnterRoom(const char* userId);

    virtual void onRemoteUserLeaveRoom(const char* userId, int reason);

    virtual void onSwitchRole(TXLiteAVError errCode, const char* errMsg);

    virtual void onConnectOtherRoom(const char* userId, TXLiteAVError errCode, const char* errMsg);

    virtual void onDisconnectOtherRoom(TXLiteAVError errCode, const char* errMsg);

    virtual void onSwitchRoom(TXLiteAVError errCode, const char* errMsg);

    virtual void onUserVideoAvailable(const char* userId, bool available);

    virtual void onUserSubStreamAvailable(const char* userId, bool available);

    virtual void onUserAudioAvailable(const char* userId, bool available);

    virtual void onUserVoiceVolume(TRTCVolumeInfo* userVolumes, unsigned int userVolumesCount, unsigned int totalVolume);

    virtual void onNetworkQuality(TRTCQualityInfo localQuality, TRTCQualityInfo* remoteQuality, unsigned int remoteQualityCount);

    virtual void onStatistics(const TRTCStatistics& statis);

    virtual void onFirstVideoFrame(const char* userId, const TRTCVideoStreamType streamType, const int width, const int height);

    virtual void onFirstAudioFrame(const char* userId);

    virtual void onSendFirstLocalVideoFrame(const TRTCVideoStreamType streamType);

    virtual void onSendFirstLocalAudioFrame();

    virtual void onPlayBGMBegin(TXLiteAVError errCode);

    virtual void onPlayBGMProgress(unsigned int progressMS, unsigned int durationMS);

    virtual void onPlayBGMComplete(TXLiteAVError errCode);

    virtual void onConnectionLost();

    virtual void onTryToReconnect();

    virtual void onConnectionRecovery();

    virtual void onSpeedTest(const TRTCSpeedTestResult& currentResult, unsigned int finishedCount, unsigned int totalCount);

    virtual void onCameraDidReady();

    virtual void onMicDidReady();

    virtual void onDeviceChange(const char* deviceId, TRTCDeviceType type, TRTCDeviceState state);

    virtual void onTestMicVolume(unsigned int volume);

    virtual void onTestSpeakerVolume(unsigned int volume);

    virtual void onAudioDeviceCaptureVolumeChanged(unsigned int volume, bool muted);

    virtual void onAudioDevicePlayoutVolumeChanged(unsigned int volume, bool muted);

    virtual void onRecvCustomCmdMsg(const char* userId, int cmdId, unsigned int seq, const unsigned char* msg, unsigned int msgSize);

    virtual void onMissCustomCmdMsg(const char* userId, int  cmdId, int errCode, int missed);

    virtual void onRecvSEIMsg(const char* userId, const unsigned char* message, unsigned int msgSize);

    virtual void onStartPublishing(int errCode, const char* errMsg);

    virtual void onStopPublishing(int errCode, const char* errMsg);

    virtual void onStartPublishCDNStream(int errCode, const char* errMsg);

    virtual void onStopPublishCDNStream(int errCode, const char* errMsg);

    virtual void onSetMixTranscodingConfig(int errCode, const char* errMsg);

    virtual void onAudioEffectFinished(int effectId, int code);

    virtual void onScreenCaptureCovered();

    virtual void onScreenCaptureStarted();

    virtual void onScreenCapturePaused(int reason);

    virtual void onScreenCaptureResumed(int reason);

    virtual void onScreenCaptureStoped(int reason);

    virtual void onSnapshotComplete(const char* userId, TRTCVideoStreamType type, char* data, uint32_t length, uint32_t width, uint32_t height, TRTCVideoPixelFormat format);

    virtual void onLocalRecordBegin(int errCode, const char* storagePath);

    virtual void onLocalRecording(long duration, const char* storagePath);

    virtual void onLocalRecordComplete(int errCode, const char* storagePath);

    virtual void onRemoteVideoStatusUpdated(const char* userId, TRTCVideoStreamType streamType, TRTCAVStatusType status, TRTCAVStatusChangeReason reason, void* extrainfo);

private:
    PFN_ERROR                    m_error;
    PFN_WARNING                  m_warning;
    PFN_ENTERROOM                m_enterRoom;
    PFN_EXITROOM                 m_exitRoom;
    PFN_USERENTER                m_userEnter;
    PFN_USEREXIT                 m_userExit;
    PFN_REMOTEUSERENTERROOM      m_remoteUserEnterRoom;
    PFN_REMOTEUSERLEAVEROOM      m_remoteUserLeaveRoom;
    PFN_SWITCHROLE               m_switchRole;
    PFN_SWITCHROOM               m_switchRoom;
    PFN_CONNECTOTHERROOM         m_connectOtherRoom;
    PFN_DISCONNECTOTHERROOM      m_disconnectOtherRoom;
    PFN_USERVIDEOAVAILABLE       m_userVideoAvailable;
    PFN_USERSUBSTREAMAVAILABLE   m_userSubstreamAvailable;
    PFN_USERAUDIOAVAILABLE       m_userAudioAvailable;
    PFN_USERVOICEVOLUME          m_userVoiceVolume;
    PFN_NETWORKQUALITY           m_networkQuality;
    PFN_STATISTICS               m_statistics;
    PFN_FIRSTVIDEOFRAME          m_firstVideoFrame;
    PFN_FIRSTAUDIOFRAME          m_firstAudioFrame;
    PFN_SENDFIRSTLOCALVIDEOFRAME m_sendFirstLocalVideoFrame;
    PFN_SENDFIRSTLOCALAUDIOFRAME m_sendFirstLocalAudioFrame;
    PFN_PLAYBGMBEGIN             m_playBGMBegin;
    PFN_PLAYBGMPROGRESS          m_playBGMProgress;
    PFN_PLAYBGMCOMPLETE          m_playBGMComplete;
    PFN_CONNECTIONLOST           m_connectionLost;
    PFN_TRYTORECONNECT           m_tryToReconnect;
    PFN_CONNECTIONRECOVERY       m_connectionRecovery;
    PFN_SPEEDTEST                m_speedTest;
    PFN_CAMERADIDREADY           m_cameraDidReady;
    PFN_MICDIDREADY              m_micDidReady;
    PFN_DEVICECHANGE             m_deviceChange;
    PFN_TESTMICVOLUME            m_testMicVolume;
    PFN_TESTSPEAKERVOLUME        m_testSpeakerVolume;
    PFN_AUDIODEVICECAPTUREVOLUMECHANGED m_audioDeviceCaptureVolumeChanged;
    PFN_AUDIODEVICEPLAYOUTVOLUMECHANGED m_audioDevicePlayoutVolumeChanged;
    PFN_RECVCUSTOMCMDMSG         m_recvCustomCmdMsg;
    PFN_MISSCUSTOMCMDMSG         m_missCustomCmdMsg;
    PFN_RECVSEIMSG               m_recvSEIMsg;
    PFN_STARTPUBLISHING          m_startPublishing;
    PFN_STOPPUBLISHING           m_stopPublishing;
    PFN_STARTPUBLISHCDNSTREAM    m_startPublishCDNStream;
    PFN_STOPPUBLISHCDNSTREAM     m_stopPublishCDNStream;
    PFN_SETMIXTRANSCODINGCONFIG  m_setMixTranscodingConfig;
    PFN_AUDIOEFFECTFINISHED      m_audioEffectFinished;
    PFN_SCREENCAPTURECOVERED     m_screenCaptureCovered;
    PFN_SCREENCAPTURESTARTED     m_screenCaptureStarted;
    PFN_SCREENCAPTUREPAUSED      m_screenCapturePaused;
    PFN_SCREENCAPTURERESUMED     m_screenCaptureResumed;
    PFN_SCREENCAPTURESTOPED      m_screenCaptureStoped;
    PFN_SNAPSHOTCOMPLETE         m_snapshotComplete;
    PFN_LOCALRECORDBEGIN         m_localRecordBegin;
    PFN_LOCALRECORDING           m_localRecording;
    PFN_LOCALRECORDCOMPLETE      m_localRecordComplete;
    PFN_REMOTEVIDEOSTATUSUPDATED m_remoteVideoStatusUpdated;
};

