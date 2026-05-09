using System;
using System.Runtime.InteropServices;
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
using UnityEngine;
using UnityEngine.UI;
#else
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
#endif

namespace trtc {
  public enum VideoRenderType {
    None = 0,
    /** The renderer for rendering Raw Image of the UI components. */
    RawImage = 1,
    /** The renderer for rendering 3D GameObject, such as Cube、Cylinder and Plane.*/
    Renderer = 2,
  }
  ;

#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
  public class TRTCVideoRender : MonoBehaviour, ITRTCVideoRenderCallback {
    private string _userId = "";
    private TRTCVideoStreamType _streamType = TRTCVideoStreamType.TRTCVideoStreamTypeBig;
    private bool _enable = true;

    private VideoRenderType _videoRenderType = VideoRenderType.None;
    private RawImage _rawImage = null;
    private Renderer _renderer = null;
    private Texture2D _nativeTexture = null;
    private TRTCRenderParams _renderParams;
    private bool _needUpdateLayout = false;

    private uint _textureWidth = 0;
    private uint _textureHeight = 0;
    private TextureFormat _textureFormat = TextureFormat.RGBA32;
    private TRTCVideoFrame _videoFrame;
    private UnityEngine.Object _videoFrameLock = new UnityEngine.Object();
    private TRTCVideoBufferType _videoBufferType = TRTCVideoBufferType.TRTCVideoBufferType_Buffer;
    private TRTCVideoPixelFormat _videoFormat = TRTCVideoPixelFormat.TRTCVideoPixelFormat_RGBA32;
#else
    public class TRTCVideoRender : ITRTCVideoRenderCallback {
    private string _userId = "";
    private TRTCVideoStreamType _streamType = TRTCVideoStreamType.TRTCVideoStreamTypeBig;
    private bool _enable = true;

    private VideoRenderType _videoRenderType = VideoRenderType.None;
    private WriteableBitmap _writeableBitmap = null;
    private Dispatcher _dispatcher = null;
    private System.Windows.Controls.Image _imageControl = null;  // Image 控件引用

    private TRTCRenderParams _renderParams;
    private bool _needUpdateLayout = false;

    private uint _textureWidth = 0;
    private uint _textureHeight = 0;
    private PixelFormat _pixelFormat = PixelFormats.Bgra32;
    private TRTCVideoFrame _videoFrame;
    private readonly object _videoFrameLock = new object();
    private TRTCVideoBufferType _videoBufferType = TRTCVideoBufferType.TRTCVideoBufferType_Buffer;
    private TRTCVideoPixelFormat _videoFormat = TRTCVideoPixelFormat.TRTCVideoPixelFormat_BGRA32;
    private TransformGroup _transformGroup = null;
    private ScaleTransform _scaleTransform = null;
    private RotateTransform _rotateTransform = null;
#endif
    bool _frameUpdated = false;
    public void SetEnable(bool enable) { _enable = enable; }

    public void Awake() {
    }
    public TRTCVideoRender() {
      _renderParams.fillMode = TRTCVideoFillMode.TRTCVideoFillMode_Fit;
    }

    public void SetForUser(string userID, TRTCVideoStreamType streamType) {
      _userId = userID;
      _streamType = streamType;
      TRTCLogger.Info($"SetForUser useID= {_userId}, streamType= {_streamType}");
      TryRegisterCallback();
    }

    public void setRenderParams(TRTCRenderParams renderParams) {
      bool paramsChanged = renderParams.fillMode != _renderParams.fillMode
         || renderParams.mirrorType != _renderParams.mirrorType
         || renderParams.rotation != _renderParams.rotation;
      _renderParams = renderParams;

#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
      if (paramsChanged) {
        _needUpdateLayout = true;
      }
#else
      if (paramsChanged && _imageControl != null && _dispatcher != null) {
        if (_dispatcher.CheckAccess()) {
          ApplyRenderParams();
        } else {
          _dispatcher.Invoke(() => {
            ApplyRenderParams();
          }, DispatcherPriority.Render);
        }
      }
#endif
    }

    private void TryRegisterCallback() {
      ITRTCCloud trtcCloud = ITRTCCloud.getTRTCShareInstance();
      if (trtcCloud == null)
        return;

      if (_userId.Length == 0) {
        trtcCloud.setLocalVideoRenderCallback(_streamType, _videoFormat, _videoBufferType, this);
      }
      else {
        trtcCloud.setRemoteVideoRenderCallback(_userId, _streamType, _videoFormat,
                                               _videoBufferType, this);
      }
    }

#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
    void Start() {
      _rawImage = GetComponent<RawImage>();
      if (_rawImage != null) {
        _videoRenderType = VideoRenderType.RawImage;
      }
      else {
        _renderer = GetComponent<Renderer>();
        if (_renderer != null) {
          _videoRenderType = VideoRenderType.Renderer;
        }
      }
    }

    void Update() {
      if (_videoRenderType == VideoRenderType.None)
        return;

      if (!_enable || !_frameUpdated)
        return;

      TRTCVideoFrame videoFrame;
      lock (_videoFrameLock) {
        videoFrame = _videoFrame;
      }

      lock (this) {
        TextureFormat newFormat = TRTCVideoFormatToTextureFormat(videoFrame.videoFormat);
        if (_nativeTexture == null || _textureFormat != newFormat) {
          try {
            _nativeTexture = new Texture2D((int)videoFrame.width, (int)videoFrame.height, newFormat, false);
            _nativeTexture.filterMode = FilterMode.Trilinear;
            _textureFormat = newFormat;
            _textureWidth = videoFrame.width;
            _textureHeight = videoFrame.height;
            _needUpdateLayout = true;

            if (_videoRenderType == VideoRenderType.RawImage && _rawImage != null) {
              _rawImage.texture = _nativeTexture;
            }
            else if (_videoRenderType == VideoRenderType.Renderer && _renderer != null) {
              _renderer.material.mainTexture = _nativeTexture;
            }
          }
          catch (Exception exception) {
            TRTCLogger.Error("VideoRenderCreate Exception: " + exception);
          }
        }
        if (_textureWidth != videoFrame.width || _textureHeight != videoFrame.height) {
          try {
#if UNITY_2021_2_OR_NEWER
            _nativeTexture.Reinitialize((int)videoFrame.width, (int)videoFrame.height);
#else
            _nativeTexture.Resize((int)videoFrame.width, (int)videoFrame.height);
#endif
            _textureWidth = videoFrame.width;
            _textureHeight = videoFrame.height;
            _needUpdateLayout = true;
          }
          catch (Exception exception) {
            TRTCLogger.Error("VideoRenderResize Exception: " + exception);
          }
        }

        if (_needUpdateLayout) {
          if (_textureWidth > 0 && _textureHeight > 0 &&
              _videoRenderType == VideoRenderType.RawImage) {
            RectTransform rectTransform = gameObject.GetComponent<RectTransform>();

            float localRatio = rectTransform.rect.width / rectTransform.rect.height;
            float videoRatio = (float)_textureWidth / (float)_textureHeight;

            float localScaleX = 1.0f;
            float localScaleY = 1.0f;
            if (_renderParams.fillMode == TRTCVideoFillMode.TRTCVideoFillMode_Fit) {
              if (localRatio > videoRatio) {
                localScaleX = videoRatio / localRatio;
                localScaleY = 1.0f;
              }
              else {
                localScaleX = 1.0f;
                localScaleY = localRatio / videoRatio;
              }
            }
            else {
              if (localRatio > videoRatio) {
                localScaleX = 1.0f;
                localScaleY = localRatio / videoRatio;
              }
              else {
                localScaleX = videoRatio / localRatio;
                localScaleY = 1.0f;
              }
            }

            if (_renderParams.mirrorType == TRTCVideoMirrorType.TRTCVideoMirrorType_Enable) {
              rectTransform.localScale = new Vector3(-localScaleX, -localScaleY, 1);
            }
            else {
              rectTransform.localScale = new Vector3(localScaleX, -localScaleY, 1);
            }

            rectTransform.localEulerAngles = new Vector3(0, 0, 360 - ((int)_renderParams.rotation) * 90);
            _needUpdateLayout = false;
          }
        }

        if (_nativeTexture) {
          try {
            _nativeTexture.LoadRawTextureData(videoFrame.data, (int)videoFrame.length);
            _nativeTexture.Apply();
            _frameUpdated = false;
          }
          catch (Exception exception) {
            TRTCLogger.Error("VideoRenderLoad Exception: " + exception);
          }
        }
      }
    }
#else
    public void Initialize(Dispatcher dispatcher, System.Windows.Controls.Image imageControl = null) {
      _imageControl = imageControl;
      _dispatcher = dispatcher;
      _videoRenderType = VideoRenderType.RawImage; 
      _needUpdateLayout = true;
    }

    public bool UpdateFrame() {
      if (!_enable)
        return false;

      TRTCVideoFrame videoFrame;

      bool hasFrame = false;

      lock (_videoFrameLock) {
        if (_frameUpdated) {
          videoFrame = _videoFrame;
          _frameUpdated = false;
          hasFrame = true;
        } else {
          videoFrame = new TRTCVideoFrame();
        }
      }

      if (!hasFrame)
        return false;

      if (videoFrame.data == IntPtr.Zero || videoFrame.width == 0 || videoFrame.height == 0)
        return false;

      if (_writeableBitmap == null ||
          _textureWidth != videoFrame.width ||
          _textureHeight != videoFrame.height) {
        lock (this) {
          if (_writeableBitmap == null ||
              _textureWidth != videoFrame.width ||
              _textureHeight != videoFrame.height) {
            
            if (_writeableBitmap != null) {
              var oldBitmap = _writeableBitmap;
              _writeableBitmap = null;
              if (_imageControl != null && _dispatcher != null) {
                _dispatcher.BeginInvoke(() => {
                  _imageControl.Source = null;
                }, DispatcherPriority.Normal);
              }
              
              oldBitmap = null;
            }

            _textureWidth = videoFrame.width;
            _textureHeight = videoFrame.height;
            _writeableBitmap = new WriteableBitmap(
              (int)_textureWidth,
              (int)_textureHeight,
              96, 96,
              _pixelFormat,
              null);
              
            if (_imageControl != null && _dispatcher != null) {
              _dispatcher.BeginInvoke(() => {
                _imageControl.Source = _writeableBitmap;
              }, DispatcherPriority.Render);
            }
          }
        }
      }
       if (_needUpdateLayout && _imageControl != null && _dispatcher != null) {
        if (_dispatcher.CheckAccess()) {
          ApplyRenderParams();
        } else {
          _dispatcher.Invoke(() => {
            ApplyRenderParams();
          }, DispatcherPriority.Render);
        }
        _needUpdateLayout = false;
      }

      if (_writeableBitmap != null) {
        try {
          int width = (int)videoFrame.width;
          int height = (int)videoFrame.height;
          int bytesPerPixel = (_pixelFormat.BitsPerPixel + 7) / 8;
          int stride = width * bytesPerPixel;
          _writeableBitmap.WritePixels(
            new Int32Rect(0, 0, width, height),
            videoFrame.data,
            stride * height,
            stride
          );
          return true;
        } catch (Exception) {
          return false;
        }
      }
      return false;
    }

    private void ApplyRenderParams() {
      if (_imageControl == null)
        return;

      if (_renderParams.fillMode == TRTCVideoFillMode.TRTCVideoFillMode_Fill) {
        _imageControl.Stretch = Stretch.UniformToFill;
      } else {
        _imageControl.Stretch = Stretch.Uniform;
      }

      _imageControl.HorizontalAlignment = HorizontalAlignment.Center;
      _imageControl.VerticalAlignment = VerticalAlignment.Center;

      if (_transformGroup == null) {
        _transformGroup = new TransformGroup();
      } else {
        _transformGroup.Children.Clear();
      }

      if (_renderParams.mirrorType == TRTCVideoMirrorType.TRTCVideoMirrorType_Enable) {
        if (_scaleTransform == null) {
          _scaleTransform = new ScaleTransform(-1, 1);
        } else {
          _scaleTransform.ScaleX = -1;
          _scaleTransform.ScaleY = 1;
        }
        _transformGroup.Children.Add(_scaleTransform);
      }

      double rotationAngle = 0;
      switch (_renderParams.rotation) {
        case TRTCVideoRotation.TRTCVideoRotation0:
          rotationAngle = 0;
          break;
        case TRTCVideoRotation.TRTCVideoRotation90:
          rotationAngle = 90;
          break;
        case TRTCVideoRotation.TRTCVideoRotation180:
          rotationAngle = 180;
          break;
        case TRTCVideoRotation.TRTCVideoRotation270:
          rotationAngle = 270;
          break;
      }

      if (rotationAngle != 0) {
        if (_rotateTransform == null) {
          _rotateTransform = new RotateTransform(rotationAngle);
        } else {
          _rotateTransform.Angle = rotationAngle;
        }
        _transformGroup.Children.Add(_rotateTransform);
      }

      if (_transformGroup.Children.Count > 0) {
        _imageControl.RenderTransform = _transformGroup;
        _imageControl.RenderTransformOrigin = new Point(0.5, 0.5);
      } else {
        _imageControl.RenderTransform = null;
      }
    }
#endif

#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
    void OnDestroy() {
#else
    public void Dispose() {
#endif
      TRTCLogger.Info("Render --- OnDestroy");
      ITRTCCloud trtcCloud = TRTCCloudImplement.queryTRTCShareInstance();
      if (trtcCloud == null)
        return;

      if (_userId != null) {
        if (_userId.Length == 0) {
          trtcCloud.setLocalVideoRenderCallback(
              _streamType, _videoFormat, TRTCVideoBufferType.TRTCVideoBufferType_Buffer, null);
        }
        else {
          trtcCloud.setRemoteVideoRenderCallback(_userId, _streamType, _videoFormat,
                                                 TRTCVideoBufferType.TRTCVideoBufferType_Buffer,
                                                 null);
        }
      }

      Clear();
    }

    public void Clear() {
      lock (_videoFrameLock) {
        if (_videoFrame.data != IntPtr.Zero) {
          Marshal.FreeHGlobal(_videoFrame.data);
        }
        _videoFrame = new TRTCVideoFrame();
      }
      lock (this) {
        _textureWidth = 0;
        _textureHeight = 0;
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL

        _nativeTexture = null;
        if (_videoRenderType == VideoRenderType.RawImage && _rawImage != null) {
          _rawImage.texture = null;
        }
        else if (_videoRenderType == VideoRenderType.Renderer && _renderer != null) {
          _renderer.material.mainTexture = null;
        }
#else
        if (_writeableBitmap != null) {
          _writeableBitmap = null;
        }
        _transformGroup = null;
        _scaleTransform = null;
        _rotateTransform = null;
#endif
      }
    }

    public void onRenderVideoFrame(string userId,
                                   TRTCVideoStreamType streamType,
                                   TRTCVideoFrame frame) {
      if (_userId != userId)
        return;

      if (_streamType != streamType)
        return;

      lock (_videoFrameLock) {
        var data = _videoFrame.data;
        if (_videoFrame.length != frame.length) {
          if (_videoFrame.data != IntPtr.Zero) {
            Marshal.FreeHGlobal(_videoFrame.data);
          }
          data = Marshal.AllocHGlobal((int)frame.length);
        }
        _videoFrame = frame;
        _videoFrame.data = data;
        TRTCCloudNative.trtc_cloud_copy_native_memery(_videoFrame.data, frame.data, (int)frame.length);
        _frameUpdated = true;
      }
#if !(UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL)
      if (_dispatcher != null) {
        _dispatcher.BeginInvoke(() => {
          UpdateFrame();
        }, DispatcherPriority.Render);
      }
#endif
    }

#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
    private TextureFormat TRTCVideoFormatToTextureFormat(TRTCVideoPixelFormat format) {
      switch (format) {
        case TRTCVideoPixelFormat.TRTCVideoPixelFormat_BGRA32:
          return TextureFormat.BGRA32;
        case TRTCVideoPixelFormat.TRTCVideoPixelFormat_RGBA32:
          return TextureFormat.RGBA32;
        default:
          TRTCLogger.Error("Invalid video format.");
          return TextureFormat.BGRA32;
      }
    }
#endif
  }
}
