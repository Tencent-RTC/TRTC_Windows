//  QTSimpleDemo
//
//  Copyright © 2020 tencent. All rights reserved.
//

#ifndef QTMACDEMO_BASE_DEFS_H_
#define QTMACDEMO_BASE_DEFS_H_

/**
 * Tencent Cloud SDKAppID. Set it to the SDKAppID of your account.
 *
 * You can view your SDKAppID after creating an application in the [TRTC console](https://console.intl.cloud.tencent.com/rav).
 * SDKAppID uniquely identifies a Tencent Cloud account.
 */
static const int SDKAppID = PLACEHOLDER;

/**
 * Follow the steps below to obtain the key required for UserSig calculation.
 *
 * Step 1.  Log in to the [TRTC console](https://console.intl.cloud.tencent.com/rav), and create an application if you don’t have one.
 * Step 2.  Find your application, click "Application Info", and select the "Quick Start" tab.
 * Step 3.  Copy and paste the key to the code, as shown below.
 *
 * Notes:  This method is for testing only. Before commercial launch, please migrate the UserSig calculation code and key to your backend server to prevent key disclosure and traffic theft.
 * Documentation: https://intl.cloud.tencent.com/document/product/647/35166#Server
 */
static const char* SDKSECRETKEY = "PLACEHOLDER";

/**
 *  Signature validity period, which should not be set too short
 *
 *  Unit: seconds
 *  Default value: 604800 (7 days)
 */
static const int EXPIRETIME = 604800;

/**
 *  Mixed-flow interface for completing the account information.
 *  Approach：Tencent cloud web->Real-time audio and video->Application->get 'appid/bizid' from account interface
 */
static const int APPID = PLACEHOLDER;
static const int BIZID = PLACEHOLDER;

/**
 *  Configuring domain names for CDN playback:
 *
 *  You can play live streams relayed to Tencent Cloud via CDNs only after you have activated CSS and configured a playback domain name.
 *  To obtain a valid URL for relayed push and CDN playback, refer to the documents below:
 *  1.  CDN Relayed Live Streaming: https://intl.cloud.tencent.com/document/product/647/35242
 *  2.  Adding Domain Name: https://intl.cloud.tencent.com/document/product/267/35970
 */
static const char* DOMAIN_URL = "PLACEHOLDER";

enum ControlButtonType {
    Audio = 0,
    Video = 1,
};

#endif  // QTMACDEMO_BASE_DEFS_H_
