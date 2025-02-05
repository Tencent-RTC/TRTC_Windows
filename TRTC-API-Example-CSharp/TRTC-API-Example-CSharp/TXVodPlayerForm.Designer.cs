namespace TRTCCSharpDemo
{
    partial class TXVodPlayerForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TXVodPlayerForm));
            this.PlayPanel = new System.Windows.Forms.Panel();
            this.panel3 = new System.Windows.Forms.Panel();
            this.exitPicBox = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.RateLabel = new System.Windows.Forms.Label();
            this.TimeLabel = new System.Windows.Forms.Label();
            this.TimeBar = new System.Windows.Forms.TrackBar();
            this.VolumeBar = new System.Windows.Forms.TrackBar();
            this.PlayBox = new System.Windows.Forms.PictureBox();
            this.StopBox = new System.Windows.Forms.PictureBox();
            this.OpenFileBox = new System.Windows.Forms.PictureBox();
            this.VolumeBox = new System.Windows.Forms.PictureBox();
            this.panel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.exitPicBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.TimeBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.VolumeBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PlayBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.StopBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.OpenFileBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.VolumeBox)).BeginInit();
            this.SuspendLayout();
            // 
            // PlayPanel
            // 
            this.PlayPanel.BackColor = System.Drawing.Color.Black;
            this.PlayPanel.Location = new System.Drawing.Point(23, 66);
            this.PlayPanel.Margin = new System.Windows.Forms.Padding(1, 1, 1, 1);
            this.PlayPanel.Name = "PlayPanel";
            this.PlayPanel.Size = new System.Drawing.Size(825, 465);
            this.PlayPanel.TabIndex = 2;
            // 
            // panel3
            // 
            this.panel3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(38)))), ((int)(((byte)(38)))), ((int)(((byte)(38)))));
            this.panel3.Controls.Add(this.exitPicBox);
            this.panel3.Controls.Add(this.label1);
            this.panel3.Location = new System.Drawing.Point(0, 0);
            this.panel3.Margin = new System.Windows.Forms.Padding(5, 6, 5, 6);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(873, 59);
            this.panel3.TabIndex = 4;
            // 
            // exitPicBox
            // 
            this.exitPicBox.BackgroundImage = global::TRTCCSharpDemo.Properties.Resources.close_white;
            this.exitPicBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.exitPicBox.Cursor = System.Windows.Forms.Cursors.Hand;
            this.exitPicBox.Location = new System.Drawing.Point(825, 9);
            this.exitPicBox.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.exitPicBox.Name = "exitPicBox";
            this.exitPicBox.Size = new System.Drawing.Size(35, 35);
            this.exitPicBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.exitPicBox.TabIndex = 25;
            this.exitPicBox.TabStop = false;
            this.exitPicBox.Click += new System.EventHandler(this.exitPicBox_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Cursor = System.Windows.Forms.Cursors.Default;
            this.label1.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(17, 13);
            this.label1.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(86, 31);
            this.label1.TabIndex = 0;
            this.label1.Text = "播放器";
            // 
            // RateLabel
            // 
            this.RateLabel.AutoSize = true;
            this.RateLabel.Enabled = false;
            this.RateLabel.Location = new System.Drawing.Point(74, 605);
            this.RateLabel.Margin = new System.Windows.Forms.Padding(1, 0, 1, 0);
            this.RateLabel.Name = "RateLabel";
            this.RateLabel.Size = new System.Drawing.Size(57, 24);
            this.RateLabel.TabIndex = 3;
            this.RateLabel.Text = "1倍速";
            // 
            // TimeLabel
            // 
            this.TimeLabel.AutoSize = true;
            this.TimeLabel.Enabled = false;
            this.TimeLabel.Location = new System.Drawing.Point(418, 605);
            this.TimeLabel.Margin = new System.Windows.Forms.Padding(1, 0, 1, 0);
            this.TimeLabel.Name = "TimeLabel";
            this.TimeLabel.Size = new System.Drawing.Size(58, 24);
            this.TimeLabel.TabIndex = 3;
            this.TimeLabel.Text = "00:00";
            // 
            // TimeBar
            // 
            this.TimeBar.AutoSize = false;
            this.TimeBar.BackColor = System.Drawing.Color.Gainsboro;
            this.TimeBar.CausesValidation = false;
            this.TimeBar.Cursor = System.Windows.Forms.Cursors.Default;
            this.TimeBar.Location = new System.Drawing.Point(61, 559);
            this.TimeBar.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.TimeBar.Maximum = 100;
            this.TimeBar.Name = "TimeBar";
            this.TimeBar.Size = new System.Drawing.Size(767, 42);
            this.TimeBar.TabIndex = 1;
            this.TimeBar.TickFrequency = 0;
            this.TimeBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.TimeBar.MouseDown += new System.Windows.Forms.MouseEventHandler(this.TimeBar_MouseDown);
            // 
            // VolumeBar
            // 
            this.VolumeBar.AutoSize = false;
            this.VolumeBar.CausesValidation = false;
            this.VolumeBar.Cursor = System.Windows.Forms.Cursors.Default;
            this.VolumeBar.Location = new System.Drawing.Point(581, 659);
            this.VolumeBar.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.VolumeBar.Maximum = 100;
            this.VolumeBar.Name = "VolumeBar";
            this.VolumeBar.Size = new System.Drawing.Size(169, 36);
            this.VolumeBar.TabIndex = 40;
            this.VolumeBar.TickFrequency = 0;
            this.VolumeBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.VolumeBar.Scroll += new System.EventHandler(this.VolumeBar_Scroll);
            // 
            // PlayBox
            // 
            this.PlayBox.BackgroundImage = global::TRTCCSharpDemo.Properties.Resources.close_white;
            this.PlayBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.PlayBox.Cursor = System.Windows.Forms.Cursors.Hand;
            this.PlayBox.Image = global::TRTCCSharpDemo.Properties.Resources.Play;
            this.PlayBox.Location = new System.Drawing.Point(78, 651);
            this.PlayBox.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.PlayBox.Name = "PlayBox";
            this.PlayBox.Size = new System.Drawing.Size(54, 54);
            this.PlayBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.PlayBox.TabIndex = 41;
            this.PlayBox.TabStop = false;
            this.PlayBox.Click += new System.EventHandler(this.PlayBox_Click);
            // 
            // StopBox
            // 
            this.StopBox.BackgroundImage = global::TRTCCSharpDemo.Properties.Resources.close_white;
            this.StopBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.StopBox.Cursor = System.Windows.Forms.Cursors.Hand;
            this.StopBox.Image = global::TRTCCSharpDemo.Properties.Resources.Stop;
            this.StopBox.Location = new System.Drawing.Point(164, 651);
            this.StopBox.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.StopBox.Name = "StopBox";
            this.StopBox.Size = new System.Drawing.Size(54, 54);
            this.StopBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.StopBox.TabIndex = 41;
            this.StopBox.TabStop = false;
            this.StopBox.Click += new System.EventHandler(this.StopBox_Click);
            // 
            // OpenFileBox
            // 
            this.OpenFileBox.BackgroundImage = global::TRTCCSharpDemo.Properties.Resources.close_white;
            this.OpenFileBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.OpenFileBox.Cursor = System.Windows.Forms.Cursors.Hand;
            this.OpenFileBox.Image = global::TRTCCSharpDemo.Properties.Resources.OpenFile;
            this.OpenFileBox.Location = new System.Drawing.Point(422, 651);
            this.OpenFileBox.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.OpenFileBox.Name = "OpenFileBox";
            this.OpenFileBox.Size = new System.Drawing.Size(54, 54);
            this.OpenFileBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.OpenFileBox.TabIndex = 41;
            this.OpenFileBox.TabStop = false;
            this.OpenFileBox.Click += new System.EventHandler(this.OpenFileBox_Click);
            // 
            // VolumeBox
            // 
            this.VolumeBox.BackgroundImage = global::TRTCCSharpDemo.Properties.Resources.close_white;
            this.VolumeBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.VolumeBox.Cursor = System.Windows.Forms.Cursors.Hand;
            this.VolumeBox.Image = global::TRTCCSharpDemo.Properties.Resources.MuteOn;
            this.VolumeBox.Location = new System.Drawing.Point(500, 651);
            this.VolumeBox.Margin = new System.Windows.Forms.Padding(5, 5, 5, 5);
            this.VolumeBox.Name = "VolumeBox";
            this.VolumeBox.Size = new System.Drawing.Size(54, 54);
            this.VolumeBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.VolumeBox.TabIndex = 41;
            this.VolumeBox.TabStop = false;
            this.VolumeBox.Click += new System.EventHandler(this.VolumeBox_Click);
            // 
            // TXVodPlayerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(144F, 144F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.Gainsboro;
            this.ClientSize = new System.Drawing.Size(875, 760);
            this.Controls.Add(this.VolumeBar);
            this.Controls.Add(this.VolumeBox);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.OpenFileBox);
            this.Controls.Add(this.PlayPanel);
            this.Controls.Add(this.StopBox);
            this.Controls.Add(this.PlayBox);
            this.Controls.Add(this.TimeBar);
            this.Controls.Add(this.RateLabel);
            this.Controls.Add(this.TimeLabel);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(1, 1, 1, 1);
            this.Name = "TXVodPlayerForm";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "TXVodPlayerForm";
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.exitPicBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.TimeBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.VolumeBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PlayBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.StopBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.OpenFileBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.VolumeBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Panel PlayPanel;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox exitPicBox;
        private System.Windows.Forms.Label RateLabel;
        private System.Windows.Forms.Label TimeLabel;
        private System.Windows.Forms.TrackBar TimeBar;
        private System.Windows.Forms.TrackBar VolumeBar;
        private System.Windows.Forms.PictureBox PlayBox;
        private System.Windows.Forms.PictureBox StopBox;
        private System.Windows.Forms.PictureBox OpenFileBox;
        private System.Windows.Forms.PictureBox VolumeBox;
    }
}