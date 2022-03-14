namespace TRTCCSharpDemo
{
    partial class TRTCRecordForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.Label label1;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TRTCRecordForm));
            this.panel3 = new System.Windows.Forms.Panel();
            this.exitPicBox = new System.Windows.Forms.PictureBox();
            this.VideoRadio = new System.Windows.Forms.RadioButton();
            this.AudioRadio = new System.Windows.Forms.RadioButton();
            this.AllRadio = new System.Windows.Forms.RadioButton();
            this.LocalRecordBox = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.FilepathBox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.RecordTab = new System.Windows.Forms.TabControl();
            this.LocalRecordPage = new System.Windows.Forms.TabPage();
            this.AudioRecord = new System.Windows.Forms.TabPage();
            this.label6 = new System.Windows.Forms.Label();
            this.AudioRecordBox = new System.Windows.Forms.TextBox();
            this.AudioBox = new System.Windows.Forms.CheckBox();
            this.ProcessAudio = new System.Windows.Forms.TabPage();
            this.AppMixRecordBox = new System.Windows.Forms.CheckBox();
            label1 = new System.Windows.Forms.Label();
            this.panel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.exitPicBox)).BeginInit();
            this.RecordTab.SuspendLayout();
            this.LocalRecordPage.SuspendLayout();
            this.AudioRecord.SuspendLayout();
            this.ProcessAudio.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Cursor = System.Windows.Forms.Cursors.Default;
            label1.Font = new System.Drawing.Font("微软雅黑", 12F);
            label1.ForeColor = System.Drawing.Color.White;
            label1.Location = new System.Drawing.Point(28, 22);
            label1.Margin = new System.Windows.Forms.Padding(8, 0, 8, 0);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(183, 52);
            label1.TabIndex = 0;
            label1.Text = "录制设置";
            // 
            // panel3
            // 
            this.panel3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(38)))), ((int)(((byte)(38)))), ((int)(((byte)(38)))));
            this.panel3.Controls.Add(this.exitPicBox);
            this.panel3.Controls.Add(label1);
            this.panel3.Location = new System.Drawing.Point(0, 0);
            this.panel3.Margin = new System.Windows.Forms.Padding(8, 10, 8, 10);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(1072, 101);
            this.panel3.TabIndex = 5;
            // 
            // exitPicBox
            // 
            this.exitPicBox.BackgroundImage = global::TRTCCSharpDemo.Properties.Resources.close_white;
            this.exitPicBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.exitPicBox.Cursor = System.Windows.Forms.Cursors.Hand;
            this.exitPicBox.Location = new System.Drawing.Point(992, 22);
            this.exitPicBox.Margin = new System.Windows.Forms.Padding(8);
            this.exitPicBox.Name = "exitPicBox";
            this.exitPicBox.Size = new System.Drawing.Size(58, 58);
            this.exitPicBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.exitPicBox.TabIndex = 25;
            this.exitPicBox.TabStop = false;
            this.exitPicBox.Click += new System.EventHandler(this.exitPicBox_Click);
            // 
            // VideoRadio
            // 
            this.VideoRadio.AutoSize = true;
            this.VideoRadio.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.VideoRadio.Location = new System.Drawing.Point(212, 495);
            this.VideoRadio.Name = "VideoRadio";
            this.VideoRadio.Size = new System.Drawing.Size(159, 49);
            this.VideoRadio.TabIndex = 1;
            this.VideoRadio.Text = "仅视频";
            this.VideoRadio.UseVisualStyleBackColor = true;
            // 
            // AudioRadio
            // 
            this.AudioRadio.AutoSize = true;
            this.AudioRadio.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.AudioRadio.Location = new System.Drawing.Point(419, 495);
            this.AudioRadio.Name = "AudioRadio";
            this.AudioRadio.Size = new System.Drawing.Size(159, 49);
            this.AudioRadio.TabIndex = 1;
            this.AudioRadio.Text = "仅音频";
            this.AudioRadio.UseVisualStyleBackColor = true;
            // 
            // AllRadio
            // 
            this.AllRadio.AutoSize = true;
            this.AllRadio.Checked = true;
            this.AllRadio.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.AllRadio.Location = new System.Drawing.Point(625, 495);
            this.AllRadio.Name = "AllRadio";
            this.AllRadio.Size = new System.Drawing.Size(159, 49);
            this.AllRadio.TabIndex = 1;
            this.AllRadio.TabStop = true;
            this.AllRadio.Text = "音视频";
            this.AllRadio.UseVisualStyleBackColor = true;
            // 
            // LocalRecordBox
            // 
            this.LocalRecordBox.AutoSize = true;
            this.LocalRecordBox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.LocalRecordBox.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.LocalRecordBox.Location = new System.Drawing.Point(93, 118);
            this.LocalRecordBox.Name = "LocalRecordBox";
            this.LocalRecordBox.Size = new System.Drawing.Size(221, 56);
            this.LocalRecordBox.TabIndex = 7;
            this.LocalRecordBox.Text = "开启录制";
            this.LocalRecordBox.UseVisualStyleBackColor = true;
            this.LocalRecordBox.CheckedChanged += new System.EventHandler(this.RecordBox_CheckedChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.label2.Location = new System.Drawing.Point(96, 247);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(183, 52);
            this.label2.TabIndex = 8;
            this.label2.Text = "录制文件";
            // 
            // FilepathBox
            // 
            this.FilepathBox.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.FilepathBox.Location = new System.Drawing.Point(315, 252);
            this.FilepathBox.Name = "FilepathBox";
            this.FilepathBox.Size = new System.Drawing.Size(666, 51);
            this.FilepathBox.TabIndex = 10;
            this.FilepathBox.Text = "D:\\Test\\LocalRecord.mp4";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.label3.Location = new System.Drawing.Point(96, 376);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(183, 52);
            this.label3.TabIndex = 8;
            this.label3.Text = "录制类型";
            // 
            // RecordTab
            // 
            this.RecordTab.Controls.Add(this.LocalRecordPage);
            this.RecordTab.Controls.Add(this.AudioRecord);
            this.RecordTab.Controls.Add(this.ProcessAudio);
            this.RecordTab.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.RecordTab.Location = new System.Drawing.Point(12, 114);
            this.RecordTab.Name = "RecordTab";
            this.RecordTab.SelectedIndex = 0;
            this.RecordTab.Size = new System.Drawing.Size(1047, 737);
            this.RecordTab.TabIndex = 12;
            // 
            // LocalRecordPage
            // 
            this.LocalRecordPage.Controls.Add(this.LocalRecordBox);
            this.LocalRecordPage.Controls.Add(this.AllRadio);
            this.LocalRecordPage.Controls.Add(this.label3);
            this.LocalRecordPage.Controls.Add(this.label2);
            this.LocalRecordPage.Controls.Add(this.AudioRadio);
            this.LocalRecordPage.Controls.Add(this.FilepathBox);
            this.LocalRecordPage.Controls.Add(this.VideoRadio);
            this.LocalRecordPage.Location = new System.Drawing.Point(10, 69);
            this.LocalRecordPage.Name = "LocalRecordPage";
            this.LocalRecordPage.Padding = new System.Windows.Forms.Padding(3);
            this.LocalRecordPage.Size = new System.Drawing.Size(1027, 658);
            this.LocalRecordPage.TabIndex = 0;
            this.LocalRecordPage.Text = "本地录制";
            this.LocalRecordPage.UseVisualStyleBackColor = true;
            // 
            // AudioRecord
            // 
            this.AudioRecord.Controls.Add(this.label6);
            this.AudioRecord.Controls.Add(this.AudioRecordBox);
            this.AudioRecord.Controls.Add(this.AudioBox);
            this.AudioRecord.Location = new System.Drawing.Point(10, 69);
            this.AudioRecord.Name = "AudioRecord";
            this.AudioRecord.Padding = new System.Windows.Forms.Padding(3);
            this.AudioRecord.Size = new System.Drawing.Size(1027, 658);
            this.AudioRecord.TabIndex = 2;
            this.AudioRecord.Text = "录音";
            this.AudioRecord.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.label6.Location = new System.Drawing.Point(75, 277);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(183, 52);
            this.label6.TabIndex = 11;
            this.label6.Text = "录音文件";
            // 
            // AudioRecordBox
            // 
            this.AudioRecordBox.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.AudioRecordBox.Location = new System.Drawing.Point(294, 282);
            this.AudioRecordBox.Name = "AudioRecordBox";
            this.AudioRecordBox.Size = new System.Drawing.Size(666, 51);
            this.AudioRecordBox.TabIndex = 12;
            this.AudioRecordBox.Text = "D:\\Test\\AudioRecord.wav";
            // 
            // AudioBox
            // 
            this.AudioBox.AutoSize = true;
            this.AudioBox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.AudioBox.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.AudioBox.Location = new System.Drawing.Point(84, 132);
            this.AudioBox.Name = "AudioBox";
            this.AudioBox.Size = new System.Drawing.Size(221, 56);
            this.AudioBox.TabIndex = 0;
            this.AudioBox.Text = "开启录音";
            this.AudioBox.UseVisualStyleBackColor = true;
            this.AudioBox.CheckedChanged += new System.EventHandler(this.AudioBox_CheckedChanged);
            // 
            // ProcessAudio
            // 
            this.ProcessAudio.Controls.Add(this.AppMixRecordBox);
            this.ProcessAudio.Location = new System.Drawing.Point(10, 69);
            this.ProcessAudio.Name = "ProcessAudio";
            this.ProcessAudio.Padding = new System.Windows.Forms.Padding(3);
            this.ProcessAudio.Size = new System.Drawing.Size(1027, 658);
            this.ProcessAudio.TabIndex = 3;
            this.ProcessAudio.Text = "进程混音";
            this.ProcessAudio.UseVisualStyleBackColor = true;
            // 
            // AppMixRecordBox
            // 
            this.AppMixRecordBox.AutoSize = true;
            this.AppMixRecordBox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.AppMixRecordBox.Location = new System.Drawing.Point(99, 142);
            this.AppMixRecordBox.Name = "AppMixRecordBox";
            this.AppMixRecordBox.Size = new System.Drawing.Size(221, 56);
            this.AppMixRecordBox.TabIndex = 0;
            this.AppMixRecordBox.Text = "进程混音";
            this.AppMixRecordBox.UseVisualStyleBackColor = true;
            this.AppMixRecordBox.CheckedChanged += new System.EventHandler(this.AppMixRecordBox_CheckedChanged);
            // 
            // TRTCRecordForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(240F, 240F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.Gainsboro;
            this.ClientSize = new System.Drawing.Size(1071, 857);
            this.ControlBox = false;
            this.Controls.Add(this.RecordTab);
            this.Controls.Add(this.panel3);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "TRTCRecordForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "TRTCRecordForm";
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.exitPicBox)).EndInit();
            this.RecordTab.ResumeLayout(false);
            this.LocalRecordPage.ResumeLayout(false);
            this.LocalRecordPage.PerformLayout();
            this.AudioRecord.ResumeLayout(false);
            this.AudioRecord.PerformLayout();
            this.ProcessAudio.ResumeLayout(false);
            this.ProcessAudio.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.PictureBox exitPicBox;
        private System.Windows.Forms.RadioButton AllRadio;
        private System.Windows.Forms.RadioButton AudioRadio;
        private System.Windows.Forms.RadioButton VideoRadio;
        private System.Windows.Forms.CheckBox LocalRecordBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox FilepathBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TabControl RecordTab;
        private System.Windows.Forms.TabPage LocalRecordPage;
        private System.Windows.Forms.TabPage AudioRecord;
        private System.Windows.Forms.TabPage ProcessAudio;
        private System.Windows.Forms.CheckBox AudioBox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox AudioRecordBox;
        private System.Windows.Forms.CheckBox AppMixRecordBox;
    }
}