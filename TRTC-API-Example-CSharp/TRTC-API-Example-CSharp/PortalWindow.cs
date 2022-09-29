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
    public partial class PortalWindow : Form
    {
        private TRTCLoginForm m_trtc;
        private LiveWindowForm m_Live;
        public PortalWindow()
        {
            InitializeComponent();
        }

        private void PortalWindow_Load(object sender, EventArgs e)
        {

        }

        private void on_btn_trtc_Click(object sender, EventArgs e)
        {
            m_trtc = new TRTCLoginForm(this);
            m_trtc.Show();
            this.Hide();
        }

        private void on_btn_live_Click(object sender, EventArgs e)
        {
            m_Live = new LiveWindowForm(this);
            m_Live.Show();
            this.Hide();
        }

        private void OnExitPicBoxClick(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }
    }
}
