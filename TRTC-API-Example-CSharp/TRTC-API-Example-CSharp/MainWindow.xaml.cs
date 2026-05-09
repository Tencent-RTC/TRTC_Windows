using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Threading;
using System.IO;
using trtc;

namespace TRTCWPFDemo {
  /// <summary>
  /// TRTC WPF Demo 主窗口
  /// </summary>
  public partial class MainWindow : Window {
    private ITRTCCloud? mTRTCCloud;
    private TRTCCallback? mCallback;
    private bool mIsInRoom = false;
    private bool mIsAudioMuted = true;
    private bool mIsLocalMirror = false;
    private bool mIsVideoMuted = false;
    private bool mIsLocalPreviewStarted = false;

    // 远端状态
    private System.Collections.Generic.Dictionary<string, bool> mRemoteVideoStates = new System.Collections.Generic.Dictionary<string, bool>();
    private System.Collections.Generic.Dictionary<string, bool> mRemoteAudioMutes = new System.Collections.Generic.Dictionary<string, bool>();

    private TRTCVideoFillMode mLocalFillMode = TRTCVideoFillMode.TRTCVideoFillMode_Fit;
    private TRTCVideoRotation mCurrentRotation = TRTCVideoRotation.TRTCVideoRotation0;


    // 远端视频管理
    private System.Collections.Generic.Dictionary<string, System.Windows.Controls.Image> mRemoteVideoImages = new System.Collections.Generic.Dictionary<string, System.Windows.Controls.Image>();
    private System.Collections.Generic.Dictionary<string, System.Windows.UIElement> mRemoteVideoContainers = new System.Collections.Generic.Dictionary<string, System.Windows.UIElement>();

    // 远端视频渲染参数状态
    private System.Collections.Generic.Dictionary<string, TRTCVideoFillMode> mRemoteFillModes = new System.Collections.Generic.Dictionary<string, TRTCVideoFillMode>();
    private System.Collections.Generic.Dictionary<string, bool> mRemoteMirrors = new System.Collections.Generic.Dictionary<string, bool>();

    // 截图功能相关变量
    private string mSnapshotFolderPath = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Snapshots");
    private System.Collections.Generic.Dictionary<string, System.Windows.Controls.Image> mSnapshotImages = new System.Collections.Generic.Dictionary<string, System.Windows.Controls.Image>();
    
    public MainWindow() {
      InitializeComponent();
      InitializeData();
      InitializeTRTC();
      AddLog("TRTC WPF Demo 初始化完成（TRTCVideoRender 自动渲染模式）");
    }

    private void InitializeData() {
      // 初始化 SDKAppId
      foreach (var app in GenerateTestUserSig.APPS) {
        cmbSdkAppId.Items.Add(app.SdkAppId);
      }
      if (cmbSdkAppId.Items.Count > 0) cmbSdkAppId.SelectedIndex = 0;

      // 初始化场景

      cmbAppScene.Items.Add(TRTCAppScene.TRTCAppSceneVideoCall);
      cmbAppScene.Items.Add(TRTCAppScene.TRTCAppSceneLIVE);
      cmbAppScene.Items.Add(TRTCAppScene.TRTCAppSceneAudioCall);
      cmbAppScene.Items.Add(TRTCAppScene.TRTCAppSceneVoiceChatRoom);
      cmbAppScene.SelectedIndex = 0;

      // 初始化角色
      cmbRole.Items.Add(TRTCRoleType.TRTCRoleAnchor);
      cmbRole.Items.Add(TRTCRoleType.TRTCRoleAudience);
      cmbRole.SelectedIndex = 0;

      // 初始状态下，VideoCall 场景没有角色选择
      cmbRole.IsEnabled = false;
      // 初始化截图功能下拉框
       InitializeSnapshotControls();
    }

    /// <summary>
    /// 初始化截图功能控件
    /// </summary>
    private void InitializeSnapshotControls() {
      // 初始化视频流类型选择框
      cmbVideoStreamType.Items.Add("主路视频流(Big)");
      cmbVideoStreamType.Items.Add("小流视频流(Small)");
      cmbVideoStreamType.Items.Add("辅路视频流(Sub)");
      cmbVideoStreamType.SelectedIndex = 0;

      // 初始化截图源类型选择框
      cmbSnapshotSourceType.Items.Add("视频流截图(Stream)");
      cmbSnapshotSourceType.Items.Add("视频视图截图(View)");
      cmbSnapshotSourceType.SelectedIndex = 0;
      // 创建截图文件夹
      if (!System.IO.Directory.Exists(mSnapshotFolderPath)) {
        System.IO.Directory.CreateDirectory(mSnapshotFolderPath);
      }
    }

    private void CmbAppScene_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e) {
      if (cmbAppScene.SelectedItem is TRTCAppScene scene) {
        if (scene == TRTCAppScene.TRTCAppSceneLIVE || scene == TRTCAppScene.TRTCAppSceneVoiceChatRoom) {
          cmbRole.IsEnabled = true;
        }
        else {
          cmbRole.IsEnabled = false;
          cmbRole.SelectedItem = TRTCRoleType.TRTCRoleAnchor;
        }
      }
    }

    /// <summary>
    /// 初始化 TRTC SDK
    /// </summary>

    private void InitializeTRTC() {
      try {
        // 获取 TRTC 实例
        mTRTCCloud = ITRTCCloud.getTRTCShareInstance();

        // 创建回调对象
        mCallback = new TRTCCallback(this);

        // 设置回调
        mTRTCCloud.addCallback(mCallback);

        // 设置日志级别
        //mTRTCCloud.setLogLevel(TRTCLogLevel.TRTCLogLevelInfo);

        AddLog("TRTC SDK 初始化成功");
        AddLog($"SDK 版本: {mTRTCCloud.getSDKVersion()}");
      }
      catch (Exception ex) {
        AddLog($"初始化失败: {ex.Message}");
        MessageBox.Show($"初始化 TRTC SDK 失败: {ex.Message}", "错误", MessageBoxButton.OK, MessageBoxImage.Error);
      }
    }

    /// <summary>
    /// 进入房间
    /// </summary>
    private void BtnEnterRoom_Click(object sender, RoutedEventArgs e) {
      try {
        if (mTRTCCloud == null) {
          AddLog("错误: TRTC 实例未初始化");
          return;
        }

        // 验证输入
        if (cmbSdkAppId.SelectedItem == null ||
            string.IsNullOrWhiteSpace(txtRoomId.Text) ||
            string.IsNullOrWhiteSpace(txtUserId.Text)) {
          MessageBox.Show("请填写完整的房间信息", "提示", MessageBoxButton.OK, MessageBoxImage.Warning);
          return;
        }

        int sdkAppId = (int)cmbSdkAppId.SelectedItem;
        uint roomId = uint.Parse(txtRoomId.Text);
        string userId = txtUserId.Text;

        // 计算 UserSig
        string userSig = GenerateTestUserSig.GetInstance().GenTestUserSig(sdkAppId, userId);
        if (string.IsNullOrEmpty(userSig)) {
          MessageBox.Show("生成 UserSig 失败，请检查配置", "错误", MessageBoxButton.OK, MessageBoxImage.Error);
          return;
        }

        // 获取场景和角色
        TRTCAppScene scene = (TRTCAppScene)cmbAppScene.SelectedItem;
        TRTCRoleType role = (TRTCRoleType)cmbRole.SelectedItem;

        // 配置进房参数
        TRTCParams trtcParams = new TRTCParams {
          sdkAppId = (uint)sdkAppId,
          roomId = roomId,
          userId = userId,
          userSig = userSig,
          role = role
        };

        AddLog($"开始进入房间 - AppId: {sdkAppId}, RoomId: {roomId}, UserId: {userId}, Scene: {scene}, Role: {role}");

        // 进入房间
        mTRTCCloud.enterRoom(ref trtcParams, scene);

        // 更新UI状态
        btnEnterRoom.IsEnabled = false;
        txtStatus.Text = "状态: 正在进入房间...";

        // 禁用配置控件
        cmbSdkAppId.IsEnabled = false;
        cmbAppScene.IsEnabled = false;
        cmbRole.IsEnabled = false;
        txtRoomId.IsEnabled = false;
        txtUserId.IsEnabled = false;
      }
      catch (Exception ex) {
        AddLog($"进入房间失败: {ex.Message}");
        MessageBox.Show($"进入房间失败: {ex.Message}", "错误", MessageBoxButton.OK, MessageBoxImage.Error);
      }
    }


    /// <summary>
    /// 退出房间
    /// </summary>
    private void BtnExitRoom_Click(object sender, RoutedEventArgs e) {
      try {
        if (mTRTCCloud == null) return;

        AddLog("退出房间");
        mTRTCCloud.exitRoom();

        // 停止本地视频
        mTRTCCloud.stopLocalPreview();

        // 清理远端视频 UI
        panelRemoteVideo.Children.Clear();
        mRemoteVideoContainers.Clear();
        mRemoteVideoImages.Clear();
        mRemoteFillModes.Clear();
        mRemoteMirrors.Clear();
        mRemoteVideoStates.Clear();
        mRemoteAudioMutes.Clear();

        mIsInRoom = false;

        // 更新UI
        btnExitRoom.IsEnabled = false;
        btnEnterRoom.IsEnabled = true;

        // 重置本地视频控制按钮状态
        mIsLocalPreviewStarted = false;
        btnLocalVideoToggle.IsEnabled = false;
        btnLocalVideoToggle.ToolTip = "开启摄像头";
        pathLocalVideoToggle.Data = Geometry.Parse(Icons.VideoOn);
        pathLocalVideoToggle.Fill = Brushes.Transparent;


        btnLocalMirror.IsEnabled = false;
        btnLocalFillMode.IsEnabled = false;
        btnLocalVideoMute.IsEnabled = false;

        mIsAudioMuted = true;
        btnLocalAudioToggle.IsEnabled = false;
        btnLocalAudioToggle.ToolTip = "开启麦克风";
        pathLocalAudioToggle.Data = Geometry.Parse(Icons.AudioOn);

        txtStatus.Text = "状态: 已退出房间";
        imgLocalVideo.Source = null;


        // 恢复配置控件
        cmbSdkAppId.IsEnabled = true;
        cmbAppScene.IsEnabled = true;
        cmbRole.IsEnabled = true;
        txtRoomId.IsEnabled = true;
        txtUserId.IsEnabled = true;
      }
      catch (Exception ex) {
        AddLog($"退出房间失败: {ex.Message}");
      }
    }


    /// <summary>
    /// 打开设置窗口
    /// </summary>
    private void BtnSettings_Click(object sender, RoutedEventArgs e) {
      if (mTRTCCloud == null) return;
      var deviceManager = mTRTCCloud.getDeviceManager();
      SettingsWindow settingsWindow = new SettingsWindow(deviceManager);
      settingsWindow.Owner = this;
      settingsWindow.ShowDialog();
    }

    /// <summary>
    /// 切换本地视频预览
    /// </summary>
    private void BtnLocalVideoToggle_Click(object sender, RoutedEventArgs e) {
      try {
        if (mTRTCCloud == null) return;

        if (mIsLocalPreviewStarted) {
          // 关闭预览
          mTRTCCloud.stopLocalPreview();
          mIsLocalPreviewStarted = false;

          // 更新UI
          imgLocalVideo.Source = null;
          btnLocalVideoToggle.ToolTip = "开启摄像头";
          // 更改图标为“开启”状态
          pathLocalVideoToggle.Data = Geometry.Parse(Icons.VideoOff);
          pathLocalVideoToggle.Fill = Brushes.Transparent;

          // 禁用相关按钮
          btnLocalMirror.IsEnabled = false;
          btnLocalFillMode.IsEnabled = false;
          btnLocalVideoMute.IsEnabled = false;

          AddLog("关闭摄像头");
        }
        else {
          // 开启预览
          mTRTCCloud.startLocalPreview(true, imgLocalVideo);
          mIsLocalPreviewStarted = true;

          // 更新UI
          btnLocalVideoToggle.ToolTip = "关闭摄像头";
          // 更改图标为“关闭”状态
          pathLocalVideoToggle.Data = Geometry.Parse(Icons.VideoOn);
          pathLocalVideoToggle.Fill = Brushes.White;

          // 启用相关按钮
          btnLocalMirror.IsEnabled = true;
          btnLocalFillMode.IsEnabled = true;
          btnLocalVideoMute.IsEnabled = true;
          btnLocalAudioToggle.IsEnabled = true;

          AddLog("开启摄像头");
        }
      }
      catch (Exception ex) {
        AddLog($"切换摄像头失败: {ex.Message}");
      }
    }

    /// <summary>
    /// 发送自定义消息测试
    /// </summary>
    private void BtnSendCustomMsg_Click(object sender, RoutedEventArgs e) {
      try {
        if (mTRTCCloud == null) {
          AddLog("错误: TRTC 实例未初始化");
          return;
        }

        if (string.IsNullOrWhiteSpace(txtCustomMessage.Text)) {
          AddLog("错误: 消息内容不能为空");
          MessageBox.Show("消息内容不能为空", "错误", MessageBoxButton.OK, MessageBoxImage.Warning);
          return;
        }

        // 写死的参数
        int cmdId = 1;           // 命令ID写死为1
        bool reliable = true;    // 可靠传输写死为true
        bool ordered = true;     // 有序传输写死为true

        // 获取用户输入的消息内容
        string messageText = txtCustomMessage.Text.Trim();

        // 将字符串转换为字节数组
        byte[] data = System.Text.Encoding.UTF8.GetBytes(messageText);
        int dataSize = data.Length;

        // 发送自定义消息
        bool success = mTRTCCloud.sendCustomCmdMsg(cmdId, data, dataSize, reliable, ordered);

        if (success) {
          AddLog($"发送自定义消息成功 - CmdId: {cmdId}, Data: \"{messageText}\", Size: {dataSize}字节, Reliable: {reliable}, Ordered: {ordered}");
        }
        else {
          AddLog($"发送自定义消息失败 - CmdId: {cmdId}");
          MessageBox.Show("发送自定义消息失败，请检查网络连接或稍后重试", "错误", MessageBoxButton.OK, MessageBoxImage.Error);
        }
      }
      catch (Exception ex) {
        AddLog($"发送自定义消息异常: {ex.Message}");
        MessageBox.Show($"发送自定义消息异常: {ex.Message}", "错误", MessageBoxButton.OK, MessageBoxImage.Error);
      }
    }


    /// <summary>
    /// 切换本地音频静音
    /// </summary>
    private void BtnLocalAudioToggle_Click(object sender, RoutedEventArgs e) {
      try {
        if (mTRTCCloud == null) return;

        mIsAudioMuted = !mIsAudioMuted;
        if (mIsAudioMuted) {
          mTRTCCloud.stopLocalAudio();
          btnLocalAudioToggle.ToolTip = "开启麦克风";
          pathLocalAudioToggle.Data = Geometry.Parse(Icons.AudioOff);
          AddLog("关闭麦克风");
        }
        else {
          mTRTCCloud.startLocalAudio(TRTCAudioQuality.TRTCAudioQualityDefault);
          btnLocalAudioToggle.ToolTip = "关闭麦克风";
          pathLocalAudioToggle.Data = Geometry.Parse(Icons.AudioOn);
          AddLog("开启麦克风");
        }
      }
      catch (Exception ex) {
        AddLog($"切换本地麦克风失败: {ex.Message}");
      }
    }


    /// <summary>
    /// 切换本地视频推流状态
    /// </summary>
    private void BtnLocalVideoMute_Click(object sender, RoutedEventArgs e) {
      try {
        if (mTRTCCloud == null) return;

        mIsVideoMuted = !mIsVideoMuted;
        mTRTCCloud.muteLocalVideo(mIsVideoMuted);

        if (mIsVideoMuted) {
          btnLocalVideoMute.ToolTip = "恢复视频推流";
          pathLocalVideoMute.Fill = Brushes.Red;
          AddLog("本地视频推流状态: 已暂停");
        }
        else {
          btnLocalVideoMute.ToolTip = "屏蔽视频推流";
          pathLocalVideoMute.Fill = Brushes.Transparent;
          AddLog("本地视频推流状态: 已恢复");
        }
      }
      catch (Exception ex) {
        AddLog($"切换本地视频推流状态失败: {ex.Message}");
      }
    }



    /// <summary>
    /// 切换本地视频镜像
    /// </summary>
    private void BtnLocalMirror_Click(object sender, RoutedEventArgs e) {
      try {
        if (mTRTCCloud == null) return;

        // 切换镜像状态
        mIsLocalMirror = !mIsLocalMirror;

        // 创建渲染参数
        TRTCRenderParams renderParams = new TRTCRenderParams {
          rotation = mCurrentRotation,
          fillMode = mLocalFillMode,
          mirrorType = mIsLocalMirror ? TRTCVideoMirrorType.TRTCVideoMirrorType_Enable : TRTCVideoMirrorType.TRTCVideoMirrorType_Disable
        };

        // 设置本地渲染参数
        mTRTCCloud.setLocalRenderParams(renderParams);

        // 更新按钮提示
        btnLocalMirror.ToolTip = mIsLocalMirror ? "镜像: 开" : "镜像: 关";

        AddLog($"切换本地视频镜像: {(mIsLocalMirror ? "开启" : "关闭")}");

      }
      catch (Exception ex) {
        AddLog($"切换本地镜像失败: {ex.Message}");
      }
    }

    /// <summary>
    /// 切换本地视频填充模式
    /// </summary>
    private void BtnLocalFillMode_Click(object sender, RoutedEventArgs e) {
      try {
        if (mTRTCCloud == null) return;

        // 切换填充模式
        mLocalFillMode = mLocalFillMode == TRTCVideoFillMode.TRTCVideoFillMode_Fit
            ? TRTCVideoFillMode.TRTCVideoFillMode_Fill
            : TRTCVideoFillMode.TRTCVideoFillMode_Fit;

        // 创建渲染参数
        TRTCRenderParams renderParams = new TRTCRenderParams {
          rotation = mCurrentRotation,
          fillMode = mLocalFillMode,
          mirrorType = mIsLocalMirror ? TRTCVideoMirrorType.TRTCVideoMirrorType_Enable : TRTCVideoMirrorType.TRTCVideoMirrorType_Disable
        };

        // 设置本地渲染参数
        mTRTCCloud.setLocalRenderParams(renderParams);

        // 更新按钮提示
        string fillModeText = mLocalFillMode == TRTCVideoFillMode.TRTCVideoFillMode_Fit ? "适应" : "填充";
        btnLocalFillMode.ToolTip = $"填充模式: {fillModeText}";

        AddLog($"切换本地视频填充模式: {fillModeText}");

      }
      catch (Exception ex) {
        AddLog($"切换本地填充模式失败: {ex.Message}");
      }
    }

    /// <summary>
    /// 截图按钮点击事件
    /// </summary>
    private void BtnSnapshot_Click(object sender, RoutedEventArgs e) {
      try {
        if (mTRTCCloud == null) {
          AddLog("错误: TRTC 实例未初始化");
          return;
        }

        if (!mIsInRoom) {
          AddLog("错误: 请先进入房间");
          MessageBox.Show("请先进入房间再进行截图操作", "提示", MessageBoxButton.OK, MessageBoxImage.Warning);
          return;
        }

        // 获取截图参数
        string userId = txtSnapshotUserId.Text;
        
        // 根据下拉框选择确定视频流类型
        TRTCVideoStreamType streamType = TRTCVideoStreamType.TRTCVideoStreamTypeBig;
        string streamTypeText = cmbVideoStreamType.SelectedItem?.ToString();
        if (streamTypeText != null)
        {
            if (streamTypeText.Contains("Small")) streamType = TRTCVideoStreamType.TRTCVideoStreamTypeSmall;
            else if (streamTypeText.Contains("Sub")) streamType = TRTCVideoStreamType.TRTCVideoStreamTypeSub;
        }
        
        // 根据下拉框选择确定截图源类型
        TRTCSnapshotSourceType sourceType = TRTCSnapshotSourceType.TRTCSnapshotSourceTypeStream;
        string sourceTypeText = cmbSnapshotSourceType.SelectedItem?.ToString();
        if (sourceTypeText != null && sourceTypeText.Contains("View")) {
            sourceType = TRTCSnapshotSourceType.TRTCSnapshotSourceTypeView;
        }
        AddLog($"开始截图 - 用户ID: {userId}, 流类型: {streamType}, 源类型: {sourceType}");

        // 调用截图接口
        mTRTCCloud.snapshotVideo(userId, streamType, sourceType);

        AddLog("截图请求已发送，等待回调...");

      } catch (Exception ex) {
        AddLog($"截图失败: {ex.Message}");
        MessageBox.Show($"截图失败: {ex.Message}", "错误", MessageBoxButton.OK, MessageBoxImage.Error);
      }
    }

    /// <summary>
    /// 打开截图文件夹按钮点击事件
    /// </summary>
    private void BtnOpenSnapshotFolder_Click(object sender, RoutedEventArgs e) {
      try {
        if (System.IO.Directory.Exists(mSnapshotFolderPath)) {
          System.Diagnostics.Process.Start("explorer.exe", mSnapshotFolderPath);
          AddLog("已打开截图文件夹");
        } else {
          AddLog("截图文件夹不存在");
        }
      } catch (Exception ex) {
        AddLog($"打开截图文件夹失败: {ex.Message}");
      }
    }

    /// <summary>
    /// 清空日志
    /// </summary>
    private void BtnClearLog_Click(object sender, RoutedEventArgs e) {
      txtLog.Clear();
    }

    /// <summary>
    /// 窗口关闭事件
    /// </summary>
    private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e) {
      // 清理资源
      if (mIsInRoom) {
        mTRTCCloud?.exitRoom();
      }

      // 清理远端视频 UI
      panelRemoteVideo.Children.Clear();
      mRemoteVideoContainers.Clear();
      mRemoteVideoImages.Clear();
      mRemoteFillModes.Clear();
      mRemoteMirrors.Clear();
      mRemoteVideoStates.Clear();
      mRemoteAudioMutes.Clear();

      ITRTCCloud.destroyTRTCShareInstance();
      AddLog("TRTC 资源已清理");
    }


    /// <summary>
    /// 添加日志
    /// </summary>
    public void AddLog(string message) {
      Dispatcher.Invoke(() => {
        string timestamp = DateTime.Now.ToString("HH:mm:ss.fff");
        txtLog.AppendText($"[{timestamp}] {message}\n");
        txtLog.ScrollToEnd();
      });
    }

    #region 回调处理方法

    public void OnEnterRoom(int result) {
      AddLog($"进房回调 - result: {result}");

      Dispatcher.Invoke(() => {
        if (result > 0) {
          mIsInRoom = true;
          txtStatus.Text = $"状态: 已进入房间 (耗时 {result}ms)";
          btnExitRoom.IsEnabled = true;
          btnEnterRoom.IsEnabled = false;
          btnLocalVideoToggle.IsEnabled = true;
          btnLocalAudioToggle.IsEnabled = true;
          pathLocalVideoToggle.Data = Geometry.Parse(Icons.VideoOff);
          pathLocalAudioToggle.Data = Geometry.Parse(Icons.AudioOff);
          AddLog("✓ 成功进入房间");
        }
        else {
          txtStatus.Text = $"状态: 进入房间失败 (错误码: {result})";
          btnEnterRoom.IsEnabled = true;
          AddLog($"✗ 进入房间失败，错误码: {result}");
          MessageBox.Show($"进入房间失败，错误码: {result}", "错误", MessageBoxButton.OK, MessageBoxImage.Error);
        }
      });
    }

    public void OnExitRoom(int reason) {
      AddLog($"退房回调 - reason: {reason}");

      Dispatcher.Invoke(() => {
        mIsInRoom = false;
        txtStatus.Text = "状态: 已退出房间";
        btnExitRoom.IsEnabled = false;
        btnEnterRoom.IsEnabled = true;
      });
    }

    public void OnRemoteUserEnterRoom(string userId) {
      AddLog($"远端用户进房: {userId}");
      Dispatcher.Invoke(() => {
        // 如果已经存在，先移除（避免重复）
        if (mRemoteVideoContainers.ContainsKey(userId)) {
          panelRemoteVideo.Children.Remove(mRemoteVideoContainers[userId]);
          mRemoteVideoContainers.Remove(userId);
          mRemoteVideoImages.Remove(userId);
        }

        // 初始化默认状态
        if (!mRemoteFillModes.ContainsKey(userId)) mRemoteFillModes[userId] = TRTCVideoFillMode.TRTCVideoFillMode_Fit;
        if (!mRemoteMirrors.ContainsKey(userId)) mRemoteMirrors[userId] = false;

        // 创建新的视频容器
        System.Windows.Controls.Border border = new System.Windows.Controls.Border {
          Width = 240,
          Height = 180,
          Margin = new Thickness(5),
          Background = Brushes.Black,
          CornerRadius = new CornerRadius(4),
          ClipToBounds = true
        };

        System.Windows.Controls.Grid grid = new System.Windows.Controls.Grid();
        System.Windows.Controls.Image image = new System.Windows.Controls.Image { Stretch = Stretch.Uniform };

        // 用户名标签
        System.Windows.Controls.Border nameBorder = new System.Windows.Controls.Border {
          Background = new SolidColorBrush(Color.FromArgb(128, 0, 0, 0)),
          HorizontalAlignment = HorizontalAlignment.Left,
          VerticalAlignment = VerticalAlignment.Top,
          Margin = new Thickness(5),
          CornerRadius = new CornerRadius(2),
          Padding = new Thickness(4, 2, 4, 2)
        };

        System.Windows.Controls.TextBlock textBlock = new System.Windows.Controls.TextBlock {
          Text = userId,
          Foreground = Brushes.White,
          FontSize = 10
        };
        nameBorder.Child = textBlock;

        // 控制栏
        System.Windows.Controls.Border controlBorder = new System.Windows.Controls.Border {
          Background = new SolidColorBrush(Color.FromArgb(153, 0, 0, 0)),
          VerticalAlignment = VerticalAlignment.Bottom,
          Height = 36,
          CornerRadius = new CornerRadius(0, 0, 4, 4)
        };

        System.Windows.Controls.StackPanel controlPanel = new System.Windows.Controls.StackPanel {
          Orientation = System.Windows.Controls.Orientation.Horizontal,
          HorizontalAlignment = HorizontalAlignment.Center,
          VerticalAlignment = VerticalAlignment.Center
        };


        // 视频开关按钮
        System.Windows.Controls.Button btnVideo = new System.Windows.Controls.Button {
          Style = (Style)FindResource("IconButtonStyle"),
          Margin = new Thickness(3),
          ToolTip = "关闭视频",
          Width = 24,
          Height = 24
        };
        System.Windows.Shapes.Path pathVideo = new System.Windows.Shapes.Path {
          Data = Geometry.Parse(Icons.VideoOn),
          Stroke = Brushes.White,
          StrokeThickness = 2,
          Fill = Brushes.Transparent,
          Width = 12,
          Height = 12,
          Stretch = Stretch.Uniform
        };
        btnVideo.Content = pathVideo;
        btnVideo.Click += (s, e) => ToggleRemoteVideo(userId, btnVideo, pathVideo);

        // 音频开关按钮
        System.Windows.Controls.Button btnAudio = new System.Windows.Controls.Button {
          Style = (Style)FindResource("IconButtonStyle"),
          Margin = new Thickness(3),
          ToolTip = "静音音频",
          Width = 24,
          Height = 24
        };
        System.Windows.Shapes.Path pathAudio = new System.Windows.Shapes.Path {
          Data = Geometry.Parse(Icons.AudioOn),
          Stroke = Brushes.White,
          StrokeThickness = 2,
          Fill = Brushes.Transparent,
          Width = 12,
          Height = 12,
          Stretch = Stretch.Uniform
        };
        btnAudio.Content = pathAudio;
        btnAudio.Click += (s, e) => ToggleRemoteAudio(userId, btnAudio, pathAudio);

        // 镜像按钮
        System.Windows.Controls.Button btnMirror = new System.Windows.Controls.Button {
          Style = (Style)FindResource("IconButtonStyle"),
          Margin = new Thickness(3),
          ToolTip = "镜像",
          Width = 24,
          Height = 24
        };

        System.Windows.Shapes.Path pathMirror = new System.Windows.Shapes.Path {
          Data = Geometry.Parse(Icons.Mirror),
          Stroke = Brushes.White,
          StrokeThickness = 2,
          Fill = Brushes.Transparent,
          Width = 12,
          Height = 12,
          Stretch = Stretch.Uniform
        };
        btnMirror.Content = pathMirror;
        btnMirror.Click += (s, e) => ToggleRemoteMirror(userId);

        // 填充按钮
        System.Windows.Controls.Button btnFill = new System.Windows.Controls.Button {
          Style = (Style)FindResource("IconButtonStyle"),
          Margin = new Thickness(3),
          ToolTip = "填充模式",
          Width = 24,
          Height = 24
        };
        System.Windows.Shapes.Path pathFill = new System.Windows.Shapes.Path {
          Data = Geometry.Parse(Icons.FillMode),
          Stroke = Brushes.White,
          StrokeThickness = 2,
          Fill = Brushes.Transparent,
          Width = 12,
          Height = 12,
          Stretch = Stretch.Uniform
        };

        btnFill.Content = pathFill;
        btnFill.Click += (s, e) => ToggleRemoteFillMode(userId, btnFill);

        controlPanel.Children.Add(btnVideo);
        controlPanel.Children.Add(btnAudio);
        controlPanel.Children.Add(btnMirror);
        controlPanel.Children.Add(btnFill);

        controlBorder.Child = controlPanel;

        grid.Children.Add(image);
        grid.Children.Add(nameBorder);
        grid.Children.Add(controlBorder);
        border.Child = grid;

        // 添加到界面
        panelRemoteVideo.Children.Add(border);

        // 记录引用
        mRemoteVideoContainers[userId] = border;
        mRemoteVideoImages[userId] = image;
      });
    }

    public void OnRemoteUserLeaveRoom(string userId, int reason) {
      AddLog($"远端用户离开: {userId}, 原因: {reason}");

      Dispatcher.Invoke(() => {
        if (mRemoteVideoContainers.ContainsKey(userId)) {
          panelRemoteVideo.Children.Remove(mRemoteVideoContainers[userId]);
          mRemoteVideoContainers.Remove(userId);
          mRemoteVideoImages.Remove(userId);
          mRemoteFillModes.Remove(userId);
          mRemoteMirrors.Remove(userId);
          mRemoteVideoStates.Remove(userId);
          mRemoteAudioMutes.Remove(userId);

        }
      });
    }

    public void OnUserVideoAvailable(string userId, bool available) {
      AddLog($"远端用户视频状态变化: {userId}, available: {available}");

      Dispatcher.Invoke(() => {
        if (available) {
          // 开始渲染
          if (mTRTCCloud != null) {
            AddLog($"开始接收远端视频流: {userId}（TRTCVideoRender 自动渲染模式）");
            mTRTCCloud.startRemoteView(userId, TRTCVideoStreamType.TRTCVideoStreamTypeBig, mRemoteVideoImages[userId]);
            // 应用默认渲染参数
            UpdateRemoteRenderParams(userId);
          }
        }
        else {
          // 停止渲染并移除
          if (mTRTCCloud != null) {
            mTRTCCloud.stopRemoteView(userId, TRTCVideoStreamType.TRTCVideoStreamTypeBig);
          }
        }
      });
    }

    private void ToggleRemoteVideo(string userId, System.Windows.Controls.Button btn, System.Windows.Shapes.Path path) {
      if (mTRTCCloud == null) return;

      bool isVideoOn = true;
      if (mRemoteVideoStates.ContainsKey(userId)) isVideoOn = mRemoteVideoStates[userId];
      else mRemoteVideoStates[userId] = true; // 默认为开启

      if (isVideoOn) {
        // 关闭视频
        mTRTCCloud.stopRemoteView(userId, TRTCVideoStreamType.TRTCVideoStreamTypeBig);
        mRemoteVideoStates[userId] = false;
        btn.ToolTip = "开启视频";
        path.Data = Geometry.Parse(Icons.VideoOff);
        path.Fill = Brushes.White;
        AddLog($"关闭远端视频: {userId}");
      }
      else {
        // 开启视频
        if (mRemoteVideoImages.ContainsKey(userId)) {
          mTRTCCloud.startRemoteView(userId, TRTCVideoStreamType.TRTCVideoStreamTypeBig, mRemoteVideoImages[userId]);
          UpdateRemoteRenderParams(userId);
        }
        mRemoteVideoStates[userId] = true;
        btn.ToolTip = "关闭视频";
        path.Data = Geometry.Parse(Icons.VideoOn);
        path.Fill = Brushes.Transparent;
        AddLog($"开启远端视频: {userId}");
      }
    }


    private void ToggleRemoteAudio(string userId, System.Windows.Controls.Button btn, System.Windows.Shapes.Path path) {
      if (mTRTCCloud == null) return;

      bool isMuted = false;
      if (mRemoteAudioMutes.ContainsKey(userId)) isMuted = mRemoteAudioMutes[userId];
      else mRemoteAudioMutes[userId] = false; // 默认为不静音

      if (isMuted) {
        // 取消静音
        mTRTCCloud.muteRemoteAudio(userId, false);
        mRemoteAudioMutes[userId] = false;
        btn.ToolTip = "静音音频";
        path.Data = Geometry.Parse(Icons.AudioOn);
        path.Fill = Brushes.Transparent;
        AddLog($"取消远端静音: {userId}");
      }
      else {
        // 静音
        mTRTCCloud.muteRemoteAudio(userId, true);
        mRemoteAudioMutes[userId] = true;
        btn.ToolTip = "取消静音";
        path.Data = Geometry.Parse(Icons.AudioOff);
        path.Fill = Brushes.Red;
        AddLog($"静音远端: {userId}");
      }
    }


    private void ToggleRemoteMirror(string userId) {
      if (!mRemoteMirrors.ContainsKey(userId)) return;
      mRemoteMirrors[userId] = !mRemoteMirrors[userId];
      UpdateRemoteRenderParams(userId);
      AddLog($"切换远端用户 {userId} 镜像: {(mRemoteMirrors[userId] ? "开启" : "关闭")}");
    }

    private void ToggleRemoteFillMode(string userId, System.Windows.Controls.Button btn) {
      if (!mRemoteFillModes.ContainsKey(userId)) return;

      mRemoteFillModes[userId] = mRemoteFillModes[userId] == TRTCVideoFillMode.TRTCVideoFillMode_Fit
          ? TRTCVideoFillMode.TRTCVideoFillMode_Fill
          : TRTCVideoFillMode.TRTCVideoFillMode_Fit;

      UpdateRemoteRenderParams(userId);

      string modeText = mRemoteFillModes[userId] == TRTCVideoFillMode.TRTCVideoFillMode_Fit ? "适应" : "填充";
      btn.ToolTip = $"填充模式: {modeText}";
      AddLog($"切换远端用户 {userId} 填充模式: {modeText}");
    }

    private void UpdateRemoteRenderParams(string userId) {
      if (mTRTCCloud == null) return;

      TRTCRenderParams renderParams = new TRTCRenderParams {
        rotation = TRTCVideoRotation.TRTCVideoRotation0,
        fillMode = mRemoteFillModes.ContainsKey(userId) ? mRemoteFillModes[userId] : TRTCVideoFillMode.TRTCVideoFillMode_Fit,
        mirrorType = (mRemoteMirrors.ContainsKey(userId) && mRemoteMirrors[userId]) ? TRTCVideoMirrorType.TRTCVideoMirrorType_Enable : TRTCVideoMirrorType.TRTCVideoMirrorType_Disable
      };

      mTRTCCloud.setRemoteRenderParams(userId, TRTCVideoStreamType.TRTCVideoStreamTypeBig, ref renderParams);

    }


    public void OnUserAudioAvailable(string userId, bool available) {
      AddLog($"远端用户音频状态变化: {userId}, available: {available}");
    }

    public void OnError(TXLiteAVError errCode, string errMsg, IntPtr extraInfo) {
      AddLog($"错误回调 - errCode: {errCode}, errMsg: {errMsg}");

      Dispatcher.Invoke(() => {
        MessageBox.Show($"发生错误: {errMsg} (错误码: {errCode})", "错误", MessageBoxButton.OK, MessageBoxImage.Error);
      });
    }

    public void OnWarning(TXLiteAVWarning warningCode, string warningMsg, IntPtr extraInfo) {
      AddLog($"警告回调 - warningCode: {warningCode}, warningMsg: {warningMsg}");
    }

    public void OnNetworkQuality(TRTCQualityInfo localQuality, TRTCQualityInfo[] remoteQuality, uint remoteQualityCount) {
      // 网络质量回调（可选实现）
    }

    public void OnStatistics(TRTCStatistics statis) {
      // 统计信息回调（可选实现）
    }

    /// <summary>
    /// 截图完成回调处理方法
    /// </summary>
    public void OnSnapshotComplete(string userId, TRTCVideoStreamType type, byte[] data, UInt32 length, UInt32 width, UInt32 height, TRTCVideoPixelFormat format) {
      AddLog($"【回调】onSnapshotComplete - UserId: {userId}, Type: {type}, Format: {format}, Width: {width}, Height: {height}, Length: {length}");

      Dispatcher.Invoke(() => {
        try {
          // 根据像素格式生成图片
          System.Windows.Media.Imaging.BitmapSource bitmapSource = null;
          
          switch (format) {
            case TRTCVideoPixelFormat.TRTCVideoPixelFormat_BGRA32:
              bitmapSource = System.Windows.Media.Imaging.BitmapSource.Create(
                (int)width, (int)height, 96, 96, 
                System.Windows.Media.PixelFormats.Bgra32, null, data, (int)width * 4);
              break;
            case TRTCVideoPixelFormat.TRTCVideoPixelFormat_RGBA32:
              // WPF中没有RGBA32格式，使用BGRA32作为替代（注意红蓝通道顺序差异）
              bitmapSource = System.Windows.Media.Imaging.BitmapSource.Create(
                (int)width, (int)height, 96, 96, 
                System.Windows.Media.PixelFormats.Bgra32, null, data, (int)width * 4);
              break;
            case TRTCVideoPixelFormat.TRTCVideoPixelFormat_I420:
              // I420格式需要转换为RGB格式
              bitmapSource = ConvertI420ToBitmap(data, (int)width, (int)height);
              break;
            default:
              AddLog($"不支持的像素格式: {format}");
              return;
          }

          if (bitmapSource != null) {
            // 保存图片到文件
            string timestamp = DateTime.Now.ToString("yyyyMMdd_HHmmssfff");
            string fileName = $"snapshot_{userId}_{type}_{timestamp}.png";
            string filePath = System.IO.Path.Combine(mSnapshotFolderPath, fileName);

            using (var fileStream = new System.IO.FileStream(filePath, System.IO.FileMode.Create)) {
              var encoder = new System.Windows.Media.Imaging.PngBitmapEncoder();
              encoder.Frames.Add(System.Windows.Media.Imaging.BitmapFrame.Create(bitmapSource));
              encoder.Save(fileStream);
            }

            AddLog($"截图保存成功: {fileName} (尺寸: {width}x{height}, 格式: {format})");

            // 显示截图预览（可选）
            ShowSnapshotPreview(userId, bitmapSource, filePath);
          }
        } catch (Exception ex) {
          AddLog($"处理截图数据失败: {ex.Message}");
        }
      });
    }

    /// <summary>
    /// 将I420格式转换为Bitmap
    /// </summary>
    private System.Windows.Media.Imaging.BitmapSource ConvertI420ToBitmap(byte[] i420Data, int width, int height) {
      try {
        // I420格式转换需要更复杂的处理，这里简化为返回null
        // 实际项目中应该实现完整的YUV到RGB的转换
        AddLog("I420格式转换暂未实现，建议使用BGRA32或RGBA32格式");
        return null;
      } catch (Exception ex) {
        AddLog($"I420格式转换失败: {ex.Message}");
        return null;
      }
    }

    /// <summary>
    /// 显示截图预览
    /// </summary>
    private void ShowSnapshotPreview(string userId, System.Windows.Media.Imaging.BitmapSource bitmapSource, string filePath) {
      // 在日志区域显示截图信息（可选）
      // 可以扩展为在界面中显示截图预览窗口
      AddLog($"截图预览 - 用户: {userId}, 文件: {System.IO.Path.GetFileName(filePath)}");
    }

    #endregion
  }

  /// <summary>
  /// TRTC 回调实现类
  /// </summary>
  internal class TRTCCallback : ITRTCCloudCallback {
    private MainWindow _mainWindow;

    public TRTCCallback(MainWindow mainWindow) {
      _mainWindow = mainWindow;
    }

    public void onEnterRoom(int result) {
      _mainWindow.OnEnterRoom(result);
      _mainWindow.AddLog($"【回调】onEnterRoom - result: {result}");
    }

    public void onExitRoom(int reason) {
      _mainWindow.OnExitRoom(reason);
      _mainWindow.AddLog($"【回调】onExitRoom - reason: {reason}");
    }

    public void onRemoteUserEnterRoom(string userId) {
      _mainWindow.OnRemoteUserEnterRoom(userId);
      _mainWindow.AddLog($"【回调】onRemoteUserEnterRoom - userId: {userId}");
    }

    public void onRemoteUserLeaveRoom(string userId, int reason) {
      _mainWindow.OnRemoteUserLeaveRoom(userId, reason);
      _mainWindow.AddLog($"【回调】onRemoteUserLeaveRoom - userId: {userId}, reason: {reason}");
    }

    public void onUserVideoAvailable(string userId, bool available) {
      _mainWindow.OnUserVideoAvailable(userId, available);
      _mainWindow.AddLog($"【回调】onUserVideoAvailable - userId: {userId}, available: {available}");
    }

    public void onUserAudioAvailable(string userId, bool available) {
      _mainWindow.OnUserAudioAvailable(userId, available);
      _mainWindow.AddLog($"【回调】onUserAudioAvailable - userId: {userId}, available: {available}");
    }

    public void onError(TXLiteAVError errCode, string errMsg, IntPtr extraInfo) {
      _mainWindow.OnError(errCode, errMsg, extraInfo);
      _mainWindow.AddLog($"【回调】onError - errCode: {errCode}, errMsg: {errMsg}");
    }

    public void onWarning(TXLiteAVWarning warningCode, string warningMsg, IntPtr extraInfo) {
      _mainWindow.OnWarning(warningCode, warningMsg, extraInfo);
      _mainWindow.AddLog($"【回调】onWarning - warningCode: {warningCode}, warningMsg: {warningMsg}");
    }

    public void onNetworkQuality(TRTCQualityInfo localQuality, TRTCQualityInfo[] remoteQuality, uint remoteQualityCount) {
      _mainWindow.OnNetworkQuality(localQuality, remoteQuality, remoteQualityCount);
    }

    public void onStatistics(TRTCStatistics statistics) {
      _mainWindow.OnStatistics(statistics);
    }
    public void onSwitchRole(TXLiteAVError errCode, string errMsg) {
      _mainWindow.AddLog($"【回调】onSwitchRole - errCode: {errCode}, errMsg: {errMsg}");
    }

    public void onSwitchRoom(TXLiteAVError errCode, string errMsg) {
      _mainWindow.AddLog($"【回调】onSwitchRoom - errCode: {errCode}, errMsg: {errMsg}");
    }

    public void onConnectOtherRoom(string userId, TXLiteAVError errCode, string errMsg) {
      _mainWindow.AddLog($"【回调】onConnectOtherRoom - userId: {userId}, errCode: {errCode}, errMsg: {errMsg}");
    }

    public void onDisconnectOtherRoom(TXLiteAVError errCode, string errMsg) {
      _mainWindow.AddLog($"【回调】onDisconnectOtherRoom - errCode: {errCode}, errMsg: {errMsg}");
    }

    public void onUserSubStreamAvailable(string userId, bool available) {
      _mainWindow.AddLog($"【回调】onUserSubStreamAvailable - userId: {userId}, available: {available}");
    }

    public void onFirstVideoFrame(string userId, TRTCVideoStreamType streamType, int width, int height) {
      _mainWindow.AddLog($"【回调】onFirstVideoFrame - userId: {userId}, streamType: {streamType}, width: {width}, height: {height}");
    }

    public void onFirstAudioFrame(string userId) {
      _mainWindow.AddLog($"【回调】onFirstAudioFrame - userId: {userId}");
    }

    public void onSendFirstLocalVideoFrame(TRTCVideoStreamType streamType) {
      _mainWindow.AddLog($"【回调】onSendFirstLocalVideoFrame - streamType: {streamType}");
    }

    public void onSendFirstLocalAudioFrame() {
      _mainWindow.AddLog($"【回调】onSendFirstLocalAudioFrame");
    }

    public void onSpeedTestResult(TRTCSpeedTestResult result) {
      _mainWindow.AddLog($"【回调】onSpeedTestResult");
    }

    public void onConnectionLost() {
      _mainWindow.AddLog($"【回调】onConnectionLost - SDK 与云端的连接已经断开");
    }

    public void onTryToReconnect() {
      _mainWindow.AddLog($"【回调】onTryToReconnect - SDK 正在尝试重新连接到云端");
    }

    public void onConnectionRecovery() {
      _mainWindow.AddLog($"【回调】onConnectionRecovery - SDK 与云端的连接已经恢复");
    }

    public void onCameraDidReady() {
      _mainWindow.AddLog($"【回调】onCameraDidReady - 摄像头准备就绪");
    }

    public void onMicDidReady() {
      _mainWindow.AddLog($"【回调】onMicDidReady - 麦克风准备就绪");
    }

    public void onAudioRouteChanged(TRTCAudioRoute route, TRTCAudioRoute fromRoute) {
      _mainWindow.AddLog($"【回调】onAudioRouteChanged - route: {route}, fromRoute: {fromRoute}");
    }

    public void onUserVoiceVolume(TRTCVolumeInfo[] userVolumes, uint userVolumesCount, uint totalVolume) {
      _mainWindow.AddLog($"【回调】onUserVoiceVolume - userVolumesCount: {userVolumesCount}, totalVolume: {totalVolume}");
    }

    public void onDeviceChange(string deviceId, TRTCDeviceType type, TRTCDeviceState state) {
      _mainWindow.AddLog($"【回调】onDeviceChange - deviceId: {deviceId}, type: {type}, state: {state}");
    }

    public void onAudioDeviceCaptureVolumeChanged(int volume, bool muted) {
      _mainWindow.AddLog($"【回调】onAudioDeviceCaptureVolumeChanged - volume: {volume}, muted: {muted}");
    }

    public void onAudioDevicePlayoutVolumeChanged(int volume, bool muted) {
      _mainWindow.AddLog($"【回调】onAudioDevicePlayoutVolumeChanged - volume: {volume}, muted: {muted}");
    }

    public void onTestMicVolume(int volume) {
      _mainWindow.AddLog($"【回调】onTestMicVolume - volume: {volume}");
    }

    public void onTestSpeakerVolume(int volume) {
      _mainWindow.AddLog($"【回调】onTestSpeakerVolume - volume: {volume}");
    }

    public void onMissCustomCmdMsg(string userId, int cmdID, int errCode, int missed) {
      _mainWindow.AddLog($"【回调】onMissCustomCmdMsg - userId: {userId}, cmdID: {cmdID}, errCode: {errCode}, missed: {missed}");
    }

    public void onRecvCustomCmdMsg(string userId, int cmdID, int seq, byte[] message, int messageSize) {
      try {
        // 将接收到的字节数组转换为字符串
        string messageText = System.Text.Encoding.UTF8.GetString(message, 0, messageSize);
        _mainWindow.AddLog($"【回调】onRecvCustomCmdMsg - UserId: {userId}, CmdId: {cmdID}, Seq: {seq}, Size: {messageSize}字节, Content: \"{messageText}\"");
      }
      catch (Exception ex) {
        _mainWindow.AddLog($"【回调】解析自定义消息失败 - UserId: {userId}, CmdId: {cmdID}, Seq: {seq}, Size: {messageSize}字节, Error: {ex.Message}");
      }
    }

    public void onRecvSEIMsg(string userId, byte[] message, uint messageSize) {
      _mainWindow.AddLog($"【回调】onRecvSEIMsg - userId: {userId}, messageSize: {messageSize}");
    }

    public void onStartPublishing(int errCode, string errMsg) {
      _mainWindow.AddLog($"【回调】onStartPublishing - errCode: {errCode}, errMsg: {errMsg}");
    }

    public void onStopPublishing(int errCode, string errMsg) {
      _mainWindow.AddLog($"【回调】onStopPublishing - errCode: {errCode}, errMsg: {errMsg}");
    }

    public void onSetMixTranscodingConfig(int errCode, string errMsg) {
      _mainWindow.AddLog($"【回调】onSetMixTranscodingConfig - errCode: {errCode}, errMsg: {errMsg}");
    }

    public void onStartPublishMediaStream(string taskId, int errCode, string errMsg, string extraInfo) {
      _mainWindow.AddLog($"【回调】onStartPublishMediaStream - taskId: {taskId}, errCode: {errCode}, errMsg: {errMsg}, extraInfo: {extraInfo}");
    }

    public void onUpdatePublishMediaStream(string taskId, int errCode, string errMsg, string extraInfo) {
      _mainWindow.AddLog($"【回调】onUpdatePublishMediaStream - taskId: {taskId}, errCode: {errCode}, errMsg: {errMsg}, extraInfo: {extraInfo}");
    }

    public void onStopPublishMediaStream(string taskId, int errCode, string errMsg, string extraInfo) {
      _mainWindow.AddLog($"【回调】onStopPublishMediaStream - taskId: {taskId}, errCode: {errCode}, errMsg: {errMsg}, extraInfo: {extraInfo}");
    }

    public void onCdnStreamStateChanged(string cdnUrl, int state, int errCode, string errMsg, string extraInfo) {
      _mainWindow.AddLog($"【回调】onCdnStreamStateChanged - cdnUrl: {cdnUrl}, state: {state}, errCode: {errCode}, errMsg: {errMsg}, extraInfo: {extraInfo}");
    }

    public void onScreenCaptureStarted() {
      _mainWindow.AddLog($"【回调】onScreenCaptureStarted - 屏幕录制开始");
    }

    public void onScreenCapturePaused(int reason) {
      _mainWindow.AddLog($"【回调】onScreenCapturePaused - 屏幕录制暂停, reason: {reason}");
    }

    public void onScreenCaptureResumed(int reason) {
      _mainWindow.AddLog($"【回调】onScreenCaptureResumed - 屏幕录制恢复, reason: {reason}");
    }

    public void onScreenCaptureStoped(int reason) {
      _mainWindow.AddLog($"【回调】onScreenCaptureStoped - 屏幕录制停止, reason: {reason}");
    }

    public void onSnapshotComplete(string userId, TRTCVideoStreamType type, byte[] data, UInt32 length, UInt32 width, UInt32 height, TRTCVideoPixelFormat format) {
      _mainWindow.OnSnapshotComplete(userId, type, data, length, width, height, format);
    }
  }
}
