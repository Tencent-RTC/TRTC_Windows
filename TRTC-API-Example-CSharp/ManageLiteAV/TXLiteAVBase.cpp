// C++ Header
#include "include/TXLiteAVBase.h"

// C++/CLI Header
#include "cli/TXLiteAVBase.h"
#include "Utils.h"

using namespace System::Runtime::InteropServices;

namespace ManageLiteAV
{
    TXLiteAVBase::TXLiteAVBase() {

    }

    TXLiteAVBase::~TXLiteAVBase() {

    }

    TXLiteAVBase^ TXLiteAVBase::getInstance() {
        if (sInstance == nullptr) {
            msclr::lock l(slocker);
            if (sInstance == nullptr) {
                sInstance = gcnew TXLiteAVBase();
            }
        }
        return sInstance;
    }

    String^ TXLiteAVBase::getLiteAvSDKVersion() {
        const char* version = ::getLiteAvSDKVersion();
        String^ CSStrVersion = Utils::CharPtrToString(version);
        return CSStrVersion;
    }

    int TXLiteAVBase::setGlobalEnv(String^ env_config) {
        char* env = Utils::StringToUTF8CharPtr(env_config);
        int ret = ::setGlobalEnv(env);
        return ret;
    }

    int TXLiteAVBase::updateNetworkTime() {
        int ret = ::updateNetworkTime();
        return ret;
    }

    Int64 TXLiteAVBase::getNetworkTimestamp() {
        Int64 timeStamp = ::getNetworkTimestamp();
        return timeStamp;
    }

    void TXLiteAVBase::setUpdateNetworkTimeCallback(NetworkTimeCallback^ callback) {
        m_callback = callback;
        if (callback) {
            if (m_updateNetworkTimeDeleagate == nullptr) {
                m_updateNetworkTimeDeleagate = gcnew UpdateNetworkTimeDelegate(this, &TXLiteAVBase::onUpdateNetworkTimeDeleagate);
            }
            ::setUpdateNetworkTimeCallback((onUpdateNetworkTime)Marshal::GetFunctionPointerForDelegate(m_updateNetworkTimeDeleagate).ToPointer());
        } else {
            ::setUpdateNetworkTimeCallback(nullptr);
        }
    }

    void TXLiteAVBase::onUpdateNetworkTimeDeleagate(int errCode, const char* errMsg) {
        if (nullptr != m_callback) {
            String^ errorMsg = Utils::CharPtrToString(errMsg);
            m_callback->onUpdateNetworkTime(errCode, errorMsg);
        }
    }
}