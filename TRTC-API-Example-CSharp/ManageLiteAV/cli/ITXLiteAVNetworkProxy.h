// ITXLiteAVNetworkProxy.h

#pragma once

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

// C++ Header
#include "include/ITXLiteAVNetworkProxy.h"

namespace ManageLiteAV
{
    public ref class ITXNetworkProxy
    {
    public:

        ~ITXNetworkProxy();

        static ITXNetworkProxy^ createTXNetworkProxy();

        static void destroyTXNetworkProxy(ITXNetworkProxy^ network_proxy);

        /// <summary>
        /// TRTCCloud 代理设置
        /// <para>socks5 代理模式，设置 socks5 tcp/udp 代理服务器 ip 和 port</para>
        /// <para>设置此接口后，在内网环境可以正常使用进房、发布本地音视频、订阅远端音视频、退房等功能</para>
        /// <para>细节请参考TRTC 代理方案<http://trtc-1252463788.file.myqcloud.com/proxy/proxy_server_deploy.zip></para>
        /// <para>设置成功返回0，域名解释失败或者ip非法返回-1</para>
        /// <para>本接口有以下限制：</para>
        /// <para>- 设置完，请 destroy TRTCCloud 重新获取 TRTC 对象</para>
        /// <para>- 如果你只是用 TRTCCloud，则只需要设置Sock5代理接口。</para>
        /// </summary>
        /// <param name="host">代理服务器的 ip 地址或者域名地址</param>
        /// <param name="port">代理服务器的端口</param>
        /// <param name="username">代理服务器用户名</param>
        /// <param name="password">代理服务器密码</param>
        int setSocks5Proxy(String^ host, UInt16 port, String^ username, String^ password);

    private:
        ::ITXNetworkProxy* m_network_proxy = nullptr;
        !ITXNetworkProxy();
        ITXNetworkProxy();

        static ITXNetworkProxy^ sInstance;
        static Object^ slocker = gcnew Object();
    };
}