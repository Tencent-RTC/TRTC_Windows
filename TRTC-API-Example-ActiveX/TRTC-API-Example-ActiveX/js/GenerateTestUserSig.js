/* eslint-disable require-jsdoc */
/*
 * Module:   GenerateTestUserSig
 *
 * Function: For generating UserSig which is security signature for cloud service.
 *           Calculated by encrypting SDKAppID, UserID and EXPIRETIME.
 *
 * Attention: Reasons for forbiding to publish following code on official app:
 *
 *            Codes calculating proper UserSig only suitable for quickly adjust the basic funtions of SDK
              but not for official app,
 *            because 'SDKSECRETKEY' in client codes is easy to reverse hack.
 *            Once secret key released, hackers can steal your Tencent cloud traffic with correct UserSig.
 *
 *            Advised to place the calculation code and encryption key on business server,
              and the client get it in real time.
 *            Server calculated solutions can better protect your encryption keys because of its higher cost.
 *
 * Reference：https://cloud.tencent.com/document/product/647/17275#Server
 */
function genTestUserSig(userID) {
  /**
   * Tencent Cloud SDKAppId should be replaced with your SDKAppId.
   *
   * Enter Tencent Cloud real-time audio and video[Console](https://console.cloud.tencent.com/rav )
     to create app then can check SDKAppId，
   * Unique identifier used by Tencent Cloud to distinguish customers.
   */
  const SDKAPPID = 0;

  /**
   * Do not set the signature expiration time too short
   * <p>
   * Time unit: second
   * Default：7 x 24 x 60 x 60 = 604800 = 7 days
   */
  const EXPIRETIME = 604800;

  /**
   * Steps for calculting encryption key:
   *
   * step1. Enter Tencent Cloud real-time audio and video[Console](https://console.cloud.tencent.com/rav )，
            if you don't have yours then create one.
   * step2. Click Application Configuration for basic configuration page
            and then locate the Account System Integration section.
   * step3. Click the "View key" button to see the encryption key used to calculate UserSig,
            copy it to the following variables
   *
   * Note: Only applicable to debugging Demo. Before the official launch, migrate the UserSig calculation code and key
           to your background server to avoid traffic theft caused by encryption key disclosure.
   * Document：https://cloud.tencent.com/document/product/647/17275#Server
   */
  const SDKSECRETKEY = '';

  // a soft reminder to guide developer to configure sdkAppId/secretKey
  if (SDKAPPID === '' || SDKSECRETKEY === '') {
    alert(
      '请先配置好您的账号信息： SDKAPPID 及 SDKSECRETKEY ' +
        '\r\n\r\nPlease configure your SDKAPPID/SDKSECRETKEY in js/GenerateTestUserSig.js'
    );
  }
  const generator = new LibGenerateTestUserSig(SDKAPPID, SDKSECRETKEY, EXPIRETIME);
  const userSig = generator.genTestUserSig(userID);
  return {
    sdkAppId: SDKAPPID,
    userSig: userSig
  };
}
