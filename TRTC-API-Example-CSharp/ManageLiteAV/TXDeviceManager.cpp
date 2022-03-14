#include "Utils.h"

// C++ Header
#include "TRTC/ITRTCCloud.h"
#include "ITXDeviceManager.h"

// C++/CLI Header
#include "cli/ITRTCDeviceManager.h"

#include <assert.h>
#include <string>

using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace ManageLiteAV {

    ref class TRTCDeviceCollectionImpl : public ITRTCDeviceCollection {
        public:
            ~TRTCDeviceCollectionImpl() {}

            void setDeviceCollection(liteav::ITXDeviceCollection* pointer) {
                device_collection_ = pointer;
            }

            virtual UInt32 getCount() {
                if (nullptr != device_collection_)
                    return device_collection_->getCount();
                else
                    return 0;
            }

            virtual String ^ getDeviceName(UInt32 index) {
                if (nullptr != device_collection_)
                    return Utils::CharPtrToString(device_collection_->getDeviceName(index));
                else
                    return nullptr;
            }

            virtual String ^ getDevicePID(UInt32 index) {
                if (nullptr != device_collection_)
                    return Utils::CharPtrToString(device_collection_->getDevicePID(index));
                else
                    return nullptr;
            }

            virtual void release() {
                if (nullptr != device_collection_)
                    device_collection_->release();
            }

        private:
            liteav::ITXDeviceCollection* device_collection_;
    };

    ref class TRTCDeviceInfoImpl : ITRTCDeviceInfo {
        public:
            ~TRTCDeviceInfoImpl() {}

        void setDeviceInfo(liteav::ITXDeviceInfo* pointer) {
            device_info_ = pointer;
        }

        virtual String ^ getDeviceName() {
            if (nullptr != device_info_)
                return Utils::CharPtrToString(device_info_->getDeviceName());
            else
                return nullptr;
        }
        
        virtual String ^ getDevicePID() {
            if (nullptr != device_info_)
                return Utils::CharPtrToString(device_info_->getDevicePID());
            else
                return nullptr;
        }

        virtual void release() {
            if (nullptr != device_info_)
                device_info_->release();
        }

        private:
        liteav::ITXDeviceInfo* device_info_;
    };

    ITXDeviceManager::ITXDeviceManager() 
        : device_manager_(::getTRTCShareInstance()->getDeviceManager())
        , m_ObserverImpl(nullptr)
        , m_callback(nullptr)
        , m_cb_locker(gcnew Object())
    {
        assert(nullptr != device_manager_);
    }

    ITXDeviceManager::~ITXDeviceManager() {
        if (m_ObserverImpl)
        {
            delete m_ObserverImpl;
            m_ObserverImpl = nullptr;
        }
    }

    ITRTCDeviceCollection ^ ITXDeviceManager::getDevicesList(TRTCDeviceType type) {
        if (nullptr != device_manager_) {
            liteav::ITRTCDeviceCollection* collection =
                device_manager_->getDevicesList(static_cast<liteav::TRTCDeviceType>(type));
            if (nullptr != collection) {
                TRTCDeviceCollectionImpl ^ impl = gcnew TRTCDeviceCollectionImpl();
                impl->setDeviceCollection(collection);
                return impl;
            }
        }
        return nullptr;
    }

    int ITXDeviceManager::setCurrentDevice(TRTCDeviceType type, String ^ deviceId) {
        int result = -1;
        if (nullptr != device_manager_) {
            char * device_id = Utils::StringToUTF8CharPtr(deviceId);
            result = device_manager_->setCurrentDevice(static_cast<liteav::TRTCDeviceType>(type), device_id);
            delete[] device_id;
            device_id = nullptr;
        }
        return result;
    }

    ITRTCDeviceInfo ^ ITXDeviceManager::getCurrentDevice(TRTCDeviceType type) {
        if (nullptr != device_manager_) {
            liteav::ITRTCDeviceInfo* info =
                device_manager_->getCurrentDevice(static_cast<liteav::TRTCDeviceType>(type));
            if (nullptr != info) {
                TRTCDeviceInfoImpl ^ impl = gcnew TRTCDeviceInfoImpl();
                impl->setDeviceInfo(info);
                return impl;
            }
        }
        return nullptr;
    }

    int ITXDeviceManager::setCurrentDeviceVolume(TRTCDeviceType type, UInt32 volume) {
        if (nullptr != device_manager_) {
            return device_manager_->setCurrentDeviceVolume(static_cast<liteav::TRTCDeviceType>(type),
                                                             volume);
        }
        return -1;
    }

    UInt32 ITXDeviceManager::getCurrentDeviceVolume(TRTCDeviceType type) {
        if (nullptr != device_manager_) {
            return device_manager_->getCurrentDeviceVolume(static_cast<liteav::TRTCDeviceType>(type));
        }
        return 0;
    }

    int ITXDeviceManager::setCurrentDeviceMute(TRTCDeviceType type, bool mute) {
        if (nullptr != device_manager_) {
            return device_manager_->setCurrentDeviceMute(static_cast<liteav::TRTCDeviceType>(type), mute);
        }
        return -1;
    }

    bool ITXDeviceManager::getCurrentDeviceMute(TRTCDeviceType type) {
        if (nullptr != device_manager_) {
            return device_manager_->getCurrentDeviceMute(static_cast<liteav::TRTCDeviceType>(type));
        }
        return false;
    }

    int ITXDeviceManager::startCameraDeviceTest(IntPtr view) {
        if (nullptr != device_manager_) {
            return device_manager_->startCameraDeviceTest((HWND)view.ToPointer());
        }
        return -1;
    }

    int ITXDeviceManager::stopCameraDeviceTest() {
        if (nullptr != device_manager_) {
            return device_manager_->stopCameraDeviceTest();
        }
        return -1;
    }

    int ITXDeviceManager::startMicDeviceTest(UInt32 interval) {
        if (nullptr != device_manager_) {
            return device_manager_->startMicDeviceTest(interval);
        }
        return -1;
    }

    int ITXDeviceManager::stopMicDeviceTest() {
        if (nullptr != device_manager_) {
            return device_manager_->stopMicDeviceTest();
        }
        return -1;
    }

    int ITXDeviceManager::startSpeakerDeviceTest(String ^ filePath) {
        int result = -1;
        if (nullptr != device_manager_) {
            char * file_path = Utils::StringToUTF8CharPtr(filePath);
            result = device_manager_->startSpeakerDeviceTest(file_path);
            delete[] file_path;
            file_path = nullptr;
        }
        return result;
    }

    int ITXDeviceManager::stopSpeakerDeviceTest() {
        if (nullptr != device_manager_) {
            return device_manager_->stopSpeakerDeviceTest();
        }
        return -1;
    }

    void ITXDeviceManager::setCameraCapturerParam(TXCameraCaptureParam^ params) {
        if (nullptr != device_manager_) {
            ::TXCameraCaptureParam temp;
            temp.mode = static_cast<liteav::TXCameraCaptureMode>(params->mode);
            temp.width = params->width;
            temp.height = params->height;
            device_manager_->setCameraCapturerParam(temp);
        }
    }

    int ITXDeviceManager::setApplicationPlayVolume(int volume) {
        if (nullptr != device_manager_) {
            return device_manager_->setApplicationPlayVolume(volume);
        }
        return -1;
    }

    int ITXDeviceManager::getApplicationPlayVolume() {
        if (nullptr != device_manager_) {
            return device_manager_->getApplicationPlayVolume();
        }
        return -1;
    }

    int ITXDeviceManager::setApplicationMuteState(bool bMute) {
        if (nullptr != device_manager_) {
            return device_manager_->setApplicationMuteState(bMute);
        }
        return -1;
    }

    bool ITXDeviceManager::getApplicationMuteState() {
        if (nullptr != device_manager_) {
            return device_manager_->getApplicationMuteState();
        }
        return false;
    }

    void ITXDeviceManager::setDeviceObserver(ITXDeviceObserver^ observer) {
        if (nullptr != device_manager_)
        {
            {
                msclr::lock l(m_cb_locker);
                m_callback = observer;
            }
            if (observer)
            {
                if (nullptr == m_ObserverImpl)
                {
                    m_ObserverImpl = new TXDeviceObserverImpl();
                    if (nullptr == m_deviceChangeDelegate)
                    {
                        m_deviceChangeDelegate = gcnew DeviceChangedDelegate(this, &ITXDeviceManager::onDeviceChanged);
                    }
                    m_ObserverImpl->setDeviceChanged((PFN_DEVICECHANGED)Marshal::GetFunctionPointerForDelegate(m_deviceChangeDelegate).ToPointer());
                }
                device_manager_->setDeviceObserver(m_ObserverImpl);
            }
            else
            {
                device_manager_->setDeviceObserver(nullptr);
            }
        }
    }

    void ITXDeviceManager::onDeviceChanged(const char* deviceId, liteav::TXMediaDeviceType type, liteav::TXMediaDeviceState state) {
        ITXDeviceObserver ^ callback = nullptr;
        {
            msclr::lock l(m_cb_locker);
            callback = m_callback;
        }
        if (callback)
        {
            TXMediaDeviceType device_type = static_cast<TXMediaDeviceType>(type);
            TXMediaDeviceState device_state = static_cast<TXMediaDeviceState>(state);
            callback->onDeviceChanged(Utils::CharPtrToString(deviceId), device_type, device_state);
        }
    }

}  // namespace ManageLiteAV