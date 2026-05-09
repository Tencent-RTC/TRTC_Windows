/**
 * Copyright (c) 2021 Tencent. All rights reserved.
 * Module:   音视频设备管理模块
 * Function: 用于管理摄像头、麦克风和扬声器等音视频相关的硬件设备
 */
using System;

namespace trtc {

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                    音视频设备相关的类型定义
    //
    /////////////////////////////////////////////////////////////////////////////////

    /**
     * 系统音量类型（仅适用于移动设备）。
     *
     * @deprecated v9.5 版本开始不推荐使用。
     * 现代智能手机中一般都具备两套系统音量类型，即“通话音量”和“媒体音量”。
     * - 通话音量：手机专门为接打电话所设计的音量类型，自带回声抵消（AEC）功能，并且支持通过蓝牙耳机上的麦克风进行拾音，缺点是音质比较一般。
     *            当您通过手机侧面的音量按键下调手机音量时，如果无法将其调至零（也就是无法彻底静音），说明您的手机当前处于通话音量。
     * - 媒体音量：手机专门为音乐场景所设计的音量类型，无法使用系统的 AEC 功能，并且不支持通过蓝牙耳机的麦克风进行拾音，但具备更好的音乐播放效果。
     *            当您通过手机侧面的音量按键下调手机音量时，如果能够将手机音量调至彻底静音，说明您的手机当前处于媒体音量。
     *
     * SDK 目前提供了三种系统音量类型的控制模式：自动切换模式、全程通话音量模式、全程媒体音量模式。
     */
    public enum TXSystemVolumeType {

        /// 自动切换模式
        TXSystemVolumeTypeAuto = 0,

        /// 全程媒体音量
        TXSystemVolumeTypeMedia = 1,

        /// 全程通话音量
        TXSystemVolumeTypeVOIP = 2,

    }

    /**
     * 音频路由（即声音的播放模式）。
     *
     * 音频路由，即声音是从手机的扬声器还是从听筒中播放出来，因此该接口仅适用于手机等移动端设备。
     * 手机有两个扬声器：一个是位于手机顶部的听筒，一个是位于手机底部的立体声扬声器。
     * - 设置音频路由为听筒时，声音比较小，只有将耳朵凑近才能听清楚，隐私性较好，适合用于接听电话。
     * - 设置音频路由为扬声器时，声音比较大，不用将手机贴脸也能听清，因此可以实现“免提”的功能。
     */
    public enum TXAudioRoute {

        /// Speakerphone：使用扬声器播放（即“免提”），扬声器位于手机底部，声音偏大，适合外放音乐。
        TXAudioRouteSpeakerphone = 0,

        /// Earpiece：使用听筒播放，听筒位于手机顶部，声音偏小，适合需要保护隐私的通话场景。
        TXAudioRouteEarpiece = 1,

    }

    /**
     * 设备类型（仅适用于桌面平台）。
     *
     * 该枚举值用于定义三种类型的音视频设备，即摄像头、麦克风和扬声器，以便让一套设备管理接口可以操控三种不同类型的设备。
     */
    public enum TXMediaDeviceType {

        /// 未定义的设备类型
        TXMediaDeviceTypeUnknown = -1,

        /// 麦克风类型设备
        TXMediaDeviceTypeMic = 0,

        /// 扬声器类型设备
        TXMediaDeviceTypeSpeaker = 1,

        /// 摄像头类型设备
        TXMediaDeviceTypeCamera = 2,

    }

    /**
     * 设备操作。
     *
     * 该枚举值用于本地设备的状态变化通知 {@link onDeviceChanged}。
     */
    public enum TXMediaDeviceState {

        /// 设备已被插入
        TXMediaDeviceStateAdd = 0,

        /// 设备已被移除
        TXMediaDeviceStateRemove = 1,

        /// 设备已启用
        TXMediaDeviceStateActive = 2,

        /// 系统默认设备变更
        TXMediaDefaultDeviceChanged = 3,

    }

    /**
     * 摄像头采集偏好。
     *
     * 该枚举类型用于摄像头采集参数设置。
     */
    public enum TXCameraCaptureMode {

        /// 自动调整采集参数。
        /// SDK 根据实际的采集设备性能及网络情况，选择合适的摄像头输出参数，在设备性能及视频预览质量之间，维持平衡。
        TXCameraResolutionStrategyAuto = 0,

        /// 优先保证设备性能。
        /// SDK 根据用户设置编码器的分辨率和帧率，选择最接近的摄像头输出参数，从而保证设备性能。
        TXCameraResolutionStrategyPerformance = 1,

        /// 优先保证视频预览质量。
        /// SDK选择较高的摄像头输出参数，从而提高预览视频的质量。在这种情况下，会消耗更多的 CPU 及内存做视频前处理。
        TXCameraResolutionStrategyHighQuality = 2,

        /// 允许用户设置本地摄像头采集的视频宽高。
        TXCameraCaptureManual = 3,

    }

    /**
     * 摄像头采集参数。
     *
     * 该设置能决定本地预览图像画质。
     */
    [Serializable]
    public struct TXCameraCaptureParam {
        /// **字段含义：** 摄像头采集偏好，请参见 {@link TXCameraCaptureMode}
        public TXCameraCaptureMode mode;

        /// **字段含义：** 采集图像长度
        public int width;

        /// **字段含义：** 采集图像宽度
        public int height;
    }

    /**
     * 音视频设备的相关信息（仅适用于桌面平台）。
     *
     * 该结构体用于描述一个音视频设备的关键信息，例如设备 ID、设备名称等等，以便用户能够在用户界面上选择自己期望使用的音视频设备。
     */
    [Serializable]
    public struct TXDeviceInfo {
        /// 设备 ID （UTF-8）
        public String devicePID;

        /// 设备名称 （UTF-8）
        public String deviceName;

        /// 设备属性
        public String deviceProperties;
    }

    public interface ITXDeviceObserver {
        /**
         * 本地设备的通断状态发生变化（仅适用于桌面系统）。
         *
         * 当本地设备（包括摄像头、麦克风以及扬声器）被插入或者拔出时，SDK 便会抛出此事件回调。
         * @param deviceId 设备 ID
         * @param type 设备类型
         * @param state 通断状态，0：设备已添加；1：设备已被移除；2：设备已启用。
         */
        void onDeviceChanged(String deviceId, TXMediaDeviceType type, TXMediaDeviceState state);
    }

    public abstract class ITXDeviceManager {
        /////////////////////////////////////////////////////////////////////////////////
        //
        //                    设备操作接口
        //
        /////////////////////////////////////////////////////////////////////////////////

        /**
         * 1.1 判断当前是否为前置摄像头（仅适用于移动端）。
         */
        public abstract bool isFrontCamera();

        /**
         * 1.2 切换前置或后置摄像头（仅适用于移动端）。
         */
        public abstract int switchCamera(bool frontCamera);

        /**
         * 1.3 获取摄像头的最大缩放倍数（仅适用于移动端）。
         */
        public abstract double getCameraZoomMaxRatio();

        /**
         * 1.4 设置摄像头的缩放倍数（仅适用于移动端）。
         *
         * @param zoomRatio 取值范围 [1, 5]，取值为1表示最远视角（正常镜头），取值为5表示最近视角（放大镜头）。最大值推荐为5，若超过5，视频数据会变得模糊不清。
         */
        public abstract int setCameraZoomRatio(double zoomRatio);

        /**
         * 1.5 查询是否支持自动识别人脸位置（仅适用于移动端）。
         */
        public abstract bool isAutoFocusEnabled();

        /**
         * 1.6 开启自动对焦功能（仅适用于移动端）。
         *
         * 开启后，SDK 会自动检测画面中的人脸位置，并将摄像头的焦点始终对焦在人脸位置上。
         */
        public abstract int enableCameraAutoFocus(bool enabled);

        /**
         * 1.7 设置摄像头的对焦位置（仅适用于移动端）。
         *
         * 您可以通过该接口实现如下交互：
         * 1. 在本地摄像头的预览画面上，允许用户单击操作。
         * 2. 在用户的单击位置显示一个矩形方框，以示摄像头会在此处对焦。
         * 3. 随后将用户点击位置的坐标通过本接口传递给 SDK，之后 SDK 会操控摄像头按照用户期望的位置进行对焦。
         * @param position 对焦位置，请传入期望对焦点的坐标值
         * @return 0：操作成功；负数：操作失败。
         * @note 使用该接口的前提是先通过 {@link enableCameraAutoFocus} 关闭自动对焦功能。
         */
        public abstract int setCameraFocusPosition(int x, int y);

        /**
         * 1.8 开启/关闭闪光灯，也就是手电筒模式（仅适用于移动端）。
         */
        public abstract int enableCameraTorch(bool enabled);

        /**
         * 1.9 设置音频路由（仅适用于移动端）。
         *
         * 手机有两个音频播放设备：一个是位于手机顶部的听筒，一个是位于手机底部的立体声扬声器。
         * 设置音频路由为听筒时，声音比较小，只有将耳朵凑近才能听清楚，隐私性较好，适合用于接听电话。
         * 设置音频路由为扬声器时，声音比较大，不用将手机贴脸也能听清，因此可以实现“免提”的功能。
         */
        public abstract int setAudioRoute(TXAudioRoute route);

        /**
         * 2.1 获取设备列表（仅适用于桌面端）。
         *
         * @param type  设备类型，指定需要获取哪种设备的列表。详见 {@link TXMediaDeviceType} 定义。
         * @note
         *   - 使用完毕后请调用 release 方法释放资源，这样可以让 SDK 维护 ITXDeviceCollection 对象的生命周期。
         *   - 不要使用 delete 释放返回的 Collection 对象，`delete ITXDeviceCollection*` 指针会导致异常崩溃。
         *   - type 只支持 `TXMediaDeviceTypeMic`、`TXMediaDeviceTypeSpeaker`、`TXMediaDeviceTypeCamera`。
         *   - 此接口只支持 Mac 和 Windows 平台。
         */
        public abstract TXDeviceInfo[] getDevicesList(TXMediaDeviceType type);

        /**
         * 2.2 设置当前要使用的设备（仅适用于桌面端）。
         *
         * @param type 设备类型，详见 TXMediaDeviceType 定义。
         * @param deviceId 设备 ID，您可以通过接口 {@link getDevicesList} 获得设备 ID。
         * @return 0：操作成功；负数：操作失败。
         */
        public abstract int setCurrentDevice(TXMediaDeviceType type, String deviceId);

        /**
         * 2.3 获取当前正在使用的设备（仅适用于桌面端）。
         */
        public abstract TXDeviceInfo getCurrentDevice(TXMediaDeviceType type);

        /**
         * 2.11 开始麦克风测试（仅适用于桌面端）。
         *
         * 该接口可以测试麦克风是否能正常工作，测试到的麦克风采集音量的大小，会以回调的形式通知给您，其中 volume 的取值范围为 [0, 100]。
         * @param interval 麦克风音量的回调间隔，单位为毫秒（ms）。
         * @note 该接口调用后默认会回播麦克风录制到的声音到扬声器中。
         */
        public abstract int startMicDeviceTest(uint interval);

        /**
         * 2.12 开始麦克风测试（仅适用于桌面端）。
         *
         * 该接口可以测试麦克风是否能正常工作，测试到的麦克风采集音量的大小，会以回调的形式通知给您，其中 volume 的取值范围为 [0, 100]。
         * @param interval 麦克风音量的回调间隔，单位为毫秒（ms）。
         * @param playback 是否开启回播麦克风声音，开启后用户测试麦克风时会听到自己的声音。
         */
        public abstract int startMicDeviceTest(uint interval, bool playback);

        /**
         * 2.13 结束麦克风测试（仅适用于桌面端）。
         */
        public abstract int stopMicDeviceTest();

        /**
         * 2.14 开始扬声器测试（仅适用于桌面端）。
         *
         * 该接口通过播放指定的音频文件，用于测试播放设备是否能正常工作。如果用户在测试时能听到声音，说明播放设备能正常工作。
         * @param filePath 声音文件的路径
         */
        public abstract int startSpeakerDeviceTest(string filePath);

        /**
         * 2.15 结束扬声器测试（仅适用于桌面端）。
         */
        public abstract int stopSpeakerDeviceTest();

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                    弃用接口（建议使用对应的新接口）
        //
        /////////////////////////////////////////////////////////////////////////////////

        /**
         * 设置系统音量类型（仅适用于移动端）。
         *
         * @deprecated v9.5 版本开始不推荐使用，建议使用 `TRTCCloud` 中的 `startLocalAudio(quality)` 接口替代之，通过 quality 参数来决策音质。
         */
        //@deprecated
        public abstract int setSystemVolumeType(TXSystemVolumeType type);
    }
}
