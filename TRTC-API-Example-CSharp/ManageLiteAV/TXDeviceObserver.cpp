#include "TXDeviceObserver.h"

void TXDeviceObserverImpl::setDeviceChanged(PFN_DEVICECHANGED pointer) {
    m_deviceChanged = pointer;
}

void TXDeviceObserverImpl::onDeviceChanged(const char* deviceId, TXMediaDeviceType type, TXMediaDeviceState state) {
    if (nullptr != m_deviceChanged)
    {
        m_deviceChanged(deviceId, type, state);
    }
}