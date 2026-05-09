using System;

namespace trtc {
    public abstract class IDeprecatedTRTCCloud {
        // 3.1
        [Obsolete("use startPublishMediaStream")]
        public abstract void startPublishing(string streamId, TRTCVideoStreamType streamType);
        // 3.2
        [Obsolete("use stopPublishMediaStream")]
        public abstract void stopPublishing();
        // 3.5
        [Obsolete("use startPublishMediaStream")]
        public abstract void setMixTranscodingConfig(TRTCTranscodingConfig? config);
        // 4.17
        [Obsolete("no use")]
        public abstract void setVideoEncoderRotation(TRTCVideoRotation rotation);
        // 4.18
        [Obsolete("no use")]
        public abstract void setVideoEncoderMirror(bool mirror);
        // 4.5
        [Obsolete("use muteLocalVideo(TRTCVideoStreamType,bool)")]
        public abstract void muteLocalVideo(bool mute);
        // 9.5
        [Obsolete("use getScreenCaptureSources(SIZE thumbnailSize, SIZE iconSize)")]
        public abstract TRTCScreenCaptureSourceInfo[] getScreenCaptureSources(int thumbnailWidth,
                                                                          int thumbnailHeight);
        // 5.12
        [Obsolete("use enableAudioVolumeEvaluation(bool,TRTCAudioVolumeEvaluateParams)")]
        public abstract void enableAudioVolumeEvaluation(uint interval);
        // 10.2
        [Obsolete("use sendCustomVideoData(TRTCVideoStreamType,TRTCVideoFrame)")]
        public abstract void sendCustomVideoData(TRTCVideoFrame frame);
        // 10.10
        [Obsolete("use setLocalVideoRenderCallback(TRTCVideoPixelFormat pixelFormat, TRTCVideoBufferType bufferType, ITRTCVideoRenderCallback callback)")]
        public abstract int setLocalVideoRenderCallback(TRTCVideoStreamType streamType,
                                                        TRTCVideoPixelFormat pixelFormat,
                                                        TRTCVideoBufferType bufferType,
                                                        ITRTCVideoRenderCallback callback);
        // 10.11
        [Obsolete("use setRemoteVideoRenderCallback(string userId, TRTCVideoPixelFormat pixelFormat, TRTCVideoBufferType bufferType, ITRTCVideoRenderCallback callback)")]
        public abstract int setRemoteVideoRenderCallback(string userId,
                                                    TRTCVideoStreamType streamType,
                                                    TRTCVideoPixelFormat pixelFormat,
                                                    TRTCVideoBufferType bufferType,
                                                    ITRTCVideoRenderCallback callback);
        // 12.1
        [Obsolete("startSpeedTest(TRTCSpeedTestParams)")]
        public abstract void startSpeedTest(int sdkAppId, string userId, string userSig);
    }
}