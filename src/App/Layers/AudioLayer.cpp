//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "AudioLayer.h"

#include <filesystem>
#include <iostream>
#include <random>

#include "miniaudio.h"

AudioLayer::AudioLayer() : AppLayer() {
    musicDirectory_ = "../assets/audio/portal2_soundtrack";
    currentSongTitle = "";
    shouldPlayNextSong_ = false;
    currentSong_ = ma_sound();
    engine_ = ma_engine();
    ma_result result = ma_engine_init(nullptr, &engine_);
    if (result != MA_SUCCESS) {
        printf("ma_engine_init failed\n");
    }
}

AudioLayer::~AudioLayer() {
    if (ma_sound_is_playing(&currentSong_)) {
        ma_sound_uninit(&currentSong_);
    }
    ma_engine_uninit(&engine_);
}

void AudioLayer::PlaySound(const char* path) {
    ma_engine_play_sound(&engine_, path, nullptr);
}

void AudioLayer::SetVolume(float volume) {
    ma_engine_set_volume(&engine_, volume);
}

float AudioLayer::GetVolume() {
    return ma_engine_get_volume(&engine_);
}

void AudioLayer::OnInit() {
    NextSong();
}

void AudioLayer::OnUpdate(float deltaTime) {
    if (shouldPlayNextSong_) {
        shouldPlayNextSong_ = false;
        NextSong();
    }
}

void AudioLayer::PlaySong(const char *soundFilePath, ma_sound_end_proc onEndCallback) {
    ma_sound_uninit(&currentSong_);
    ma_result result = ma_sound_init_from_file(&engine_, soundFilePath, 0, nullptr, nullptr, &currentSong_);
    if (result != MA_SUCCESS) {
        printf("ma_sound_init failed\n");
    }
    ma_sound_start(&currentSong_);
    if (onEndCallback != nullptr) {
        ma_sound_set_end_callback(&currentSong_, onEndCallback, this);
    }
}

void AudioLayer::NextSong() {
    PlayRandomSongFromDirectory(musicDirectory_, OnSongFinished);
}

void AudioLayer::PlayRandomSongFromDirectory(const char *directoryPath, ma_sound_end_proc onEndCallback) {
    namespace fs = std::filesystem;

    std::vector<fs::path> audioFiles;

    // Collect all files in the directory
    try {
        for (const auto &entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".mp3") {
                audioFiles.push_back(entry.path());
            }
        }
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error accessing directory: " << e.what() << "\n";
        return;
    }

    if (audioFiles.empty()) {
        std::cerr << "No audio files found in directory: " << directoryPath << "\n";
        return;
    }

    // Pick a random file
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, audioFiles.size() - 1);
    fs::path randomFile = audioFiles[dist(gen)];

    currentSongTitle = randomFile.filename().string();

    // Play it
    PlaySong(randomFile.string().c_str(), onEndCallback);
}

void AudioLayer::OnEvent(Core::Event &event) {
}

void AudioLayer::OnRender() {
}