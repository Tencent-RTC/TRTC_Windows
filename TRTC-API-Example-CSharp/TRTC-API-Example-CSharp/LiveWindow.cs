using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ManageLiteAV;
using TRTCCSharpDemo.Common;

namespace TRTCCSharpDemo
{
    public partial class LiveWindowForm : Form , V2TXLivePlayerObserver, V2TXLivePusherObserver
    {
        enum LiveStatus{
            Live_Play,
            Live_Stop,
        }

        // glyphicons
        private string icon_play = "\ue87c";
        private string icon_stop = "\ue87a";
        private string icon_snapshot = "\ue611";
        private string icon_mirror = "\ue6da";
        private string icon_new = "\ue8ed";

        private Color icon_color = Color.FromArgb(205, 205, 205);

        private readonly float x; // Defines the width of the current form
        private readonly float y; // Defines the height of the current form
        private PortalWindow m_portal;
        private LiveStatus m_LiveStatus;
        private LiveStatus m_PusherStatus;
        private bool isPlayerVideo = false;
        private bool isPlayerAudio = false;
        private bool isPlayerDebugShow = false;
        private bool isStatisticsUpdate = false;
        private int palyerVolume = 100;

        private V2TXLivePlayer m_player;
        private V2TXLivePusher m_pusher;
        private ITXDeviceManager m_deviceManager;
        private ITRTCDeviceCollection m_CameraList;
        private bool isPusherDebugShow = false;
        private bool isPusherVideo = false;
        private bool isPusherAudio = false;
        private bool isPusherMirror = false;
        private bool isSelectCaptureSource = false;
        private IV2TXLiveScreenCaptureSourceList m_ScreenList;

        V2TXLiveVideoEncoderParam m_EncoderParam = new V2TXLiveVideoEncoderParam(V2TXLiveVideoResolution.V2TXLiveVideoResolution640x360);
        V2TXLiveAudioQuality  m_AudioQuality = V2TXLiveAudioQuality.V2TXLiveAudioQualityDefault;
        public LiveWindowForm(PortalWindow portal)
        {
            InitializeComponent();
            m_portal = portal;
            x = Width;
            y = Height;
            setTag(this);
            m_player = V2TXLivePlayer.createV2TXLivePlayer();
            m_LiveStatus = LiveStatus.Live_Stop;
            m_player.setObserver(this);
            m_player.setRenderView(PlayerViewPanel.Handle);
            m_player.setPlayoutVolume(palyerVolume);

            m_pusher = V2TXLivePusher.createV2TXLivePusher(V2TXLiveMode.V2TXLiveModeRTMP);
            m_pusher.setObserver(this);
            m_PusherStatus = LiveStatus.Live_Stop;
            m_pusher.setRenderView(PusherViewPanel.Handle);
            initUi();
        }

        private void initUi(){
            PlayerPauseVideoButton.Enabled = false;
            PlayerPauseAudioButton.Enabled = false;
            PlayerDebugViewButton.Enabled = false;

            PlayerVolTextBox.Text = Convert.ToString(palyerVolume);

            PlayerRenderModeComboBox.Items.Add("Fill");
            PlayerRenderModeComboBox.Items.Add("Fit");
            PlayerRenderModeComboBox.Items.Add("Scale Fill");
            PlayerRenderModeComboBox.SelectedIndex = 1;

            PlayerRotationComboBox.Items.Add("正常");
            PlayerRotationComboBox.Items.Add("顺时针90°");
            PlayerRotationComboBox.Items.Add("顺时针180°");
            PlayerRotationComboBox.Items.Add("顺时针270°");
            PlayerRotationComboBox.SelectedIndex = 0;

            PlayerPlayButton.Text = icon_play;
            PlayerPlayButton.Font = new Font(Util.PFCC.Families[0], PlayerPlayButton.Font.Size);
            PlayerPlayButton.ForeColor = icon_color;

            PlayerSnapshotButton.Text = icon_snapshot;
            PlayerSnapshotButton.Font = new Font(Util.PFCC.Families[0], PlayerPlayButton.Font.Size);
            PlayerSnapshotButton.ForeColor = icon_color;

            PlayerMsgListBox.Text = "";

            PusherPlayButton.Text = icon_play;
            PusherPlayButton.Font = new Font(Util.PFCC.Families[0], PusherPlayButton.Font.Size);
            PusherPlayButton.ForeColor = icon_color;

            PusherSnapshotButton.Text = icon_snapshot;
            PusherSnapshotButton.Font = new Font(Util.PFCC.Families[0], PlayerPlayButton.Font.Size);
            PusherSnapshotButton.ForeColor = icon_color;

            PusherMirrorButton.Text = icon_mirror;
            PusherMirrorButton.Font = new Font(Util.PFCC.Families[0], PusherPlayButton.Font.Size);
            PusherMirrorButton.ForeColor = icon_color;

            m_deviceManager = m_pusher.getDeviceManager();


            PusherRotationComboBox.Items.Add("正常");
            PusherRotationComboBox.Items.Add("顺时针90°");
            PusherRotationComboBox.Items.Add("顺时针180°");
            PusherRotationComboBox.Items.Add("顺时针270°");
            PusherRotationComboBox.SelectedIndex = 0;
            m_pusher.setRenderRotation(V2TXLiveRotation.V2TXLiveRotation0);

            PusherResolutionComboBox.Items.Add("640 * 360");
            PusherResolutionComboBox.Items.Add("1280 * 720");
            PusherResolutionComboBox.Items.Add("1920 * 1080");
            PusherResolutionComboBox.SelectedIndex = 0;

            PusherFPSTextBox.Text = Convert.ToString(m_EncoderParam.videoFps);
            PusherMinBitrateTextBox.Text = Convert.ToString(m_EncoderParam.minVideoBitrate);
            PusherMaxBitrateTextBox.Text = Convert.ToString(m_EncoderParam.videoBitrate);

            PusherAudioQualityComboBox.Items.Add("16kHz/单声道");
            PusherAudioQualityComboBox.Items.Add("48kHz/单声道");
            PusherAudioQualityComboBox.Items.Add("48kHz/双声道");
            PusherAudioQualityComboBox.SelectedIndex = 1;

            PusherMsgTypeComboBox.Items.Add("5");
            PusherMsgTypeComboBox.Items.Add("242");
            PusherMsgTypeComboBox.SelectedIndex = 1;

            PusherCameraRadioButton.Checked = true;
        }

        private void setTag(Control cons){
            foreach(Control con in cons.Controls){
                con.Tag = con.Width + ";" + con.Height + ";" + con.Left + ";" + con.Top + ";" + con.Font.Size;
                if(con.Controls.Count > 0){
                    setTag(con);
                }
            }
        }

        private void setControls(float newX, float newY, Control cons){
            // Iterate over the controls in the form, resetting the values of the controls
            foreach(Control con in cons.Controls){
                // Get the Tag attribute value of the control and stores the string array after splitting it
                if(con.Tag != null){
                    var myTag = con.Tag.ToString().Split(';');
                    // Determines the value of the control based on the scale of the form scaling
                    con.Width  = Convert.ToInt32(Convert.ToSingle(myTag[0]) * newX); // width
                    con.Height = Convert.ToInt32(Convert.ToSingle(myTag[1]) * newY); // height
                    con.Left   = Convert.ToInt32(Convert.ToSingle(myTag[2]) * newX); // left margin
                    con.Top    = Convert.ToInt32(Convert.ToSingle(myTag[3]) * newY); // top margin
                    var currentSize = Convert.ToSingle(myTag[4]) * newY; // text size
                    if(currentSize > 0){
                        if(con.Font.Name == "iconfont"){
                            con.Font = new Font(Util.PFCC.Families[0], currentSize);
                        }
                        else
                        {
                            con.Font = new Font(con.Font.Name, currentSize, con.Font.Style, con.Font.Unit);
                        }
                    }
                    con.Focus();
                    if(con.Controls.Count > 0){
                        setControls(newX, newY, con);
                    }
                }
            }
        }

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

        private void LiveWindow_Resize(object sender, EventArgs e)
        {
            var newx = Width / x;
            var newy = Height / y;
            setControls(newx, newy, this);
        }

        private void FrmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Pop-up dialog box
            DialogResult result = MessageBox.Show("确定要关闭窗体吗?", "提示", MessageBoxButtons.YesNo, MessageBoxIcon.Information);
            if (result == DialogResult.Yes)
            {
                // Close message box
                e.Cancel = false;
            }
            else
            {
                // Keep message box
                e.Cancel = true;
            }
        }

        private void OnExitPicBoxClick(object sender, FormClosedEventArgs e)
        {
            V2TXLivePlayer.releaseV2TXLivePlayer(m_player);
            V2TXLivePusher.releaseV2TXLivePusher(m_pusher);
            m_portal.Show();
        }

        private void PlayerPlayButton_Click(object sender, EventArgs e)
        {
            switch(m_LiveStatus)
            {
                case LiveStatus.Live_Stop:
                    string url = PlayerTextBox.Text;
                    if (url.Length > 0){
                        m_player.startPlay(url);
                    }
                    else
                    {
                        ShowMessage("请输入拉流地址");
                    }
                    break;
                case LiveStatus.Live_Play:
                    m_LiveStatus = LiveStatus.Live_Stop;
                    isPlayerVideo = false;
                    PlayerPlayButton.Text = icon_play;
                    m_player.stopPlay();
                    PlayerViewPanel.BackColor = Color.Black;
                    break;
            }            
        }

        public void onError(V2TXLivePlayer player, int code, ref string msg, IntPtr extraInfo)
        {
            string text = String.Format("onError code:{0}, msg:{1}, extraInfo:{2}", code, msg, extraInfo);
            PlayerMsgListBox.Items.Add(text);
        }

        public void onWarning(V2TXLivePlayer player, int code, ref string msg, IntPtr extraInfo)
        {
            string text = String.Format("onWarning code:{0}, msg:{1}, extraInfo:{2}", code, msg, extraInfo);
            PlayerMsgListBox.Items.Add(text);
        }

        public void onVideoResolutionChanged(V2TXLivePlayer player, int width, int height)
        {
            string text = String.Format("onVideoResolutionChanged width:{0}, height:{1}", width, height);
            PlayerMsgListBox.Items.Add(text);
        }

        public void onConnected(V2TXLivePlayer player, IntPtr extraInfo)
        {
            string text = String.Format("onConnected extraInfo:{0}", extraInfo);
            PlayerMsgListBox.Items.Add(text);
        }

        public void onVideoPlaying(V2TXLivePlayer player, bool firstPlay, IntPtr extraInfo)
        {
            string text = String.Format("onVideoPlaying firstPlay:{0}, extraInfo:{1}", firstPlay, extraInfo);
            PlayerMsgListBox.Items.Add(text);
            m_LiveStatus = LiveStatus.Live_Play;
            PlayerPlayButton.Text = icon_stop;
            PlayerViewPanel.BackColor = Color.White;
            isPlayerVideo = true;
            PlayerPauseVideoButton.Enabled = true;
            PlayerDebugViewButton.Enabled = true;
        }

        public void onAudioPlaying(V2TXLivePlayer player, bool firstPlay, IntPtr extraInfo)
        {
            string text = String.Format("onAudioPlaying firstPlay:{0}, extraInfo:{1}", firstPlay, extraInfo);
            PlayerMsgListBox.Items.Add(text);
            PlayerPauseAudioButton.Enabled = true;
            isPlayerAudio = true;
        }

        public void onVideoLoading(V2TXLivePlayer player, IntPtr extraInfo)
        {
            string text = String.Format("onVideoLoading  extraInfo:{0}", extraInfo);
            PlayerMsgListBox.Items.Add(text);
        }

        public void onAudioLoading(V2TXLivePlayer player, IntPtr extraInfo)
        {
            string text = String.Format("onAudioLoading  extraInfo:{0}", extraInfo);
            PlayerMsgListBox.Items.Add(text);
        }

        public void onPlayoutVolumeUpdate(V2TXLivePlayer player, int volume)
        {
            string text = String.Format("onPlayoutVolumeUpdate  volume:{0}", volume);
            PlayerMsgListBox.Items.Add(text);
        }

        public void onStatisticsUpdate(V2TXLivePlayer player, V2TXLivePlayerStatistics statistics)
        {
            if (isStatisticsUpdate)
            {
                string text = 
                    String.Format("onStatisticsUpdate,appCpu={0},systemCpu={1},width={2},height={3},fps={4},videoBitrate={5}Kbps,audioBitrate={6}Kbps", 
                        statistics.appCpu, statistics.systemCpu, statistics.width, statistics.height, statistics.fps, statistics.videoBitrate,
                        statistics.audioBitrate);
                PlayerMsgListBox.Items.Add(text);
            }
        }

        public void onSnapshotComplete(V2TXLivePlayer player, ref byte[] image, int length, int width, int height, V2TXLivePixelFormat format)
        {
            string text = String.Format("onSnapshotComplete  length:{0}, width:{1}, height:{2}, format:{3}", length, width, height, format);
            PlayerMsgListBox.Items.Add(text);
        }

        public void onRenderVideoFrame(V2TXLivePlayer player, ref V2TXLiveVideoFrame videoFrame)
        {

        }

        public void onReceiveSeiMessage(V2TXLivePlayer player, int payloadType, ref byte[] data, uint dataSize)
        {
            string msg = System.Text.Encoding.Default.GetString(data);
            string text = String.Format("onReceiveSeiMessage  payloadType:{0}, data:{1},dataSize:{2}", payloadType, msg, dataSize);
            PlayerMsgListBox.Items.Add(text);
        }

        private void PlayerPauseVideoButton_Click(object sender, EventArgs e)
        {
            isPlayerVideo = !isPlayerVideo;
            if (isPlayerVideo)
            {
                m_player.pauseVideo();
                PlayerPauseVideoButton.Text = "resumeVideo";
            }
            else
            {
                m_player.resumeVideo();
                PlayerPauseVideoButton.Text = "pauseVideo";
            }

        }

        private void PlayerPauseAudioButton_Click(object sender, EventArgs e)
        {
            isPlayerAudio = !isPlayerAudio;
            if (isPlayerAudio)
            {
                m_player.pauseAudio();
                PlayerPauseAudioButton.Text = "resumeAudio";
            }
            else
            {
                m_player.resumeAudio();
                PlayerPauseAudioButton.Text = "pauseAudio";
            }

        }

        private void PlayerDebugViewButton_Click(object sender, EventArgs e)
        {
            isPlayerDebugShow = !isPlayerDebugShow;
            m_player.showDebugView(isPlayerDebugShow);
        }

        private void PlayerSnapshotButton_Click(object sender, EventArgs e)
        {
            m_player.snapshot();
        }

        private void PlayerRenderModeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            
            m_player.setRenderFillMode(V2TXLiveFillMode.V2TXLiveFillModeFill);
            switch (PlayerRenderModeComboBox.SelectedIndex)
            {
                case 0:
                    m_player.setRenderFillMode(V2TXLiveFillMode.V2TXLiveFillModeFill);
                    break;
                case 2:
                    m_player.setRenderFillMode(V2TXLiveFillMode.V2TXLiveFillModeScaleFill);
                    break;
                default:
                    m_player.setRenderFillMode(V2TXLiveFillMode.V2TXLiveFillModeFit);
                    break;
            }
        }

        private void PlayerRotationComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (PlayerRotationComboBox.SelectedIndex)
            {
                case 1:
                    m_player.setRenderRotation(V2TXLiveRotation.V2TXLiveRotation90);
                    break;
                case 2:
                    m_player.setRenderRotation(V2TXLiveRotation.V2TXLiveRotation180);
                    break;
                case 3:
                    m_player.setRenderRotation(V2TXLiveRotation.V2TXLiveRotation270);
                    break;
                default:
                    m_player.setRenderRotation(V2TXLiveRotation.V2TXLiveRotation0);
                    break;
            }
        }

        private void PlayerVolButton_Click(object sender, EventArgs e)
        {
            palyerVolume = Convert.ToInt32(PlayerVolTextBox.Text);
            m_player.setPlayoutVolume(palyerVolume);
        }

        private void PlayerVolTextBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((e.KeyChar < 48 || e.KeyChar > 57) && e.KeyChar != 8)
                e.Handled = true;
        }

        private void PlayerEnableVolEvaluationCheckBox_Click(object sender, EventArgs e)
        {
            if (PlayerIntervalTextBox.Text.Length > 0)
            {
                int interval = Convert.ToInt32(PlayerIntervalTextBox.Text);
                m_player.enableVolumeEvaluation(interval);
            }
        }

        private void PlayerSEICheckBox_Click(object sender, EventArgs e)
        {
            m_player.enableReceiveSeiMessage(PlayerSEICheckBox.Checked, 242);
            m_player.enableReceiveSeiMessage(PlayerSEICheckBox.Checked, 5);
        }

        private void PlayerShowStatisticCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            isStatisticsUpdate = PlayerShowStatisticCheckBox.Checked;
        }

        private void PlayerTimeButton_Click(object sender, EventArgs e)
        {
            float min = Convert.ToSingle(PlayerMixTimeTextBox.Text);
            float max = Convert.ToSingle(PlayerMixTimeTextBox.Text);
            m_player.setCacheParams(min, max);
        }

        private void UpdatePusherStatus(bool isPush){
            if (isPush)
            {
                m_PusherStatus = LiveStatus.Live_Play;
                PusherPlayButton.Text = icon_stop;
            }
            else
            {
                m_PusherStatus = LiveStatus.Live_Stop;
                PusherPlayButton.Text = icon_play;
                PusherViewPanel.BackColor = Color.Black;
            }
        }

        private void PusherPlayButton_Click(object sender, EventArgs e)
        {
            switch (m_PusherStatus)
            {
                case LiveStatus.Live_Stop:
                    string url = PusherUrlTextBox.Text;
                    if (url.Length > 0)
                    {
                        m_pusher.setVideoQuality(m_EncoderParam);
                        m_pusher.setAudioQuality(m_AudioQuality);
                        m_pusher.startMicrophone();
                        if (PusherCameraRadioButton.Checked)
                        {
                            m_pusher.startCamera(m_CameraList.getDevicePID((uint)PusherCameraComboBox.SelectedIndex));
                        }
                        if (PusherScreenRadioButton.Checked)
                        {
                            if (!isSelectCaptureSource)
                            {
                                SelectScreenCaptureSource();
                            }
                            m_pusher.startScreenCapture();
                        }
                        m_pusher.startPush(url);
                    }
                    else
                    {
                        ShowMessage("请先获取推流地址");
                    }
                    break;
                case LiveStatus.Live_Play:
                    if (PusherCameraRadioButton.Checked)
                    {
                        m_pusher.stopCamera();
                    }
                    if (PusherScreenRadioButton.Checked)
                    {
                        m_pusher.stopScreenCapture();
                    }
                    m_pusher.stopMicrophone();
                    m_pusher.stopPush();
                    UpdatePusherStatus(false);
                    break;
            }
        }


        public void onError(int code, string msg, IntPtr extraInfo)
        {
            string text = String.Format("onError code:{0}, msg:{1}, extraInfo:{2}", code, msg, extraInfo);
            PusherMsgListBox.Items.Add(text);
        }

        public void onWarning(int code, string msg, IntPtr extraInfo)
        {
            string text = String.Format("onWarning code:{0}, msg:{1}, extraInfo:{2}", code, msg, extraInfo);
            PusherMsgListBox.Items.Add(text);
        }

        public void onCaptureFirstAudioFrame()
        {
            string text = String.Format("onCaptureFirstAudioFrame");
            PusherMsgListBox.Items.Add(text);
        }

        public void onCaptureFirstVideoFrame()
        {
            string text = String.Format("onCaptureFirstVideoFrame");
            PusherMsgListBox.Items.Add(text);
        }

        public void onMicrophoneVolumeUpdate(int volume)
        {
            string text = String.Format("onMicrophoneVolumeUpdate volume = {0}", volume);
            PusherMsgListBox.Items.Add(text);
        }

        public void onPushStatusUpdate(V2TXLivePushStatus state, string msg, IntPtr extraInfo)
        {
            string text = "";
            switch(state){
                case V2TXLivePushStatus.V2TXLivePushStatusDisconnected:
                    text = "Disconnected";
                    UpdatePusherStatus(false);
                    break;
                case V2TXLivePushStatus.V2TXLivePushStatusConnecting:
                    text = "Connecting";
                    break;
                case V2TXLivePushStatus.V2TXLivePushStatusConnectSuccess:
                    text = "ConnectSuccess";
                    UpdatePusherStatus(true);
                    break;
                case V2TXLivePushStatus.V2TXLivePushStatusReconnecting:
                    text = "Reconnecting";
                    break;
            }
            text = String.Format("onPushStatusUpdate state = {0}, msg = {1}", text, msg);
            PusherMsgListBox.Items.Add(text);
        }

        public void onStatisticsUpdate(V2TXLivePusherStatistics statistics)
        {
            if (PusherStatisticCheckBox.Checked)
            {
                string text =
                    String.Format("onStatisticsUpdate,appCpu={0},systemCpu={1},width={2},height={3},fps={4},videoBitrate={5}Kbps,audioBitrate={6}Kbps",
                        statistics.appCpu, statistics.systemCpu, statistics.width, statistics.height, statistics.fps, statistics.videoBitrate,
                        statistics.audioBitrate);
                PusherMsgListBox.Items.Add(text);
            }
        }

        public void onSnapshotComplete(string image, int length, int width, int height, V2TXLivePixelFormat format)
        {
            string text = String.Format("onSnapshotComplete  length:{0}, width:{1}, height:{2}, format:{3}", length, width, height, format);
            PusherMsgListBox.Items.Add(text);
        }

        public void onRenderVideoFrame(V2TXLiveVideoFrame videoFrame)
        {

        }

        public int onProcessVideoFrame(V2TXLiveVideoFrame srcFrame, V2TXLiveVideoFrame dstFrame)
        {
            return 0;
        }

        public void onScreenCaptureStarted()
        {
            string text = String.Format("onScreenCaptureStarted");
            PusherMsgListBox.Items.Add(text);
        }

        public void onScreenCaptureStoped(int reason)
        {
            string text = String.Format("onScreenCaptureStoped");
            PusherMsgListBox.Items.Add(text);
        }

        private void PusherCameraComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            uint index = (uint)PusherCameraComboBox.SelectedIndex;
            m_deviceManager.setCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeCamera, m_CameraList.getDevicePID(index));
        }

        private void PusherRotationComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch(PusherRotationComboBox.SelectedIndex){
                case 1:
                    m_pusher.setRenderRotation(V2TXLiveRotation.V2TXLiveRotation90);
                    break;
                case 2:
                    m_pusher.setRenderRotation(V2TXLiveRotation.V2TXLiveRotation180);
                    break;
                case 3:
                    m_pusher.setRenderRotation(V2TXLiveRotation.V2TXLiveRotation270);
                    break;
                default:
                    m_pusher.setRenderRotation(V2TXLiveRotation.V2TXLiveRotation0);
                    break;
            }
        }

        private void PusherFPSTextBox_TextChanged(object sender, EventArgs e)
        {
            m_EncoderParam.videoFps = Convert.ToInt32(PusherFPSTextBox.Text);
        }

        private void PusherMinBitrateTextBox_TextChanged(object sender, EventArgs e)
        {
            m_EncoderParam.minVideoBitrate = Convert.ToInt32(PusherMinBitrateTextBox.Text);
        }

        private void PusherMaxBitrateTextBox_TextChanged(object sender, EventArgs e)
        {
            m_EncoderParam.videoBitrate = Convert.ToInt32(PusherMaxBitrateTextBox.Text);
        }

        private void PusherLoopbackCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (PusherLoopbackCheckBox.Checked)
            {
                m_pusher.startSystemAudioLoopback(null);
            }
            else
            {
                m_pusher.stopSystemAudioLoopback();
            }
        }

        private void PusherResolutionComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (PusherResolutionComboBox.SelectedIndex)
            {
                case 1:
                    m_EncoderParam.videoResolution = V2TXLiveVideoResolution.V2TXLiveVideoResolution1280x720;
                    break;
                case 2:
                    m_EncoderParam.videoResolution = V2TXLiveVideoResolution.V2TXLiveVideoResolution1920x1080;
                    break;
                default:
                    m_EncoderParam.videoResolution = V2TXLiveVideoResolution.V2TXLiveVideoResolution640x360;
                    break;
            }
        }

        private void PusherUpdateEncoderButton_Click(object sender, EventArgs e)
        {
            m_pusher.setVideoQuality(m_EncoderParam);
        }

        private void PusherAudioQualityComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (PusherAudioQualityComboBox.SelectedIndex)
            {
                case 0:
                    m_AudioQuality = V2TXLiveAudioQuality.V2TXLiveAudioQualitySpeech;
                    break;
                case 2:
                    m_AudioQuality = V2TXLiveAudioQuality.V2TXLiveAudioQualityMusic;
                    break;
                default:
                    m_AudioQuality = V2TXLiveAudioQuality.V2TXLiveAudioQualityDefault;
                    break;
            }
        }

        private void PusherDebugViewButton_Click(object sender, EventArgs e)
        {
            isPusherDebugShow = !isPusherDebugShow;
            m_pusher.showDebugView(isPusherDebugShow);
        }

        private void PusherPauseVideoButton_Click(object sender, EventArgs e)
        {
            isPusherVideo = !isPusherVideo;
            if (isPusherVideo)
            {
                m_pusher.pauseVideo();
                PusherPauseVideoButton.Text = "resumeVideo";
            }
            else
            {
                m_pusher.resumeVideo();
                PusherPauseVideoButton.Text = "pauseVideo";
            }
        }

        private void PusherPauseAudioButton_Click(object sender, EventArgs e)
        {
            isPusherAudio = !isPusherAudio;
            if (isPusherAudio)
            {
                m_pusher.pauseAudio();
                PusherPauseAudioButton.Text = "resumeAudio";
            }
            else
            {
                m_pusher.resumeAudio();
                PusherPauseAudioButton.Text = "pauseAudio";
            }
        }

        private void PusherMirrorButton_Click(object sender, EventArgs e)
        {
            isPusherMirror = !isPusherMirror;
            if (isPusherMirror)
            {
                PusherMirrorButton.ForeColor = Color.FromArgb(10, 142, 222);
                m_pusher.setRenderMirror(V2TXLiveMirrorType.V2TXLiveMirrorTypeEnable);
            }
            else
            {
                PusherMirrorButton.ForeColor = icon_color;
                m_pusher.setRenderMirror(V2TXLiveMirrorType.V2TXLiveMirrorTypeDisable);
            }
            m_pusher.setEncoderMirror(isPusherMirror);
        }

        private void PusherScreenRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            V2TXLiveSize size = new V2TXLiveSize();
            size.width = 0;
            size.height = 0;
            m_ScreenList = m_pusher.getScreenCaptureSources(size, size);
            for (uint i = 0; i < m_ScreenList.getCount(); i++)
            {
                PusherScreenComboBox.Items.Add(m_ScreenList.getSourceInfo(i).sourceName);
            }
            PusherScreenComboBox.SelectedIndex = 0;
        }

        private void PusherScreenRectButton_Click(object sender, EventArgs e)
        {
            SelectScreenCaptureSource();
        }

        private void SelectScreenCaptureSource(){
            int index = PusherScreenComboBox.SelectedIndex;
            V2TXLiveScreenCaptureSourceInfo info = m_ScreenList.getSourceInfo((uint)index);

            V2TXLiveRect rect = new V2TXLiveRect();
            rect.left = Convert.ToUInt64(PusherScreenRectLeftTextBox.Text);
            rect.top = Convert.ToUInt64(PusherScreenRectTopTextBox.Text);
            rect.right = Convert.ToUInt64(PusherScreenRectRightTextBox.Text);
            rect.bottom = Convert.ToUInt64(PusherScreenRectBottomTextBox.Text);

            V2TXLiveScreenCaptureProperty property = new V2TXLiveScreenCaptureProperty();
            m_pusher.setScreenCaptureSource(info, rect, property);
            isSelectCaptureSource = true;
        }

        private void PusherCameraRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            m_CameraList = m_deviceManager.getDevicesList(TXMediaDeviceType.TXMediaDeviceTypeCamera);
            ITRTCDeviceInfo camera = m_deviceManager.getCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeCamera);
            uint cameraIndex = 0;
            for (uint i = 0; i < m_CameraList.getCount(); i++)
            {
                if (camera.getDevicePID() == m_CameraList.getDevicePID(i))
                {
                    cameraIndex = i;
                }
                PusherCameraComboBox.Items.Add(m_CameraList.getDeviceName(i));
            }
            PusherCameraComboBox.SelectedIndex = (int)cameraIndex;
        }

        private void PusherSnapshotButton_Click(object sender, EventArgs e)
        {
            m_pusher.snapshot();
        }

        private void PusherMsgTypeButton_Click(object sender, EventArgs e)
        {
            int payloadType = 242;
            switch (PusherMsgTypeComboBox.SelectedIndex)
            {
                case 0:
                    payloadType = 5;
                    break;
                default:
                    break;
            }
            byte[] Data = System.Text.Encoding.Default.GetBytes(PusherMsgTypeTextBox.Text);
            int ret = m_pusher.sendSeiMessage(payloadType, Data, (uint)Data.Length);
            string text = String.Format("sendSeiMessage msg = {0}, ret = {1}", PusherMsgTypeTextBox.Text, ret);
            PusherMsgListBox.Items.Add(text);
        }

        private void PlayerIntervalTextBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            // Determine whether the key is the type to be entered.
            if (((int)e.KeyChar < 48 || (int)e.KeyChar > 57) && (int)e.KeyChar != 8 && (int)e.KeyChar != 46)
                e.Handled = true;

            // Handling of decimal points.
            if ((int)e.KeyChar == 46)                           // decimal points
            {
                if (PlayerIntervalTextBox.Text.Length <= 0)
                    e.Handled = true;   // The decimal point cannot be in the first place
                else
                {
                    float f;
                    float oldf;
                    bool b1 = false, b2 = false;
                    b1 = float.TryParse(PlayerIntervalTextBox.Text, out oldf);
                    b2 = float.TryParse(PlayerIntervalTextBox.Text + e.KeyChar.ToString(), out f);
                    if (b2 == false)
                    {
                        if (b1 == true)
                            e.Handled = true;
                        else
                            e.Handled = false;
                    }
                }
            }
        }
    }
}
