#include "cli/Live/V2TXLivePusher.h"

#include "V2TXLivePusherObserverImpl.h"
#include "Utils.h"

using namespace System::Runtime::InteropServices;

namespace ManageLiteAV {
    ref class IV2TXLiveScreenCaptureSourceListImpl : IV2TXLiveScreenCaptureSourceList
    {
    public:
        ~IV2TXLiveScreenCaptureSourceListImpl() {}

        void setScreenCaptureSourceList(liteav::IV2TXLiveScreenCaptureSourceList* pointer)
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

        virtual V2TXLiveScreenCaptureSourceInfo^ getSourceInfo(UInt32 index)
        {
            if (nullptr != m_screenCaptureSourceList)
            {
                liteav::V2TXLiveScreenCaptureSourceInfo info = m_screenCaptureSourceList->getSourceInfo(index);
                V2TXLiveScreenCaptureSourceInfo^ temp = gcnew V2TXLiveScreenCaptureSourceInfo();
                temp->sourceType = static_cast<V2TXLiveScreenCaptureSourceType>(info.sourceType);
                temp->sourceId = IntPtr(info.sourceId);
                temp->sourceName = Utils::CharPtrToString(info.sourceName);
                temp->isMainScreen = info.isMainScreen;
                temp->isMinimizeWindow = info.isMinimizeWindow;

                V2TXLiveImage^ thumbTemp = gcnew V2TXLiveImage();
                thumbTemp->imageSrc = Utils::CharPtrToString(info.thumbBGRA.imageSrc);
                thumbTemp->imageHeight = info.thumbBGRA.imageHeight;
                thumbTemp->imageLength = info.thumbBGRA.imageLength;
                thumbTemp->imageWidth = info.thumbBGRA.imageWidth;

                V2TXLiveImage^ iconTemp = gcnew V2TXLiveImage();
                iconTemp->imageSrc = Utils::CharPtrToString(info.iconBGRA.imageSrc);
                iconTemp->imageHeight = info.iconBGRA.imageHeight;
                iconTemp->imageLength = info.iconBGRA.imageLength;
                iconTemp->imageWidth = info.iconBGRA.imageWidth;

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
        liteav::IV2TXLiveScreenCaptureSourceList* m_screenCaptureSourceList;
    };


    ref class IV2TXLiveScreenCaptureSourceListImpl;
    V2TXLivePusher::V2TXLivePusher(V2TXLiveMode mode)
        : m_pusher(nullptr)
        , m_V2TXLivePusherObserverImpl(nullptr)
        , m_ErrorDelegate(nullptr)
        , m_WarningDelegate(nullptr)
        , m_CaptureFirstAudioFrameDelegate(nullptr)
        , m_CaptureFirstVideoFrameDelegate(nullptr)
        , m_MicrophoneVolumeUpdateDelegate(nullptr)
        , m_PushStatusUpdateDelegate(nullptr)
        , m_StatisticsUpdateDelegate(nullptr)
        , m_SnapshotCompleteDelegate(nullptr)
        , m_RenderVideoFrameDelegate(nullptr)
        , m_ProcessVideoFrameDelegate(nullptr)
        , m_ScreenCaptureStartedDelegate(nullptr)
        , m_ScreenCaptureStopedDelegate(nullptr) 
        //, m_deviceManager(nullptr)
        , m_audioEffectManager(nullptr)
        , m_video_frame_cache(gcnew Dictionary<String^, V2TXLiveVideoFrame^>()) {
        liteav::V2TXLiveMode m = static_cast<liteav::V2TXLiveMode>(mode);
        m_pusher = ::createV2TXLivePusher(m);
    }

    V2TXLivePusher::~V2TXLivePusher() {
        ::releaseV2TXLivePusher(m_pusher);
        m_pusher = nullptr;
    }

    V2TXLivePusher^ V2TXLivePusher::createV2TXLivePusher(V2TXLiveMode mode) {
        V2TXLivePusher^ sInstance = gcnew V2TXLivePusher(mode);
        return sInstance;
    }

    void V2TXLivePusher::releaseV2TXLivePusher(V2TXLivePusher^ pusher) {
        pusher->~V2TXLivePusher();
    }

    void V2TXLivePusher::setObserver(V2TXLivePusherObserver^ observer) {
        m_observer = observer;
        if (m_observer && m_pusher)
        {
            if (nullptr == m_V2TXLivePusherObserverImpl)
            {
                m_V2TXLivePusherObserverImpl = new V2TXLivePusherObserverImpl();
            }

            if (m_V2TXLivePusherObserverImpl)
            {

                if (nullptr == m_ErrorDelegate) {
                    m_ErrorDelegate = gcnew ErrorDelegate(this, &V2TXLivePusher::onError);
                }
                m_V2TXLivePusherObserverImpl->setError((PFN_ERROR)Marshal::GetFunctionPointerForDelegate(m_ErrorDelegate).ToPointer());

                if (nullptr == m_WarningDelegate) {
                    m_WarningDelegate = gcnew WarningDelegate(this, &V2TXLivePusher::onWarning);
                }
                m_V2TXLivePusherObserverImpl->setWarning((PFN_WARNING)Marshal::GetFunctionPointerForDelegate(m_WarningDelegate).ToPointer());

                if (nullptr == m_CaptureFirstAudioFrameDelegate) {
                    m_CaptureFirstAudioFrameDelegate = gcnew CaptureFirstAudioFrameDelegate(this,&V2TXLivePusher::onCaptureFirstAudioFrame);
                }
                m_V2TXLivePusherObserverImpl->setCaptureFirstAudioFrame((PFN_CAPTUREFIRSTADUIOFRAME)Marshal::GetFunctionPointerForDelegate(m_CaptureFirstAudioFrameDelegate).ToPointer());

                if (nullptr == m_CaptureFirstVideoFrameDelegate) {
                    m_CaptureFirstVideoFrameDelegate = gcnew CaptureFirstVideoFrameDelegate(this, &V2TXLivePusher::onCaptureFirstVideoFrame);
                }
                m_V2TXLivePusherObserverImpl->setCaptureFirstVideoFrame((PFN_CAPTUREFIRSTVIDEOFRAME)Marshal::GetFunctionPointerForDelegate(m_CaptureFirstVideoFrameDelegate).ToPointer());

                if (nullptr == m_MicrophoneVolumeUpdateDelegate) {
                    m_MicrophoneVolumeUpdateDelegate = gcnew MicrophoneVolumeUpdateDelegate(this, &V2TXLivePusher::onMicrophoneVolumeUpdate);
                }
                m_V2TXLivePusherObserverImpl->setMicrophoneVolumeUpdate((PFN_MICROPHONEVOLUMEUPDATE)Marshal::GetFunctionPointerForDelegate(m_MicrophoneVolumeUpdateDelegate).ToPointer());

                if (nullptr == m_PushStatusUpdateDelegate) {
                    m_PushStatusUpdateDelegate = gcnew PushStatusUpdateDelegate(this, &V2TXLivePusher::onPushStatusUpdate);
                }
                m_V2TXLivePusherObserverImpl->setPushStatusUpdate((PFN_PUSHSTATUSUPDATE)Marshal::GetFunctionPointerForDelegate(m_PushStatusUpdateDelegate).ToPointer());

                if (nullptr == m_StatisticsUpdateDelegate) {
                    m_StatisticsUpdateDelegate = gcnew StatisticsUpdateDelegate(this, &V2TXLivePusher::onStatisticsUpdate);
                }
                m_V2TXLivePusherObserverImpl->setStatisticsUpdate((PFN_STATISTICEUPDATE)Marshal::GetFunctionPointerForDelegate(m_StatisticsUpdateDelegate).ToPointer());

                if (nullptr == m_SnapshotCompleteDelegate) {
                    m_SnapshotCompleteDelegate = gcnew SnapshotCompleteDelegate(this, &V2TXLivePusher::onSnapshotComplete);
                }
                m_V2TXLivePusherObserverImpl->setSnapshotComplete((PFN_SNAPSHOTCOMPLETE)Marshal::GetFunctionPointerForDelegate(m_SnapshotCompleteDelegate).ToPointer());

                if (nullptr == m_RenderVideoFrameDelegate) {
                    m_RenderVideoFrameDelegate = gcnew RenderVideoFrameDelegate(this, &V2TXLivePusher::onRenderVideoFrame);
                }
                m_V2TXLivePusherObserverImpl->setRenderVideoFrame((PFN_RENDERVIDEOFRAME)Marshal::GetFunctionPointerForDelegate(m_RenderVideoFrameDelegate).ToPointer());

                if (nullptr == m_ProcessVideoFrameDelegate) {
                    m_ProcessVideoFrameDelegate = gcnew ProcessVideoFrameDelegate(this, &V2TXLivePusher::onProcessVideoFrame);
                }
                m_V2TXLivePusherObserverImpl->setProcessVideoFrame((PFN_PROCESSVIDEOFRAME)Marshal::GetFunctionPointerForDelegate(m_ProcessVideoFrameDelegate).ToPointer());

                if (nullptr == m_ScreenCaptureStartedDelegate) {
                    m_ScreenCaptureStartedDelegate = gcnew ScreenCaptureStartedDelegate(this, &V2TXLivePusher::onScreenCaptureStarted);
                }
                m_V2TXLivePusherObserverImpl->setScreenCaptureStarted((PFN_SCREENCAPTURESTARTED)Marshal::GetFunctionPointerForDelegate(m_ScreenCaptureStartedDelegate).ToPointer());

                if (nullptr == m_ScreenCaptureStopedDelegate) {
                    m_ScreenCaptureStopedDelegate = gcnew ScreenCaptureStopedDelegate(this, &V2TXLivePusher::onScreenCaptureStoped);
                }
                m_V2TXLivePusherObserverImpl->setScreenCaptureStoped((PFN_SCREENCAPTURESTOPED)Marshal::GetFunctionPointerForDelegate(m_ScreenCaptureStopedDelegate).ToPointer());
                m_pusher->setObserver(m_V2TXLivePusherObserverImpl);
            }
        }
        else
        {
            m_pusher->setObserver(nullptr);
        }
    }

    Int32 V2TXLivePusher::setRenderView(IntPtr view) {
        if (m_pusher)
        {
            return m_pusher->setRenderView(view.ToPointer());
        }
        return -1;
    }

    Int32 V2TXLivePusher::setRenderMirror(V2TXLiveMirrorType mirrorType) {
        if (m_pusher)
        {
            liteav::V2TXLiveMirrorType param = static_cast<liteav::V2TXLiveMirrorType>(mirrorType);
            return m_pusher->setRenderMirror(param);
        }
        return -1;
    }

    Int32 V2TXLivePusher::setEncoderMirror(bool mirror) {
        if (m_pusher)
        {
            return m_pusher->setEncoderMirror(mirror);
        }
        return -1;
    }

    Int32 V2TXLivePusher::setRenderRotation(V2TXLiveRotation rotation) {
        if (m_pusher)
        {
            liteav::V2TXLiveRotation param = static_cast<liteav::V2TXLiveRotation>(rotation);
            return m_pusher->setRenderRotation(param);
        }
        return -1;
    }

    Int32 V2TXLivePusher::startCamera(String^ cameraId) {
        if (m_pusher)
        {
            char* id = Utils::StringToUTF8CharPtr(cameraId);
            return m_pusher->startCamera(id);
            delete[] id;
            id = nullptr;
        }
        return -1;
    }

    Int32 V2TXLivePusher::stopCamera() {
        if (m_pusher)
        {
            return m_pusher->stopCamera();
        }
        return -1;
    }

    Int32 V2TXLivePusher::startMicrophone() {
        if (m_pusher)
        {
            return m_pusher->startMicrophone();
        }
        return -1;
    }

    Int32 V2TXLivePusher::stopMicrophone() {
        if (m_pusher)
        {
            return m_pusher->stopMicrophone();
        }
        return -1;
    }

    Int32 V2TXLivePusher::startVirtualCamera(V2TXLiveImage^ image) {
        if (m_pusher)
        {
            char* src = Utils::StringToUTF8CharPtr(image->imageSrc);
            liteav::V2TXLiveImage param;
            param.imageWidth = image->imageWidth;
            param.imageHeight = image->imageHeight;
            param.imageLength = image->imageLength;
            param.imageSrc = src;
            param.imageType = static_cast<liteav::V2TXLiveImageType>(image->imageType);
            int32_t ret = m_pusher->startVirtualCamera(&param);
            delete[] src;
            src = nullptr;
            return ret;
        }
        return -1;
    }

    Int32 V2TXLivePusher::stopVirtualCamera() {
        if (m_pusher)
        {
            return m_pusher->stopVirtualCamera();
        }
        return -1;
    }

    Int32 V2TXLivePusher::pauseAudio() {
        if (m_pusher)
        {
            return m_pusher->pauseAudio();
        }
        return -1;
    }

    Int32 V2TXLivePusher::resumeAudio() {
        if (m_pusher)
        {
            return m_pusher->resumeAudio();
        }
        return -1;
    }

    Int32 V2TXLivePusher::pauseVideo() {
        if (m_pusher)
        {
            return m_pusher->pauseVideo();
        }
        return -1;
    }

    Int32 V2TXLivePusher::resumeVideo() {
        if (m_pusher)
        {
            return m_pusher->resumeVideo();
        }
        return -1;
    }

    Int32 V2TXLivePusher::startPush(String^ url) {
        if (m_pusher)
        {
            char* str = Utils::StringToUTF8CharPtr(url);
            int32_t ret = m_pusher->startPush(str);
            delete[] str;
            str = nullptr;
            return ret;
        }
        return -1;
    }

    Int32 V2TXLivePusher::stopPush() {
        if (m_pusher)
        {
            return m_pusher->stopPush();
        }
        return -1;
    }

    Int32 V2TXLivePusher::isPushing() {
        if (m_pusher)
        {
            return m_pusher->isPushing();
        }
        return -1;
    }

    Int32 V2TXLivePusher::setAudioQuality(V2TXLiveAudioQuality quality) {
        if (m_pusher)
        {
            liteav::V2TXLiveAudioQuality param = static_cast<liteav::V2TXLiveAudioQuality>(quality);
            return m_pusher->setAudioQuality(param);
        }
        return -1;
    }

    Int32 V2TXLivePusher::setVideoQuality(V2TXLiveVideoEncoderParam^ param) {
        if (m_pusher)
        {
            liteav::V2TXLiveVideoResolution resolution = static_cast<liteav::V2TXLiveVideoResolution>(param->videoResolution);
            liteav::V2TXLiveVideoEncoderParam quality(resolution);
            quality.minVideoBitrate = param->minVideoBitrate;
            quality.videoBitrate = param->videoBitrate;
            quality.videoFps = param->videoFps;
            quality.videoResolution = resolution;
            quality.videoResolutionMode = static_cast<liteav::V2TXLiveVideoResolutionMode>(param->videoResolutionMode);
            return m_pusher->setVideoQuality(quality);
        }
        return -1;
    }

    ITXAudioEffectManager^ V2TXLivePusher::getAudioEffectManager() {
        if (m_pusher)
        {
            if (!m_audioEffectManager)
            {
                liteav::ITXAudioEffectManager* manager = m_pusher->getAudioEffectManager();
                m_audioEffectManager = gcnew ITXAudioEffectManager(manager);
            }
        }
        return m_audioEffectManager;
    }

    ITXDeviceManager^ V2TXLivePusher::getDeviceManager() {
        if (m_pusher)
        {
            if (!m_deviceManager)
            {
                liteav::ITXDeviceManager *manager = m_pusher->getDeviceManager();
                m_deviceManager = gcnew ITXDeviceManager(manager);
            }
        }
        return m_deviceManager;
    }

    Int32 V2TXLivePusher::snapshot() {
        if (m_pusher)
        {
            return m_pusher->snapshot();
        }
        return -1;
    }

    Int32 V2TXLivePusher::setWatermark(String^ watermarkPath, float x, float y, float scale) {
        if (m_pusher)
        {
            char* path = Utils::StringToUTF8CharPtr(watermarkPath);
            int32_t ret = m_pusher->setWatermark(path,x,y, scale);
            delete[] path;
            path = nullptr;
            return ret;
        }
        return -1;
    }

    Int32 V2TXLivePusher::enableVolumeEvaluation(Int32 intervalMs) {
        if (m_pusher)
        {
            return m_pusher->enableVolumeEvaluation(intervalMs);
        }
        return -1;
    }

    Int32 V2TXLivePusher::enableCustomVideoCapture(bool enable) {
        if (m_pusher)
        {
            return m_pusher->enableCustomVideoCapture(enable);
        }
        return -1;
    }

    Int32 V2TXLivePusher::enableCustomVideoProcess(bool enable, V2TXLivePixelFormat pixelFormat, V2TXLiveBufferType bufferType) {
        if (m_pusher)
        {
            liteav::V2TXLivePixelFormat pixel = static_cast<liteav::V2TXLivePixelFormat>(pixelFormat);
            liteav::V2TXLiveBufferType type = static_cast<liteav::V2TXLiveBufferType>(bufferType);
            return m_pusher->enableCustomVideoProcess(enable, pixel, type);
        }
        return -1;
    }

    Int32 V2TXLivePusher::enableCustomVideoRender(bool enable, V2TXLivePixelFormat pixelFormat, V2TXLiveBufferType bufferType) {
        if (m_pusher)
        {
            liteav::V2TXLivePixelFormat pixel = static_cast<liteav::V2TXLivePixelFormat>(pixelFormat);
            liteav::V2TXLiveBufferType type = static_cast<liteav::V2TXLiveBufferType>(bufferType);
            return m_pusher->enableCustomVideoRender(enable, pixel, type);
        }
        return -1;
    }

    Int32 V2TXLivePusher::enableCustomAudioCapture(bool enable) {
        if (m_pusher)
        {
            return m_pusher->enableCustomAudioCapture(enable);
        }
        return -1;
    }

    Int32 V2TXLivePusher::sendCustomVideoFrame(V2TXLiveVideoFrame^ videoFrame) {
        if (m_pusher)
        {
            liteav::V2TXLiveVideoFrame temp;
            temp.bufferType = static_cast<liteav::V2TXLiveBufferType>(videoFrame->bufferType);
            pin_ptr<System::Byte> p = &videoFrame->data[0];
            unsigned char* pby = p;
            temp.data = reinterpret_cast<char*>(pby);
            temp.width = videoFrame->width;
            temp.height = videoFrame->height;
            temp.length = videoFrame->length;
            temp.rotation = static_cast<liteav::V2TXLiveRotation>(videoFrame->rotation);
            temp.pixelFormat = static_cast<liteav::V2TXLivePixelFormat>(videoFrame->pixelFormat);
            return m_pusher->sendCustomVideoFrame(&temp);
        }
        return -1;
    }

    Int32 V2TXLivePusher::sendCustomAudioFrame(V2TXLiveAudioFrame^ audioFrame) {
        if (m_pusher)
        {
            liteav::V2TXLiveAudioFrame temp;
            pin_ptr<System::Byte> p = &audioFrame->data[0];
            unsigned char* pby = p;
            temp.data = reinterpret_cast<char*>(pby);
            temp.length = audioFrame->length;
            temp.sampleRate = audioFrame->sampleRate;
            temp.channel = audioFrame->channel;
            return m_pusher->sendCustomAudioFrame(&temp);
        }
        return -1;
    }

    Int32 V2TXLivePusher::sendSeiMessage(int payloadType, array<Byte>^ data, UInt32 dataSize) {
        if (m_pusher)
        {
            pin_ptr<System::Byte> p = &data[0];
            unsigned char* pby = p;
            return m_pusher->sendSeiMessage(payloadType, pby, dataSize);
        }
        return -1;
    }

    Int32 V2TXLivePusher::startScreenCapture() {
        if (m_pusher)
        {
            return m_pusher->startScreenCapture();
        }
        return -1;
    }

    Int32 V2TXLivePusher::stopScreenCapture() {
        if (m_pusher)
        {
            return m_pusher->stopScreenCapture();
        }
        return -1;
    }

    Int32 V2TXLivePusher::startSystemAudioLoopback(String^ path) {
        if (m_pusher)
        {
            char* temp = Utils::StringToUTF8CharPtr(path);
            int32_t ret = m_pusher->startSystemAudioLoopback(temp);
            return ret;
        }
        return -1;
    }

    Int32 V2TXLivePusher::stopSystemAudioLoopback() {
        if (m_pusher)
        {
            return m_pusher->stopSystemAudioLoopback();
        }
        return -1;
    }

    IV2TXLiveScreenCaptureSourceList^ V2TXLivePusher::getScreenCaptureSources(V2TXLiveSize^ thumbSize, V2TXLiveSize^ iconSize) {
        if (m_pusher)
        {
            liteav::V2TXLiveSize thumbSizeTemp, iconSizeTemp;
            if (nullptr != thumbSize)
            {
                thumbSizeTemp.width = thumbSize->width;
                thumbSizeTemp.height = thumbSize->height;
            }
            if (nullptr != iconSize)
            {
                iconSizeTemp.width = iconSize->width;
                iconSizeTemp.height = iconSize->height;
            }
            liteav::IV2TXLiveScreenCaptureSourceList* info = m_pusher->getScreenCaptureSources(thumbSizeTemp, iconSizeTemp);
            if (nullptr != info)
            {
                IV2TXLiveScreenCaptureSourceListImpl^ impl = gcnew IV2TXLiveScreenCaptureSourceListImpl();
                impl->setScreenCaptureSourceList(info);
                return impl;
            }
        }
        return nullptr;
    }

    Int32 V2TXLivePusher::setScreenCaptureSource(V2TXLiveScreenCaptureSourceInfo^ source, V2TXLiveRect^ captureRect, V2TXLiveScreenCaptureProperty^ property) {
        if (m_pusher)
        {
            liteav::V2TXLiveScreenCaptureSourceInfo info;
            info.sourceId = source->sourceId.ToPointer();
            info.sourceName = Utils::StringToUTF8CharPtr(source->sourceName);
            info.isMainScreen = source->isMainScreen;
            info.isMinimizeWindow = source->isMinimizeWindow;
            info.sourceType = static_cast<liteav::V2TXLiveScreenCaptureSourceType>(source->sourceType);
            
            info.thumbBGRA.imageSrc = Utils::StringToUTF8CharPtr(source->thumbBGRA->imageSrc);
            info.thumbBGRA.imageLength = source->thumbBGRA->imageLength;
            info.thumbBGRA.imageType = static_cast<liteav::V2TXLiveImageType>(source->thumbBGRA->imageType);
            info.thumbBGRA.imageWidth = source->thumbBGRA->imageWidth;
            info.thumbBGRA.imageHeight = source->thumbBGRA->imageHeight;

            info.iconBGRA.imageSrc = Utils::StringToUTF8CharPtr(source->iconBGRA->imageSrc);
            info.iconBGRA.imageLength = source->iconBGRA->imageLength;
            info.iconBGRA.imageType = static_cast<liteav::V2TXLiveImageType>(source->iconBGRA->imageType);
            info.iconBGRA.imageWidth = source->iconBGRA->imageWidth;
            info.iconBGRA.imageHeight = source->iconBGRA->imageHeight;

            liteav::V2TXLiveRect rect;
            rect.left = captureRect->left;
            rect.right = captureRect->right;
            rect.top = captureRect->top;
            rect.bottom = captureRect->bottom;
        
            liteav::V2TXLiveScreenCaptureProperty p;
            p.enableCaptureChildWindow = property->enableCaptureChildWindow;
            p.enableCaptureMouse = property->enableCaptureMouse;
            p.enableHighlightBorder = property->enableHighlightBorder;
            p.enableHighPerformance = property->enableHighPerformance;
            p.highlightBorderColor = property->highlightBorderColor;
            p.highlightBorderSize = property->highlightBorderSize;

            int32_t ret = m_pusher->setScreenCaptureSource(info, rect, p);
            delete[] info.sourceName;
            info.sourceName = nullptr;
            delete[] info.thumbBGRA.imageSrc;
            info.thumbBGRA.imageSrc = nullptr;
            delete[] info.iconBGRA.imageSrc;
            info.iconBGRA.imageSrc = nullptr;
            return ret;
        }
        return -1;
    }

    void V2TXLivePusher::showDebugView(bool isShow) {
        if (m_pusher)
        {
            m_pusher->showDebugView(isShow);
        }
    }

    void V2TXLivePusher::onError(int32_t code, const char* msg, void* extraInfo){
        if (m_observer)
        {
            m_observer->onError(code, Utils::CharPtrToString(msg), IntPtr(extraInfo));
        }
    }

    void V2TXLivePusher::onWarning(int32_t code, const char* msg, void* extraInfo) {
        if (m_observer)
        {
            m_observer->onWarning(code, Utils::CharPtrToString(msg), IntPtr(extraInfo));
        }
    }

    void V2TXLivePusher::onCaptureFirstAudioFrame() {
        if (m_observer)
        {
            m_observer->onCaptureFirstAudioFrame();
        }
    }

    void V2TXLivePusher::onCaptureFirstVideoFrame() {
        if (m_observer)
        {
            m_observer->onCaptureFirstVideoFrame();
        }
    }

    void V2TXLivePusher::onMicrophoneVolumeUpdate(int32_t volume) {
        if (m_observer)
        {
            m_observer->onMicrophoneVolumeUpdate(volume);
        }
    }

    void V2TXLivePusher::onPushStatusUpdate(liteav::V2TXLivePushStatus state, const char* msg, void* extraInfo) {
        if (m_observer)
        {
            V2TXLivePushStatus temp = static_cast<V2TXLivePushStatus>(state);
            m_observer->onPushStatusUpdate(temp, Utils::CharPtrToString(msg), IntPtr(extraInfo));
        }
    }

    void V2TXLivePusher::onStatisticsUpdate(liteav::V2TXLivePusherStatistics statistics) {
        if (m_observer)
        {
            V2TXLivePusherStatistics^ stat = gcnew V2TXLivePusherStatistics();
            stat->appCpu = statistics.appCpu;
            stat->systemCpu = statistics.systemCpu;
            stat->width = statistics.width;
            stat->height = statistics.height;
            stat->fps = statistics.fps;
            stat->videoBitrate = statistics.videoBitrate;
            stat->audioBitrate = statistics.audioBitrate;
            m_observer->onStatisticsUpdate(stat);
        }
    }

    void V2TXLivePusher::onSnapshotComplete(const char* image, int length, int width, int height, liteav::V2TXLivePixelFormat format) {
        if (m_observer)
        {
        V2TXLivePixelFormat temp = static_cast<V2TXLivePixelFormat>(format);
        m_observer->onSnapshotComplete(Utils::CharPtrToString(image), length, width, height, temp);
        }
    }

    void V2TXLivePusher::onRenderVideoFrame(const liteav::V2TXLiveVideoFrame* videoFrame) {
        if (m_observer)
        {
            String^ key = "render";
            V2TXLiveVideoFrame^ frame = nullptr;
            {
                msclr::lock l(m_video_frame_cache);
                if (m_video_frame_cache->TryGetValue(key, frame)) {
                    if (frame->length != videoFrame->length) {
                        frame->width = videoFrame->width;
                        frame->height = videoFrame->height;
                        frame->length = videoFrame->length;
                        frame->data = gcnew array<Byte>(videoFrame->length);
                    }
                }
                else
                {
                    frame = gcnew V2TXLiveVideoFrame();
                    frame->width = videoFrame->width;
                    frame->height = videoFrame->height;
                    frame->length = videoFrame->length;
                    frame->data = gcnew array<Byte>(videoFrame->length);
                    m_video_frame_cache->Add(key, frame);
                }
            }

            frame->pixelFormat = static_cast<ManageLiteAV::V2TXLivePixelFormat>(videoFrame->pixelFormat);
            frame->bufferType = static_cast<ManageLiteAV::V2TXLiveBufferType>(videoFrame->bufferType);
            frame->rotation = static_cast<ManageLiteAV::V2TXLiveRotation>(videoFrame->rotation);

            Marshal::Copy(IntPtr((void*)videoFrame->data), frame->data, 0, videoFrame->length);
            frame->width = videoFrame->width;
            frame->height = videoFrame->height;
            m_observer->onRenderVideoFrame(frame);
        }
    }

    int  V2TXLivePusher::onProcessVideoFrame(liteav::V2TXLiveVideoFrame* srcFrame, liteav::V2TXLiveVideoFrame* dstFrame) {
        if (m_observer)
        {
            V2TXLiveVideoFrame^ srcVideoFrame, ^ dstVideoFrame;

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
                    srcVideoFrame = gcnew V2TXLiveVideoFrame();
                    srcVideoFrame->width = srcFrame->width;
                    srcVideoFrame->height = srcFrame->height;
                    srcVideoFrame->length = srcFrame->length;
                    srcVideoFrame->data = gcnew array<Byte>(srcFrame->length);
                    m_video_frame_cache->Add(key, srcVideoFrame);
                }
            }
            srcVideoFrame->bufferType = static_cast<V2TXLiveBufferType>(srcFrame->bufferType);
            srcVideoFrame->rotation = static_cast<V2TXLiveRotation>(srcFrame->rotation);
            srcVideoFrame->pixelFormat = static_cast<V2TXLivePixelFormat>(srcFrame->pixelFormat);
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
                    dstVideoFrame = gcnew V2TXLiveVideoFrame();
                    dstVideoFrame->width = dstFrame->width;
                    dstVideoFrame->height = dstFrame->height;
                    dstVideoFrame->length = dstFrame->length;
                    dstVideoFrame->data = gcnew array<Byte>(dstFrame->length);
                    m_video_frame_cache->Add(key, dstVideoFrame);
                }
            }
            int ret = m_observer->onProcessVideoFrame(srcVideoFrame, dstVideoFrame);

            dstFrame->bufferType = static_cast<::V2TXLiveBufferType>(dstVideoFrame->bufferType);
            dstFrame->rotation = static_cast<::V2TXLiveRotation>(dstVideoFrame->rotation);
            dstFrame->pixelFormat = static_cast<::V2TXLivePixelFormat>(dstVideoFrame->pixelFormat);
            dstFrame->width = dstVideoFrame->width;
            dstFrame->height = dstVideoFrame->height;
            dstFrame->length = dstVideoFrame->length;
            pin_ptr<Byte> pByte = &dstVideoFrame->data[0];
            memcpy(dstFrame->data, pByte, dstVideoFrame->data->Length);
            return ret;
        }
    }

    void V2TXLivePusher::onScreenCaptureStarted() {
        if (m_observer)
        {
            m_observer->onScreenCaptureStarted();
        }
    }

    void V2TXLivePusher::onScreenCaptureStoped(int reason) {
        if (m_observer)
        {
            m_observer->onScreenCaptureStoped(reason);
        }
    }
}