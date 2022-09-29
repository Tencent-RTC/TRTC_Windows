#ifndef __ITRTCAUDIOEFFECTMANAGER_H__
#define __ITRTCAUDIOEFFECTMANAGER_H__
#include <windows.h>
// C++ Header
#include "include/TRTC/ITXAudioEffectManager.h"

class AudioMusicParam;
enum TXVoiceReverbType;
using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
class TXMusicPlayObserverImpl;
namespace ManageLiteAV
{
    /// @defgroup ITXAudioEffectManager_csharp ITXAudioEffectManager
    /// 腾讯云视频通话功能音乐和人声设置接口
    /// @{
    public ref class AudioMusicParam {
    public:
        /// <summary>
        /// 【字段含义】音乐 ID
        /// 【特殊说明】SDK 允许播放多路音乐，因此需要音乐 ID 进行标记，用于控制音乐的开始、停止、音量等
        /// </summary>
        int id;
        /// <summary>
        /// 【字段含义】音乐文件的绝对路径
        /// </summary>
        String^ path;

        /// <summary>
        /// 【字段含义】音乐循环播放的次数
        /// 【推荐取值】取值范围为0 - 任意正整数，默认值：0。0表示播放音乐一次；1表示播放音乐两次；以此类推
        /// </summary>
        int loopCount;

        /// <summary>
        /// 【字段含义】是否将音乐传到远端
        /// 【推荐取值】YES：音乐在本地播放的同时，会上行至云端，因此远端用户也能听到该音乐；NO：音乐不会上行至云端，因此只能在本地听到该音乐。默认值：NO
        /// </summary>
        bool publish;

        /// <summary>
        /// 【字段含义】播放的是否为短音乐文件
        /// 【推荐取值】YES：需要重复播放的短音乐文件；NO：正常的音乐文件。默认值：NO
        /// </summary>
        bool isShortFile;

        /// <summary>
        /// 【字段含义】音乐开始播放时间点，单位毫秒
        /// </summary>
        long startTimeMS;

        /// <summary>
        /// 【字段含义】音乐结束播放时间点，单位毫秒，0表示播放至文件结尾。
        /// </summary>
        long endTimeMS;

        AudioMusicParam(int id_, String^ path_) {
            path = path_;
            id = id_;
            loopCount = 0;
            publish = false;
            isShortFile = false;
            startTimeMS = 0;
            endTimeMS = 0;
        }
    };


    public enum class TXVoiceReverbType
    {
        /// <summary>
        /// 关闭混响
        /// </summary>
        TXLiveVoiceReverbType_0 = 0,
        /// <summary>
        /// KTV
        /// </summary>
        TXLiveVoiceReverbType_1 = 1,
        /// <summary>
        /// 小房间
        /// </summary>
        TXLiveVoiceReverbType_2 = 2,
        /// <summary>
        /// 大会堂
        /// </summary>
        TXLiveVoiceReverbType_3 = 3,
        /// <summary>
        /// 低沉
        /// </summary>
        TXLiveVoiceReverbType_4 = 4,
        /// <summary>
        /// 洪亮
        /// </summary>
        TXLiveVoiceReverbType_5 = 5,
        ///<summary>
        /// 磁性
        /// </summary>
        TXLiveVoiceReverbType_6 = 6,
    };

    public interface class ITXMusicPlayObserver {
    public:
       
        /// <summary>
        /// 背景音乐开始播放
        /// </summary>
        void onStart(int id, int errCode);

        /// <summary>
        /// 背景音乐的播放进度
        /// </summary>
        void onPlayProgress(int id, long curPtsMS, long durationMS);

        /// <summary>
        /// 背景音乐已播放完毕
        /// </summary>
        void onComplete(int id, int errCode);
    };


    public ref class ITXAudioEffectManager
    {
    protected:
        ~ITXAudioEffectManager();

    public:
        ITXAudioEffectManager();
        ITXAudioEffectManager(liteav::ITXAudioEffectManager *manager);
        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （一）人声相关特效函数
        //
        /////////////////////////////////////////////////////////////////////////////////

        /// @name 人声相关特效函数
        /// @{

        /// <summary>
        /// 设置人声的混响效果（KTV、小房间、大会堂、低沉、洪亮...）
        /// </summary>
        void setVoiceReverbType(TXVoiceReverbType type);

        /// <summary>
        /// 1.2 设置麦克风采集人声的音量
        /// </summary>
        /// <param name="volume">音量大小，100为正常音量，取值范围为0 - 100；默认值：100</param>
        void setVoiceCaptureVolume(int volume);
        /// @}

        /////////////////////////////////////////////////////////////////////////////////
        //
        //                      （二）背景音乐特效函数
        //
        /////////////////////////////////////////////////////////////////////////////////
        /// @name 背景音乐特效函数
        /// @{

        /// <summary>
        /// 2.1 设置背景音乐的播放进度回调接口
        /// </summary>
        /// <param name="id">音乐 ID</param>
        /// <param name="observer">具体参考 ITXMusicPlayObserver 中定义接口</param>
        void setMusicObserver(int id, ITXMusicPlayObserver^ observer);

        /// <summary>
        /// 2.2 开始播放背景音乐
        /// 每个音乐都需要您指定具体的 ID，您可以通过该 ID 对音乐的开始、停止、音量等进行设置。
        /// </summary>
        /// <remarks>
        /// 若您想同时播放多个音乐，请分配不同的 ID 进行播放。
        /// 如果使用同一个 ID 播放不同音乐，SDK 会先停止播放旧的音乐，再播放新的音乐。
        /// </remarks>
        /// <param name="musicParam">音乐参数</param>
        void startPlayMusic(AudioMusicParam^ musicParam);

        /// <summary>
        /// 2.3 停止播放背景音乐
        /// </summary>
        /// <param name="id">音乐 ID</param>
        void stopPlayMusic(int id);

        /// <summary>
        /// 2.4 暂停播放背景音乐
        /// </summary>
        /// <param name="id">音乐 ID</param>
        void pausePlayMusic(int id);

        /// <summary>
        /// 2.5 恢复播放背景音乐
        /// </summary>
        /// <param name="id">音乐 ID</param>
        void resumePlayMusic(int id);

        /// <summary>
        /// 2.6 设置背景音乐的远端音量大小，即主播可以通过此接口设置远端观众能听到的背景音乐的音量大小。
        /// </summary>
        /// <param name="id">音乐 ID</param>
        /// <param name="volume">音量大小，100为正常音量，取值范围为0 - 100；默认值：100</param>
        void setMusicPublishVolume(int id, int volume);

        /// <summary>
        /// 2.7 设置背景音乐的本地音量大小，即主播可以通过此接口设置主播自己本地的背景音乐的音量大小。
        /// </summary>
        /// <param name="id">音乐 ID</param>
        /// <param name="volume">音量大小，100为正常音量，取值范围为0 - 100；默认值：100</param>
        void setMusicPlayoutVolume(int id, int volume);


        /// <summary>
        /// 2.8 设置全局背景音乐的本地和远端音量的大小
        /// </summary>
        /// <param name="volume">音量大小，100为正常音量，取值范围为0 - 100；默认值：100</param>
        void setAllMusicVolume(int volume);

        /// <summary>
        /// 2.9 调整背景音乐的音调高低
        /// </summary>
        /// <param name="id">音乐 ID</param>
        /// <param name="pitch">音调，默认值是0.0f，范围是：[-1 ~ 1] 之间的浮点数；</param>
        void setMusicPitch(int id, float pitch);

        /// <summary>
        /// 2.10 调整背景音乐的变速效果
        /// </summary>
        /// <param name="id">音乐 ID</param>
        /// <param name="speedRate">速度，默认值是1.0f，范围是：[0.5 ~ 2] 之间的浮点数；</param>
        void setMusicSpeedRate(int id, float speedRate);

        /// <summary>
        /// 2.11 获取背景音乐当前的播放进度（单位：毫秒）
        /// </summary>
        /// <param name="id">音乐 ID</param>
        /// <returns>成功返回当前播放时间，单位：毫秒，失败返回-1</returns>
        long getMusicCurrentPosInMS(int id);

         /// <summary>
         /// 2.12 设置背景音乐的播放进度（单位：毫秒）
         /// 每个音乐都需要您指定具体的 ID，您可以通过该 ID 对音乐的开始、停止、音量等进行设置。
         /// </summary>
         /// <remarks>
         /// 请尽量避免频繁地调用该接口，因为该接口可能会再次读写音乐文件，耗时稍高。
         /// 当配合进度条使用时，请在进度条拖动完毕的回调中调用，而避免在拖动过程中实时调用。
         /// </remarks>
         /// <param name="id">音乐 ID</param>
         /// <param name="pts">单位: 毫秒</param>
        void seekMusicToPosInTime(int id, int pts);

        /// <summary>
        /// 2.13 获取景音乐文件的总时长（单位：毫秒）
        /// </summary>
        /// <param name="path">音乐文件路径，如果 path 为空，那么返回当前正在播放的 music 时长。</param>
        /// <returns>成功返回时长，失败返回-1</returns>
        long getMusicDurationInMS(String^ path);

    private:
        !ITXAudioEffectManager();

        //ITXMusicPlayObserver
        delegate void StartDelegate(int id, int errCode);
        delegate void PlayProgressDelegate(int id, long curPtsMS, long durationMS);
        delegate void CompleteDelegate(int id, int errCode);

        void onStart(int id, int errCode);
        void onPlayProgress(int id, long curPtsMS, long durationMS);
        void onComplete(int id, int errCode);
    private:
        static ITXAudioEffectManager^ sInstance;
        static Object^ slocker = gcnew Object();

    private:
        liteav::ITXAudioEffectManager* audio_effect_manager_;

        TXMusicPlayObserverImpl* music_play_observer_impl_;

        ITXMusicPlayObserver^ music_play_observer_callback_;
        
        // ITRTCCloudCallback
        StartDelegate^ startDelegate_;
        PlayProgressDelegate^ playProgressDelegate_;
        CompleteDelegate^ completeDelegate_;

    };
    /// @}
}

#endif /* __ITXAUDIOEFFECTMANAGER_H__ */
