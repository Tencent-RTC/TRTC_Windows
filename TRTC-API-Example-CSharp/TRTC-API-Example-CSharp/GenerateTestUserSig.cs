using System;
using System.Text;
using TRTCCSharpDemo.Common;
using tencentyun;

/// <summary>
/// Module： GenerateTestUserSig
/// 
/// Function：UserSig for test is generated. UserSig is a security protection signature designed.
///           Calculation method is to encrypt SDKAppID, UserID and EXPIRETIME,
///           and the encryption algorithm is HMAC-SHA256.
///      
/// Attention：Do not publish the following code to your online official version of the App for the following reasons:
/// 
///            Although the code in this file can correctly calculate UserSig, it is only suitable
///            for quickly tuning the basic functions of the SDK, and is not suitable for online products.
///            This is because SDKSECRETKEY in client code is easy to decompile and reverse crack,
///            especially on the Web side, the difficulty of cracking is almost zero.
///            Once your key compromised, an attacker can calculate the correct UserSig to steal your cloud traffic.
///             
///            The correct approach is to put the calculation code and encryption key of UserSig on your
///            business server, and then the App obtains the real-time calculated UserSig from your server on demand.
///            Since the cost of cracking the server is higher than cracking the client App,
///            the solution calculated by the server can better protect your encryption key.
///            
/// Reference：https://cloud.tencent.com/document/product/647/17275#Server
/// </summary>

namespace TRTCCSharpDemo
{
    class GenerateTestUserSig
    {
        /// <summary>
        /// Tencent Cloud SDKAppId, need to be replaced with your own account SDKAppId.
        /// 
        /// Create in Tencent Yunyun communication[Console](https://console.cloud.tencent.com/avc)
        /// and SDKAppId can be seen，
        /// </summary>
        /// <remarks>
        /// It is a unique identifier used by Tencent Cloud to distinguish customers.
        /// </remarks>
        public const int SDKAPPID = 0;

        /// <summary>
        /// Calculate the encryption key for the signature
        /// 
        /// step1. Enter Tencent Cloud real-time audio and video[Console](https://console.cloud.tencent.com/rav),
        ///        if you create one without an application,
        /// step2. Click Application Configuration to go to the basic configuration page,
        ///        and further locate the Account System Integration section.
        /// step3. Click the "View key" button,
        ///        you can see the encryption key used to calculate UserSig,
        ///        please copy and copy it to the following variables
        /// </summary>
        /// <remarks>
        /// Note: This solution is only applicable to debugging Demo. Before the official launch,
        ///       migrate the UserSig calculation code and key to your background server
        ///       to avoid traffic theft caused by encryption key disclosure.
        /// Document：https://cloud.tencent.com/document/product/647/17275#GetFromServer
        /// </remarks>
        public const string SDKSECRETKEY = @"";

        /// <summary>
        /// Do not set the signature expiration time too short
        /// 
        /// Time unit: second
        /// Default time：7 x 24 x 60 x 60 = 604800 = 7 days
        /// </summary>
        public const int EXPIRETIME = 604800;

        /// <summary>
        /// Mixed-flow interface needs to complete the account information.
        /// 
        /// Access: Tencent Cloud web console -> real-time audio and video ->
        ///         Your application (eg customer service call)-> Account information panel can obtain appid/bizid
        /// </summary>
        public const int APPID = 0;
        public const int BIZID = 0;

        private static GenerateTestUserSig mInstance;

        private GenerateTestUserSig()
        {
        }

        public static GenerateTestUserSig GetInstance()
        {
            if (mInstance == null)
            {
                mInstance = new GenerateTestUserSig();
            }
            return mInstance;
        }

        /// <summary>
        /// Calculate the UserSig signature
        /// 
        /// The HMAC-SHA256 asymmetric encryption algorithm is used to encrypt SDKAPPID, userId, and EXPIRETIME
        /// 
        /// This solution is only suitable for local runthrough demo and function debugging. When the product is really
        /// released online, the server should be used to obtain the solution to avoid the private key being cracked.
        /// </summary>
        /// <remarks>
        /// Do not publish the following code to your online official version of the App for the following reasons:
        /// 
        /// Although the code in this file can correctly calculate UserSig,
        /// only suitable for quickly tuning the basic functions of the SDK, and is not suitable for online products.
        /// This is because SDKSECRETKEY in client code is easy to decompile and reverse crack,
        /// especially on the Web side, the difficulty of cracking is almost zero.
        /// Once your key is compromised, an attacker can calculate the correct UserSig to steal your cloud traffic.
        /// 
        /// The correct approach is to put the calculation code and encryption key of UserSig on your business server,
        /// and then the App obtains the real-time calculated UserSig from your server on demand.
        /// Since the cost of cracking the server is higher than cracking the client App,
        /// the solution calculated by the server can better protect your encryption key.
        /// 
        /// Document：https://cloud.tencent.com/document/product/647/17275#GetFromServer
        /// </remarks>
        public string GenTestUserSig(string userId)
        {
            if (SDKAPPID == 0 || string.IsNullOrEmpty(SDKSECRETKEY)) return null;
            TLSSigAPIv2 api = new TLSSigAPIv2(SDKAPPID, SDKSECRETKEY);
            // Unified conversion to UTF8, SDK internal is using UTF8 encoding.
            return api.GenSig(Util.UTF16To8(userId));
        }
        
    }
}
