class GenerateTestUserSig
{
/**
 * UserSig Calculation
 * 
 * UserSig is a security signature calculated using the HMAC SHA256 encryption algorithm.
 * Basically, it involves hashing crucial information such as SDKAppID, UserID, and ExpireTime.
 * 
 * @note:  Do not use the code below in your commercial application. This is because:
 * 
 * The code may be able to calculate UserSig correctly, but it is only for quick testing of the SDK’s basic features, not for commercial applications.
 * SDKSECRETKEY in client code can be easily decompiled and reversed, especially on web.
 * Once your key is disclosed, attackers will be able to steal your Tencent Cloud traffic.
 * 
 * The correct method is to deploy the UserSig calculation code and encryption key on your project server so that your application can request from your server a UserSig that is calculated whenever one is needed.
 * Given that it is more difficult to hack a server than a client application, server-end calculation can better protect your key.
 * 
 * Documentation:  https://intl.cloud.tencent.com/document/product/647/35166#Server
 */

public:
   static const char* genTestUserSig(const char* identifier, int sdkAppId, const char* secretKey);
};

