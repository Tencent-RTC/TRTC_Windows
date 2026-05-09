/**
 * 错误码。
 */
namespace trtc {
    public enum TXLiteAVError {

        /////////////////////////////////////////////////////////////////////////////////
        //       基础错误码
        /////////////////////////////////////////////////////////////////////////////////

        /// 无错误
        ERR_NULL = 0,

        /////////////////////////////////////////////////////////////////////////////////
        //       视频相关错误码
        /////////////////////////////////////////////////////////////////////////////////

        /// 打开摄像头失败，在移动端中是授权后调用系统接口开启摄像头异常，在 Windows 或 Mac 设备，摄像头的配置程序（驱动程序）异常，禁用后重新启用设备，或者重启机器，或者更新配置程序
        ERR_CAMERA_START_FAIL = -1301,

        /// 摄像头设备未授权，通常在移动设备出现，可能是权限被用户拒绝了
        ERR_CAMERA_NOT_AUTHORIZED = -1314,

        /// 摄像头参数设置出错（参数不支持或其它）
        ERR_CAMERA_SET_PARAM_FAIL = -1315,

        /// 摄像头正在被占用中，可尝试打开其他摄像头
        ERR_CAMERA_OCCUPY = -1316,

        /// 视频帧编码失败，编码器异常导致编码失败
        ERR_VIDEO_ENCODE_FAIL = -1303,

        /// 不支持的视频分辨率
        ERR_UNSUPPORTED_RESOLUTION = -1305,

        /// 自定视频采集：设置的 pixel format 不支持
        ERR_PIXEL_FORMAT_UNSUPPORTED = -1327,

        /// 自定视频采集：设置的 buffer type 不支持
        ERR_BUFFER_TYPE_UNSUPPORTED = -1328,

        /////////////////////////////////////////////////////////////////////////////////
        //       音频相关错误码
        /////////////////////////////////////////////////////////////////////////////////

        /// 打开麦克风失败，例如在 Windows 或 Mac 设备，麦克风的配置程序（驱动程序）异常，禁用后重新启用设备，或者重启机器，或者更新配置程序
        ERR_MIC_START_FAIL = -1302,

        /// 麦克风设备未授权，通常在移动设备出现，可能是权限被用户拒绝了
        ERR_MIC_NOT_AUTHORIZED = -1317,

        /// 麦克风正在被占用中，例如移动设备正在通话时，打开麦克风会失败
        ERR_MIC_OCCUPY = -1319,

        /// 停止麦克风失败
        ERR_MIC_STOP_FAIL = -1320,

        /// 打开扬声器失败，例如在 Windows 或 Mac 设备，扬声器的配置程序（驱动程序）异常，禁用后重新启用设备，或者重启机器，或者更新配置程序
        ERR_SPEAKER_START_FAIL = -1321,

        /// 扬声器设置参数失败
        ERR_SPEAKER_SET_PARAM_FAIL = -1322,

        /// 停止扬声器失败
        ERR_SPEAKER_STOP_FAIL = -1323,

        /// 开启系统声音录制失败，例如音频驱动插件不可用
        ERR_AUDIO_PLUGIN_START_FAIL = -1330,

        /// 安装音频驱动插件未授权
        ERR_AUDIO_PLUGIN_INSTALL_NOT_AUTHORIZED = -1331,

        /// 安装音频驱动插件失败
        ERR_AUDIO_PLUGIN_INSTALL_FAILED = -1332,

        /// 安装虚拟声卡插件成功，但首次安装后功能暂时不可用，此为 Mac 系统限制，请在收到此错误码后提示用户重启当前 App
        ERR_AUDIO_PLUGIN_INSTALLED_BUT_NEED_RESTART = -1333,

        /// 音频帧编码失败，例如传入自定义音频数据，SDK 无法处理
        ERR_AUDIO_ENCODE_FAIL = -1304,

        /// 不支持的音频采样率
        ERR_UNSUPPORTED_SAMPLERATE = -1306,

        /////////////////////////////////////////////////////////////////////////////////
        //       网络相关错误码
        /////////////////////////////////////////////////////////////////////////////////

        /// 进入房间失败，请查看 onError 中的 -3301 对应的 msg 提示确认失败原因
        ERR_ROOM_ENTER_FAIL = -3301,

        /// 请求进房超时，请检查是否断网或者是否开启vpn，您也可以切换4G进行测试确认
        ERR_ROOM_REQUEST_ENTER_ROOM_TIMEOUT = -3308,

        /// 进房参数为空，请检查： enterRoom:appScene: 接口调用是否传入有效的 param
        ERR_ENTER_ROOM_PARAM_NULL = -3316,

        /// 进房参数 sdkAppId 错误，请检查 TRTCParams.sdkAppId 是否为空
        ERR_SDK_APPID_INVALID = -3317,

        /// 进房参数 roomId 错误，请检查 TRTCParams.roomId 或 TRTCParams.strRoomId 是否为空，注意 roomId 和 strRoomId 不可混用
        ERR_ROOM_ID_INVALID = -3318,

        /// 进房参数 userId 不正确，请检查 TRTCParams.userId 是否为空
        ERR_USER_ID_INVALID = -3319,

        /// 进房参数 userSig 不正确，请检查 TRTCParams.userSig 是否为空
        ERR_USER_SIG_INVALID = -3320,

        /// 服务不可用。请检查：套餐包剩余分钟数是否大于0，腾讯云账号是否欠费。
        /// 您可参考 [套餐包管理](https://cloud.tencent.com/document/product/647/50492) 进行查看与配置
        ERR_SERVER_INFO_SERVICE_SUSPENDED = -100013,

        /// UserSig 校验失败，请检查参数 TRTCParams.userSig 是否填写正确，或是否已经过期。
        /// 您可参考 [UserSig 生成与校验](https://cloud.tencent.com/document/product/647/50686) 进行校验
        ERR_SERVER_INFO_ECDH_GET_TINYID = -100018,

        /////////////////////////////////////////////////////////////////////////////////
        //       背景音乐播放相关错误码
        /////////////////////////////////////////////////////////////////////////////////

    }

    /**
     * 警告码。
     */
    public enum TXLiteAVWarning {

        /////////////////////////////////////////////////////////////////////////////////
        //       视频相关警告码
        /////////////////////////////////////////////////////////////////////////////////

        /// 硬编码启动出现问题，自动切换到软编码
        WARNING_HW_ENCODER_START_FAIL = 1103,

        /// 当前 CPU 使用率太高，无法满足软件编码需求，自动切换到硬件编码
        WARNING_VIDEO_ENCODER_SW_TO_HW = 1107,

        /// 摄像头采集帧率不足，部分自带美颜算法的 Android 手机上会出现
        WARNING_INSUFFICIENT_CAPTURE_FPS = 1108,

        /// 软编码启动失败
        WARNING_SW_ENCODER_START_FAIL = 1109,

        /// 摄像头采集分辨率被降低，以满足当前帧率和性能最优解。
        WARNING_REDUCE_CAPTURE_RESOLUTION = 1110,

        /// 没有检测到可用的摄像头设备
        WARNING_CAMERA_DEVICE_EMPTY = 1111,

        /// 用户未授权当前应用使用摄像头
        WARNING_CAMERA_NOT_AUTHORIZED = 1112,

        /// 摄像头被占用.
        WARNING_CAMERA_IS_OCCUPIED = 1114,

        /// 摄像头设备异常.
        WARNING_CAMERA_DEVICE_ERROR = 1115,

        /// 摄像头无法连接.
        WARNING_CAMERA_DISCONNECTED = 1116,

        /// 摄像头启动失败.
        WARNING_CAMERA_START_FAILED = 1117,

        /// 系统异常.
        WARNING_CAMERA_SERVER_DIED = 1118,

        /// 用户未授权当前应用使用屏幕录制
        WARNING_SCREEN_CAPTURE_NOT_AUTHORIZED = 1206,

        /// 当前视频帧解码失败
        WARNING_VIDEO_FRAME_DECODE_FAIL = 2101,

        /// 硬解启动失败，采用软解码
        WARNING_HW_DECODER_START_FAIL = 2106,

        /// 当前流硬解第一个 I 帧失败，SDK 自动切软解
        WARNING_VIDEO_DECODER_HW_TO_SW = 2108,

        /// 软解码器启动失败
        WARNING_SW_DECODER_START_FAIL = 2109,

        /// 视频渲染失败
        WARNING_VIDEO_RENDER_FAIL = 2110,

        /////////////////////////////////////////////////////////////////////////////////
        //       音频相关警告码
        /////////////////////////////////////////////////////////////////////////////////

        /// 人脸测距回调通知
        /// 没有检测到可用的麦克风设备
        WARNING_MICROPHONE_DEVICE_EMPTY = 1201,

        /// 没有检测到可用的扬声器设备
        WARNING_SPEAKER_DEVICE_EMPTY = 1202,

        /// 用户未授权当前应用使用麦克风
        WARNING_MICROPHONE_NOT_AUTHORIZED = 1203,

        /// 音频采集设备不可用（例如被占用或者PC判定无效设备）
        WARNING_MICROPHONE_DEVICE_ABNORMAL = 1204,

        /// 音频播放设备不可用（例如被占用或者PC判定无效设备）
        WARNING_SPEAKER_DEVICE_ABNORMAL = 1205,

        /// 蓝牙设备连接失败（例如其他应用通过设置通话音量占用音频通道）
        WARNING_BLUETOOTH_DEVICE_CONNECT_FAIL = 1207,

        /// 音频采集设备被占用
        WARNING_MICROPHONE_IS_OCCUPIED = 1208,

        /// 当前音频帧解码失败
        WARNING_AUDIO_FRAME_DECODE_FAIL = 2102,

        /// 音频录制写入文件失败
        WARNING_AUDIO_RECORDING_WRITE_FAIL = 7001,

        /// 录制音频时监测到啸叫
        WARNING_MICROPHONE_HOWLING_DETECTED = 7002,

        /////////////////////////////////////////////////////////////////////////////////
        //       网络相关警告码
        /////////////////////////////////////////////////////////////////////////////////i

        /// 当前是观众角色，不支持发布音视频，需要先切换成主播角色
        WARNING_IGNORE_UPSTREAM_FOR_AUDIENCE = 6001,

        /// 音视频发送时间戳异常，可能引起音画不同步问题
        WARNING_UPSTREAM_AUDIO_AND_VIDEO_OUT_OF_SYNC = 6006,

        ///  服务器状态异常，正在进行重连
        WARNING_RECONNECT_ON_SERVER_STATUS_ABNORMAL = 6007,

    }
}
