#include "Utils.h"
#include "cli/ITXLiteAVNetworkProxy.h"
#include <assert.h>
#include <string>
#include <msclr\marshal_cppstd.h>
#include <msclr\lock.h> 
#include <codecvt>

namespace ManageLiteAV
{
    ITXNetworkProxy::~ITXNetworkProxy()
    {
        this->!ITXNetworkProxy();
    }

    ITXNetworkProxy::ITXNetworkProxy()
    {
        if (m_network_proxy == nullptr)
        {
            m_network_proxy = ::createTXNetworkProxy();
        }
    }

    ITXNetworkProxy::!ITXNetworkProxy()
    {
        if (m_network_proxy != nullptr)
        {
            pin_ptr<::ITXNetworkProxy*> p = &m_network_proxy;
            ::destroyTXNetworkProxy(p);
            m_network_proxy = nullptr;
        }
    }

    ITXNetworkProxy^ ITXNetworkProxy::createTXNetworkProxy()
    {
        if (sInstance == nullptr)
        {
            msclr::lock l(slocker);
            if (sInstance == nullptr)
            {
                sInstance = gcnew ITXNetworkProxy();
            }
        }
        return sInstance;
    }

    void ITXNetworkProxy::destroyTXNetworkProxy(ITXNetworkProxy^ network_proxy)
    {
        msclr::lock l(slocker);
        if (network_proxy != nullptr && sInstance != nullptr && network_proxy == sInstance)
        {
            sInstance->~ITXNetworkProxy();
            sInstance = nullptr;
        }
    }

    int ITXNetworkProxy::setSocks5Proxy(String^ host, UInt16 port, String^ username, String^ password)
    {
        int ret = 0;
        if (m_network_proxy != nullptr)
        {
            char* _host = Utils::StringToUTF8CharPtr(host);
            char* _username = Utils::StringToUTF8CharPtr(username);
            char* _password = Utils::StringToUTF8CharPtr(password);
            ret = m_network_proxy->setSocks5Proxy(_host, port, _username, _password);
            delete[] _host;
            _host = nullptr;
            delete[] _username;
            _username = nullptr;
            delete[] _password;
            _password = nullptr;
        }
        return ret;
    }
}