using System;
using System.Drawing;
using System.Windows.Forms;
using TRTCCSharpDemo.Common;
using ManageLiteAV;

/// <summary>
/// Module： TRTCLoginForm
/// 
/// Function： The interface allows the user to enter a room number and a user name.
/// 
/// Notice：
/// （1）The room number is a number, and the user name is a string
///
/// （2）In real usage scenarios, room numbers are mostly not manually entered by users, but assigned by the system.
///      For example, the meeting number in the video conference is booked in advance by the control system,
///      and the room number in the customer service system is also determined by the job number of
///      the customer service employee.
/// </summary>

namespace TRTCCSharpDemo
{
    public partial class TRTCLoginForm : Form
    {
        PortalWindow m_portal;
        public TRTCLoginForm(PortalWindow portal)
        {
            InitializeComponent();

            this.Disposed += new EventHandler(OnDisposed);
            m_portal = portal;
        }

        private void OnDisposed(object sender, EventArgs e)
        {
            //Resource cleaning
        }

        private void ShowMessage(string text)
        {
            MessageForm msgBox = new MessageForm();
            msgBox.setText(text);
            msgBox.setCancelBtn(false);
            msgBox.ShowDialog();
        }

        private void OnLoad(object sender, EventArgs e)
        {
            this.roomTextBox.Focus();
            this.userTextBox.Text = DataManager.GetInstance().userId;
            this.roomTextBox.Text = DataManager.GetInstance().roomId.ToString();

            if (Util.IsTestEnv())
            {
                this.formalEnvRadioBtn.Visible = true;
                this.testEnvRadioBtn.Visible = true;
                this.lifeEnvRadioBtn.Visible = true;
                this.audioRadioBtn.Visible = true;
                this.videoRadioBtn.Visible = true;

                if (DataManager.GetInstance().testEnv == 0)
                    this.formalEnvRadioBtn.Checked = true;
                else if (DataManager.GetInstance().testEnv == 1)
                    this.testEnvRadioBtn.Checked = true;
                else if (DataManager.GetInstance().testEnv == 2)
                    this.lifeEnvRadioBtn.Checked = true;
                if (DataManager.GetInstance().pureAudioStyle)
                    this.audioRadioBtn.Checked = true;
                else
                    this.videoRadioBtn.Checked = true;
            }
            else
            {
                this.formalEnvRadioBtn.Visible = false;
                this.testEnvRadioBtn.Visible = false;
                this.lifeEnvRadioBtn.Visible = false;
                this.audioRadioBtn.Visible = false;
                this.videoRadioBtn.Visible = false;

                DataManager.GetInstance().testEnv = 0;
                DataManager.GetInstance().pureAudioStyle = false;
            }
        }

        #region Form Move

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

        private void OnJoinRoomBtnClick(object sender, EventArgs e)
        {
            if (GenerateTestUserSig.SDKAPPID == 0)
            {
                ShowMessage("Error: 请先在 GenerateTestUserSig 填写 sdkappid 信息");
                return;
            }

            SetTestEnv();
            SetPureAudioStyle();

            string userId = this.userTextBox.Text;
            string roomId = this.roomTextBox.Text;
            if (string.IsNullOrEmpty(userId) || string.IsNullOrEmpty(roomId))
            {
                ShowMessage("房间号或用户号不能为空！");
                return;
            }

            uint room = 0;
            if (!uint.TryParse(roomId, out room))
            {
                ShowMessage(String.Format("目前支持的最大房间号为{0}", uint.MaxValue));
                return;
            }

            DataManager.GetInstance().userId = userId;
            DataManager.GetInstance().roomId = room;

            // Obtain the userSig corresponding to the userId from the local calculation
            // Attention: Local computing is suitable for debugging in a local environment,
            // the correct practice is to put the UserSig calculation code and encryption key on your business server.
            // The App then gets the UserSig calculated in real time from your server on demand.
            // Since the cost of cracking the server is higher than cracking the client App,
            // the solution calculated by the server can better protect your encryption key.
            string userSig = GenerateTestUserSig.GetInstance().GenTestUserSig(userId);
            if (string.IsNullOrEmpty(userSig))
            {
                ShowMessage("userSig 获取失败，请检查是否填写账号信息！");
                return;
            }

            this.Hide();
            TRTCMainForm mainForm  = new TRTCMainForm(this);
            mainForm.Show();
            mainForm.EnterRoom();
        }

        /// <summary>
        /// Setting the connection environment applies only to the local debugging test
        /// </summary>
        private void SetTestEnv()
        {
            if (this.formalEnvRadioBtn.Checked)
                DataManager.GetInstance().testEnv = 0;
            else if (this.testEnvRadioBtn.Checked)
                DataManager.GetInstance().testEnv = 1;
            else if (this.lifeEnvRadioBtn.Checked)
                DataManager.GetInstance().testEnv = 2;
        }

        /// <summary>
        /// Set whether to use audio-only environment to enter the room
        /// </summary>
        private void SetPureAudioStyle()
        {
            if (this.audioRadioBtn.Checked)
                DataManager.GetInstance().pureAudioStyle = true;
            else if (this.videoRadioBtn.Checked)
                DataManager.GetInstance().pureAudioStyle = false;
        }

        private void OnExitPicBoxClick(object sender, EventArgs e)
        {
            this.Close();
            m_portal.Show();
        }

        private void OnRoomTextBoxKeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')
            {
                if (e.KeyChar < 48 || e.KeyChar > 57)
                    e.Handled = true;
            }
        }

        /// <summary>
        /// Mainly used when the local debugging environment, and the test function is started when the file exists
        /// </summary>
        
    }
}
