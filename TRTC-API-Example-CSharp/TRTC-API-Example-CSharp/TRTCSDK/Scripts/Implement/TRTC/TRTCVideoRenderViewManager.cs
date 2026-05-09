// Copyright (c) 2024 Tencent. All rights reserved.
// Author: bardshang

using System.Collections.Generic;
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
using UnityEngine;
#else
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Threading;
#endif

namespace trtc {

  public class TRTCVideoRenderViewManager {
    private object _viewMapLock = new object();
    static TRTCVideoRenderViewManager _sInstance = new TRTCVideoRenderViewManager();
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
    private Dictionary<RenderKey, GameObject> _videoRenderViewMap = new Dictionary<RenderKey, GameObject>();
#else
    private Dictionary<RenderKey, VideoRenderContainer> _videoRenderViewMap = new Dictionary<RenderKey, VideoRenderContainer>();
#endif

    public static TRTCVideoRenderViewManager getInstance() {
      return _sInstance;
    }


    private TRTCVideoRenderViewManager() { }


    ~TRTCVideoRenderViewManager() {
      Destroy();
    }

    private void Destroy() {
      removeAllVideoRenderView();
    }

#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
    public void addVideoRenderView(GameObject view, string userID, TRTCVideoStreamType streamType) {
      lock (_viewMapLock) {
        RenderKey key = new RenderKey(userID, streamType);
        if (_videoRenderViewMap.ContainsKey(key)) {
          GameObject oldView = _videoRenderViewMap[key];
          if (oldView == view) {
            return;
          }
          releaseVideoRender(oldView);
          _videoRenderViewMap.Remove(key);
        }

        TRTCVideoRender render = view.AddComponent<TRTCVideoRender>();
        render.SetForUser(userID, streamType);
        _videoRenderViewMap[key] = view;
      }
    }
#else
    public void addVideoRenderView(System.Windows.Controls.Image view, string userID, TRTCVideoStreamType streamType) {
      lock (_viewMapLock) {
        RenderKey key = new RenderKey(userID, streamType);
        if (_videoRenderViewMap.ContainsKey(key)) {
          VideoRenderContainer oldContainer = _videoRenderViewMap[key];
          if (oldContainer.View == view) {
            return;
          }
          releaseVideoRender(oldContainer);
          _videoRenderViewMap.Remove(key);
        }

        TRTCVideoRender render = new TRTCVideoRender();
        render.Initialize(view.Dispatcher ?? view.Dispatcher, view);
        render.SetForUser(userID, streamType);
        VideoRenderContainer container = new VideoRenderContainer {
          View = view,
          Render = render
        };
        
        _videoRenderViewMap[key] = container;
      }
    }
#endif

    public void removeVideoRenderView(string userID, TRTCVideoStreamType streamType) {
      lock (_viewMapLock) {
        RenderKey key = new RenderKey(userID, streamType);
        if (_videoRenderViewMap.ContainsKey(key)) {
          releaseVideoRender(_videoRenderViewMap[key]);
          _videoRenderViewMap.Remove(key);
        }
      }
    }

    public void removeAllRemoteVideoRenderView() {
      lock (_viewMapLock) {
        List<RenderKey> keysToRemove = new List<RenderKey>();
        foreach (var key in _videoRenderViewMap.Keys) {
          if (!string.IsNullOrEmpty(key.UserId)) {
            keysToRemove.Add(key);
          }
        }

        foreach (var key in keysToRemove) {
          _videoRenderViewMap.Remove(key);
        }
      }
    }

    public void removeAllVideoRenderView() {
      lock (_viewMapLock) {
        foreach (var item in _videoRenderViewMap.Values) {
          releaseVideoRender(item);
        }
        _videoRenderViewMap.Clear();
      }
    }

    public void setVideoRenderParams(string userID, TRTCVideoStreamType streamType, TRTCRenderParams renderParams) {
      lock (_viewMapLock) {
        RenderKey key = new RenderKey(userID, streamType);
        if (_videoRenderViewMap.ContainsKey(key)) {
#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
          TRTCVideoRender render = _videoRenderViewMap[key].GetComponent<TRTCVideoRender>();
          render.setRenderParams(renderParams);
#else
          VideoRenderContainer container = _videoRenderViewMap[key];
          if (container != null && container.Render != null) {
            container.Render.setRenderParams(renderParams);
          }
#endif
        }
      }
    }

#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL
    private void releaseVideoRender(GameObject view) {
      if (!view) {
        return;
      }
      var render = view.GetComponent<TRTCVideoRender>();
      if (render) {
        GameObject.Destroy(render);
      }
    }
#else
    private void releaseVideoRender(VideoRenderContainer container) {
      if (container == null || container.Render == null) {
        return;
      }
      try {
        container.Render.Dispose();
      } catch (Exception ex) {
        System.Diagnostics.Debug.WriteLine($"Release video render error: {ex.Message}");
      }
    }
#endif
  }
#if !(UNITY_EDITOR || UNITY_STANDALONE || UNITY_ANDROID || UNITY_IOS || UNITY_OPENHARMONY || UNITY_WEBGL)
  internal class VideoRenderContainer {
    public System.Windows.Controls.Image View { get; set; }
    public TRTCVideoRender Render { get; set; }
  }
#endif
}