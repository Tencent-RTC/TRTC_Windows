#include "TRTCCloudCallback.h"

/// TRTCLogCallbackImpl
TRTCLogCallbackImpl::~TRTCLogCallbackImpl() {}

void TRTCLogCallbackImpl::setLog(PFN_LOG log)
{
    m_log = log;
}

void TRTCLogCallbackImpl::onLog(const char* log, TRTCLogLevel level, const char* module)
{
    if (nullptr != m_log) {
        m_log(log, level, module);
    }
}
/// end TRTCLogCallbackImpl

/// TRTCVideoRenderCallbackImpl
TRTCVideoRenderCallbackImpl::~TRTCVideoRenderCallbackImpl()
{
    m_renderVideoFrame = nullptr;
}

void TRTCVideoRenderCallbackImpl::setRenderVideoFrame(PFN_RENDERVIDEOFRAME frame)
{
    m_renderVideoFrame = frame;
}

void TRTCVideoRenderCallbackImpl::onRenderVideoFrame(const char* userId, liteav::TRTCVideoStreamType streamType, liteav::TRTCVideoFrame* frame)
{
    if (nullptr != m_renderVideoFrame) {
        m_renderVideoFrame(userId, streamType, frame);
    }
}
/// end TRTCVideoRenderCallbackImpl

/// ITRTCVideoFrameCallbackImpl
ITRTCVideoFrameCallbackImpl::~ITRTCVideoFrameCallbackImpl()
{
    m_processVideoFrame = nullptr;
}

void ITRTCVideoFrameCallbackImpl::setProcessVideoFrame(PFN_PROCESSVIDEOFRAME frame) {
    m_processVideoFrame = frame;
}

int ITRTCVideoFrameCallbackImpl::onProcessVideoFrame(TRTCVideoFrame* srcFrame, TRTCVideoFrame* dstFrame) {
    if (nullptr != m_processVideoFrame) {
        return m_processVideoFrame(srcFrame, dstFrame);
    }
    return -1;
}
/// end ITRTCVideoFrameCallbackImpl

/// TRTCAudioFrameCallbackImpl

TRTCAudioFrameCallbackImpl::~TRTCAudioFrameCallbackImpl() {}

void TRTCAudioFrameCallbackImpl::setCapturedAudioFrame(PFN_CAPTUREDAUDIOFRAME frame)
{
    m_capturedAudioFrame = frame;
}

void TRTCAudioFrameCallbackImpl::setPlayAudioFrame(PFN_PLAYAUDIOFRAME frame)
{
    m_playAudioFrame = frame;
}

void TRTCAudioFrameCallbackImpl::setMixedPlayAudioFrame(PFN_MIXEDPLAYAUDIOFRAME frame)
{
    m_mixedPlayAudioFrame = frame;
}

void TRTCAudioFrameCallbackImpl::setLocalProcessedAudioFrame(PFN_CAPTUREDAUDIOFRAME frame) {
    m_localProcessedAudioFrame = frame;
}

void TRTCAudioFrameCallbackImpl::onCapturedRawAudioFrame(TRTCAudioFrame* frame)
{
    if (nullptr != m_capturedAudioFrame)
        m_capturedAudioFrame(frame);
}

void TRTCAudioFrameCallbackImpl::onPlayAudioFrame(TRTCAudioFrame* frame, const char* userId)
{
    if (nullptr != m_playAudioFrame)
        m_playAudioFrame(frame, userId);
}

void TRTCAudioFrameCallbackImpl::onMixedPlayAudioFrame(TRTCAudioFrame* frame)
{
    if (nullptr != m_mixedPlayAudioFrame)
        m_mixedPlayAudioFrame(frame);
}

void TRTCAudioFrameCallbackImpl::onLocalProcessedAudioFrame(TRTCAudioFrame* frame)
{
    if (nullptr != m_localProcessedAudioFrame) {
        m_localProcessedAudioFrame(frame);
    }
}
/// end TRTCAudioFrameCallbackImpl

TRTCCloudCallbackImpl::~TRTCCloudCallbackImpl() {

}

void TRTCCloudCallbackImpl::setError(PFN_ERROR pointer)
{
    m_error = pointer;
}

void TRTCCloudCallbackImpl::setWarning(PFN_WARNING pointer)
{
    m_warning = pointer;
}

void TRTCCloudCallbackImpl::setEnterRoom(PFN_ENTERROOM pointer)
{
    m_enterRoom = pointer;
}

void TRTCCloudCallbackImpl::setExitRoom(PFN_EXITROOM pointer)
{
    m_exitRoom = pointer;
}

void TRTCCloudCallbackImpl::setUserEnter(PFN_USERENTER pointer)
{
    m_userEnter = pointer;
}

void TRTCCloudCallbackImpl::setUserExit(PFN_USEREXIT pointer)
{
    m_userExit = pointer;
}

void TRTCCloudCallbackImpl::setRemoteUserEnterRoom(PFN_REMOTEUSERENTERROOM pointer)
{
    m_remoteUserEnterRoom = pointer;
}

void TRTCCloudCallbackImpl::setRemoteUserLeaveRoom(PFN_REMOTEUSERLEAVEROOM pointer)
{
    m_remoteUserLeaveRoom = pointer;
}

void TRTCCloudCallbackImpl::setSwitchRole(PFN_SWITCHROLE pointer)
{
    m_switchRole = pointer;
}

void TRTCCloudCallbackImpl::setSwitchRoom(PFN_SWITCHROOM pointer) {
    m_switchRoom = pointer;
}

void TRTCCloudCallbackImpl::setConnectOtherRoom(PFN_CONNECTOTHERROOM pointer)
{
    m_connectOtherRoom = pointer;
}

void TRTCCloudCallbackImpl::setDisconnectOtherRoom(PFN_DISCONNECTOTHERROOM pointer)
{
    m_disconnectOtherRoom = pointer;
}

void TRTCCloudCallbackImpl::setUserVideoAvailable(PFN_USERVIDEOAVAILABLE pointer)
{
    m_userVideoAvailable = pointer;
}

void TRTCCloudCallbackImpl::setUserSubStreamAvailable(PFN_USERSUBSTREAMAVAILABLE pointer)
{
    m_userSubstreamAvailable = pointer;
}

void TRTCCloudCallbackImpl::setUserAudioAvailable(PFN_USERAUDIOAVAILABLE pointer)
{
    m_userAudioAvailable = pointer;
}

void TRTCCloudCallbackImpl::setUserVoiceVolume(PFN_USERVOICEVOLUME pointer)
{
    m_userVoiceVolume = pointer;
}

void TRTCCloudCallbackImpl::setNetworkQuality(PFN_NETWORKQUALITY pointer)
{
    m_networkQuality = pointer;
}

void TRTCCloudCallbackImpl::setStatistics(PFN_STATISTICS pointer)
{
    m_statistics = pointer;
}

void TRTCCloudCallbackImpl::setFirstVideoFrame(PFN_FIRSTVIDEOFRAME pointer)
{
    m_firstVideoFrame = pointer;
}

void TRTCCloudCallbackImpl::setFirstAudioFrame(PFN_FIRSTAUDIOFRAME pointer)
{
    m_firstAudioFrame = pointer;
}

void TRTCCloudCallbackImpl::setSendFirstLocalVideoFrame(PFN_SENDFIRSTLOCALVIDEOFRAME pointer)
{
    m_sendFirstLocalVideoFrame = pointer;
}

void TRTCCloudCallbackImpl::setSendFirstLocalAudioFrame(PFN_SENDFIRSTLOCALAUDIOFRAME pointer)
{
    m_sendFirstLocalAudioFrame = pointer;
}

void TRTCCloudCallbackImpl::setPlayBGMBegin(PFN_PLAYBGMBEGIN pointer)
{
    m_playBGMBegin = pointer;
}

void TRTCCloudCallbackImpl::setPlayBGMProgress(PFN_PLAYBGMPROGRESS pointer)
{
    m_playBGMProgress = pointer;
}

void TRTCCloudCallbackImpl::setPlayBGMComplete(PFN_PLAYBGMCOMPLETE pointer)
{
    m_playBGMComplete = pointer;
}

void TRTCCloudCallbackImpl::setConnectionLost(PFN_CONNECTIONLOST pointer)
{
    m_connectionLost = pointer;
}

void TRTCCloudCallbackImpl::setTryToReconnect(PFN_TRYTORECONNECT pointer)
{
    m_tryToReconnect = pointer;
}

void TRTCCloudCallbackImpl::setConnectionRecovery(PFN_CONNECTIONRECOVERY pointer)
{
    m_connectionRecovery = pointer;
}

void TRTCCloudCallbackImpl::setSpeedTest(PFN_SPEEDTEST pointer)
{
    m_speedTest = pointer;
}

void TRTCCloudCallbackImpl::setCameraDidReady(PFN_CAMERADIDREADY pointer)
{
    m_cameraDidReady = pointer;
}

void TRTCCloudCallbackImpl::setMicDidReady(PFN_MICDIDREADY pointer)
{
    m_micDidReady = pointer;
}

void TRTCCloudCallbackImpl::setDeviceChange(PFN_DEVICECHANGE pointer)
{
    m_deviceChange = pointer;
}

void TRTCCloudCallbackImpl::setTestMicVolume(PFN_TESTMICVOLUME pointer)
{
    m_testMicVolume = pointer;
}

void TRTCCloudCallbackImpl::setTestSpeakerVolume(PFN_TESTSPEAKERVOLUME pointer)
{
    m_testSpeakerVolume = pointer;
}

void TRTCCloudCallbackImpl::setAudioDeviceCaptureVolumeChanged(PFN_AUDIODEVICECAPTUREVOLUMECHANGED pointer) {
    m_audioDeviceCaptureVolumeChanged = pointer;
}

void TRTCCloudCallbackImpl::setAudioDevicePlayoutVolumeChanged(PFN_AUDIODEVICEPLAYOUTVOLUMECHANGED pointer) {
    m_audioDevicePlayoutVolumeChanged = pointer;
}

void TRTCCloudCallbackImpl::setRecvCustomCmdMsg(PFN_RECVCUSTOMCMDMSG pointer)
{
    m_recvCustomCmdMsg = pointer;
}

void TRTCCloudCallbackImpl::setMissCustomCmdMsg(PFN_MISSCUSTOMCMDMSG pointer)
{
    m_missCustomCmdMsg = pointer;
}

void TRTCCloudCallbackImpl::setRecvSEIMsg(PFN_RECVSEIMSG pointer)
{
    m_recvSEIMsg = pointer;
}

void TRTCCloudCallbackImpl::setStartPublishing(PFN_STARTPUBLISHING pointer)
{
    m_startPublishing = pointer;
}

void TRTCCloudCallbackImpl::setStopPublishing(PFN_STOPPUBLISHING pointer)
{
    m_stopPublishing = pointer;
}

void TRTCCloudCallbackImpl::setStartPublishCDNStream(PFN_STARTPUBLISHCDNSTREAM pointer)
{
    m_startPublishCDNStream = pointer;
}

void TRTCCloudCallbackImpl::setStopPublishCDNStream(PFN_STOPPUBLISHCDNSTREAM pointer)
{
    m_stopPublishCDNStream = pointer;
}

void TRTCCloudCallbackImpl::setSetMixTranscodingConfig(PFN_SETMIXTRANSCODINGCONFIG pointer)
{
    m_setMixTranscodingConfig = pointer;
}

void TRTCCloudCallbackImpl::setAudioEffectFinished(PFN_AUDIOEFFECTFINISHED pointer)
{
    m_audioEffectFinished = pointer;
}

void TRTCCloudCallbackImpl::setScreenCaptureCovered(PFN_SCREENCAPTURECOVERED pointer)
{
    m_screenCaptureCovered = pointer;
}

void TRTCCloudCallbackImpl::setScreenCaptureStarted(PFN_SCREENCAPTURESTARTED pointer)
{
    m_screenCaptureStarted = pointer;
}

void TRTCCloudCallbackImpl::setScreenCapturePaused(PFN_SCREENCAPTUREPAUSED pointer)
{
    m_screenCapturePaused = pointer;
}

void TRTCCloudCallbackImpl::setScreenCaptureResumed(PFN_SCREENCAPTURERESUMED pointer)
{
    m_screenCaptureResumed = pointer;
}

void TRTCCloudCallbackImpl::setScreenCaptureStoped(PFN_SCREENCAPTURESTOPED pointer)
{
    m_screenCaptureStoped = pointer;
}

void TRTCCloudCallbackImpl::setSnapshotComplete(PFN_SNAPSHOTCOMPLETE pointer)
{
    m_snapshotComplete = pointer;
}

void TRTCCloudCallbackImpl::setLocalRecordBegin(PFN_LOCALRECORDBEGIN pointer)
{
    m_localRecordBegin = pointer;
}

void TRTCCloudCallbackImpl::setLocalRecording(PFN_LOCALRECORDING pointer)
{
    m_localRecording = pointer;
}

void TRTCCloudCallbackImpl::setLocalRecordComplete(PFN_LOCALRECORDCOMPLETE pointer)
{
    m_localRecordComplete = pointer;
}

void TRTCCloudCallbackImpl::setRemoteVideoStatusUpdated(PFN_REMOTEVIDEOSTATUSUPDATED pointer)
{
    m_remoteVideoStatusUpdated = pointer;
}

void TRTCCloudCallbackImpl::onError(TXLiteAVError errCode, const char* errMsg,void* arg)
{
    if (nullptr != m_error)
        m_error(errCode, errMsg, arg);
}

void TRTCCloudCallbackImpl::onWarning(TXLiteAVWarning warningCode, const char* warningMsg,void* arg)
{
    if (nullptr != m_warning)
        m_warning(warningCode, warningMsg, arg);
}

void TRTCCloudCallbackImpl::onEnterRoom(int result)
{
    if (nullptr != m_enterRoom)
        m_enterRoom(result);
}

void TRTCCloudCallbackImpl::onExitRoom(int reason)
{
    if (nullptr != m_exitRoom)
        m_exitRoom(reason);
}

void TRTCCloudCallbackImpl::onUserEnter(const char* userId)
{
    if (nullptr != m_userEnter)
        m_userEnter(userId);
}

void TRTCCloudCallbackImpl::onUserExit(const char* userId, int reason)
{
    if (nullptr != m_userExit)
        m_userExit(userId, reason);
}

void TRTCCloudCallbackImpl::onRemoteUserEnterRoom(const char* userId)
{
    if (nullptr != m_remoteUserEnterRoom)
        m_remoteUserEnterRoom(userId);
}

void TRTCCloudCallbackImpl::onRemoteUserLeaveRoom(const char* userId, int reason)
{
    if (nullptr != m_remoteUserLeaveRoom)
        m_remoteUserLeaveRoom(userId, reason);
}

void TRTCCloudCallbackImpl::onSwitchRole(TXLiteAVError errCode, const char* errMsg)
{
    if (nullptr != m_switchRole)
        m_switchRole(errCode, errMsg);
}

void TRTCCloudCallbackImpl::onConnectOtherRoom(const char* userId, TXLiteAVError errCode, const char* errMsg)
{
    if (nullptr != m_connectOtherRoom)
        m_connectOtherRoom(userId, errCode, errMsg);
}

void TRTCCloudCallbackImpl::onDisconnectOtherRoom(TXLiteAVError errCode, const char* errMsg)
{
    if (nullptr != m_disconnectOtherRoom)
        m_disconnectOtherRoom(errCode, errMsg);
}

void TRTCCloudCallbackImpl::onSwitchRoom(TXLiteAVError errCode, const char* errMsg) {
    if (nullptr != m_switchRoom)
        m_switchRoom(errCode, errMsg);
}

void TRTCCloudCallbackImpl::onUserVideoAvailable(const char* userId, bool available)
{
    if (nullptr != m_userVideoAvailable)
        m_userVideoAvailable(userId, available);
}

void TRTCCloudCallbackImpl::onUserSubStreamAvailable(const char* userId, bool available)
{
    if (nullptr != m_userSubstreamAvailable)
        m_userSubstreamAvailable(userId, available);
}

void TRTCCloudCallbackImpl::onUserAudioAvailable(const char* userId, bool available)
{
    if (nullptr != m_userAudioAvailable)
        m_userAudioAvailable(userId, available);
}

void TRTCCloudCallbackImpl::onUserVoiceVolume(TRTCVolumeInfo* userVolumes, unsigned int userVolumesCount, unsigned int totalVolume)
{
    if (nullptr != m_userVoiceVolume)
        m_userVoiceVolume(userVolumes, userVolumesCount, totalVolume);
}

void TRTCCloudCallbackImpl::onNetworkQuality(TRTCQualityInfo localQuality, TRTCQualityInfo* remoteQuality, unsigned int remoteQualityCount)
{
    if (nullptr != m_networkQuality)
        m_networkQuality(localQuality, remoteQuality, remoteQualityCount);
}

void TRTCCloudCallbackImpl::onStatistics(const TRTCStatistics& statis)
{
    if (nullptr != m_statistics)
        m_statistics(statis);
}

void TRTCCloudCallbackImpl::onFirstVideoFrame(const char* userId, const TRTCVideoStreamType streamType, const int width, const int height)
{
    if (nullptr != m_firstVideoFrame)
        m_firstVideoFrame(userId, streamType, width, height);
}

void TRTCCloudCallbackImpl::onFirstAudioFrame(const char* userId)
{
    if (nullptr != m_firstAudioFrame)
        m_firstAudioFrame(userId);
}

void TRTCCloudCallbackImpl::onSendFirstLocalVideoFrame(const TRTCVideoStreamType streamType)
{
    if (nullptr != m_sendFirstLocalVideoFrame)
        m_sendFirstLocalVideoFrame(streamType);
}

void TRTCCloudCallbackImpl::onSendFirstLocalAudioFrame()
{
    if (nullptr != m_sendFirstLocalAudioFrame)
        m_sendFirstLocalAudioFrame();
}

void TRTCCloudCallbackImpl::onPlayBGMBegin(TXLiteAVError errCode)
{
    if (nullptr != m_playBGMBegin)
        m_playBGMBegin(errCode);
}

void TRTCCloudCallbackImpl::onPlayBGMProgress(unsigned int progressMS, unsigned int durationMS)
{
    if (nullptr != m_playBGMProgress)
        m_playBGMProgress(progressMS, durationMS);
}

void TRTCCloudCallbackImpl::onPlayBGMComplete(TXLiteAVError errCode)
{
    if (nullptr != m_playBGMComplete)
        m_playBGMComplete(errCode);
}

void TRTCCloudCallbackImpl::onConnectionLost()
{
    if (nullptr != m_connectionLost)
        m_connectionLost();
}

void TRTCCloudCallbackImpl::onTryToReconnect()
{
    if (nullptr != m_tryToReconnect)
        m_tryToReconnect();
}

void TRTCCloudCallbackImpl::onConnectionRecovery()
{
    if (nullptr != m_connectionRecovery)
        m_connectionRecovery();
}

void TRTCCloudCallbackImpl::onSpeedTest(const TRTCSpeedTestResult& currentResult, unsigned int finishedCount, unsigned int totalCount)
{
    if (nullptr != m_speedTest)
        m_speedTest(currentResult, finishedCount, totalCount);
}

void TRTCCloudCallbackImpl::onCameraDidReady()
{
    if (nullptr != m_cameraDidReady)
        m_cameraDidReady();
}

void TRTCCloudCallbackImpl::onMicDidReady()
{
    if (nullptr != m_micDidReady)
        m_micDidReady();
}

void TRTCCloudCallbackImpl::onDeviceChange(const char* deviceId, TRTCDeviceType type, TRTCDeviceState state)
{
    if (nullptr != m_deviceChange)
        m_deviceChange(deviceId, type, state);
}

void TRTCCloudCallbackImpl::onTestMicVolume(unsigned int volume)
{
    if (nullptr != m_testMicVolume)
        m_testMicVolume(volume);
}

void TRTCCloudCallbackImpl::onTestSpeakerVolume(unsigned int volume)
{
    if (nullptr != m_testSpeakerVolume)
        m_testSpeakerVolume(volume);
}

void TRTCCloudCallbackImpl::onAudioDeviceCaptureVolumeChanged(unsigned int volume, bool muted) {
    if (nullptr != m_audioDeviceCaptureVolumeChanged) {
        m_audioDeviceCaptureVolumeChanged(volume, muted);
    }
}

void TRTCCloudCallbackImpl::onAudioDevicePlayoutVolumeChanged(unsigned int volume, bool muted) {
    if (nullptr != m_audioDevicePlayoutVolumeChanged) {
        m_audioDevicePlayoutVolumeChanged(volume, muted);
    }
}

void TRTCCloudCallbackImpl::onRecvCustomCmdMsg(const char* userId, int cmdId, unsigned int seq, const unsigned char* msg, unsigned int msgSize)
{
    if (nullptr != m_recvCustomCmdMsg)
        m_recvCustomCmdMsg(userId, cmdId, seq, msg, msgSize);
}

void TRTCCloudCallbackImpl::onMissCustomCmdMsg(const char* userId, int  cmdId, int errCode, int missed)
{
    if (nullptr != m_missCustomCmdMsg)
        m_missCustomCmdMsg(userId, cmdId, errCode, missed);
}

void TRTCCloudCallbackImpl::onRecvSEIMsg(const char* userId, const unsigned char* message, unsigned int msgSize)
{
    if (nullptr != m_recvSEIMsg)
        m_recvSEIMsg(userId, message, msgSize);
}

void TRTCCloudCallbackImpl::onStartPublishing(int errCode, const char* errMsg)
{
    if (nullptr != m_startPublishing)
        m_startPublishing(errCode, errMsg);
}

void TRTCCloudCallbackImpl::onStopPublishing(int errCode, const char* errMsg)
{
    if (nullptr != m_stopPublishing)
        m_stopPublishing(errCode, errMsg);
}

void TRTCCloudCallbackImpl::onStartPublishCDNStream(int errCode, const char* errMsg)
{
    if (nullptr != m_startPublishCDNStream)
        m_startPublishCDNStream(errCode, errMsg);
}

void TRTCCloudCallbackImpl::onStopPublishCDNStream(int errCode, const char* errMsg)
{
    if (nullptr != m_stopPublishCDNStream)
        m_stopPublishCDNStream(errCode, errMsg);
}

void TRTCCloudCallbackImpl::onSetMixTranscodingConfig(int errCode, const char* errMsg)
{
    if (nullptr != m_setMixTranscodingConfig)
        m_setMixTranscodingConfig(errCode, errMsg);
}

void TRTCCloudCallbackImpl::onAudioEffectFinished(int effectId, int code)
{
    if (nullptr != m_audioEffectFinished)
        m_audioEffectFinished(effectId, code);
}

void TRTCCloudCallbackImpl::onScreenCaptureCovered()
{
    if (nullptr != m_screenCaptureCovered)
        m_screenCaptureCovered();
}

void TRTCCloudCallbackImpl::onScreenCaptureStarted()
{
    if (nullptr != m_screenCaptureStarted)
        m_screenCaptureStarted();
}

void TRTCCloudCallbackImpl::onScreenCapturePaused(int reason)
{
    if (nullptr != m_screenCapturePaused)
        m_screenCapturePaused(reason);
}

void TRTCCloudCallbackImpl::onScreenCaptureResumed(int reason)
{
    if (nullptr != m_screenCaptureResumed)
        m_screenCaptureResumed(reason);
}

void TRTCCloudCallbackImpl::onScreenCaptureStoped(int reason)
{
    if (nullptr != m_screenCaptureStoped)
        m_screenCaptureStoped(reason);
}

void TRTCCloudCallbackImpl::onSnapshotComplete(const char* userId, TRTCVideoStreamType type, char* data, uint32_t length, uint32_t width, uint32_t height, TRTCVideoPixelFormat format)
{
    if (nullptr != m_snapshotComplete)
        m_snapshotComplete(userId, type, data, length, width, height, format);
}

void TRTCCloudCallbackImpl::onLocalRecordBegin(int errCode, const char* storagePath)
{
    if (nullptr != m_localRecordBegin)
    {
        m_localRecordBegin(errCode, storagePath);
    }
}

void TRTCCloudCallbackImpl::onLocalRecording(long duration, const char* storagePath)
{
    if (nullptr != m_localRecording)
    {
        m_localRecording(duration, storagePath);
    }
}

void TRTCCloudCallbackImpl::onLocalRecordComplete(int errCode, const char* storagePath)
{
    if (nullptr != m_localRecordComplete)
    {
        m_localRecordComplete(errCode, storagePath);
    }
}

void TRTCCloudCallbackImpl::onRemoteVideoStatusUpdated(const char* userId, TRTCVideoStreamType streamType, TRTCAVStatusType status, TRTCAVStatusChangeReason reason,void* extrainfo)
{
    if (nullptr != m_remoteVideoStatusUpdated) {
        m_remoteVideoStatusUpdated(userId, streamType, status, reason, extrainfo);
    }
}
