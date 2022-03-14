using ManageLiteAV;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace TRTCCSharpDemo
{
    public partial class ShareSelectForm : Form
    {
        TRTCMainForm mMainForm;
        uint ms_nLastSelectedIndex = 0;
        private ImageList mImageList;
        ITRTCScreenCaptureSourceList mScreenList;

        public ShareSelectForm(TRTCMainForm mainForm)
        {
            InitializeComponent();

            mMainForm = mainForm;
            mImageList = new ImageList();
            mImageList.ImageSize = new Size(120, 70);
            Load += ShareSelectForm_Load;

        }

        private void ShareSelectForm_Load(object sender, EventArgs e)
        {
            mScreenList = mMainForm.GetScreenCaptureSources();

            if(ms_nLastSelectedIndex > mScreenList.getCount())
            {
                ms_nLastSelectedIndex = mScreenList.getCount() - 1;
            }

           // mImageList.Images.Clear();
            //screenListView.Clear();
            for (uint i =0;i< mScreenList.getCount();++i )
            {
                TRTCScreenCaptureSourceInfo sourse = mScreenList.getSourceInfo(i);

                string name;
                if (sourse.sourceName.Equals("Screen1"))
                    name = "显示器-1";
                else if (sourse.sourceName.Equals("Screen2"))
                    name = "显示器-2";
                else if (sourse.sourceName.Equals("Screen3"))
                    name = "显示器-3";
                else if (sourse.sourceName.Equals("Screen4"))
                    name = "显示器-4";
                else if (sourse.sourceName.Equals("Screen5"))
                    name = "显示器-5";
                else
                    name = sourse.sourceName;

                // 设置屏幕缩略图

                int width = 120;
                int height = 70;
                Bitmap bmp = new Bitmap(width, height, PixelFormat.Format32bppRgb);
                if (sourse.thumbBGRA.length <= 0)
                {
                    // 未找到缩略图，不显示
                    using (Graphics g = Graphics.FromImage(bmp))
                    {
                        g.Clear(Color.White);
                    }
                    mImageList.Images.Add(name, bmp);
                    continue;
                }

                BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.WriteOnly, PixelFormat.Format32bppRgb);

                int stride = bmpData.Stride;
                IntPtr iptr = bmpData.Scan0;
                int scanBytes = stride * height;
                int posScan = 0, posReal = 0;
                byte[] pixelValues = new byte[scanBytes];

                for (int j = 0; j < sourse.thumbBGRA.buffer.Length; j++)
                    pixelValues[posScan++] = sourse.thumbBGRA.buffer[posReal++];

                Marshal.Copy(pixelValues, 0, iptr, scanBytes);
                bmp.UnlockBits(bmpData);

                mImageList.Images.Add(name, bmp);
            }

            this.screenListView.LargeImageList = mImageList;
            this.screenListView.BeginUpdate();
            for (int i = 0; i < mImageList.Images.Count; i++)
            {
                ListViewItem item = new ListViewItem();
                item.ImageIndex = i;
                item.Text = mImageList.Images.Keys[i];
                this.screenListView.Items.Add(item);
            }
            this.screenListView.EndUpdate();
            this.screenListView.HideSelection = true;
            if (this.screenListView.Items.Count > 0)
            {
                this.screenListView.Items[0].Selected = true;
                this.screenListView.Select();
            }
        }

        private void exitPicBox_Click(object sender, EventArgs e)
        {
            mMainForm.SetmScreenCaptureSourceInfo(null);
            this.Close();
        }

        private void OkBtn_Click(object sender, EventArgs e)
        {
            if (this.screenListView.SelectedItems.Count == 0)
            {
                MessageForm msg = new MessageForm();
                msg.setCancelBtn(false);
                msg.setText("请选择一个需要共享的屏幕！");
                msg.ShowDialog();
                return;
            }
            TRTCScreenCaptureSourceInfo sourceinfo = mScreenList.getSourceInfo(0);
            for (uint i = 0; i < mScreenList.getCount(); i++)
            {
                TRTCScreenCaptureSourceInfo info = mScreenList.getSourceInfo(i);

                string name = this.screenListView.SelectedItems[0].Text;

                if (this.screenListView.SelectedItems[0].Text.Equals("显示器-1"))
                    name = "Screen1";
                else if (this.screenListView.SelectedItems[0].Text.Equals("显示器-2"))
                    name = "Screen2";
                else if (this.screenListView.SelectedItems[0].Text.Equals("显示器-3"))
                    name = "Screen3";
                else if (this.screenListView.SelectedItems[0].Text.Equals("显示器-4"))
                    name = "Screen4";
                else if (this.screenListView.SelectedItems[0].Text.Equals("显示器-5"))
                    name = "Screen5";

                if (name.Equals(mScreenList.getSourceInfo(i).sourceName))
                {
                    sourceinfo = info;
                    break;
                }
            }

            mMainForm.SetmScreenCaptureSourceInfo(sourceinfo);
            this.Close();
        }

        private void CancelBtn_Click(object sender, EventArgs e)
        {
            mMainForm.SetmScreenCaptureSourceInfo(null);
            this.Close();
        }
    }
}
