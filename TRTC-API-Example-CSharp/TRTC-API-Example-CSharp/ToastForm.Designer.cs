namespace TRTCCSharpDemo
{
    partial class ToastForm
    {
        /// <summary> 
        /// Required designer variables.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up all resource in use
        /// </summary>
        /// <param name="disposing">'true' for the managed resource should be freed; Otherwise it is false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 组件设计器生成的代码

        /// <summary> 
        /// Functions supprted by Designer - remain
        /// Modify the contents of this method using the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ToastForm));
            this.textLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // textLabel
            // 
            this.textLabel.AutoSize = true;
            this.textLabel.BackColor = System.Drawing.SystemColors.Control;
            this.textLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textLabel.Font = new System.Drawing.Font("微软雅黑", 11F);
            this.textLabel.ForeColor = System.Drawing.Color.Red;
            this.textLabel.Location = new System.Drawing.Point(0, 0);
            this.textLabel.Name = "textLabel";
            this.textLabel.Padding = new System.Windows.Forms.Padding(20, 4, 20, 4);
            this.textLabel.Size = new System.Drawing.Size(139, 28);
            this.textLabel.TabIndex = 0;
            this.textLabel.Text = "正在屏幕共享";
            this.textLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ToastForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 17F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.AutoValidate = System.Windows.Forms.AutoValidate.Disable;
            this.CausesValidation = false;
            this.ClientSize = new System.Drawing.Size(139, 28);
            this.ControlBox = false;
            this.Controls.Add(this.textLabel);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("微软雅黑", 9F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ToastForm";
            this.Opacity = 0.8D;
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.TopMost = true;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label textLabel;
    }
}
