/**
 * Copyright (c) 2021 Tencent. All rights reserved.
 * Module:   V2TXLivePremier @ TXLiteAVSDK
 * Function: V2TXLive 高级接口
 */
namespace liteav {

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      V2TXLive 高级接口
    //
    /////////////////////////////////////////////////////////////////////////////////

    public abstract class V2TXLivePremier {
        /**
         * 设置 SDK 的授权 License
         *
         * 文档地址：https://cloud.tencent.com/document/product/454/34750。
         * @param url license的地址。
         * @param key license的秘钥。
         */
        public static void setLicense(string url, string key) {
            V2TXLivePremierNative.v2tx_live_premier_set_license(url, key);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      V2TXLive 高级回调接口
    //
    /////////////////////////////////////////////////////////////////////////////////

}
