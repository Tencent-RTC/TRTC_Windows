
namespace TRTCCSharpDemo
{
    partial class PortalWindow
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
            this.on_btn_trtc = new System.Windows.Forms.Button();
            this.on_btn_live = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // on_btn_trtc
            // 
            this.on_btn_trtc.Location = new System.Drawing.Point(67, 87);
            this.on_btn_trtc.Margin = new System.Windows.Forms.Padding(1);
            this.on_btn_trtc.Name = "on_btn_trtc";
            this.on_btn_trtc.Size = new System.Drawing.Size(172, 67);
            this.on_btn_trtc.TabIndex = 0;
            this.on_btn_trtc.Text = "TRTC Demo";
            this.on_btn_trtc.UseVisualStyleBackColor = true;
            this.on_btn_trtc.Click += new System.EventHandler(this.on_btn_trtc_Click);
            // 
            // on_btn_live
            // 
            this.on_btn_live.Location = new System.Drawing.Point(330, 87);
            this.on_btn_live.Margin = new System.Windows.Forms.Padding(1);
            this.on_btn_live.Name = "on_btn_live";
            this.on_btn_live.Size = new System.Drawing.Size(172, 67);
            this.on_btn_live.TabIndex = 1;
            this.on_btn_live.Text = "Live Demo";
            this.on_btn_live.UseVisualStyleBackColor = true;
            this.on_btn_live.Click += new System.EventHandler(this.on_btn_live_Click);
            // 
            // PortalWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(584, 261);
            this.Controls.Add(this.on_btn_live);
            this.Controls.Add(this.on_btn_trtc);
            this.Margin = new System.Windows.Forms.Padding(1);
            this.Name = "PortalWindow";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "PortalWindow";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnExitPicBoxClick);
            this.Load += new System.EventHandler(this.PortalWindow_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button on_btn_trtc;
        private System.Windows.Forms.Button on_btn_live;
    }
}