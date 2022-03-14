using ManageLiteAV;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TRTCCSharpDemo
{
    public partial class TRTCRecordForm : Form
    {

        private TRTCMainForm mMainForm = null;
        private string mLocalFilePath; //存放文件的路径和名称

        private bool mIsMouseDown = false;
        private Point mFormLocation;     // Form的location
        private Point mMouseOffset;      // 鼠标的按下位置

        public TRTCRecordForm(TRTCMainForm mainForm)
        {
            InitializeComponent();

            mMainForm = mainForm;
            panel3.MouseDown += Panel3_MouseDown;
            panel3.MouseUp += Panel3_MouseUp;
            panel3.MouseMove += Panel3_MouseMove;
            Load += TRTCRecordForm_Load;
        }

        private void TRTCRecordForm_Load(object sender, EventArgs e)
        {
            AllRadio.Checked = true;
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

        private void exitPicBox_Click(object sender, EventArgs e)
        {
            if (LocalRecordBox.Checked)
            {
                mMainForm.StopLocalRecording();
            }
            if(AudioBox.Checked)
            {
                mMainForm.StopAudioRecording();
            }
            if(AppMixRecordBox.Checked)
            {
                mMainForm.StopSystemAudioLoopback();
            }
            Close();
        }

        ////////////////////////////////////////////////////////////////////
        /// 
        /// 
        ///                         本地录制接口
        /// 
        /// 
        ////////////////////////////////////////////////////////////////////
        private void RecordBox_CheckedChanged(object sender, EventArgs e)
        {
            if(LocalRecordBox.Checked)
            {
                if(string.IsNullOrWhiteSpace(FilepathBox.Text))
                {
                    LocalRecordBox.Checked = false;
                    MessageBox.Show("请输入文件路径");
                    return;
                }
                mLocalFilePath = FilepathBox.Text;
                
                TRTCLocalRecordingParams param = new TRTCLocalRecordingParams();
                param.filePath = mLocalFilePath;
                if(VideoRadio.Checked)
                {
                    param.recordType = TRTCLocalRecordType.TRTCLocalRecordType_Video;
                }
                else if(AudioRadio.Checked)
                {
                    param.recordType = TRTCLocalRecordType.TRTCLocalRecordType_Audio;
                }
                else
                {
                    param.recordType = TRTCLocalRecordType.TRTCLocalRecordType_Both;
                }
               
                mMainForm.StartLocalRecording(param);
            }
            else
            {
                mMainForm.StopLocalRecording();
            }
        }

        private void ChooseBtn_Click(object sender, EventArgs e)
        {
            mMainForm.ShowShareSelectWin();
        }

        ////////////////////////////////////////////////////////////////////
        /// 
        /// 
        ///                        录音接口
        /// 
        /// 
        ////////////////////////////////////////////////////////////////////
        private void AudioBox_CheckedChanged(object sender, EventArgs e)
        {
            if(AudioBox.Checked)
            {
                if(string.IsNullOrWhiteSpace(AudioRecordBox.Text))
                {
                    AudioBox.Checked = false;
                    MessageBox.Show("请输入文件路径");
                    return;
                }
                TRTCAudioRecordingParams param = new TRTCAudioRecordingParams();
                param.filePath = AudioRecordBox.Text;
                mMainForm.StartAudioRecording(param);
            }
            else
            {
                mMainForm.StopAudioRecording();
            }
        }

        ////////////////////////////////////////////////////////////////////
        /// 
        /// 
        ///                        进程混音接口
        /// 
        /// 
        ////////////////////////////////////////////////////////////////////
        private void AppMixRecordBox_CheckedChanged(object sender, EventArgs e)
        {
            if(AppMixRecordBox.Checked)
            {
                // path为空，代表采集整个操作系统的声音
                // 填写 exe 程序（如 QQ音乐）所在的路径，将会启动此程序并只采集此程序的声音
                mMainForm.StartSystemAudioLoopback(null);
            }
            else
            {
                mMainForm.StopSystemAudioLoopback();
            }
        }
    }
}
