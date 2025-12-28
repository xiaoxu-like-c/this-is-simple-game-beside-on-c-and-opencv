#pragma once
#include<iostream>
#include<thread>
#include <fstream>
#include <iostream>
#include<Windows.h>
//调用系统多媒体库播放音乐
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
// 音效播放辅助类
class all_music 
{
public:
    // 播放游戏循环音效 //初始化游戏对象时就开始战斗播放音乐
    //static void PlayMusic()
    //{
    //    static const std::string gameSound = "G:/game_resource/music.mp3\" repeat";//有问题
    //    PlaySoundAsync(gameSound);
    //}
    // 播放爆炸音效
    static void PlayExplosion()
    {
        static const std::string explosionSound = "G:/game_resource/explosion.mp3";
        PlaySoundAsync(explosionSound);
    }

    // 播放射击音效
    static void PlayShoot() 
    {
        static const std::string shootSound = "G:/game_resource//shoot2.mp3";
        PlaySoundAsync(shootSound);
    }

    //// 播放受伤音效
    //static void PlayHurt() {
    //    static const std::string hurtSound = "  ";
    //    PlaySoundAsync(hurtSound);
    //}
private:
    // 异步播放音效（不阻塞主循环）
    static void PlaySoundAsync(const std::string& filepath) {
        std::thread([filepath]() {
            std::ifstream file(filepath);
            if (file.good()) {
                std::string command = "play \"" + filepath + "\"";
                mciSendString(std::wstring(command.begin(), command.end()).c_str(), NULL, 0, NULL);
            }
            }).detach();
    }
};
