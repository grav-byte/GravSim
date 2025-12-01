//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once

#include "miniaudio.h"

class AudioSystem {
public:
    void OnSoundFinished(ma_sound *pSound, void *pUserData);

    AudioSystem();
    ~AudioSystem();

    void PlaySound(const char *path);
    void SetVolume(float volume);
    float GetVolume();

    static void OnSongFinished(void * data, ma_sound * p_sound) {
        AudioSystem* audioSystem = static_cast<AudioSystem*>(data);
        audioSystem->StartMusic();
    }
    void PlaySong(const char *soundFilePath, ma_sound_end_proc onEndCallback=nullptr);
    void StartMusic();
    void PlayRandomSongFromDirectory(const char* directoryPath, ma_sound_end_proc onEndCallback=nullptr);

private:
    ma_engine engine;
    ma_sound currentSong;
    const char* musicDirectory;
};
