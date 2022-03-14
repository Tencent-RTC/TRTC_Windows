#pragma once
#include <Windows.h>
// C++ Header
#include "include/TRTC/ITXDeviceManager.h"

typedef void (CALLBACK* PFN_DEVICECHANGED)(const char* deviceId, TXMediaDeviceType type, TXMediaDeviceState state);

// 非托管类无法直接引用托管类，故此通过函数指针做桥接
class TXDeviceObserverImpl :public ITXDeviceObserver
{
public:
    void setDeviceChanged(PFN_DEVICECHANGED pointer);

    void onDeviceChanged(const char* deviceId, TXMediaDeviceType type, TXMediaDeviceState state);
private:
    PFN_DEVICECHANGED m_deviceChanged;
};

