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

    std::unique_ptr<ma_sound> PlaySoundRepeating(const char *path);

    static void AdjustSoundVolume(float volume, ma_sound *sound);
    static void AdjustSoundPitch(float pitch, ma_sound *sound);
    void StopSound(std::unique_ptr<ma_sound> ma_sound);


    void SetGlobalVolume(float volume);
    float GetGlobalVolume();


    static void OnSongFinished(void * data, ma_sound * p_sound) {
        auto* audioSystem = static_cast<AudioLayer*>(data);
        audioSystem->shouldPlayNextSong_ = true;
    }

    void OnInit() override;
    void OnUpdate(float deltaTime) override;

    void PlaySong(const char *soundFilePath, ma_sound_end_proc onEndCallback=nullptr);
    void NextSong();
    void PlayRandomSongFromDirectory(const char* directoryPath, ma_sound_end_proc onEndCallback=nullptr);

    void OnEvent(Core::Event &event) override;

    void OnRender() override;

private:
    std::vector<std::unique_ptr<ma_sound>> soundsToStop_;
    bool shouldPlayNextSong_;
    ma_engine engine_;
    ma_sound currentSong_;
    const char* musicDirectory_;
};
