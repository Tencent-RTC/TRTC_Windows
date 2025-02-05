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
            this.LocalAVRecord = new System.Windows.Forms.TabPage();
            this.LocalAVFileBox = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.ChooseBtn = new System.Windows.Forms.Button();
            this.LocalAVRecordVolum = new System.Windows.Forms.CheckBox();
            this.LocalAVRecordBox = new System.Windows.Forms.CheckBox();
            this.AudioRecord = new System.Windows.Forms.TabPage();
            this.label6 = new System.Windows.Forms.Label();
            this.AudioRecordBox = new System.Windows.Forms.TextBox();
            this.AudioBox = new System.Windows.Forms.CheckBox();
            this.ProcessAudio = new System.Windows.Forms.TabPage();
            this.label7 = new System.Windows.Forms.Label();
            this.AppMixBox = new System.Windows.Forms.TextBox();
            this.AppMixRecordBox = new System.Windows.Forms.CheckBox();
            label1 = new System.Windows.Forms.Label();
            this.panel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.exitPicBox)).BeginInit();
            this.RecordTab.SuspendLayout();
            this.LocalRecordPage.SuspendLayout();
            this.LocalAVRecord.SuspendLayout();
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
            label1.Location = new System.Drawing.Point(17, 13);
            label1.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(110, 31);
            label1.TabIndex = 0;
            label1.Text = "录制设置";
            // 
            // panel3
            // 
            this.panel3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(38)))), ((int)(((byte)(38)))), ((int)(((byte)(38)))));
            this.panel3.Controls.Add(this.exitPicBox);
            this.panel3.Controls.Add(label1);
            this.panel3.Location = new System.Drawing.Point(0, 0);
            this.panel3.Margin = new System.Windows.Forms.Padding(5, 6, 5, 6);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(643, 61);
            this.panel3.TabIndex = 5;
            // 
            // exitPicBox
            // 
            this.exitPicBox.BackgroundImage = global::TRTCCSharpDemo.Properties.Resources.close_white;
            this.exitPicBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.exitPicBox.Cursor = System.Windows.Forms.Cursors.Hand;
            this.exitPicBox.Location = new System.Drawing.Point(595, 13);
            this.exitPicBox.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.exitPicBox.Name = "exitPicBox";
            this.exitPicBox.Size = new System.Drawing.Size(35, 35);
            this.exitPicBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.exitPicBox.TabIndex = 25;
            this.exitPicBox.TabStop = false;
            this.exitPicBox.Click += new System.EventHandler(this.exitPicBox_Click);
            // 
            // VideoRadio
            // 
            this.VideoRadio.AutoSize = true;
            this.VideoRadio.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.VideoRadio.Location = new System.Drawing.Point(127, 297);
            this.VideoRadio.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.VideoRadio.Name = "VideoRadio";
            this.VideoRadio.Size = new System.Drawing.Size(97, 31);
            this.VideoRadio.TabIndex = 1;
            this.VideoRadio.TabStop = true;
            this.VideoRadio.Text = "仅视频";
            this.VideoRadio.UseVisualStyleBackColor = true;
            // 
            // AudioRadio
            // 
            this.AudioRadio.AutoSize = true;
            this.AudioRadio.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.AudioRadio.Location = new System.Drawing.Point(251, 297);
            this.AudioRadio.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.AudioRadio.Name = "AudioRadio";
            this.AudioRadio.Size = new System.Drawing.Size(97, 31);
            this.AudioRadio.TabIndex = 1;
            this.AudioRadio.TabStop = true;
            this.AudioRadio.Text = "仅音频";
            this.AudioRadio.UseVisualStyleBackColor = true;
            // 
            // AllRadio
            // 
            this.AllRadio.AutoSize = true;
            this.AllRadio.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.AllRadio.Location = new System.Drawing.Point(375, 297);
            this.AllRadio.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.AllRadio.Name = "AllRadio";
            this.AllRadio.Size = new System.Drawing.Size(97, 31);
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
            this.LocalRecordBox.Location = new System.Drawing.Point(56, 71);
            this.LocalRecordBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.LocalRecordBox.Name = "LocalRecordBox";
            this.LocalRecordBox.Size = new System.Drawing.Size(136, 35);
            this.LocalRecordBox.TabIndex = 7;
            this.LocalRecordBox.Text = "开启录制";
            this.LocalRecordBox.UseVisualStyleBackColor = true;
            this.LocalRecordBox.CheckedChanged += new System.EventHandler(this.RecordBox_CheckedChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.label2.Location = new System.Drawing.Point(58, 148);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(110, 31);
            this.label2.TabIndex = 8;
            this.label2.Text = "录制文件";
            // 
            // FilepathBox
            // 
            this.FilepathBox.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.FilepathBox.Location = new System.Drawing.Point(189, 151);
            this.FilepathBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.FilepathBox.Name = "FilepathBox";
            this.FilepathBox.Size = new System.Drawing.Size(401, 34);
            this.FilepathBox.TabIndex = 10;
            this.FilepathBox.Text = "D:\\Test\\LocalRecord.mp4";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.label3.Location = new System.Drawing.Point(58, 226);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(110, 31);
            this.label3.TabIndex = 8;
            this.label3.Text = "录制类型";
            // 
            // RecordTab
            // 
            this.RecordTab.Controls.Add(this.LocalRecordPage);
            this.RecordTab.Controls.Add(this.LocalAVRecord);
            this.RecordTab.Controls.Add(this.AudioRecord);
            this.RecordTab.Controls.Add(this.ProcessAudio);
            this.RecordTab.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.RecordTab.Location = new System.Drawing.Point(7, 68);
            this.RecordTab.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.RecordTab.Name = "RecordTab";
            this.RecordTab.SelectedIndex = 0;
            this.RecordTab.Size = new System.Drawing.Size(628, 442);
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
            this.LocalRecordPage.Location = new System.Drawing.Point(4, 40);
            this.LocalRecordPage.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.LocalRecordPage.Name = "LocalRecordPage";
            this.LocalRecordPage.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.LocalRecordPage.Size = new System.Drawing.Size(620, 398);
            this.LocalRecordPage.TabIndex = 0;
            this.LocalRecordPage.Text = "本地录制";
            this.LocalRecordPage.UseVisualStyleBackColor = true;
            // 
            // LocalAVRecord
            // 
            this.LocalAVRecord.Controls.Add(this.LocalAVFileBox);
            this.LocalAVRecord.Controls.Add(this.label5);
            this.LocalAVRecord.Controls.Add(this.label4);
            this.LocalAVRecord.Controls.Add(this.ChooseBtn);
            this.LocalAVRecord.Controls.Add(this.LocalAVRecordVolum);
            this.LocalAVRecord.Controls.Add(this.LocalAVRecordBox);
            this.LocalAVRecord.Location = new System.Drawing.Point(4, 40);
            this.LocalAVRecord.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.LocalAVRecord.Name = "LocalAVRecord";
            this.LocalAVRecord.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.LocalAVRecord.Size = new System.Drawing.Size(620, 398);
            this.LocalAVRecord.TabIndex = 1;
            this.LocalAVRecord.Text = "本地AV录制";
            this.LocalAVRecord.UseVisualStyleBackColor = true;
            // 
            // LocalAVFileBox
            // 
            this.LocalAVFileBox.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.LocalAVFileBox.Location = new System.Drawing.Point(213, 277);
            this.LocalAVFileBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.LocalAVFileBox.Name = "LocalAVFileBox";
            this.LocalAVFileBox.Size = new System.Drawing.Size(295, 34);
            this.LocalAVFileBox.TabIndex = 5;
            this.LocalAVFileBox.Text = "D:\\Test\\LocalAV.mp4";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(47, 274);
            this.label5.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(110, 31);
            this.label5.TabIndex = 4;
            this.label5.Text = "录制文件";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(47, 176);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(110, 31);
            this.label4.TabIndex = 3;
            this.label4.Text = "录制内容";
            // 
            // ChooseBtn
            // 
            this.ChooseBtn.Location = new System.Drawing.Point(213, 173);
            this.ChooseBtn.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.ChooseBtn.Name = "ChooseBtn";
            this.ChooseBtn.Size = new System.Drawing.Size(181, 36);
            this.ChooseBtn.TabIndex = 2;
            this.ChooseBtn.Text = "选择内容";
            this.ChooseBtn.UseVisualStyleBackColor = true;
            this.ChooseBtn.Click += new System.EventHandler(this.ChooseBtn_Click);
            // 
            // LocalAVRecordVolum
            // 
            this.LocalAVRecordVolum.AutoSize = true;
            this.LocalAVRecordVolum.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.LocalAVRecordVolum.Location = new System.Drawing.Point(291, 80);
            this.LocalAVRecordVolum.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.LocalAVRecordVolum.Name = "LocalAVRecordVolum";
            this.LocalAVRecordVolum.Size = new System.Drawing.Size(136, 35);
            this.LocalAVRecordVolum.TabIndex = 1;
            this.LocalAVRecordVolum.Text = "声音录制";
            this.LocalAVRecordVolum.UseVisualStyleBackColor = true;
            this.LocalAVRecordVolum.CheckedChanged += new System.EventHandler(this.LocalAVRecordVolum_CheckedChanged);
            // 
            // LocalAVRecordBox
            // 
            this.LocalAVRecordBox.AutoSize = true;
            this.LocalAVRecordBox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.LocalAVRecordBox.Location = new System.Drawing.Point(46, 80);
            this.LocalAVRecordBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.LocalAVRecordBox.Name = "LocalAVRecordBox";
            this.LocalAVRecordBox.Size = new System.Drawing.Size(136, 35);
            this.LocalAVRecordBox.TabIndex = 0;
            this.LocalAVRecordBox.Text = "开启录制";
            this.LocalAVRecordBox.UseVisualStyleBackColor = true;
            this.LocalAVRecordBox.CheckedChanged += new System.EventHandler(this.LocalAVRecordBox_CheckedChanged);
            // 
            // AudioRecord
            // 
            this.AudioRecord.Controls.Add(this.label6);
            this.AudioRecord.Controls.Add(this.AudioRecordBox);
            this.AudioRecord.Controls.Add(this.AudioBox);
            this.AudioRecord.Location = new System.Drawing.Point(4, 40);
            this.AudioRecord.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.AudioRecord.Name = "AudioRecord";
            this.AudioRecord.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.AudioRecord.Size = new System.Drawing.Size(620, 398);
            this.AudioRecord.TabIndex = 2;
            this.AudioRecord.Text = "录音";
            this.AudioRecord.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.label6.Location = new System.Drawing.Point(45, 166);
            this.label6.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(110, 31);
            this.label6.TabIndex = 11;
            this.label6.Text = "录音文件";
            // 
            // AudioRecordBox
            // 
            this.AudioRecordBox.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.AudioRecordBox.Location = new System.Drawing.Point(176, 169);
            this.AudioRecordBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.AudioRecordBox.Name = "AudioRecordBox";
            this.AudioRecordBox.Size = new System.Drawing.Size(401, 34);
            this.AudioRecordBox.TabIndex = 12;
            this.AudioRecordBox.Text = "D:\\Test\\AudioRecord.wav";
            // 
            // AudioBox
            // 
            this.AudioBox.AutoSize = true;
            this.AudioBox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.AudioBox.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.AudioBox.Location = new System.Drawing.Point(50, 79);
            this.AudioBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.AudioBox.Name = "AudioBox";
            this.AudioBox.Size = new System.Drawing.Size(136, 35);
            this.AudioBox.TabIndex = 0;
            this.AudioBox.Text = "开启录音";
            this.AudioBox.UseVisualStyleBackColor = true;
            this.AudioBox.CheckedChanged += new System.EventHandler(this.AudioBox_CheckedChanged);
            // 
            // ProcessAudio
            // 
            this.ProcessAudio.Controls.Add(this.label7);
            this.ProcessAudio.Controls.Add(this.AppMixBox);
            this.ProcessAudio.Controls.Add(this.AppMixRecordBox);
            this.ProcessAudio.Location = new System.Drawing.Point(4, 40);
            this.ProcessAudio.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.ProcessAudio.Name = "ProcessAudio";
            this.ProcessAudio.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.ProcessAudio.Size = new System.Drawing.Size(620, 398);
            this.ProcessAudio.TabIndex = 3;
            this.ProcessAudio.Text = "进程混音";
            this.ProcessAudio.UseVisualStyleBackColor = true;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.label7.Location = new System.Drawing.Point(63, 187);
            this.label7.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(110, 31);
            this.label7.TabIndex = 13;
            this.label7.Text = "混音进程";
            // 
            // AppMixBox
            // 
            this.AppMixBox.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.AppMixBox.Location = new System.Drawing.Point(194, 190);
            this.AppMixBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.AppMixBox.Name = "AppMixBox";
            this.AppMixBox.Size = new System.Drawing.Size(401, 34);
            this.AppMixBox.TabIndex = 14;
            this.AppMixBox.Text = "‪D:\\QQMusic\\QQMusic.exe";
            // 
            // AppMixRecordBox
            // 
            this.AppMixRecordBox.AutoSize = true;
            this.AppMixRecordBox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.AppMixRecordBox.Location = new System.Drawing.Point(59, 85);
            this.AppMixRecordBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.AppMixRecordBox.Name = "AppMixRecordBox";
            this.AppMixRecordBox.Size = new System.Drawing.Size(136, 35);
            this.AppMixRecordBox.TabIndex = 0;
            this.AppMixRecordBox.Text = "进程混音";
            this.AppMixRecordBox.UseVisualStyleBackColor = true;
            this.AppMixRecordBox.CheckedChanged += new System.EventHandler(this.AppMixRecordBox_CheckedChanged);
            // 
            // TRTCRecordForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(144F, 144F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.Gainsboro;
            this.ClientSize = new System.Drawing.Size(643, 514);
            this.ControlBox = false;
            this.Controls.Add(this.RecordTab);
            this.Controls.Add(this.panel3);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
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
            this.LocalAVRecord.ResumeLayout(false);
            this.LocalAVRecord.PerformLayout();
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
        private System.Windows.Forms.TabPage LocalAVRecord;
        private System.Windows.Forms.TabPage AudioRecord;
        private System.Windows.Forms.TabPage ProcessAudio;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button ChooseBtn;
        private System.Windows.Forms.CheckBox LocalAVRecordVolum;
        private System.Windows.Forms.CheckBox LocalAVRecordBox;
        private System.Windows.Forms.TextBox LocalAVFileBox;
        private System.Windows.Forms.CheckBox AudioBox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox AudioRecordBox;
        private System.Windows.Forms.CheckBox AppMixRecordBox;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox AppMixBox;
    }
}