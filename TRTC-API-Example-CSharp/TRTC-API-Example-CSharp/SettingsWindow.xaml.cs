using System.Windows;
using System.Windows.Controls;
using trtc;

namespace TRTCWPFDemo {
  // 设备信息包装类，用于 WPF 数据绑定
  public class DeviceViewModel {
    private TXDeviceInfo _deviceInfo;

    public DeviceViewModel(TXDeviceInfo deviceInfo) {
      _deviceInfo = deviceInfo;
    }

    // WPF 绑定需要属性而不是字段
    public string devicePID => _deviceInfo.devicePID;
    public string deviceName => _deviceInfo.deviceName;
    public string deviceProperties => _deviceInfo.deviceProperties;
  }

  public partial class SettingsWindow : Window {
    private ITXDeviceManager mDeviceManager;

    public SettingsWindow(ITXDeviceManager deviceManager) {
      InitializeComponent();
      mDeviceManager = deviceManager;
      Loaded += SettingsWindow_Loaded;
    }

    private void SettingsWindow_Loaded(object sender, RoutedEventArgs e) {
      RefreshDeviceList();
    }

    private void RefreshDeviceList() {
      if (mDeviceManager == null) return;

      // 摄像头
      var cameraList = mDeviceManager.getDevicesList(TXMediaDeviceType.TXMediaDeviceTypeCamera);
      var cameraViewModels = cameraList.Select(d => new DeviceViewModel(d)).ToList();
      cmbCameras.ItemsSource = cameraViewModels;
      var currentCamera = mDeviceManager.getCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeCamera);

      int cameraIndex = -1;
      if (!string.IsNullOrEmpty(currentCamera.deviceName)) {
        for (int i = 0; i < cameraViewModels.Count; i++) {
          if (cameraViewModels[i].deviceName == currentCamera.deviceName) {
            cameraIndex = i;
            break;
          }
        }
      }

      if (cameraIndex != -1) {
        cmbCameras.SelectedIndex = cameraIndex;
      }
      else if (cameraViewModels.Count > 0) {
        cmbCameras.SelectedIndex = 0;
      }


      // 麦克风
      var micList = mDeviceManager.getDevicesList(TXMediaDeviceType.TXMediaDeviceTypeMic);
      var micViewModels = micList.Select(d => new DeviceViewModel(d)).ToList();
      cmbMics.ItemsSource = micViewModels;
      var currentMic = mDeviceManager.getCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeMic);

      int micIndex = -1;
      if (!string.IsNullOrEmpty(currentMic.deviceName)) {
        for (int i = 0; i < micViewModels.Count; i++) {
          if (micViewModels[i].deviceName == currentMic.deviceName) {
            micIndex = i;
            break;
          }
        }
      }

      if (micIndex != -1) {
        cmbMics.SelectedIndex = micIndex;
      }
      else if (micViewModels.Count > 0) {
        cmbMics.SelectedIndex = 0;
      }


      // 扬声器
      var speakerList = mDeviceManager.getDevicesList(TXMediaDeviceType.TXMediaDeviceTypeSpeaker);
      var speakerViewModels = speakerList.Select(d => new DeviceViewModel(d)).ToList();
      cmbSpeakers.ItemsSource = speakerViewModels;
      var currentSpeaker = mDeviceManager.getCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeSpeaker);

      int speakerIndex = -1;
      if (!string.IsNullOrEmpty(currentSpeaker.deviceName)) {
        for (int i = 0; i < speakerViewModels.Count; i++) {
          if (speakerViewModels[i].deviceName == currentSpeaker.deviceName) {
            speakerIndex = i;
            break;
          }
        }
      }

      if (speakerIndex != -1) {
        cmbSpeakers.SelectedIndex = speakerIndex;
      }
      else if (speakerViewModels.Count > 0) {
        cmbSpeakers.SelectedIndex = 0;
      }

    }

    private void CmbCameras_SelectionChanged(object sender, SelectionChangedEventArgs e) {
      if (cmbCameras.SelectedItem is DeviceViewModel info && mDeviceManager != null) {
        mDeviceManager.setCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeCamera, info.devicePID);
      }
    }



    private void CmbMics_SelectionChanged(object sender, SelectionChangedEventArgs e) {
      if (cmbMics.SelectedItem is DeviceViewModel info && mDeviceManager != null) {
        mDeviceManager.setCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeMic, info.devicePID);
      }
    }



    private void CmbSpeakers_SelectionChanged(object sender, SelectionChangedEventArgs e) {
      if (cmbSpeakers.SelectedItem is DeviceViewModel info && mDeviceManager != null) {
        mDeviceManager.setCurrentDevice(TXMediaDeviceType.TXMediaDeviceTypeSpeaker, info.devicePID);
      }
    }



    private void BtnClose_Click(object sender, RoutedEventArgs e) {
      Close();
    }
  }
}