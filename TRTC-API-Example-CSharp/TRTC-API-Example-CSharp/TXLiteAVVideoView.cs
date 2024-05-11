using ManageLiteAV;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using TRTCCSharpDemo.Common;

/// <summary>
/// Custom video rendering View, assocaited with TRTC SDK, can be copied and extended
/// 
/// API：
/// 1.RegEngine(string userId, TRTCVideoStreamType type, ITRTCCloud engine, bool local = false);
/// 2.RemoveEngine(ITRTCCloud engine);
/// 3.SetRenderMode(TRTCVideoFillMode mode);
/// 4.IsViewOccupy();
/// 5.SetPause(bool pause);
/// 6.RemoveAllRegEngine();
/// 
/// Main uses:
/// 1. Local image rendering:
/// Before/After opening startLocalPreview(IntPtr.Zero),
/// set the SDK rendering callback bound to the View via TXLiteAVVideoView.RegEngine(localUserId, type, engine, true)，
/// and add the View to the parent View that you want to display.
/// 2. Remote image rendering：
/// Before/After opening startRemoteView(remoteUserId, IntPtr.Zero) or startRemoteSubStreamView(
/// remoteUserId, IntPtr.Zero), set the SDK rendering callback bound to the View via TXLiteAVVideoView.RegEngine(
/// remoteUserId, type, engine, true), and add the View to the parent View that you want to display.
/// 3. Remove screen：
/// When the user exit the room or the remote user exit using TXLiteAVVideoView. RemoveEngine (engine).
/// </summary>
/// <remarks>
/// The interface is not thread safe, please call it from the main thread.
/// 
/// Note: due to the distal mainstream and you just need to set up a setRemoteVideoRenderCallback auxiliary flow,
/// it's best to unity in the remote user check-out removed in listening,
/// Otherwise, when a first-class data call is canceled, it will cause the user's other streams to receive no data.
/// </remarks>
namespace TRTCCSharpDemo
{
    public class TXLiteAVVideoView : Panel
    {
        private bool mOccupy = false;     // if view occupied
        private bool mLocalView = false;  // if local image
        private bool mPause = false;
        private bool mFirstFrame = false;

        private string mUserId;
        private TRTCVideoStreamType mStreamType;
        private TRTCVideoFillMode mRenderMode = TRTCVideoFillMode.TRTCVideoFillMode_Fit;  // 0：fill，1：adapt

        private volatile FrameBufferInfo mArgbFrame = new FrameBufferInfo();  // frame buffer

        public TXLiteAVVideoView()
        {
            this.BorderStyle = BorderStyle.None;

            // Use double buffering to prevent flickering during the drawing process
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true); // Erasing the background is prohibited.
            SetStyle(ControlStyles.DoubleBuffer, true); // double buffering

            this.Disposed += new EventHandler(OnDispose);
        }

        private void OnDispose(object sender, EventArgs e)
        {
            // Resource cleaning
            ReleaseBuffer(mArgbFrame);
        }

        /// <summary>
        /// Set parameters for View
        /// </summary>
        /// <param name="userId">UserId of the scene aim to render. If local scene, pass an empty string.</param>
        /// <param name="type">Render type</param>
        /// <param name="engine">TRTCCloud Example, user registration video data callback.</param>
        /// <param name="local">Render local screen, SDK returns userId as ""</param>
        /// <returns>true：bind successfully，false：failed</returns>
        public bool RegEngine(string userId, TRTCVideoStreamType type, ITRTCCloud engine, bool local = false)
        {
            if (mOccupy) return false;
            mLocalView = local;
            mUserId = userId;
            mStreamType = type;
            int count = TXLiteAVVideoViewManager.GetInstance().Count;
            if (engine != null)
            {
                if (count == 0)
                {
                    engine.setLocalVideoRenderCallback(TRTCVideoPixelFormat.TRTCVideoPixelFormat_BGRA32,
                        TRTCVideoBufferType.TRTCVideoBufferType_Buffer, TXLiteAVVideoViewManager.GetInstance());
                }
                if (!mLocalView)
                {
                    engine.setRemoteVideoRenderCallback(userId, TRTCVideoPixelFormat.TRTCVideoPixelFormat_BGRA32,
                        TRTCVideoBufferType.TRTCVideoBufferType_Buffer, TXLiteAVVideoViewManager.GetInstance());
                }
            }
            if (mLocalView)
                TXLiteAVVideoViewManager.GetInstance().AddView("", type, this);
            else
                TXLiteAVVideoViewManager.GetInstance().AddView(userId, type, this);
            lock (mArgbFrame)
                ReleaseBuffer(mArgbFrame);
            mOccupy = true;
            this.Refresh();
            return true;
        }

        /// <summary>
        /// Remove parameters of View
        /// </summary>
        /// <param name="engine">TRTCCloud Example, user registration video data callback.</param>
        public void RemoveEngine(ITRTCCloud engine)
        {
            if (mLocalView)
                TXLiteAVVideoViewManager.GetInstance().RemoveView("", mStreamType, this);
            else
                TXLiteAVVideoViewManager.GetInstance().RemoveView(mUserId, mStreamType, this);
            int count = TXLiteAVVideoViewManager.GetInstance().Count;
            if (engine != null)
            {
                if (count == 0)
                {
                    engine.setLocalVideoRenderCallback(TRTCVideoPixelFormat.TRTCVideoPixelFormat_Unknown,
                        TRTCVideoBufferType.TRTCVideoBufferType_Unknown, null);
                }
                if (!mLocalView && !TXLiteAVVideoViewManager.GetInstance().HasUserId(mUserId))
                {
                    engine.setRemoteVideoRenderCallback(mUserId, TRTCVideoPixelFormat.TRTCVideoPixelFormat_Unknown,
                        TRTCVideoBufferType.TRTCVideoBufferType_Unknown, null);
                }
            }
            lock (mArgbFrame)
                ReleaseBuffer(mArgbFrame);
            mUserId = "";
            mOccupy = false;
            mLocalView = false;
            mFirstFrame = false;
            mRenderMode = TRTCVideoFillMode.TRTCVideoFillMode_Fit;
            this.Refresh();
        }

        /// <summary>
        /// Set the rendering mode of the View
        /// </summary>
        /// <param name="mode">Render mode</param>
        public void SetRenderMode(TRTCVideoFillMode mode)
        {
            mRenderMode = mode;
        }

        /// <summary>
        /// If View occupied
        /// </summary>
        /// <returns>true：Current View occupied，false：current View unoccupied</returns>
        public bool IsViewOccupy()
        {
            return mOccupy;
        }

        /// <summary>
        /// Pause rendering to display the default image
        /// </summary>
        /// <param name="pause">if paused</param>
        public void SetPause(bool pause)
        {
            if (mPause != pause)
            {
                mPause = pause;
                if (mPause)
                {
                    this.BackColor = Color.FromArgb(0xFF, 0x20, 0x20, 0x20);
                }
                else
                {
                    this.BackColor = Color.FromArgb(0xFF, 0x00, 0x00, 0x00);
                    // Avoid refreshing the last frame of data
                    lock (mArgbFrame)
                        ReleaseBuffer(mArgbFrame);
                }
                this.Refresh();
            }
        }

        /// <summary>
        /// Clear all mapping information
        /// </summary>
        public static void RemoveAllRegEngine()
        {
            TXLiteAVVideoViewManager.GetInstance().RemoveAllView();
        }

        public bool AppendVideoFrame(byte[] data, int width, int height, TRTCVideoPixelFormat videoFormat, TRTCVideoRotation rotation)
        {
            if (!mFirstFrame)
                mFirstFrame = true;
            if (mPause)
                return false;
            if (data == null || data.Length <= 0)
                return false;
            // data data error
            if (videoFormat == TRTCVideoPixelFormat.TRTCVideoPixelFormat_BGRA32 && width * height * 4 != data.Length)
                return false;
            // Other YUV types are not supported
            if (videoFormat == TRTCVideoPixelFormat.TRTCVideoPixelFormat_I420 && width * height * 3 / 2 != data.Length)
                return false;

            // Only the data type of BGRA32 is implemented, if you need other types, please rewrite and set the data type of the callback
            if (videoFormat == TRTCVideoPixelFormat.TRTCVideoPixelFormat_BGRA32)
            {
                lock (mArgbFrame)
                {
                    if (mArgbFrame.data == null || mArgbFrame.width != width || mArgbFrame.height != height)
                    {
                        ReleaseBuffer(mArgbFrame);
                        mArgbFrame.width = width;
                        mArgbFrame.height = height;
                        mArgbFrame.data = new byte[data.Length];
                    }
                    Buffer.BlockCopy(data, 0, mArgbFrame.data, 0, (int)data.Length);
                    mArgbFrame.newFrame = true;
                    mArgbFrame.rotation = rotation;
                }
            }

            // Return to the main thread to refresh the current screen
            this.InvokeOnUiThreadIfRequired(new Action(() =>
            {
                this.Refresh();
            }));
            return true;
        }

        private void InvokeOnUiThreadIfRequired(Action action)
        {
            try
            {
                if (!this.IsDisposed)
                {
                    if (this.InvokeRequired)
                    {
                        this.Invoke(action);
                    }
                    else
                    {
                        action.Invoke();
                    }
                }
                else
                    System.Threading.Thread.CurrentThread.Abort();
            }
            catch (Exception ex)
            {
                Log.E(ex.Message);
            }
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            bool needDrawFrame = true;
            if (mPause)
                needDrawFrame = false;
            if (mArgbFrame.data == null)
                needDrawFrame = false;
            if (!needDrawFrame)
            {
                return;
            }

            lock (mArgbFrame)
            {
                if (mArgbFrame.data == null)
                    return;
                if (mRenderMode == TRTCVideoFillMode.TRTCVideoFillMode_Fill)
                {
                    RenderFillMode(pe, mArgbFrame.data, mArgbFrame.width, mArgbFrame.height, (int)mArgbFrame.rotation * 90);
                }
                else if (mRenderMode == TRTCVideoFillMode.TRTCVideoFillMode_Fit)
                {
                    RenderFitMode(pe, mArgbFrame.data, mArgbFrame.width, mArgbFrame.height, (int)mArgbFrame.rotation * 90);
                }
            }
        }

        private void RenderFillMode(PaintEventArgs pe, byte[] data, int width, int height, int rotation)
        {
            Graphics graphics = pe.Graphics;
            // Set the background to all black
            graphics.Clear(Color.Black);

            Bitmap bmp = new Bitmap(width, height, PixelFormat.Format32bppPArgb);
            BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.WriteOnly, PixelFormat.Format32bppPArgb);

            // Get image parameters
            int stride = bmpData.Stride;      // The width of the scan line
            IntPtr iptr = bmpData.Scan0;      // Get the memory start location of bmpData
            int scanBytes = stride * height;  // Stride width, which is the size of the memory area

            // Using Marshal's Copy method, copy the memory byte array you just got into BitmapData
            Marshal.Copy(data, 0, iptr, scanBytes);
            bmp.UnlockBits(bmpData);

            if (rotation > 0)
                bmp = Rotate(bmp, rotation);

            // Fill full screen
            int viewWidth = this.ClientSize.Width;
            int viewHeight = this.ClientSize.Height;
            AdjustSize(viewWidth, viewHeight, bmp.Width, bmp.Height, out width, out height);
            if (width == viewWidth)
                height = viewHeight;
            else
                width = viewWidth;

            int x = (viewWidth - width) / 2;
            int y = (viewHeight - height) / 2;
            Rectangle rect = new Rectangle(x, y, width, height);

            graphics.DrawImage(bmp, rect);
            bmp.Dispose();
        }

        private void RenderFitMode(PaintEventArgs pe, byte[] data, int width, int height, int rotation)
        {
            Graphics graphics = pe.Graphics;
            // Set the background to all black
            graphics.Clear(Color.Black);

            Bitmap bmp = new Bitmap(width, height, PixelFormat.Format32bppPArgb);
            BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.WriteOnly, PixelFormat.Format32bppPArgb);

            // Get image parameters
            int stride = bmpData.Stride;      // The width of the scan line
            IntPtr iptr = bmpData.Scan0;      // Gets the memory start location of bmpData
            int scanBytes = stride * height;  // stride width, which is the size of the memory area

            // Using Marshal's Copy method, copy the memory byte array you just got into BitmapData
            Marshal.Copy(data, 0, iptr, scanBytes);
            bmp.UnlockBits(bmpData);

            if (rotation > 0)
                bmp = Rotate(bmp, rotation);

            // Gets the size of the scaled rectangle
            int viewWidth = this.ClientSize.Width;
            int viewHeight = this.ClientSize.Height;
            AdjustSize(viewWidth, viewHeight, bmp.Width, bmp.Height, out width, out height);
            int x = (viewWidth - width) / 2;
            int y = (viewHeight - height) / 2;
            Rectangle rect = new Rectangle(x, y, width, height);

            graphics.DrawImage(bmp, rect);
            bmp.Dispose();
        }

        private void ReleaseBuffer(FrameBufferInfo info)
        {
            if (info.data != null)
                info.data = null;
            info.width = 0;
            info.height = 0;
            info.newFrame = false;
            info.rotation = TRTCVideoRotation.TRTCVideoRotation0;
        }

        private Bitmap Rotate(Bitmap b, int angle)
        {
            if (angle == 0) return b;
            angle = angle % 360;
            // Radian conversion
            double radian = angle * Math.PI / 180.0;
            double cos = Math.Cos(radian);
            double sin = Math.Sin(radian);
            // Width and height of the original drawing
            int w = b.Width;
            int h = b.Height;
            int W = (int)(Math.Max(Math.Abs(w * cos - h * sin), Math.Abs(w * cos + h * sin)));
            int H = (int)(Math.Max(Math.Abs(w * sin - h * cos), Math.Abs(w * sin + h * cos)));
            // Target bitmap
            Bitmap dsImage = new Bitmap(W, H);
            System.Drawing.Graphics g = System.Drawing.Graphics.FromImage(dsImage);
            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Bilinear;
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            // Calculated offset
            Point Offset = new Point((W - w) / 2, (H - h) / 2);
            // Construct the image display area: Align the center of the image with the center point of the window
            Rectangle rect = new Rectangle(Offset.X, Offset.Y, w, h);
            Point center = new Point(rect.X + rect.Width / 2, rect.Y + rect.Height / 2);
            g.TranslateTransform(center.X, center.Y);
            g.RotateTransform(360 - angle);
            // Restore images in horizontal and vertical directions by panning
            g.TranslateTransform(-center.X, -center.Y);
            g.DrawImage(b, rect);
            // All transformations retraced to the plot
            g.ResetTransform();
            g.Save();
            b.Dispose();
            g.Dispose();
            return dsImage;
        }

        private void AdjustSize(int spcWidth, int spcHeight, int orgWidth, int orgHeight, out int width, out int height)
        {
            if (orgWidth <= spcWidth && orgHeight <= spcHeight)
            {
                // Ratio of acquisition
                float w = spcWidth / (float)orgWidth;
                float h = spcHeight / (float)orgHeight;
                if (w > h)
                {
                    height = spcHeight;
                    width = (int)(Math.Round(orgWidth * h));
                }
                else if (w < h)
                {
                    width = spcWidth;
                    height = (int)(Math.Round(orgHeight * w));
                }
                else
                {
                    width = spcWidth;
                    height = spcWidth;
                }
            }
            else
            {
                // Ratio of acquisition
                float w = orgWidth / (float)spcWidth;
                float h = orgHeight / (float)spcHeight;
                // The width ratio is greater than the height ratio
                if (w > h)
                {
                    width = spcWidth;
                    height = (int)(w >= 1 ? Math.Round(orgHeight / w) : Math.Round(orgHeight * w));
                }
                // The width ratio is less than the height ratio
                else if (w < h)
                {
                    height = spcHeight;
                    width = (int)(h >= 1 ? Math.Round(orgWidth / h) : Math.Round(orgWidth * h));
                }
                // Width ratio is equal to height ratio
                else
                {
                    width = spcWidth;
                    height = spcHeight;
                }
            }
        }
    }

    class TXLiteAVVideoViewManager : ITRTCVideoRenderCallback
    {
        private volatile Dictionary<string, TXLiteAVVideoView> mMapViews;

        public static TXLiteAVVideoViewManager sInstance;

        private static Object mLocker = new Object();

        public static TXLiteAVVideoViewManager GetInstance()
        {
            if (sInstance == null)
            {
                lock (mLocker)
                {
                    if (sInstance == null)
                        sInstance = new TXLiteAVVideoViewManager();
                }
            }
            return sInstance;
        }

        private TXLiteAVVideoViewManager()
        {
            mMapViews = new Dictionary<string, TXLiteAVVideoView>();
        }

        private string GetKey(string userId, TRTCVideoStreamType type)
        {
            return String.Format("{0}_{1}", userId, type);
        }

        // Mainly used to determine whether the current user still has a stream screen,
        // and do not remove the listener if it does.
        public bool HasUserId(string userId)
        {
            bool exit = false;
            lock (mMapViews)
            {
                exit = mMapViews.ContainsKey(GetKey(userId, TRTCVideoStreamType.TRTCVideoStreamTypeBig)) ||
                    mMapViews.ContainsKey(GetKey(userId, TRTCVideoStreamType.TRTCVideoStreamTypeSub));
            }
            return exit;
        }

        public void AddView(string userId, TRTCVideoStreamType type, TXLiteAVVideoView view)
        {
            lock (mMapViews)
            {
                bool find = false;
                foreach (var item in mMapViews)
                {
                    if (item.Key.Equals(GetKey(userId, type)))
                    {
                        find = true;
                        break;
                    }
                }
                if (!find)
                {
                    mMapViews.Add(GetKey(userId, type), view);
                }
            }
        }

        public void RemoveView(string userId, TRTCVideoStreamType type, TXLiteAVVideoView view)
        {
            lock (mMapViews)
            {
                foreach (var item in mMapViews.ToList())
                {
                    if (item.Key.Equals(GetKey(userId, type)))
                    {
                        if (item.Value != null)
                        {
                            item.Value.Dispose();
                        }
                        mMapViews.Remove(item.Key);
                        break;
                    }
                }
            }
        }

        public void RemoveAllView()
        {
            lock (mMapViews)
                mMapViews.Clear();
        }

        public int Count
        {
            get
            {
                lock (mMapViews)
                    return mMapViews.Count;
            }
        }

        public void onRenderVideoFrame(string userId, TRTCVideoStreamType streamType, TRTCVideoFrame frame)
        {
            // The size of the video is occupied by one video bit, and the underlying support for dynamic switching.
            if (streamType == TRTCVideoStreamType.TRTCVideoStreamTypeSmall)
                streamType = TRTCVideoStreamType.TRTCVideoStreamTypeBig;
            TXLiteAVVideoView view = null;
            lock (mMapViews)
            {
                foreach (var item in mMapViews)
                {
                    if (item.Key.Equals(GetKey(userId, streamType)) && item.Value != null)
                    {
                        view = item.Value;
                        break;
                    }
                }
            }

            if (view != null)
            {
                view.AppendVideoFrame(frame.data, (int)frame.width, (int)frame.height, frame.videoFormat, frame.rotation);
            }
        }
    }

    class FrameBufferInfo
    {
        public byte[] data { get; set; }

        public int width { get; set; }

        public int height { get; set; }

        public bool newFrame { get; set; }

        public TRTCVideoRotation rotation { get; set; }

        public FrameBufferInfo()
        {
            rotation = TRTCVideoRotation.TRTCVideoRotation0;
            newFrame = false;
            width = 0;
            height = 0;
            data = null;
        }
    }
}
