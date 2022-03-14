using System;
using System.Drawing;
using TRTCCSharpDemo.Common;
using System.Windows.Forms;
using ManageLiteAV;

namespace TRTCCSharpDemo
{

    enum VodStatus
    {
        Vod_Play,
        Vod_Pause,
        Vod_Stop,
    };

    enum VolumeStatus
    {
        MuteOn,
        MuteOff
    }

    public partial class TXVodPlayerForm : Form,ITXVodPlayerEventCallback,ITXVodPlayerDataCallback
    {

        private TRTCMainForm mMainForm = null;
        ITXVodPlayer mVodPlayerController = null;

        UInt64 mDurationMS = 0;
        VodStatus mVodStatus = VodStatus.Vod_Stop;
        VolumeStatus mVolumeStatus = VolumeStatus.MuteOn;

        float mPlaySpeed = 1.0f;  //默认正常倍速播放
        const float maxSpeed = 2.0f;
        const float minSpeed = 0.4f;

        int mVolume = 100;
        string mFileName = "http://1252463788.vod2.myqcloud.com/95576ef5vodtransgzp1252463788/e1ab85305285890781763144364/v.f30.mp4";

        private bool mIsMouseDown = false;
        private Point mFormLocation;     // Form的location
        private Point mMouseOffset;      // 鼠标的按下位置

        public TXVodPlayerForm(TRTCMainForm mainform)
        {
            InitializeComponent();

            mMainForm = mainform;
            Load += TXVodPlayerForm_Load;
            this.Disposed += new EventHandler(OnDisposed);
            panel3.MouseDown += Panel3_MouseDown;
            panel3.MouseUp += Panel3_MouseUp;
            panel3.MouseMove += Panel3_MouseMove;
            TimeBar.MouseClick += TimeBar_MouseClick;
            VolumeBar.MouseClick += VolumeBar_MouseClick;
        }

        private void VolumeBar_MouseClick(object sender, MouseEventArgs e)
        {
            if (mVodPlayerController != null)
            {
                mVolume = 100 * e.X / VolumeBar.Width;
                VolumeBar.Value = mVolume;
                //mVolume = VolumeBar.Value;
                mVodPlayerController.setVolume(mVolume);
                if (mVolume == 0)
                {
                    mVodPlayerController.mute(true);
                    VolumeBox.Image = Properties.Resources.MuteOff;
                }
                else
                {
                    mVodPlayerController.mute(false);
                    VolumeBox.Image = Properties.Resources.MuteOn;
                }
            }
        }

        private void Panel3_MouseMove(object sender, MouseEventArgs e)
        {
            if (mIsMouseDown)
            {
                Point pt = Control.MousePosition;
                int x = mMouseOffset.X - pt.X;
                int y = mMouseOffset.Y - pt.Y;

                this.Location = new Point(mFormLocation.X - x, mFormLocation.Y - y);
            }
        }

        private void Panel3_MouseUp(object sender, MouseEventArgs e)
        {
            mIsMouseDown = false;
        }

        private void Panel3_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                mIsMouseDown = true;
                mFormLocation = this.Location;
                mMouseOffset = Control.MousePosition;
            }
        }

        private void OnDisposed(object sender, EventArgs e)
        {
            //清理资源
            if (mVodPlayerController != null)
            {
                ITXVodPlayer.destroyTXVodPlayer(mVodPlayerController);
                mVodPlayerController = null;
            }
            if (mMainForm != null)
            {
                mMainForm.CloseVodPlayer();
            }
        }

        private void TXVodPlayerForm_Load(object sender, EventArgs e)
        {
            TimeBar.Maximum = 1000;
            TimeBar.Value = 0;
            VolumeBar.Maximum = 100;
            VolumeBar.Value = 100;
            StopBox.Enabled = false;
            mVolumeStatus = VolumeStatus.MuteOn;
            mVodStatus = VodStatus.Vod_Stop;
        }

        private void TimeBar_MouseClick(object sender, MouseEventArgs e)
        {
            if (mVodPlayerController != null)
            {
                ulong vodPos = Convert.ToUInt64(mDurationMS * (1.0*e.X / TimeBar.Width));
                mVodPlayerController.seek(vodPos);
            }
        }

        public void onVodPlayerError(int error)
        {
            string strLog = "播放错误,错误码:" + error;
            strLog += "错误描述:";
            // enum class MediaPlayerError {
            //  kNoError = 0,
            //  kPreExist = -2,
            //  kErrorUrl = -3,
            //  kErrorMeidaFormat = -5,
            //  kErrorAudioDevice = -6,
            //  kErrorMediaInfo = -7,
            //  kUnsupportMediaFormat = -8,
            //};
            if (error == -2)
            {
                strLog += "文件不存在";
            }
            else if (error == -3)
            {
                strLog += "文件url错误";
            }
            else if (error == -5)
            {
                strLog += "文件格式错误";
            }
            else if (error == -6)
            {
                strLog += "设备错误";
            }
            else if (error == -7)
            {
                strLog += "媒体信息错误";
            }
            else if (error == -8)
            {
                strLog += "不支持的文件格式";
            }
            MessageBox.Show(string.Format("Error : {0}", strLog));
        }

        public void onVodPlayerPaused()
        {
            mVodStatus = VodStatus.Vod_Pause;
            ChangeUiStatus(mVodStatus);
        }

        public void onVodPlayerProgress(ulong msPos)
        {
            TimeBar.BeginInvoke(new MethodInvoker(() =>
            {
                TimeBar.Value = Convert.ToInt32(msPos * 1000 / mDurationMS);
                TimeLabel.Text = Common.Util.ConvertMSToTime(msPos, mDurationMS);
            }));
        }

        public void onVodPlayerResumed()
        {
            mVodStatus = VodStatus.Vod_Play;
            ChangeUiStatus(mVodStatus);
        }

        public void onVodPlayerStarted(ulong msLength)
        {
            mDurationMS = msLength;
            mVodStatus = VodStatus.Vod_Play;
            ChangeUiStatus(mVodStatus);
            TimeBar.Value = 0;
            TimeLabel.Text = Common.Util.ConvertMSToTime(0, mDurationMS);
        }

        public void onVodPlayerStoped(int reason)
        {
            mVodStatus = VodStatus.Vod_Stop;
            ChangeUiStatus(mVodStatus);
            TimeBar.Value = 0;
            TimeLabel.Text = Common.Util.ConvertMSToTime(0, 0);

            if(mVodPlayerController == null)
            {
                return;
            }
            mVodPlayerController.setView(IntPtr.Zero);
            //刷新页面，清除最后一帧视频数据。
            PlayPanel.Invalidate();
        }

        public void StartVideo()
        {
            if (mVodPlayerController == null)
            {
                mVodPlayerController = ITXVodPlayer.createTXVodPlayer(mFileName, false);
            }
            if(mVodPlayerController == null)
            {
                return;
            }
            mVodPlayerController.setDataCallback(this);
            mVodPlayerController.setEventCallback(this);
            mVodPlayerController.setRate(mPlaySpeed);
            mVodPlayerController.attachTRTC();
            mVodPlayerController.setVolume(mVolume);
            mVodPlayerController.setView(this.PlayPanel.Handle);
            mVodPlayerController.publishAudio();
            mVodPlayerController.publishVideo();
            mVodPlayerController.start();
            TimeBar.Value = 0;
        }

        private void StopVideo()
        {
            if (mVodPlayerController == null)
            {
                return;
            }
            mVodPlayerController.unpublishAudio();
            mVodPlayerController.unpublishVideo();
            mVodPlayerController.detachTRTC();
            mVodPlayerController.pause();
            mVodPlayerController.stop();
            mVodPlayerController.setView(IntPtr.Zero);
            mVodPlayerController.setDataCallback(null);
            mVodPlayerController.setEventCallback(null);
            ITXVodPlayer.destroyTXVodPlayer(mVodPlayerController);
            mVodPlayerController = null;
            mVodStatus = VodStatus.Vod_Stop;
            ChangeUiStatus(mVodStatus);
            TimeBar.Value = 0;
            TimeLabel.Text = Common.Util.ConvertMSToTime(0, 0);
            //刷新页面，清除最后一帧视频数据。
            PlayPanel.Invalidate();
        }

        private void PauseVideo()
        {
            if(mVodPlayerController!=null)
            {
                mVodPlayerController.pause();
            }
        }

        private void ResumeVideo()
        {
            if (mVodPlayerController != null)
            {
                mVodPlayerController.resume();
            }
        }

        private void exitPicBox_Click(object sender, EventArgs e)
        {
            if(mVodStatus != VodStatus.Vod_Stop)
            {
                StopVideo();
            }
            this.Close();
        }

        private void ChangeUiStatus(VodStatus status)
        {
            switch(status)
            {
                case VodStatus.Vod_Play:
                    StopBox.Enabled = true;
                    PlayBox.Image = Properties.Resources.Pause;
                    break;

                case VodStatus.Vod_Pause:
                    StopBox.Enabled = true;
                    PlayBox.Image = Properties.Resources.Play;
                    break;

                case VodStatus.Vod_Stop:
                    StopBox.Enabled = false;
                    PlayBox.Image = Properties.Resources.Play;
                    break;

                default:
                    break;
            }
        }

        private void VolumeBar_Scroll(object sender, EventArgs e)
        {
            if (mVodPlayerController != null)
            {
                mVolume = VolumeBar.Value;
                mVodPlayerController.setVolume(mVolume);
                if (mVolume == 0)
                {
                    mVodPlayerController.mute(true);
                    VolumeBox.Image = Properties.Resources.MuteOff;
                }
                else
                {
                    mVodPlayerController.mute(false);
                    VolumeBox.Image = Properties.Resources.MuteOn;
                }
            }
        }

        private void TimeBar_Scroll(object sender, EventArgs e)
        {
            if(mVodPlayerController != null)
            {
                int iPos = TimeBar.Value;
                ulong vodPos =Convert.ToUInt64(mDurationMS * (iPos / 1000.0));
                mVodPlayerController.seek(vodPos);
            }
        }
        private void PlayBox_Click(object sender, EventArgs e)
        {
            if (mVodStatus == VodStatus.Vod_Stop)
            {
                StartVideo();
            }
            else if (mVodStatus == VodStatus.Vod_Pause)
            {
                ResumeVideo();
            }
            else if (mVodStatus == VodStatus.Vod_Play)
            {
                PauseVideo();
            }
        }

        private void StopBox_Click(object sender, EventArgs e)
        {
            StopVideo();
        }

        private void SpeedUpBox_Click(object sender, EventArgs e)
        {
            if (mVodPlayerController != null && mPlaySpeed < maxSpeed)
            {
                mPlaySpeed += 0.2f;
                mVodPlayerController.setRate(mPlaySpeed);
                RateLabel.Text = string.Format("{0}倍速", mPlaySpeed);
                SpeedDownBox.Enabled = true;
            }
            if (Math.Abs(maxSpeed - mPlaySpeed) < 1e-6)
            {
                SpeedUpBox.Enabled = false;
            }
        }

        private void SpeedDownBox_Click(object sender, EventArgs e)
        {
            if (mVodPlayerController != null && mPlaySpeed > minSpeed)
            {
                mPlaySpeed -= 0.2f;
                mVodPlayerController.setRate(mPlaySpeed);
                RateLabel.Text = string.Format("{0}倍速", mPlaySpeed);
                SpeedUpBox.Enabled = true;
            }
            if (Math.Abs(mPlaySpeed - minSpeed) < 1e-6)
            {
                SpeedDownBox.Enabled = false;
            }
        }

        private void OpenFileBox_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog
            {
                Multiselect = false,
                Title = "请选择文件夹",
                Filter = "Video(*.avi;*mp4;*flv;*wmv;*mkv;*m4v;)|*.avi;*.wmv;*.flv;*.mp4;*.mkv;*.m4v|"
                          + "Audio(*.mp3;*wav;*aac;*wma)|*.wav;*.mp3;*.aac;*.wma|"
                          + "All Files(*.*)|*.*"
            };

            if(dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                mFileName = dialog.FileName;
                StopVideo();
                
                StartVideo();
            }
        }

        private void VolumeBox_Click(object sender, EventArgs e)
        {
            if(mVodPlayerController == null)
            {
                return;
            }
            if(mVolumeStatus == VolumeStatus.MuteOn)
            {
                mVodPlayerController.mute(true);
                VolumeBox.Image = Properties.Resources.MuteOff;
                mVolumeStatus = VolumeStatus.MuteOff;
                VolumeBar.Value = 0;
            }
            else
            {
                mVodPlayerController.mute(false);
                VolumeBox.Image = Properties.Resources.MuteOn;
                mVolumeStatus = VolumeStatus.MuteOn;
                VolumeBar.Value = mVolume;
            }
        }

        public int onVodVideoFrame(ref TRTCVideoFrame frame)
        {
            return 0;
        }

        public int onVodAudioFrame(ref TRTCAudioFrame frame)
        {
            return 0;
        }
    }
}
