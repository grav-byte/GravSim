//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once

#include <string>

#include "miniaudio.h"
#include "Core/AppLayer.h"

class AudioLayer : public Core::AppLayer {
public:
    AudioLayer();
    ~AudioLayer() override;

    std::string currentSongTitle;

    void PlaySound(const char *path);
    void SetVolume(float volume);
    float GetVolume();


    static void OnSongFinished(void * data, ma_sound * p_sound) {
        AudioLayer* audioSystem = static_cast<AudioLayer*>(data);
        audioSystem->shouldPlayNextSong = true;
    }

    void OnInit() override;
    void OnUpdate(float deltaTime) override;

    void PlaySong(const char *soundFilePath, ma_sound_end_proc onEndCallback=nullptr);
    void NextSong();
    void PlayRandomSongFromDirectory(const char* directoryPath, ma_sound_end_proc onEndCallback=nullptr);

    void OnEvent(Core::Event &event) override;

    void OnRender() override;

private:
    bool shouldPlayNextSong;
    ma_engine engine;
    ma_sound currentSong;
    const char* musicDirectory;
};
