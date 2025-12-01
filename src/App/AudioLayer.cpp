//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "AudioLayer.h"

#include <filesystem>
#include <iostream>
#include <random>

#include "miniaudio.h"

AudioLayer::AudioLayer() : AppLayer() {
    musicDirectory = "../assets/audio/portal2_soundtrack";
    currentSongTitle = "";
    shouldPlayNextSong = false;
    currentSong = ma_sound();
    engine = ma_engine();
    ma_result result = ma_engine_init(nullptr, &engine);
    if (result != MA_SUCCESS) {
        printf("ma_engine_init failed\n");
    }
}

AudioLayer::~AudioLayer() {
    if (ma_sound_is_playing(&currentSong)) {
        ma_sound_uninit(&currentSong);
    }
    ma_engine_uninit(&engine);
}

void AudioLayer::PlaySound(const char* path) {
    ma_engine_play_sound(&engine, path, nullptr);
}

void AudioLayer::SetVolume(float volume) {
    ma_engine_set_volume(&engine, volume);
}

float AudioLayer::GetVolume() {
    return ma_engine_get_volume(&engine);
}

void AudioLayer::OnInit() {
    NextSong();
}

void AudioLayer::OnUpdate(float deltaTime) {
    if (shouldPlayNextSong) {
        shouldPlayNextSong = false;
        NextSong();
    }
}

void AudioLayer::PlaySong(const char *soundFilePath, ma_sound_end_proc onEndCallback) {
    ma_sound_uninit(&currentSong);
    ma_result result = ma_sound_init_from_file(&engine, soundFilePath, 0, nullptr, nullptr, &currentSong);
    if (result != MA_SUCCESS) {
        printf("ma_sound_init failed\n");
    }
    ma_sound_start(&currentSong);
    if (onEndCallback != nullptr) {
        ma_sound_set_end_callback(&currentSong, onEndCallback, this);
    }
}

void AudioLayer::NextSong() {
    PlayRandomSongFromDirectory(musicDirectory, OnSongFinished);
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