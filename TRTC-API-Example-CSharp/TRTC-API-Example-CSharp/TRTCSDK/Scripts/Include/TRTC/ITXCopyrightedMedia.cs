/**
 * Copyright (c) 2023 Tencent. All rights reserved.
 * Module: 音速达版权音乐
 * Function: 用于下载音速达版权音乐数据
 * 此功能默认没有打包到 SDK 中，如果想使用此文件中的功能，联系腾讯单独提供 SDK。
 */
using System;
namespace trtc {

    public interface ITXMusicPreloadCallback {
        /**
         *  版权音乐开始下载
         */
        void onPreloadStart(string musicId, string bitrateDefinition);

        /**
         *  版权音乐下载进度回调
         */
        void onPreloadProgress(string musicId, string bitrateDefinition, float progress);

        /**
         *  版权音乐下载完成回调
         */
        void onPreloadComplete(string musicId, string bitrateDefinition, int errorCode, string msg);
    }

    public abstract class ITXCopyrightedMedia {
        /**
         * 创建版权音乐实例
         */
        public static ITXCopyrightedMedia createCopyRightMedia() {
            return new TXCopyrightedMediaImplement();
        }

        /**
         * 销毁版权音乐实例
         */
        public abstract void destroyCopyRightMedia();

        /**
         * 设置 license
         *
         * @param key
         * @param license_url
         */
        public abstract int setCopyrightedLicense(string key, string licenseUrl);

        /**
         * 生成音乐
         *
         * URI，App客户端，播放时候调用，传给trtc进行播放。与preloadMusic一一对应
         * @param musicId 歌曲Id
         * @param bgmType 0：原唱，1：伴奏  2:  歌词  3: 音高文件  4: 原唱高潮 5: 伴奏高潮
         * @param bitrateDefinition 码率，传nil为改音频默认码率
         * @param out 用户传入的 buffer，用来存放 genMusicURI 返回的字符串
         * @param out_size 用户 buffer 的大小
         * @return 成功：true 失败：false
         * 由于 ios 的接口样式为 virtual const char* genMusicURI(const
         *     *char*musicId,int bgmType, const char* bitrateDefinition) = 0; 但是 C++
         * 这样是不安全的，因此此接口无法对齐 ios
         */
        public abstract bool genMusicURI(string musicId, int bgmType, string bitrateDefinition, IntPtr outData, int outDataSize);

        /**
         * 设置预加载回调函数
         *
         * @param callback 回调结束后响应对象
         */
        public abstract void setMusicPreloadCallback(ITXMusicPreloadCallback callback);

        /**
         * 预加载音乐数据。
         *
         * @param musicId 歌曲Id，通过 AME 后台获取
         * @param playToken 播放Token，通过 AME 后台获取
         * @param bitrateDefinition 码率，传nil为音频默认码率，一般格式为：audio/mi:
         * 32,audio/lo: 64,audio/hi: 128
         */
        public abstract int preloadMusic(string musicId, string bitrateDefinition, string playToken);

        /**
         * 取消预加载音乐数据。
         *
         * @param musicId 歌曲Id，通过 AME 后台获取
         * @param bitrateDefinition 码率，传nil为改音频默认码率
         */
        public abstract int cancelPreloadMusic(string musicId, string bitrateDefinition);

        /**
         * 检测是否已预加载音乐数据。
         *
         * @param musicId 歌曲Id
         * @param bitrateDefinition 码率，传nil为改音频默认码率
         */
        public abstract bool isMusicPreload(string musicId, string bitrateDefinition);

        /**
         * 清理音乐缓存
         */
        public abstract int clearMusicCache();

        /**
         * 设置最大歌曲缓存数目，默认100
         *
         * @param maxCount 歌曲最大数目
         */
        public abstract int setMusicCacheMaxCount(int maxCount);
    }
}
