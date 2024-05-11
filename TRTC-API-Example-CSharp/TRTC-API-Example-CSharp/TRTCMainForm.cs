using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using ManageLiteAV;
using TRTCCSharpDemo.Common;
using System.Threading;
using System.Timers;
using System.Runtime.InteropServices;

/// <summary>
/// Module： TRTCMainForm
/// 
/// Function： Use the TRTC SDK to complete 1v1 and 1vn video call functions
/// 
/// Notice：
/// 1. Only static video layout is supported
///
/// 2. Support for video call resolution, frame rate and flow mode adjustment. Implemented by TRTCSettingForm
///
/// 3. To create or join a call room, you need to specify the roomid and userid first. Implemented by TRTCLoginForm
/// 
/// 4. The following implements the SDK's internal callbacks
///    and finally calls back to the main thread operation after
///    the data is called back (see onEnterRoom) to ensure that the UI doesn't result in cross-threading operations
/// </summary>

namespace TRTCCSharpDemo
{
    public partial class TRTCMainForm : Form, ITRTCCloudCallback, ITRTCLogCallback, ITRTCAudioFrameCallback, ITXDeviceObserver
    {
        private ITRTCCloud mTRTCCloud;
        private ITXDeviceManager mDeviceManager;
        //private ITXLiteAVLocalRecord mLiteAVLocalRecord;

        // Render mode
        // 1 for true window rendering (passed to SDK via window handle),
        // 2 for custom rendering (rendered using TXLiteAVVideoView)
        // default: 1(True window rendering)
        private int mRenderMode = 1;
        private IntPtr mCameraVideoHandle;

        private string mUserId;          // Local user Id
        private uint mRoomId;            // Room Id

        private bool mIsSetScreenSuccess;   // Whether the screen parameters are successfully set

        private List<UserVideoMeta> mMixStreamVideoMeta;   // Mixed flow information
        private List<RemoteUserInfo> mRemoteUsers;    // Remote users in the current room (except local users)
        private List<PKUserInfo> mPKUsers;  // The current Line user in the room

        // Cameras, microphones, and speaker devices currently in use
        private string mCurCameraDevice;
        private string mCurMicDevice;
        private string mCurSpeakerDevice;

        private TRTCScreenCaptureSourceInfo mScreenCaptureSourceInfo;
        // Log level: 0 indicates that the dashboard is not displayed, and 1 indicates that the dashboard is displayed
        private int mLogLevel = 0;

        // Window instance
        private TRTCLoginForm mLoginForm;
        private TRTCSettingForm mSettingForm;
        private VedioSettingForm mVedioSettingForm;
        private AudioSettingForm mAudioSettingForm;
        private OtherSettingForm mOtherSettingForm;
        private TRTCBeautyForm mBeautyForm;
        private AudioEffectOldForm mAudioEffectOldForm;
        private AudioEffectForm mAudioEffectForm;
        private TRTCConnectionForm mConnectionForm;
        private ToastForm mToastForm;
        private TXVodPlayerForm mVodPlayerForm;

        // If first time to eixt
        private bool mIsFirstExitRoom;

        private Dictionary<string, TXLiteAVVideoView> mVideoViews;

        public TRTCMainForm(TRTCLoginForm loginForm)
        {
            // Init Form
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;

            this.Disposed += new EventHandler(OnDisposed);
            // The window exits event listening
            this.FormClosing += new FormClosingEventHandler(OnExitLabelClick);

            // Init data
            mLoginForm = loginForm;
            mTRTCCloud = DataManager.GetInstance().trtcCloud;
            mDeviceManager = mTRTCCloud.getDeviceManager();
            mDeviceManager.setDeviceObserver(this);
            //mLiteAVLocalRecord = DataManager.GetInstance().txLiteAVLocalRecord;
            mMixStreamVideoMeta = new List<UserVideoMeta>();
            mRemoteUsers = new List<RemoteUserInfo>();
            mPKUsers = new List<PKUserInfo>();
            mVideoViews = new Dictionary<string, TXLiteAVVideoView>();

            // Init SDK configuration and set callback
            Log.I(String.Format(" SDKVersion : {0}", mTRTCCloud.getSDKVersion()));
            mTRTCCloud.addCallback(this);
            mTRTCCloud.setLogCallback(this);
            mTRTCCloud.setConsoleEnabled(false);
            mTRTCCloud.setLogLevel(TRTCLogLevel.TRTCLogLevelVerbose);

            // Listen for a single instance event, and when the application icon is clicked, the event is returned
            ThreadPool.RegisterWaitForSingleObject(Program.ProgramStarted, OnProgramStarted, null, -1, false);
        }

        private void SetRenderMode(int mode)
        {
            mRenderMode = mode;
        }


        private void OnProgramStarted(object state, bool timeout)
        {
            // Go back to the UI thread to do this, in case the window handle has not been created yet
            if (this.IsHandleCreated)
            {
                this.BeginInvoke(new Action(() =>
                {
                    if (this.IsHandleCreated)
                    {
                        this.Show();
                        this.WindowState = FormWindowState.Normal; // Note:Sure to set properties after form displayed
                        this.Activate();
                    }
                }));
            }
        }

        /// <summary>
        /// Add a custom render View and bind render callbacks
        /// </summary>
        private void AddCustomVideoView(Control parent, string userId, TRTCVideoStreamType streamType, bool local = false)
        {
            TXLiteAVVideoView videoView = new TXLiteAVVideoView();
            videoView.RegEngine(userId, streamType, mTRTCCloud, local);
            videoView.SetRenderMode(DataManager.GetInstance().videoFillMode);
            videoView.Size = new Size(parent.Width, parent.Height);
            parent.Controls.Add(videoView);
            string key = String.Format("{0}_{1}", userId, streamType);
            mVideoViews.Add(key, videoView);
        }

        /// <summary>
        /// Remove the custom render View and unbind the render callback
        /// </summary>
        private void RemoveCustomVideoView(Control parent, string userId, TRTCVideoStreamType streamType, bool local = false)
        {
            TXLiteAVVideoView videoView = null;
            string key = String.Format("{0}_{1}", userId, streamType);
            if (mVideoViews.TryGetValue(key, out videoView))
            {
                videoView.RemoveEngine(mTRTCCloud);
                mVideoViews.Remove(key);
                parent.Controls.Remove(videoView);
            }
        }

        private void OnDisposed(object sender, EventArgs e)
        {
            // Resource cleaning
            if (mTRTCCloud != null)
            {
                mTRTCCloud.removeCallback(this);
                mTRTCCloud.setLogCallback(null);
                mTRTCCloud = null;
            }
            mDeviceManager = null;
        }

        private void OnFormLoad(object sender, EventArgs e)
        {
            if (mRenderMode == 1)
                mCameraVideoHandle = this.localVideoPanel.Handle;
            else if (mRenderMode == 2)
                mCameraVideoHandle = IntPtr.Zero;
        }

        /// <summary>
        /// Set the local configuration information to the SDK
        /// </summary>
        private void InitLocalConfig()
        {
            // To determine whether to enable local and remote mirroring, users can use them separately
            if (DataManager.GetInstance().isLocalVideoMirror && DataManager.GetInstance().isRemoteVideoMirror)
            {
                Mirror(true);
            }
            // Determine whether to enable the volume notification function
            if (DataManager.GetInstance().isShowVolume)
            {
                VoicePrompt(true);
            }
            // Determine whether to enable the remote mixing function
            if (DataManager.GetInstance().isMixTranscoding)
            {
                this.mixTransCodingCheckBox.Checked = true;
                OnMixTransCodingCheckBoxClick(null, null);
            }
            // Determine whether to enable the beauty function
            if (DataManager.GetInstance().isOpenBeauty)
                mTRTCCloud.setBeautyStyle(DataManager.GetInstance().beautyStyle, DataManager.GetInstance().beauty,
                    DataManager.GetInstance().white, DataManager.GetInstance().ruddiness);
        }

        #region Form Move Function

        private bool mIsMouseDown = false;
        private Point mFormLocation;     // Location of Form
        private Point mMouseOffset;      // Mouse press position

        private void OnFormMouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                mIsMouseDown = true;
                mFormLocation = this.Location;
                mMouseOffset = Control.MousePosition;
            }
        }

        private void OnFormMouseUp(object sender, MouseEventArgs e)
        {
            mIsMouseDown = false;
        }

        private void OnFormMouseMove(object sender, MouseEventArgs e)
        {
            if (mIsMouseDown)
            {
                Point pt = Control.MousePosition;
                int x = mMouseOffset.X - pt.X;
                int y = mMouseOffset.Y - pt.Y;

                this.Location = new Point(mFormLocation.X - x, mFormLocation.Y - y);
            }
        }

        #endregion

        /// <summary>
        /// Displays the Message Dialog
        /// </summary>
        private void ShowMessage(string text, int delay = 0)
        {
            // Whether HWND created, in case of exception
            if (this.IsHandleCreated)
            {
                this.BeginInvoke(new Action(() =>
                {
                    MessageForm msgBox = new MessageForm();
                    msgBox.setText(text, delay);
                    msgBox.setCancelBtn(false);
                    msgBox.ShowDialog();
                }));
            }
        }

        /// <summary>
        /// Users turn on the camera and microphone before entering the room and after entering the room
        /// </summary>
        /// <param name="params"></param>
        public void EnterRoom()
        {
            // Determine whether a network proxy is required
            if (DataManager.GetInstance().isUseProxy)
            {
                ITXNetworkProxy networkProxy = ITXNetworkProxy.createTXNetworkProxy();
                networkProxy.setSocks5Proxy(DataManager.GetInstance().socks5ProxyIp, DataManager.GetInstance().socks5ProxyPort, "", "");
                ITXNetworkProxy.destroyTXNetworkProxy(networkProxy);
            }
            // Set the parameters required to enter the room
            TRTCParams trtcParams = new TRTCParams();
            trtcParams.sdkAppId = GenerateTestUserSig.SDKAPPID;
            trtcParams.roomId = DataManager.GetInstance().roomId;
            trtcParams.userId = DataManager.GetInstance().userId;
            trtcParams.strRoomId = DataManager.GetInstance().strRoomId;
            trtcParams.userSig = GenerateTestUserSig.GetInstance().GenTestUserSig(DataManager.GetInstance().userId);
            // If you have the rights protection requirements into the room
            // Reference document {https://cloud.tencent.com/document/product/647/32240} to complete this function.
            // Add privateMapKey obtained on the server in the following field for users who have access to the room.
            trtcParams.privateMapKey = "";
            trtcParams.businessInfo = "";
            trtcParams.role = DataManager.GetInstance().roleType;
            // If your project has audio-only bypass live broadcast requirements, please configure the parameters.
            // After this parameter is set, the audio reaches the server, and the automatic bypass starts.
            // Otherwise, if this parameter not specified,
            // the bypass discards the received audio packets before receiving the first video frame.
            if (DataManager.GetInstance().pureAudioStyle)
                trtcParams.businessInfo = "{\"Str_uc_params\":{\"pure_audio_push_mod\": 1}}";
            else
                trtcParams.businessInfo = "";


            // Customer check-in
            mTRTCCloud.enterRoom(ref trtcParams, DataManager.GetInstance().appScene);

            // If in video call mode, weak network resolution reduction is enabled by default
            if (DataManager.GetInstance().appScene == TRTCAppScene.TRTCAppSceneVideoCall)
            {
                DataManager.GetInstance().videoEncParams.enableAdjustRes = true;
            }

            // Set the default parameter Settings
            TRTCVideoEncParam encParams = DataManager.GetInstance().videoEncParams;   // Video coding parameter Settings
            TRTCNetworkQosParam qosParams = DataManager.GetInstance().qosParams;      // Set network flow control paras
            mTRTCCloud.setVideoEncoderParam(ref encParams);
            mTRTCCloud.setNetworkQosParam(ref qosParams);
            TRTCRenderParams renderParams = DataManager.GetInstance().GetRenderParams();
            mTRTCCloud.setLocalRenderParams(ref renderParams);
            mTRTCCloud.setVideoEncoderMirror(DataManager.GetInstance().isRemoteVideoMirror);
            // Set beauty
            if (DataManager.GetInstance().isOpenBeauty)
                mTRTCCloud.setBeautyStyle(DataManager.GetInstance().beautyStyle, DataManager.GetInstance().beauty,
                    DataManager.GetInstance().white, DataManager.GetInstance().ruddiness);

            // Set size stream
            if (DataManager.GetInstance().pushSmallVideo)
            {
                TRTCVideoEncParam param = new TRTCVideoEncParam
                {
                    videoFps = 15,
                    videoBitrate = 100,
                    videoResolution = TRTCVideoResolution.TRTCVideoResolution_320_240
                };
                mTRTCCloud.enableSmallVideoStream(true, ref param);
            }
            // Room information
            mUserId = trtcParams.userId;
            mRoomId = trtcParams.roomId;
            string userid = trtcParams.userId;
            userid = userid.Length > 10 ? userid.Substring(0, 10) + "..." : userid;
            this.roomLabel.Text = "房间号：" + trtcParams.roomId.ToString() + "   用户名：" + userid;
            this.localUserLabel.Text = mUserId;

            // Whether to process only audio mode
            bool audioCallStyle = DataManager.GetInstance().pureAudioStyle;
            if (audioCallStyle)
            {
                this.localInfoLabel.Visible = true;
                this.localInfoLabel.Text = "纯音频模式";
                this.muteVideoCheckBox.Checked = true;
            }
            else
            {
                this.localInfoLabel.Text = "视频已关闭";
                this.localInfoLabel.Visible = false;
                StartLocalVideo();
            }
            // If not the audience character enters the room, turn on the local camera to capture and preview
            if (trtcParams.role != TRTCRoleType.TRTCRoleAudience)
                mTRTCCloud.startLocalAudio(DataManager.GetInstance().AudioQuality);

            InitLocalDevice();
            InitLocalConfig();
        }

        private void StartLocalVideo()
        {
            mTRTCCloud.startLocalPreview(mCameraVideoHandle);
            if (mRenderMode == 2)
            {
                AddCustomVideoView(this.localVideoPanel, mUserId, TRTCVideoStreamType.TRTCVideoStreamTypeBig, true);
            }
        }

        private void StopLocalVideo()
        {
            if (mRenderMode == 1)
            {
                mTRTCCloud.stopLocalPreview();
            }
            else if (mRenderMode == 2)
            {
                mTRTCCloud.stopLocalPreview();
                RemoveCustomVideoView(this.localVideoPanel, mUserId, TRTCVideoStreamType.TRTCVideoStreamTypeBig, true);
            }
        }

        /// <summary>
        /// The result of the callback after entering the room
        /// </summary>
        /// <param name="result">If less than 0, the incoming room fails thendisplayed in the onError callback</param>
        public void onEnterRoom(int result)
        {
            // The thread after the callback is not necessarily in the main thread,
            // so you need to uniformly return to the main thread operation at the place of the callback
            // to ensure that it does not cause cross-thread operation UI.
            if (this.IsHandleCreated)
                this.Invoke(new Action(() =>
                {
                    if (result >= 0)
                    {
                        // Successful admission
                        DataManager.GetInstance().enterRoom = true;
                        // Make sure audio and video capture inside the SDK is turned on.
                        mTRTCCloud.muteLocalVideo(TRTCVideoStreamType.TRTCVideoStreamTypeBig, false);
                        mTRTCCloud.muteLocalAudio(false);
                        this.startLocalPreviewCheckBox.Checked = true;
                        this.startLocalAudioCheckBox.Checked = true;
                        // Update mixed flow information
                        UpdateMixTranCodeInfo();
                        mTRTCCloud.setAudioFrameCallback(this);
                    }
                    else
                    {
                        // Entry failure
                        DataManager.GetInstance().enterRoom = false;
                        ShowMessage("进房失败，请重试");
                    }
                }));
        }

        /// <summary>
        /// Init local device
        /// </summary>
        private void InitLocalDevice()
        {
            ITRTCDeviceCollection cameraList = mDeviceManager.getDevicesList(TXMediaDeviceType.TXMediaDeviceTypeCamera);
            mCurCameraDevice = "";
            if (cameraList.getCount() > 0)
            {
                ITRTCDeviceInfo camera = mDeviceManager.getCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeCamera);
                mCurCameraDevice = camera.getDevicePID();
            }
            cameraList.release();
            ITRTCDeviceCollection micList = mDeviceManager.getDevicesList(TXMediaDeviceType.TXMediaDeviceTypeMic);
            mCurMicDevice = "";
            if (micList.getCount() > 0)
            {
                ITRTCDeviceInfo mic = mDeviceManager.getCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeMic);
                mCurMicDevice = mic.getDevicePID();
            }
            micList.release();
            ITRTCDeviceCollection speakerList = mDeviceManager.getDevicesList(TXMediaDeviceType.TXMediaDeviceTypeSpeaker);
            mCurSpeakerDevice = "";
            if (speakerList.getCount() > 0)
            {
                ITRTCDeviceInfo speaker = mDeviceManager.getCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeSpeaker);
                mCurSpeakerDevice = speaker.getDevicePID();
            }
            speakerList.release();
        }



        /// <summary>
        /// Callback SDK thrown error information, thrown errors should not continue to use, but to troubleshoot errors.
        /// Several error notification:
        /// </summary>
        public void onError(TXLiteAVError errCode, string errMsg, IntPtr arg)
        {
            Log.E(String.Format("errCode : {0}, errMsg : {1}, arg = {2}", errCode, errMsg, arg));
            if (errCode == TXLiteAVError.ERR_SERVER_CENTER_ANOTHER_USER_PUSH_SUB_VIDEO ||
                errCode == TXLiteAVError.ERR_SERVER_CENTER_NO_PRIVILEDGE_PUSH_SUB_VIDEO ||
                errCode == TXLiteAVError.ERR_SERVER_CENTER_INVALID_PARAMETER_SUB_VIDEO)
            {
                ShowMessage("Error: 屏幕分享发起失败，是否当前已经有人发起了共享！");
            }
            else if (errCode == TXLiteAVError.ERR_MIC_START_FAIL || errCode == TXLiteAVError.ERR_CAMERA_START_FAIL ||
                errCode == TXLiteAVError.ERR_SPEAKER_START_FAIL)
            {
                ShowMessage("Error: 请检查本地电脑设备。");
            }
            else
            {
                ShowMessage(String.Format("SDK出错[err:{0},msg:{1}]", errCode, errMsg));
            }
        }

        /// <summary>
        /// The local user exits the room callback
        /// </summary>
        public void onExitRoom(int reason)
        {
            DataManager.GetInstance().enterRoom = false;
            this.Close();
        }

        /// <summary>
        /// Clean up after checking out and disable SDK internal functions.
        /// </summary>
        private void Uninit()
        {
            // If custom capture and rendering are enabled, disable the function to clear resources
            if (DataManager.GetInstance().isLocalVideoMirror && DataManager.GetInstance().isRemoteVideoMirror)
            {
                mTRTCCloud.enableCustomVideoCapture(TRTCVideoStreamType.TRTCVideoStreamTypeBig, false);
                mTRTCCloud.enableCustomAudioCapture(false);
            }
            mTRTCCloud.stopAllRemoteView();
            if (mRenderMode == 1)
            {
                mTRTCCloud.stopLocalPreview();
            }
            else if (mRenderMode == 2)
            {
                StopLocalVideo();
                foreach (var item in mVideoViews)
                {
                    if (item.Value != null)
                    {
                        item.Value.RemoveEngine(mTRTCCloud);
                    }
                }
                TXLiteAVVideoView.RemoveAllRegEngine();
            }

            mTRTCCloud.stopLocalAudio();
            mTRTCCloud.muteLocalAudio(true);
            mTRTCCloud.muteLocalVideo(TRTCVideoStreamType.TRTCVideoStreamTypeBig, true);

            // Clear mixed flow information and user information
            mMixStreamVideoMeta.Clear();
            mRemoteUsers.Clear();
            mPKUsers.Clear();

            if (this.screenShareCheckBox.Checked)
            {
                mTRTCCloud.stopScreenCapture();
                if (mToastForm != null)
                    mToastForm.Close();
            }
            if (this.mixTransCodingCheckBox.Checked)
                mTRTCCloud.setMixTranscodingConfig(null);

        }

        /// <summary>
        /// Whether to enable uplink audio
        /// </summary>
        public void onUserAudioAvailable(string userId, bool available)
        {
            Log.I(String.Format("onUserAudioAvailable : userId = {0}, available = {1}", userId, available));
            mTRTCCloud.setAudioFrameCallback(this);
        }

        /// <summary>
        /// Note: this one abandoned, use onRemoteUserEnterRoom instead.
        /// </summary>
        public void onUserEnter(string userId) { }

        /// <summary>
        /// Note：this one abandoned, use onRemoteUserLeaveRoom instead.
        /// </summary>
        public void onUserExit(string userId, int reason) { }

        public void onRemoteVideoStatusUpdated(string userId, TRTCVideoStreamType streamType, TRTCAVStatusType status, TRTCAVStatusChangeReason reason, IntPtr extrainfo)
        {

        }

        public void onRemoteUserEnterRoom(string userId)
        {
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    // Example Add information about the remote incoming user
                    mRemoteUsers.Add(new RemoteUserInfo() { userId = userId, position = -1 });
                    OnPKUserEnter(userId);
                }));
        }

        public void onRemoteUserLeaveRoom(string userId, int reason)
        {
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    // Clear the remote user check-out information and reclaim the window
                    OnPKUserExit(userId);
                    int pos = FindOccupyRemoteVideoPosition(userId, true);
                    if (pos != -1)
                    {
                        foreach (RemoteUserInfo user in mRemoteUsers)
                        {
                            if (user.userId.Equals(userId))
                            {
                                mRemoteUsers.Remove(user);
                                break;
                            }
                        }
                    }
                }));
        }

        /// <summary>
        /// The remote user enters the room
        /// </summary>
        private void OnPKUserEnter(string userId)
        {
            foreach (PKUserInfo info in mPKUsers)
            {
                if (info.userId.Equals(userId))
                {
                    info.isEnterRoom = true;
                    Log.I(String.Format("连麦用户[{0}]进入房间", userId));
                    break;
                }
            }
        }

        /// <summary>
        /// Get free window handles to distribute to incoming users or screen sharing interfaces
        /// </summary>
        private IntPtr GetHandleAndSetUserId(int pos, string userId, bool isOpenSubStream)
        {
            switch (pos)
            {
                case 1:
                    this.remoteUserLabel1.Text = userId + (isOpenSubStream ? "(屏幕分享)" : "");
                    return this.remoteVideoPanel1.Handle;
                case 2:
                    this.remoteUserLabel2.Text = userId + (isOpenSubStream ? "(屏幕分享)" : "");
                    return this.remoteVideoPanel2.Handle;
                case 3:
                    this.remoteUserLabel3.Text = userId + (isOpenSubStream ? "(屏幕分享)" : "");
                    return this.remoteVideoPanel3.Handle;
                case 4:
                    this.remoteUserLabel4.Text = userId + (isOpenSubStream ? "(屏幕分享)" : "");
                    return this.remoteVideoPanel4.Handle;
                case 5:
                    this.remoteUserLabel5.Text = userId + (isOpenSubStream ? "(屏幕分享)" : "");
                    return this.remoteVideoPanel5.Handle;
                default:
                    return IntPtr.Zero;
            }
        }

        /// <summary>
        /// Get the window location of the remote user
        /// </summary>
        private int GetRemoteVideoPosition(String userId)
        {
            if (this.remoteUserLabel1.Text.Equals(userId))
                return 1;
            else if (this.remoteUserLabel2.Text.Equals(userId))
                return 2;
            else if (this.remoteUserLabel3.Text.Equals(userId))
                return 3;
            else if (this.remoteUserLabel4.Text.Equals(userId))
                return 4;
            else if (this.remoteUserLabel5.Text.Equals(userId))
                return 5;
            return -1;
        }

        /// <summary>
        /// Get the location of the free window
        /// </summary>
        private int GetIdleRemoteVideoPosition(String userId)
        {
            if (string.IsNullOrEmpty(this.remoteUserLabel1.Text) || this.remoteUserLabel1.Text.Equals(userId))
                return 1;
            else if (string.IsNullOrEmpty(this.remoteUserLabel2.Text) || this.remoteUserLabel2.Text.Equals(userId))
                return 2;
            else if (string.IsNullOrEmpty(this.remoteUserLabel3.Text) || this.remoteUserLabel3.Text.Equals(userId))
                return 3;
            else if (string.IsNullOrEmpty(this.remoteUserLabel4.Text) || this.remoteUserLabel4.Text.Equals(userId))
                return 4;
            else if (string.IsNullOrEmpty(this.remoteUserLabel5.Text) || this.remoteUserLabel5.Text.Equals(userId))
                return 5;
            return -1;
        }

        /// <summary>
        /// Line user exits the room and clears user information
        /// </summary>
        /// <param name="userId"></param>
        private void OnPKUserExit(string userId)
        {
            foreach (PKUserInfo info in mPKUsers)
            {
                if (info.userId.Equals(userId) && info.isEnterRoom)
                {
                    mPKUsers.Remove(info);
                    Log.I(String.Format("连麦用户[{0}]离开房间", userId));
                    break;
                }
            }
            if (mPKUsers.Count <= 0)
                this.connectRoomCheckBox.Checked = false;
        }

        /// <summary>
        /// Find the location of the current window of user screen based on whether the user has checked out
        /// </summary>
        private int FindOccupyRemoteVideoPosition(string userId, bool isExitRoom)
        {
            int pos = -1;
            if (this.remoteUserLabel1.Text.Equals(userId))
            {
                pos = 1;
                if (isExitRoom)
                    this.remoteUserLabel1.Text = "";
            }
            if (this.remoteUserLabel2.Text.Equals(userId))
            {
                pos = 2;
                if (isExitRoom)
                    this.remoteUserLabel2.Text = "";
            }
            if (this.remoteUserLabel3.Text.Equals(userId))
            {
                pos = 3;
                if (isExitRoom)
                    this.remoteUserLabel3.Text = "";
            }
            if (this.remoteUserLabel4.Text.Equals(userId))
            {
                pos = 4;
                if (isExitRoom)
                    this.remoteUserLabel4.Text = "";
            }
            if (this.remoteUserLabel5.Text.Equals(userId))
            {
                pos = 5;
                if (isExitRoom)
                    this.remoteUserLabel5.Text = "";
            }
            if (isExitRoom)
                SetVisableInfoView(pos, true);
            return pos;
        }

        /// <summary>
        /// Whether the user enables the secondary stream
        /// </summary>
        public void onUserSubStreamAvailable(string userId, bool available)
        {
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    if (available)
                    {
                        // The remote secondary flow page is displayed
                        int pos = GetIdleRemoteVideoPosition(userId + "(屏幕分享)");
                        if (pos != -1)
                        {
                            SetVisableInfoView(pos, false);
                            if (mRenderMode == 1)
                            {
                                IntPtr ptr = GetHandleAndSetUserId(pos, userId, true);
                                TRTCRenderParams renderParams = DataManager.GetInstance().GetRenderParams();
                                mTRTCCloud.setRemoteRenderParams(userId, TRTCVideoStreamType.TRTCVideoStreamTypeSub, ref renderParams);
                                mTRTCCloud.startRemoteView(userId, TRTCVideoStreamType.TRTCVideoStreamTypeSub, ptr);
                            }
                            else if (mRenderMode == 2)
                            {
                                Panel panel = GetPanelAndSetUserId(pos, userId, true, true);
                                mTRTCCloud.startRemoteView(userId, TRTCVideoStreamType.TRTCVideoStreamTypeSub, IntPtr.Zero);
                                if (panel != null)
                                    AddCustomVideoView(panel, userId, TRTCVideoStreamType.TRTCVideoStreamTypeSub);
                            }
                        }
                    }
                    else
                    {
                        // The remote secondary flow interface is removed
                        int pos = FindOccupyRemoteVideoPosition(userId + "(屏幕分享)", true);
                        if (pos != -1)
                        {
                            mTRTCCloud.stopRemoteView(userId, TRTCVideoStreamType.TRTCVideoStreamTypeSub);
                            if (mRenderMode == 2)
                            {
                                Panel panel = GetPanelAndSetUserId(pos, userId, false, true);
                                if (panel != null)
                                    RemoveCustomVideoView(panel, userId, TRTCVideoStreamType.TRTCVideoStreamTypeSub);
                            }
                            RemoveVideoMeta(userId, TRTCVideoStreamType.TRTCVideoStreamTypeSub);
                            UpdateMixTranCodeInfo();
                        }
                    }
                }));
        }

        /// <summary>
        /// Whether to enable camera video
        /// </summary>
        public void onUserVideoAvailable(string userId, bool available)
        {
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    // Whether the user has exited the room
                    bool isExit = mRemoteUsers.Exists((user) =>
                    {
                        if (user.userId.Equals(userId)) return true;
                        else return false;
                    });
                    if (!isExit) return;
                    if (available)
                    {
                        // Display the mainstream screen of the remote user
                        int pos = GetIdleRemoteVideoPosition(userId);
                        if (pos != -1)
                        {
                            SetVisableInfoView(pos, false);
                            TRTCVideoStreamType streamType = DataManager.GetInstance().playSmallVideo ?
                                    TRTCVideoStreamType.TRTCVideoStreamTypeSmall : TRTCVideoStreamType.TRTCVideoStreamTypeBig;
                            if (mRenderMode == 1)
                            {
                                IntPtr ptr = GetHandleAndSetUserId(pos, userId, false);
                                TRTCRenderParams renderParams = DataManager.GetInstance().GetRenderParams();
                                mTRTCCloud.setRemoteRenderParams(userId, TRTCVideoStreamType.TRTCVideoStreamTypeBig, ref renderParams);
                                
                                mTRTCCloud.startRemoteView(userId, streamType, ptr);
                            }
                            else if (mRenderMode == 2)
                            {
                                Panel panel = GetPanelAndSetUserId(pos, userId);
                                mTRTCCloud.startRemoteView(userId, streamType, IntPtr.Zero);
                                if (panel != null)
                                    AddCustomVideoView(panel, userId, TRTCVideoStreamType.TRTCVideoStreamTypeBig);
                            }

                            foreach (RemoteUserInfo info in mRemoteUsers)
                            {
                                if (info.userId.Equals(userId))
                                {
                                    info.position = pos;
                                    break;
                                }
                            }
                            if (DataManager.GetInstance().isShowVolume)
                                SetRemoteVoiceVisable(pos, true);
                        }
                    }
                    else
                    {
                        // Remove the mainstream screen of the remote user
                        int pos = GetRemoteVideoPosition(userId);
                        if (pos != -1)
                        {
                            SetVisableInfoView(pos, true);
                            mTRTCCloud.stopRemoteView(userId, TRTCVideoStreamType.TRTCVideoStreamTypeBig);
                            if (mRenderMode == 2)
                            {
                                Panel panel = GetPanelAndSetUserId(pos, userId, false, false);
                                if (panel != null)
                                    RemoveCustomVideoView(panel, userId, TRTCVideoStreamType.TRTCVideoStreamTypeBig);
                            }
                            // Disable the remote volume notification
                            if (DataManager.GetInstance().isShowVolume)
                                SetRemoteVoiceVisable(pos, false);
                            RemoveVideoMeta(userId, TRTCVideoStreamType.TRTCVideoStreamTypeBig);
                            UpdateMixTranCodeInfo();
                        }
                    }
                }));
        }

        private Panel GetPanelAndSetUserId(int pos, string userId, bool isSetUserId = true, bool isOpenSubStream = false)
        {
            switch (pos)
            {
                case 1:
                    if (isSetUserId)
                        this.remoteUserLabel1.Text = userId + (isOpenSubStream ? "(屏幕分享)" : "");
                    return this.remoteVideoPanel1;
                case 2:
                    if (isSetUserId)
                        this.remoteUserLabel2.Text = userId + (isOpenSubStream ? "(屏幕分享)" : "");
                    return this.remoteVideoPanel2;
                case 3:
                    if (isSetUserId)
                        this.remoteUserLabel3.Text = userId + (isOpenSubStream ? "(屏幕分享)" : "");
                    return this.remoteVideoPanel3;
                case 4:
                    if (isSetUserId)
                        this.remoteUserLabel4.Text = userId + (isOpenSubStream ? "(屏幕分享)" : "");
                    return this.remoteVideoPanel4;
                case 5:
                    if (isSetUserId)
                        this.remoteUserLabel5.Text = userId + (isOpenSubStream ? "(屏幕分享)" : "");
                    return this.remoteVideoPanel5;
                default:
                    return null;
            }
        }

        /// <summary>
        /// Whether the remote user is prompted to open the video
        /// </summary>
        private void SetVisableInfoView(int pos, bool visable)
        {
            switch (pos)
            {
                case 1:
                    this.remoteInfoLabel1.Visible = visable;
                    break;
                case 2:
                    this.remoteInfoLabel2.Visible = visable;
                    break;
                case 3:
                    this.remoteInfoLabel3.Visible = visable;
                    break;
                case 4:
                    this.remoteInfoLabel4.Visible = visable;
                    break;
                case 5:
                    this.remoteInfoLabel5.Visible = visable;
                    break;
            }
        }

        private void OnExitLabelClick(object sender, EventArgs e)
        {
            // Exit the room
            if (mIsFirstExitRoom) return;
            mIsFirstExitRoom = true;
            if (mBeautyForm != null)
                mBeautyForm.Close();
            if (mAudioEffectOldForm != null)
                mAudioEffectOldForm.Close();
            if (mAudioEffectForm != null)
                mAudioEffectForm.Close();
            if (mSettingForm != null)
                mSettingForm.Close();
            if (mVedioSettingForm != null)
                mVedioSettingForm.Close();
            if (mAudioSettingForm != null)
                mAudioSettingForm.Close();
            if (mOtherSettingForm != null)
                mOtherSettingForm.Close();
            if (mConnectionForm != null)
                mConnectionForm.Close();
            if (mVodPlayerForm != null)
                mVodPlayerForm.Close();
            Uninit();
            // If enter successfully, check out normally and then close the window to keep resources remained
            if (DataManager.GetInstance().enterRoom)
                mTRTCCloud.exitRoom();
            else
                onExitRoom(0);
            mLoginForm.Show();
        }

        private void OnSettingLabelClick(object sender, EventArgs e)
        {
            if (mSettingForm == null)
                mSettingForm = new TRTCSettingForm();
            mSettingForm.ShowDialog();
        }

        /// <summary>
        /// Open dashboard information (custom rendering is temporarily unable to display dashboard information)
        /// </summary>
        private void OnLogCheckBoxClick(object sender, EventArgs e)
        {
            mLogLevel = mLogLevel == 0 ? 1 : 0;
            
            if(mLogLevel > 0)
            {
                this.logCheckBox.Checked = true;
            }
            else
            {
                this.logCheckBox.Checked = false;
            }
            if (mTRTCCloud != null)
            {
                mTRTCCloud.showDebugView(mLogLevel);
            }
        }

        public void onLog(string log, TRTCLogLevel level, string module)
        {
            // SDK Internal log display
            //Log.I(String.Format("onLog : log = {0}, level = {1}, module = {2}", log, level, module));
        }

        /// <summary>
        /// Begins rendering the first frame of the local or remote user
        /// </summary>
        public void onFirstVideoFrame(string userId, TRTCVideoStreamType streamType, int width, int height)
        {
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    if (!this.screenShareCheckBox.Checked)
                    {
                        if (string.IsNullOrEmpty(userId) && streamType == TRTCVideoStreamType.TRTCVideoStreamTypeSub)
                            return;
                    }
                    if (!string.IsNullOrEmpty(userId))
                    {
                        // Only up to 6 people can video at the same time
                        if (streamType == TRTCVideoStreamType.TRTCVideoStreamTypeBig && FindOccupyRemoteVideoPosition(userId, false) == -1)
                            return;
                        if (streamType == TRTCVideoStreamType.TRTCVideoStreamTypeSub && FindOccupyRemoteVideoPosition(userId + "(屏幕分享)", false) == -1)
                            return;
                    }

                    // Used to add mixed flow information of users (including local and remote users)
                    // and update mixed flow information in real time
                    if (string.IsNullOrEmpty(userId)) userId = mUserId;
                    bool find = false;
                    foreach (UserVideoMeta info in mMixStreamVideoMeta)
                    {
                        if (info.userId.Equals(userId) && info.streamType == streamType)
                        {
                            info.width = width;
                            info.height = height;
                            find = true;
                            break;
                        }
                        else if (info.userId.Equals(userId) && streamType != TRTCVideoStreamType.TRTCVideoStreamTypeSub)
                        {
                            info.streamType = streamType;
                            find = true;
                            break;
                        }
                    }
                    if (!find && !(streamType == TRTCVideoStreamType.TRTCVideoStreamTypeBig && userId == mUserId))
                    {
                        UserVideoMeta info = new UserVideoMeta();
                        info.streamType = streamType;
                        info.userId = userId;
                        info.width = width;
                        info.height = height;
                        mMixStreamVideoMeta.Add(info);
                        UpdateMixTranCodeInfo();
                    }
                    else
                    {
                        if (userId != mUserId)
                            UpdateMixTranCodeInfo();
                    }
                }));
        }

        public void onFirstAudioFrame(string userId)
        {
            Log.I(String.Format("onFirstAudioFrame : userId = {0}", userId));
        }

        public void onSendFirstLocalVideoFrame(TRTCVideoStreamType streamType)
        {
            Log.I(String.Format("onSendFirstLocalVideoFrame : streamType = {0}", streamType));
        }

        public void onSendFirstLocalAudioFrame()
        {
            Log.I(String.Format("onSendFirstLocalAudioFrame"));
        }

        private void OnScreenShareCheckBoxClick(object sender, EventArgs e)
        {
            if (!DataManager.GetInstance().enterRoom)
            {
                ShowMessage("进房失败，请重试");
                this.screenShareCheckBox.Checked = false;
                return;
            }
            if (this.screenShareCheckBox.Checked)
            {
                // Enable screen sharing function
                TRTCScreenForm screenForm = new TRTCScreenForm(this);
                screenForm.ShowDialog();
            }
            else
            {
                // Disable screen sharing function
                if (!mIsSetScreenSuccess) return;
                mTRTCCloud.stopScreenCapture();
                if (mToastForm != null)
                    mToastForm.Hide();

                // Removes screen sharing in the mix stream
                RemoveVideoMeta(mUserId, TRTCVideoStreamType.TRTCVideoStreamTypeSub);
                UpdateMixTranCodeInfo();
            }
        }

        private void OnMixTransCodingCheckBoxClick(object sender, EventArgs e)
        {
            if (this.mixTransCodingCheckBox.Checked)
            {
                // Enable the cloud screen mixing function
                DataManager.GetInstance().isMixTranscoding = true;
                UpdateMixTranCodeInfo();
            }
            else
            {
                // Disable cloud image mixing
                DataManager.GetInstance().isMixTranscoding = false;
                mTRTCCloud.setMixTranscodingConfig(null);
            }
        }

        /// <summary>
        /// Remove user information from the mixing screen
        /// </summary>
        private void RemoveVideoMeta(string userId, TRTCVideoStreamType streamType)
        {
            foreach (UserVideoMeta info in mMixStreamVideoMeta)
            {
                if (info.userId == userId && (info.streamType == streamType ||
                    info.streamType != TRTCVideoStreamType.TRTCVideoStreamTypeSub))
                {
                    mMixStreamVideoMeta.Remove(info);
                    break;
                }
            }
        }

        /// <summary>
        /// Update cloud mixing interface information
        /// (local users enter the room or remote users enter the room or open the local screen sharing screen needs to be updated)
        /// </summary>
        private void UpdateMixTranCodeInfo()
        {
            // Exit if cloud mixing function not enabled
            if (!this.mixTransCodingCheckBox.Checked)
                return;

            // Exit(without mixing) if cloud mixing without auxiliary flow interface
            if (mMixStreamVideoMeta.Count == 0)
            {
                mTRTCCloud.setMixTranscodingConfig(null);
                return;
            }

            // If enter with pure audio, et each way to pure audio, and the cloud will only mix audio data
            if (DataManager.GetInstance().pureAudioStyle)
            {
                foreach (UserVideoMeta meta in mMixStreamVideoMeta)
                    meta.pureAudio = true;
            }

            // No mainstream, just stop mixing.
            if (this.muteVideoCheckBox.Checked && this.muteAudioCheckBox.Checked)
            {
                mTRTCCloud.setMixTranscodingConfig(null);
                return;
            }

            // Configure the local mainstream mixed traffic information
            UserVideoMeta localMainVideo = new UserVideoMeta()
            {
                userId = mUserId
            };
            // The User after connecting the microphone can set the corresponding roomId
            foreach (UserVideoMeta info in mMixStreamVideoMeta)
            {
                foreach (PKUserInfo pKUserInfo in mPKUsers)
                {
                    if (pKUserInfo.userId.Equals(info.userId))
                    {
                        info.roomId = pKUserInfo.roomId;
                        break;
                    }
                }
            }

            // Mixed flow here only for reference. If use other display methods, please refer to the following methods
            int canvasWidth = 960, canvasHeight = 720;
            int appId = GenerateTestUserSig.APPID;
            int bizId = GenerateTestUserSig.BIZID;

            if (appId == 0 || bizId == 0)
            {
                ShowMessage("混流功能不可使用，请在TRTCGetUserIDAndUserSig.h->TXCloudAccountInfo填写混流的账号信息\n");
                return;
            }
            TRTCTranscodingConfig config = new TRTCTranscodingConfig();
            config.mode = TRTCTranscodingConfigMode.TRTCTranscodingConfigMode_Manual;
            config.appId = (uint)appId;
            config.bizId = (uint)bizId;
            config.videoWidth = (uint)canvasWidth;
            config.videoHeight = (uint)canvasHeight;
            config.videoBitrate = 800;
            config.videoFramerate = 15;
            config.videoGOP = 1;
            config.audioSampleRate = 48000;
            config.audioBitrate = 64;
            config.audioChannels = 1;
            config.mixUsersArraySize = (uint)(1 + mMixStreamVideoMeta.Count);
            // Set the location information of each path subroutine (reference only)
            TRTCMixUser[] mixUsersArray = new TRTCMixUser[config.mixUsersArraySize];
            for (int i = 0; i < config.mixUsersArraySize; i++)
                mixUsersArray[i] = new TRTCMixUser();

            int zOrder = 1, index = 0;
            mixUsersArray[index].roomId = null;
            mixUsersArray[index].userId = localMainVideo.userId;
            mixUsersArray[index].pureAudio = localMainVideo.pureAudio;
            RECT rect = new RECT()
            {
                left = 0,
                top = 0,
                right = canvasWidth,
                bottom = canvasHeight
            };
            mixUsersArray[index].rect = rect;
            mixUsersArray[index].streamType = localMainVideo.streamType;
            mixUsersArray[index].zOrder = zOrder++;
            index++;
            foreach (UserVideoMeta info in mMixStreamVideoMeta)
            {
                int left = 20, top = 40;

                if (zOrder == 2)
                {
                    left = 240 / 4 * 3 + 240 * 2;
                    top = 240 / 3 * 1;
                }
                if (zOrder == 3)
                {
                    left = 240 / 4 * 3 + 240 * 2;
                    top = 240 / 3 * 2 + 240 * 1;
                }
                if (zOrder == 4)
                {
                    left = 240 / 4 * 2 + 240 * 1;
                    top = 240 / 3 * 1;
                }
                if (zOrder == 5)
                {
                    left = 240 / 4 * 2 + 240 * 1;
                    top = 240 / 3 * 2 + 240 * 1;
                }
                if (zOrder == 6)
                {
                    left = 240 / 4 * 1;
                    top = 240 / 3 * 1;
                }
                if (zOrder == 7)
                {
                    left = 240 / 4 * 1;
                    top = 240 / 3 * 2 + 240 * 1;
                }

                int right = 240 + left, bottom = 240 + top;
                if (info.roomId <= 0)
                    mixUsersArray[index].roomId = null;
                else
                    mixUsersArray[index].roomId = info.roomId.ToString();
                mixUsersArray[index].userId = info.userId;
                mixUsersArray[index].pureAudio = info.pureAudio;
                RECT rt = new RECT()
                {
                    left = left,
                    top = top,
                    right = right,
                    bottom = bottom
                };
                mixUsersArray[index].rect = rt;
                mixUsersArray[index].streamType = info.streamType;
                mixUsersArray[index].zOrder = zOrder;
                zOrder++;
                index++;
            }
            config.mixUsersArray = mixUsersArray;
            // Set cloud mixed flow configuration information
            mTRTCCloud.setMixTranscodingConfig(config);
        }

        private void OnStartLocalAudioClick(object sender, EventArgs e)
        {
            if (!DataManager.GetInstance().enterRoom)
            {
                ShowMessage("进房失败，请重试");
                this.startLocalAudioCheckBox.Checked = false;
                return;
            }
            
            if (this.startLocalAudioCheckBox.Checked)
            {
                mTRTCCloud.startLocalAudio(DataManager.GetInstance().AudioQuality);
            }
            else
            {
                mTRTCCloud.stopLocalAudio();
            }
        }

        private void OnStartLocalPreviewClick(object sender, EventArgs e)
        {
            if (!DataManager.GetInstance().enterRoom)
            {
                ShowMessage("进房失败，请重试");
                this.startLocalPreviewCheckBox.Checked = false;
                return;
            }
            if (DataManager.GetInstance().pureAudioStyle)
            {
                ShowMessage("Error: 纯音频场景，无法打开视频，请退房重新选择模式");
                this.startLocalPreviewCheckBox.Checked = false;
                return;
            }
            if (this.startLocalPreviewCheckBox.Checked)
            {
                StartLocalVideo();
                if (!this.muteVideoCheckBox.Checked)
                {
                    this.localInfoLabel.Visible = false;
                }
            }
            else
            {
                StopLocalVideo();
                this.localInfoLabel.Visible = true;
            }
        }

        private void OnMuteAudioCheckBoxClick(object sender, EventArgs e)
        {
            if (!DataManager.GetInstance().enterRoom)
            {
                ShowMessage("进房失败，请重试");
                this.muteAudioCheckBox.Checked = false;
                return;
            }
            if (this.muteAudioCheckBox.Checked)
            {
                // Mute local audio
                mTRTCCloud.muteLocalAudio(true);
            }
            else
            {
                // Enable local audio
                mTRTCCloud.muteLocalAudio(false);
            }
        }

        private void OnMuteVideoCheckBoxClick(object sender, EventArgs e)
        {
            if (!DataManager.GetInstance().enterRoom)
            {
                ShowMessage("进房失败，请重试");
                this.muteVideoCheckBox.Checked = false;
                return;
            }
            if (DataManager.GetInstance().pureAudioStyle)
            {
                ShowMessage("Error: 纯音频场景，无法打开视频，请退房重新选择模式");
                this.muteVideoCheckBox.Checked = true;
                return;
            }
            if (!this.muteVideoCheckBox.Checked)
            {
                // Enable the local video screen
                mTRTCCloud.muteLocalVideo(TRTCVideoStreamType.TRTCVideoStreamTypeBig, false);
                this.localInfoLabel.Visible = false;
            }
            else
            {
                // Disable the local video screen
                this.localInfoLabel.Visible = true;
                mTRTCCloud.muteLocalVideo(TRTCVideoStreamType.TRTCVideoStreamTypeBig, true);
            }
        }

        private void Mirror(bool isMirror)
        {
            // Local and remote mirroring modes are synchronized. Can be splited
            if (isMirror)
            {
                DataManager.GetInstance().isLocalVideoMirror = true;
                DataManager.GetInstance().isRemoteVideoMirror = true;
                mTRTCCloud.setVideoEncoderMirror(true);
            }
            else
            {
                DataManager.GetInstance().isLocalVideoMirror = false;
                DataManager.GetInstance().isRemoteVideoMirror = false;
                mTRTCCloud.setVideoEncoderMirror(false);
            }
            TRTCRenderParams renderParams = DataManager.GetInstance().GetRenderParams();
            mTRTCCloud.setLocalRenderParams(ref renderParams);
        }


        private void OnShareUrlLabelClick(object sender, EventArgs e)
        {
            // Obtain the url of the bypass live stream
            if (!this.mixTransCodingCheckBox.Checked)
            {
                ShowMessage("请勾选云端混流选项！");
                return;
            }
            // Calculate CDN address(Format： http://[bizid].liveplay.myqcloud.com/live/sdkappid_roomId_userId_main.flv)
            int bizId = GenerateTestUserSig.BIZID;
            // streamid = SDKAPPID_roomId_Username_streamType
            string streamId = String.Format("{0}_{1}_{2}_{3}", GenerateTestUserSig.SDKAPPID, mRoomId, Util.UTF16To8(mUserId), "main");
            string shareUrl = String.Format("http://{0}.liveplay.myqcloud.com/live/{1}.flv", bizId, streamId);
            Log.I("播放地址： " + shareUrl);
            Clipboard.SetDataObject(shareUrl);
            ShowMessage("播放地址：（已复制到剪切板）\n" + shareUrl);
        }

        private void OnBgmCheckBoxClick(object sender, EventArgs e)
        {
            if(Util.IsTestEnv())
            {
                if (mAudioEffectOldForm == null)
                    mAudioEffectOldForm = new AudioEffectOldForm();
                mAudioEffectOldForm.ShowDialog();
                this.bgmCheckBox.Checked = false;
            }
            else
            {
                if (mAudioEffectForm == null)
                    mAudioEffectForm = new AudioEffectForm();
                mAudioEffectForm.ShowDialog();
                this.bgmCheckBox.Checked = false;
            }
           
        }

        private void OnBeautyLabelClick(object sender, EventArgs e)
        {
            if (!DataManager.GetInstance().enterRoom)
            {
                ShowMessage("进房失败，请重试");
                return;
            }
            if (mBeautyForm == null)
                mBeautyForm = new TRTCBeautyForm();
            mBeautyForm.ShowDialog();
        }

        /// <summary>
        /// Indicates the volume callback(volume of each userId and the total volume of the remote end)
        /// </summary>
        public void onUserVoiceVolume(TRTCVolumeInfo[] userVolumes, uint userVolumesCount, uint totalVolume)
        {
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    if (userVolumesCount <= 0) return;
                    foreach (TRTCVolumeInfo info in userVolumes)
                    {
                        if (string.IsNullOrEmpty(info.userId) && this.localVoiceProgressBar.Visible)
                            this.localVoiceProgressBar.Value = (int)info.volume;
                        else
                            SetRemoteVoiceVolume(info.userId, (int)info.volume);
                    }
                }));
        }

        /// <summary>
        /// Set the volume prompt function for remote users based on their location
        /// </summary>
        private void SetRemoteVoiceVisable(int pos, bool visable)
        {
            if (pos == 1)
                this.remoteVoiceProgressBar1.Visible = visable;
            if (pos == 2)
                this.remoteVoiceProgressBar2.Visible = visable;
            if (pos == 3)
                this.remoteVoiceProgressBar3.Visible = visable;
            if (pos == 4)
                this.remoteVoiceProgressBar4.Visible = visable;
            if (pos == 5)
                this.remoteVoiceProgressBar5.Visible = visable;
        }

        /// <summary>
        /// Set the volume of the remote mainstream microphone
        /// </summary>
        private void SetRemoteVoiceVolume(string userId, int volume)
        {
            if (this.remoteUserLabel1.Text.Equals(userId) && this.remoteVoiceProgressBar1.Visible)
                this.remoteVoiceProgressBar1.Value = volume;
            else if (this.remoteUserLabel2.Text.Equals(userId) && this.remoteVoiceProgressBar2.Visible)
                this.remoteVoiceProgressBar2.Value = volume;
            else if (this.remoteUserLabel3.Text.Equals(userId) && this.remoteVoiceProgressBar3.Visible)
                this.remoteVoiceProgressBar3.Value = volume;
            else if (this.remoteUserLabel4.Text.Equals(userId) && this.remoteVoiceProgressBar4.Visible)
                this.remoteVoiceProgressBar4.Value = volume;
            else if (this.remoteUserLabel5.Text.Equals(userId) && this.remoteVoiceProgressBar5.Visible)
                this.remoteVoiceProgressBar5.Value = volume;
        }

        public void VoicePrompt(bool bIsPrompt)
        {
            if (bIsPrompt)
            {
                // Turn on volume cues
                DataManager.GetInstance().isShowVolume = true;
                if (mTRTCCloud != null)
                    mTRTCCloud.enableAudioVolumeEvaluation(300);
                this.localVoiceProgressBar.Visible = true;
                foreach (RemoteUserInfo info in mRemoteUsers)
                {
                    if (info.position != -1)
                        SetRemoteVoiceVisable(info.position, true);
                }
            }
            else
            {
                // Turn off volume prompt
                DataManager.GetInstance().isShowVolume = false;
                if (mTRTCCloud != null)
                    mTRTCCloud.enableAudioVolumeEvaluation(0);
                this.localVoiceProgressBar.Visible = false;
                foreach (RemoteUserInfo info in mRemoteUsers)
                {
                    if (info.position != -1)
                        SetRemoteVoiceVisable(info.position, false);
                }
            }
        }

        public void onWarning(TXLiteAVWarning warningCode, string warningMsg, IntPtr arg)
        {
            Log.I(String.Format("warningCode : {0}, warningMsg : {1}", warningCode, warningMsg));

            if (warningCode == TXLiteAVWarning.WARNING_MICROPHONE_DEVICE_EMPTY)
            {
                ShowMessage("Warning: 未检出到麦克风，请检查本地电脑设备。");
            }
            else if (warningCode == TXLiteAVWarning.WARNING_CAMERA_DEVICE_EMPTY)
            {
                ShowMessage("Warning: 未检出到摄像头，请检查本地电脑设备。");
            }
            else if (warningCode == TXLiteAVWarning.WARNING_SPEAKER_DEVICE_EMPTY)
            {
                ShowMessage("Warning: 未检出到扬声器，请检查本地电脑设备。");
            }
        }

        public void onCameraDidReady()
        {
            Log.I(String.Format("onCameraDidReady"));
            // Obtain real-time information about the currently used camera devices
            if (mTRTCCloud != null)
                mCurCameraDevice = mDeviceManager.getCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeCamera).getDevicePID();
        }

        public void onMicDidReady()
        {
            Log.I(String.Format("onMicDidReady"));
            // Obtain real-time information about the currently used microphone device
            if (mTRTCCloud != null)
            mCurMicDevice = mDeviceManager.getCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeMic).getDevicePID();
        }

        public void onConnectionLost()
        {
            Log.I(String.Format("onConnectionLost"));
            ShowMessage("网络异常，请重试");
        }

        public void onTryToReconnect()
        {
            Log.I(String.Format("onTryToReconnect"));
            ShowMessage("尝试重进房...");
        }

        public void onConnectionRecovery()
        {
            Log.I(String.Format("onConnectionRecovery"));
            ShowMessage("网络恢复，重进房成功");
        }

        public void OnConnectionFormClose()
        {
            if (mPKUsers.Count > 0)
                this.connectRoomCheckBox.Checked = true;
            else
                this.connectRoomCheckBox.Checked = false;
        }

        public void OnCameraDeviceChange(string deviceName)
        {
            mCurCameraDevice = deviceName;
        }

        public void OnMicDeviceChange(string deviceName)
        {
            mCurMicDevice = deviceName;
        }

        public void OnSpeakerDeviceChange(string deviceName)
        {
            mCurSpeakerDevice = deviceName;
        }

        private void OnConnectRoomCheckBoxClick(object sender, EventArgs e)
        {
            if (!DataManager.GetInstance().enterRoom)
            {
                ShowMessage("进房失败，请重试");
                return;
            }
            if (mPKUsers.Count > 0)
                this.connectRoomCheckBox.Checked = true;
            else
                this.connectRoomCheckBox.Checked = false;
            if (mConnectionForm == null)
                mConnectionForm = new TRTCConnectionForm(this);
            mConnectionForm.SetDisconnectBtnEnabled(mPKUsers.Count > 0);
            mConnectionForm.ShowDialog();
        }

        public void onConnectOtherRoom(string userId, TXLiteAVError errCode, string errMsg)
        {
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    // Post-wheat event callback, success if 'errCode == ERR_NULL'
                    if (mConnectionForm != null)
                        mConnectionForm.OnConnectOtherRoom(userId, errCode, errMsg);
                }));
        }

        public void onDisconnectOtherRoom(TXLiteAVError errCode, string errMsg)
        {
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    // Cancel the callback of the event after wheat connection，success if 'errCode == ERR_NULL'
                    if (mConnectionForm != null)
                        mConnectionForm.OnDisconnectOtherRoom(errCode, errMsg);
                }));
        }

        public void AddPKUser(uint roomId, string userId)
        {
            mPKUsers.Add(new PKUserInfo() { roomId = roomId, userId = userId });
        }

        public void ClearPKUsers()
        {
            mPKUsers.Clear();
            this.connectRoomCheckBox.Checked = false;
        }

        public void onMissCustomCmdMsg(string userId, int cmdId, int errCode, int missed)
        {
            Log.I(String.Format("onMissCustomCmdMsg : userId = {0}, cmdId = {1}, errCode = {2}, missed = {3}", userId, cmdId, errCode, missed));
        }

        public void onPlayBGMBegin(TXLiteAVError errCode)
        {
            Log.I(String.Format("onPlayBGMBegin : errCode = {0}", errCode));
        }

        public void onPlayBGMComplete(TXLiteAVError errCode)
        {
            Log.I(String.Format("onPlayBGMComplete : errCode = {0}", errCode));
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    if (mAudioEffectOldForm != null)
                        mAudioEffectOldForm.OnPlayBGMComplete(errCode);
                }));
        }

        public void onPlayBGMProgress(uint progressMS, uint durationMS)
        {
            Log.I(String.Format("onPlayBGMProgress : progressMs = {0}, durationMS = {1}", progressMS, durationMS));
        }

        public void onRecvCustomCmdMsg(string userId, int cmdId, uint seq, byte[] msg, uint msgSize)
        {
            Log.I(String.Format("onRecvCustomCmdMsg : userId = {0}, cmdId = {1}, seq = {2}, msg = {3}, msgSize = {4}", userId, cmdId, seq, msg, msgSize));
        }

        public void onRecvSEIMsg(string userId, byte[] message, uint msgSize)
        {
            Log.I(String.Format("onRecvSEIMsg : userId = {0}, message = {1}, msgSize = {2}", userId, message, msgSize));
        }

        public void onScreenCaptureCovered()
        {
            Log.I(String.Format("onScreenCaptureCovered"));
        }

        public void onScreenCapturePaused(int reason)
        {
            Log.I(String.Format("onScreenCapturePaused : reason = {0}", reason));
        }

        public void onScreenCaptureResumed(int reason)
        {
            Log.I(String.Format("onScreenCaptureResumed : reason = {0}", reason));
        }

        public void onScreenCaptureStarted()
        {
            Log.I(String.Format("onScreenCaptureStarted"));
        }

        public void onScreenCaptureStoped(int reason)
        {
            Log.I(String.Format("onScreenCaptureStoped : reason = {0}", reason));
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    this.screenShareCheckBox.Checked = false;
                    if (mToastForm != null && mToastForm.Visible)
                        mToastForm.Hide();
                }));
        }

        public void onSetMixTranscodingConfig(int errCode, string errMsg)
        {
            Log.I(String.Format("onSetMixTranscodingConfig : errCode = {0}, errMsg = {1}", errCode, errMsg));
        }

        public void onAudioEffectFinished(int effectId, int code)
        {
            Log.I(String.Format("onAudioEffectFinished : effectId = {0}, code = {1}", effectId, code));
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    if (mAudioEffectOldForm != null)
                        mAudioEffectOldForm.onAudioEffectFinished(effectId, code);
                }));
        }

        public void onSpeedTest(TRTCSpeedTestResult currentResult, uint finishedCount, uint totalCount)
        {
            Log.I(String.Format(@"onSpeedTest : currentResult.ip = {0}, currentResult.quality = {1}, 
                currentResult.upLostRate = {2}, currentResult.downLostRate = {3}, currentResult.rtt = {4}, 
                finishedCount = {5}, totalCount = {6}", currentResult.ip, currentResult.quality, currentResult.upLostRate,
                currentResult.downLostRate, currentResult.rtt, finishedCount, totalCount));
        }

        public void onStartPublishCDNStream(int errCode, string errMsg)
        {
            Log.I(String.Format("onStartPublishCDNStream : errCode = {0}, errMsg = {1}", errCode, errMsg));
        }

        public void onStartPublishing(int errCode, string errMsg)
        {
            Log.I(String.Format("onStartPublishing : errCode = {0}, errMsg = {1}", errCode, errMsg));
        }

        public void onStopPublishing(int errCode, string errMsg)
        {
            Log.I(String.Format("onStopPublishing : errCode = {0}, errMsg = {1}", errCode, errMsg));
        }

        public void onStopPublishCDNStream(int errCode, string errMsg)
        {
            Log.I(String.Format("onStopPublishCDNStream : errCode = {0}, errMsg = {1}", errCode, errMsg));
        }

        /// <summary>
        /// Network quality: This callback triggered every 2 seconds
        /// to collect statistics on the uplink and downlink quality of the current network
        /// </summary>
        public void onNetworkQuality(TRTCQualityInfo localQuality, TRTCQualityInfo[] remoteQuality, uint remoteQualityCount)
        {
        }

        public void onStatistics(TRTCStatistics statis)
        {
            if (statis.localStatisticsArray != null && statis.localStatisticsArraySize > 0)
            {
                // Local screen share information is recorded and mixed streams are updated in real time
                TRTCLocalStatistics[] localStatisticsArray = statis.localStatisticsArray;
                for (int i = 0; i < statis.localStatisticsArraySize; i++)
                {
                    if (localStatisticsArray[i].streamType == TRTCVideoStreamType.TRTCVideoStreamTypeSub)
                    {
                        int width = (int)localStatisticsArray[i].width;
                        int height = (int)localStatisticsArray[i].height;
                        TRTCVideoStreamType streamType = localStatisticsArray[i].streamType;
                        onFirstVideoFrame(null, TRTCVideoStreamType.TRTCVideoStreamTypeSub, width, height);
                    }
                }
            }
        }

        public void onSwitchRole(TXLiteAVError errCode, string errMsg)
        {
            Log.I(String.Format("onSwitchRole : errCode = {0}, errMsg = {1}", errCode, errMsg));
        }

        /// <summary>
        /// Test the volume callback of the microphone device
        /// </summary>
        public void onTestMicVolume(uint volume)
        {
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    if (mAudioSettingForm != null)
                        mAudioSettingForm.OnTestMicVolume(volume);
                }));
        }

        /// <summary>
        /// Test the volume callback of the speaker device
        /// </summary>
        public void onTestSpeakerVolume(uint volume)
        {
            if (this.IsHandleCreated)
                this.BeginInvoke(new Action(() =>
                {
                    if (mAudioSettingForm != null)
                        mAudioSettingForm.OnTestSpeakerVolume(volume);
                }));
        }

        public void OnSetScreenParamsCallback(bool success)
        {
            mIsSetScreenSuccess = success;
            if (success)
            {
                if (mTRTCCloud != null)
                {
                    mTRTCCloud.startScreenCapture(IntPtr.Zero, TRTCVideoStreamType.TRTCVideoStreamTypeSub, null);
                    this.Invoke(new Action(() =>
                    {
                        if (mToastForm == null)
                            mToastForm = new ToastForm();
                        mToastForm.SetText(mUserId + " 正在屏幕共享");
                        mToastForm.Show();
                    }));
                }
            }
            else
            {
                this.screenShareCheckBox.Checked = false;
            }
        }

        /// <summary>
        /// Start sending the collected audio data
        /// </summary>
        private void OnAudioTimerEvent(object sender, ElapsedEventArgs e)
        {
            if (mOtherSettingForm != null && mTRTCCloud != null)
                mOtherSettingForm.SendCustomAudioFrame();
        }

        /// <summary>
        /// Start sending collected video data
        /// </summary>
        public void OnVideoTimerEvent(object sender, ElapsedEventArgs e)
        {
            if (mOtherSettingForm != null && mTRTCCloud != null)
                mOtherSettingForm.SendCustomVideoFrame();
        }

        public void OnCustomCaptureAudioCallback(bool stop)
        {
            if (!stop)
            {
                // Enable custom audio collection and stop local audio collection
                mTRTCCloud.stopLocalAudio();
            }
            else
            {
                // Disable custom audio collection and enable local audio collection
                if (this.startLocalAudioCheckBox.Checked)
                {
                    mTRTCCloud.startLocalAudio(DataManager.GetInstance().AudioQuality);
                }
                if (this.muteAudioCheckBox.Checked)
                    mTRTCCloud.muteLocalAudio(true);
                else
                    mTRTCCloud.muteLocalAudio(false);
            }
        }

        public void OnCustomCaptureVideoCallback(bool stop)
        {
            if (!stop)
            {
                // Enable custom video capture, stop local video capture, and enable custom video rendering
                mTRTCCloud.stopLocalPreview();
                this.localInfoLabel.Visible = false;
                if (mRenderMode == 1)
                {
                    AddCustomVideoView(this.localVideoPanel, mUserId, TRTCVideoStreamType.TRTCVideoStreamTypeBig, true);
                }
            }
            else
            {
                // Disable Custom video capture, enable local video capture, and disable custom rendered video
                if (this.startLocalPreviewCheckBox.Checked)
                {
                    mTRTCCloud.startLocalPreview(mCameraVideoHandle);
                }
                else
                {
                    this.localInfoLabel.Visible = true;
                }
                if (mRenderMode == 1)
                {
                    RemoveCustomVideoView(this.localVideoPanel, mUserId, TRTCVideoStreamType.TRTCVideoStreamTypeBig, true);
                }
                if (this.muteVideoCheckBox.Checked)
                {
                    mTRTCCloud.muteLocalVideo(TRTCVideoStreamType.TRTCVideoStreamTypeBig, true);
                    this.localInfoLabel.Visible = true;
                }
                else
                {
                    mTRTCCloud.muteLocalVideo(TRTCVideoStreamType.TRTCVideoStreamTypeBig, false);
                }
            }
        }

        public ITRTCScreenCaptureSourceList GetScreenCaptureSources()
        {
            SIZE thumbSize = new SIZE { cx = 120, cy = 70 };
            SIZE iconSize = new SIZE { cx = 20, cy = 20 };
            return mTRTCCloud.getScreenCaptureSources(ref thumbSize,ref iconSize);
        }

        public void StartAudioRecording(TRTCAudioRecordingParams param)
        {
            mTRTCCloud.startAudioRecording(ref param);
        }

        public void StopAudioRecording()
        {
            mTRTCCloud.stopAudioRecording();
        }

        public void StartSystemAudioLoopback(string path)
        {
            mTRTCCloud.startSystemAudioLoopback(path);
        }

        public void StopSystemAudioLoopback()
        {
            mTRTCCloud.stopSystemAudioLoopback();
        }

        public void StartLocalRecording(TRTCLocalRecordingParams param)
        {
            mTRTCCloud.startLocalRecording(ref param);
        }

        public void StopLocalRecording()
        {
            mTRTCCloud.stopLocalRecording();
        }

        public void onLocalRecordBegin(int errCode, String storagePath)
        {
            Log.I(String.Format("onLocalRecordBegin : errCode = {0}, storagePath = {1}", errCode, storagePath));
        }

        public void onLocalRecording(int duration, String storagePath)
        {
            Log.I(String.Format("onLocalRecording : duration = {0}, storagePath = {1}", duration, storagePath));
        }

        public void onLocalRecordComplete(int errCode, String storagePath)
        {
            Log.I(String.Format("onLocalRecordComplete : errCode = {0}, storagePath = {1}", errCode, storagePath));
        }

        public void onCapturedRawAudioFrame(TRTCAudioFrame frame)
        {

            //throw new NotImplementedException();
        }

        public void onPlayAudioFrame(TRTCAudioFrame frame, string userId)
        {
            Log.I(String.Format("onPlayAudioFrame : userId = {0}", userId));
        }

        public void onMixedPlayAudioFrame(TRTCAudioFrame frame)
        {
            //throw new NotImplementedException();
        }

        public void onLocalProcessedAudioFrame(TRTCAudioFrame frame)
        {

        }

        public void onSwitchRoom(TXLiteAVError errCode, string errMsg)
        {
            Log.I(String.Format("onSwitchRoom : errCode = {0}, errMsg = {1}", errCode, errMsg));

        }

        public void onAudioDeviceCaptureVolumeChanged(uint volume, bool muted)
        {
            Log.I(String.Format("onAudioDeviceCaptureVolumeChanged : volume = {0}, muted = {1}", volume, muted));
        }

        public void onAudioDevicePlayoutVolumeChanged(uint volume, bool muted)
        {
            Log.I(String.Format("onAudioDevicePlayoutVolumeChanged : volume = {0}, muted = {1}", volume, muted));
        }

        public void onSnapshotComplete(string userId, TRTCVideoStreamType type, byte[] data,
            UInt32 length, UInt32 width, UInt32 height,
            TRTCVideoPixelFormat format)
        {
            if(length <= 0)
            {
                return;
            }

            try
            {
                if(string.IsNullOrWhiteSpace(userId))
                {
                    userId = "local";
                }
                string str_type;
                switch(type)
                {
                    case TRTCVideoStreamType.TRTCVideoStreamTypeSub:
                        str_type = "_SubStream_";
                        break;
                    case TRTCVideoStreamType.TRTCVideoStreamTypeBig:
                        str_type = "_BigStream_";
                        break;
                    case TRTCVideoStreamType.TRTCVideoStreamTypeSmall:
                        str_type = "_SmallStream_";
                        break;
                    default:
                        str_type = "_Stream_";
                        break;
                }

                Bitmap bitmap = new Bitmap((int)width,(int)height, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
                Rectangle rect = new Rectangle(0,0,(int)width,(int)height);

                // Store the bitmap in memory
                System.Drawing.Imaging.BitmapData _data = bitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite, bitmap.PixelFormat);
                IntPtr p = _data.Scan0;
                Marshal.Copy(data, 0, p, (int)length);
                bitmap.UnlockBits(_data);

                // Save pictures
                TimeSpan ts = DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0, 0);
                string fileName = AppDomain.CurrentDomain.BaseDirectory + 
                    userId + str_type + Convert.ToInt64(ts.TotalSeconds).ToString() + ".bmp";
                bitmap.Save(fileName);
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        /// <summary>
        /// Add the specified window to the inclusion list of screen sharing.
        /// The window in the inclusion list will be shared if it is in the capture window area
        /// </summary>
        /// <param name="window">HWND</param>
        public void addIncludedShareWindow(IntPtr window)
        {
            mTRTCCloud.addIncludedShareWindow(window);
        }

        /// <summary>
        /// Removes the specified window from the inclusion list of screen shares
        /// </summary>
        /// <param name="window">HWND</param>
        public void removeIncludedShareWindow(IntPtr window)
        {
            mTRTCCloud.removeIncludedShareWindow(window);
        }

        /// <summary>
        /// Removes all Windows from the inclusion list of screen shares
        /// </summary>
        public void removeAllIncludedShareWindow()
        {
            mTRTCCloud.removeAllIncludedShareWindow();
        }

        private void vedioSettingLabel_Click(object sender, EventArgs e)
        {
            if (mVedioSettingForm == null)
                mVedioSettingForm = new VedioSettingForm(this);
            mVedioSettingForm.ShowDialog();
        }

        private void audioSettingLabel_Click(object sender, EventArgs e)
        {
            if (mAudioSettingForm == null)
                mAudioSettingForm = new AudioSettingForm(this);
            mAudioSettingForm.ShowDialog();
        }

        private void otherSettingLabel_Click(object sender, EventArgs e)
        {
            if (mOtherSettingForm == null)
                mOtherSettingForm = new OtherSettingForm(this);
            mOtherSettingForm.ShowDialog();
        }

        private void VodPlayerLabel_Click(object sender, EventArgs e)
        {
            if(mVodPlayerForm == null)
                mVodPlayerForm = new TXVodPlayerForm(this);
            mVodPlayerForm.Owner = this;
            mVodPlayerForm.Show();

        }

        private void SnapshotLabel_Click(object sender, EventArgs e)
        {
            TRTCVideoStreamType streamType = TRTCVideoStreamType.TRTCVideoStreamTypeBig;
            TRTCSnapshotSourceType sourceType = TRTCSnapshotSourceType.TRTCSnapshotSourceTypeStream;
            mTRTCCloud.snapshotVideo("",streamType,sourceType);
        }
        private void RecordLabel_Click(object sender, EventArgs e)
        {
            TRTCRecordForm mRecordForm = new TRTCRecordForm(this);
            mRecordForm.ShowDialog();
        }

        public void ShowShareSelectWin()
        {
            ShareSelectForm mShareSelectForm = new ShareSelectForm(this);
            mShareSelectForm.ShowDialog();
        }

        public void SetmScreenCaptureSourceInfo(TRTCScreenCaptureSourceInfo info)
        {
            mScreenCaptureSourceInfo = info;
        }

        public TRTCScreenCaptureSourceInfo GetScreenCaptureSourceInfo()
        {
            return mScreenCaptureSourceInfo;
        }

        /// <summary>
        /// Local recording function
        /// </summary>
        public bool StartLocalRecord(string path)
        {
            if(mScreenCaptureSourceInfo == null)
            {
                return false;
            }
            RECT rect = new RECT();
            //mLiteAVLocalRecord.startLocalRecord(ref mScreenCaptureSourceInfo, ref rect, path);
            return true;
        }

        public void CloseVodPlayer()
        {
            mVodPlayerForm = null;
        }

        public void StopLocalRecord()
        {
            //mLiteAVLocalRecord.stopLocalRecord();
        }

        public void PauseLocalRecord()
        {
            //mLiteAVLocalRecord.pauseLocalRecord();
        }

        public void resumeLocalRecord()
        {
            //mLiteAVLocalRecord.resumeLocalRecord();
        }

        public void onDeviceChanged(string deviceId, TXMediaDeviceType type, TXMediaDeviceState state)
        {
            // Real-time monitoring of the removal and insertion of local devices
            // The SDK allows you to determine whether the camera, microphone, and speaker need to be
            // automatically turned on or switched when the camera, microphone, and speaker are removed or inserted
            Log.I(String.Format("onDeviceChange : deviceId = {0}, type = {1}, state = {2}", deviceId, type, state));
            this.BeginInvoke(new Action(() =>
            {
                if (type == TXMediaDeviceType.TXMediaDeviceTypeCamera)
                {
                    if (mVedioSettingForm != null)
                        mVedioSettingForm.OnDeviceChange(deviceId, type, state);
                }
                else if (type == TXMediaDeviceType.TXMediaDeviceTypeMic || type == TXMediaDeviceType.TXMediaDeviceTypeSpeaker)
                {
                    if (mAudioSettingForm != null)
                        mAudioSettingForm.OnDeviceChange(deviceId, type, state);
                }
            }));
        }

        public void onDeviceChange(string deviceId, TXMediaDeviceType type, TRTCDeviceState state)
        {
            // Recommend onDeviceChanged
        }
    }
}
