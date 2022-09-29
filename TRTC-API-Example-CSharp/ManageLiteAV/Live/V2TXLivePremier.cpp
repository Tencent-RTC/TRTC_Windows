#include "cli/Live/V2TXLivePremier.h"
#include "Utils.h"

using namespace System::Runtime::InteropServices;

class V2TXLivePremierObserverImpl;

typedef void (CALLBACK* PFN_LOG)(liteav::V2TXLiveLogLevel, const char*);

typedef void (CALLBACK* PFN_CAPTUREAUDIOFRAME)(liteav::V2TXLiveAudioFrame*);

class V2TXLivePremierObserverImpl
    : public liteav::V2TXLivePremierObserver
{
public:
    V2TXLivePremierObserverImpl()
        : m_Log(nullptr)
        , m_CaptureAudioFrame(nullptr) {
    }

    ~V2TXLivePremierObserverImpl() {

    }

    void setLog(PFN_LOG pointer) {
        m_Log = pointer;
    }

    void onLog(liteav::V2TXLiveLogLevel level, const char* log) {
        if (m_Log)
        {
            m_Log(level, log);
        }
    }

    void setCaptureAudioFrame(PFN_CAPTUREAUDIOFRAME pointer) {
        m_CaptureAudioFrame = pointer;
    }

    void onCaptureAudioFrame(liteav::V2TXLiveAudioFrame* frame) {
        if (m_CaptureAudioFrame)
        {
            m_CaptureAudioFrame(frame);
        }
    }
private:
    PFN_LOG m_Log;
    PFN_CAPTUREAUDIOFRAME m_CaptureAudioFrame;
};

namespace ManageLiteAV {
    V2TXLivePremier::V2TXLivePremier()
        : m_CaptureAudioFrameDelegate(nullptr)
        , m_observer(nullptr)
        , m_LogDelegate(nullptr)
        , m_frame(nullptr)
        , m_V2TXLivePremierObserverImpl(nullptr)
        , m_LivePremier(nullptr)
    {
        m_LivePremier = liteav::V2TXLivePremier::getV2TXLivePremierShareInstance();
    }

    V2TXLivePremier::~V2TXLivePremier() {
        if (m_V2TXLivePremierObserverImpl)
        {
            delete m_V2TXLivePremierObserverImpl;
            m_V2TXLivePremierObserverImpl = nullptr;
        }

        liteav::V2TXLivePremier::destroyV2TXLivePremierShareInstance();
    }


    V2TXLivePremier^ V2TXLivePremier::getV2TXLivePremierShareInstance() {
        if (sInstance == nullptr)
        {
            msclr::lock l(slocker);
            if (sInstance == nullptr)
            {
                sInstance = gcnew V2TXLivePremier();
            }
        }
        return sInstance;
    }

    void V2TXLivePremier::destroyV2TXLivePremierShareInstance() {
        if (sInstance)
        {
            sInstance = nullptr;
        }
    }

    String^% V2TXLivePremier::getSDKVersionStr() {
        if (m_LivePremier)
        {
            const char* str = m_LivePremier->getSDKVersionStr();
            String^ version = Utils::CharPtrToString(str);
            return version;
        }
        String^ str = gcnew String("");
        return str;
    }

    void V2TXLivePremier::setObserver(V2TXLivePremierObserver^ observer) {
        m_observer = observer;
        if (observer && m_LivePremier)
        {
            if (nullptr == m_V2TXLivePremierObserverImpl)
            {
                m_V2TXLivePremierObserverImpl = new V2TXLivePremierObserverImpl;
            }

            if (!m_LogDelegate)
            {
                m_LogDelegate = gcnew LogDelegate(this, &V2TXLivePremier::onError);
                m_V2TXLivePremierObserverImpl->setLog((PFN_LOG)Marshal::GetFunctionPointerForDelegate(m_LogDelegate).ToPointer());
            }
            if (!m_CaptureAudioFrameDelegate)
            {
                m_CaptureAudioFrameDelegate = gcnew CaptureAudioFrameDelegate(this, &V2TXLivePremier::onCaptureAudioFrame);
                m_V2TXLivePremierObserverImpl->setCaptureAudioFrame((PFN_CAPTUREAUDIOFRAME)Marshal::GetFunctionPointerForDelegate(m_CaptureAudioFrameDelegate).ToPointer());
            }
            m_LivePremier->setObserver(m_V2TXLivePremierObserverImpl);
        }
        else
        {
            m_LivePremier->setObserver(nullptr);
        }
    }

    Int32 V2TXLivePremier::setLogConfig(V2TXLiveLogConfig^ config) {
        int32_t ret = -1;
        if (m_LivePremier)
        {
            liteav::V2TXLiveLogConfig config_;
            config_.enableConsole = config->enableConsole;
            config_.enableLogFile = config->enableLogFile;
            config_.enableObserver = config->enableObserver;
            config_.logLevel = static_cast<liteav::V2TXLiveLogLevel>(config->logLevel);
            config_.logPath = Utils::StringToUTF8CharPtr(config->logPath);
            ret = m_LivePremier->setLogConfig(config_);
            delete[] config_.logPath;
            config_.logPath = nullptr;
        }
        return ret;
    }

    Int32 V2TXLivePremier::setEnvironment(String^ env) {
        int32_t ret = -1;
        if (m_LivePremier)
        {
            const char* env_ = Utils::StringToUTF8CharPtr(env);
            ret = m_LivePremier->setEnvironment(env_);
            delete[] env_;
            env_ = nullptr;
        }
        return ret;
    }

    Int32 V2TXLivePremier::setSocks5Proxy(String^ host, UInt16 port, String^ username, String^ password) {
        int32_t ret = -1;
        if (m_LivePremier)
        {
            const char* host_ = Utils::StringToUTF8CharPtr(host);
            uint16_t port_ = port;
            const char* username_ = Utils::StringToUTF8CharPtr(username);
            const char* password_ = Utils::StringToUTF8CharPtr(password);
            ret = m_LivePremier->setSocks5Proxy(host_, port_, username_, password_);
            delete[] host_;
            delete[] username_;
            delete[] password_;
            host_ = nullptr;
            username_ = nullptr;
            password_ = nullptr;
        }
        return ret;
    }

    Int32 V2TXLivePremier::enableAudioCaptureObserver(bool enable, V2TXLiveAudioFrameObserverFormat^ format) {
        int32_t ret = -1;
        if (m_LivePremier) {
            liteav::V2TXLiveAudioFrameObserverFormat format_;
            format_.channel = format->channel;
            format_.sampleRate = format->sampleRate;
            format_.samplesPerCall = format->samplesPerCall;
            ret = m_LivePremier->enableAudioCaptureObserver(enable, format_);
        }
        return ret;
    }

    void V2TXLivePremier::onError(liteav::V2TXLiveLogLevel level, const char* log) {
        if (m_observer)
        {
            ManageLiteAV::V2TXLiveLogLevel level_ = static_cast<ManageLiteAV::V2TXLiveLogLevel>(level);
            String^ log_ = Utils::CharPtrToString(log);
            m_observer->onLog(level_, log_);
        }
    }

    void V2TXLivePremier::onCaptureAudioFrame(liteav::V2TXLiveAudioFrame* frame) {
        if (m_observer)
        {
            if (!m_frame)
            {
                m_frame = gcnew ManageLiteAV::V2TXLiveAudioFrame;
            }
            if (!m_frame->data || m_frame->length != frame->length)
            {
                m_frame->data = gcnew array<Byte>(frame->length);
            }

            Marshal::Copy(IntPtr((void*)frame->data), m_frame->data, 0, frame->length);
            m_frame->length = frame->length;
            m_frame->sampleRate = frame->sampleRate;
            m_frame->channel = frame->channel;
            m_observer->onCaptureAudioFrame(m_frame);
        }
    }
}