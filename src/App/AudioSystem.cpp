//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "AudioSystem.h"

#include <filesystem>
#include <iostream>
#include <random>

#include "miniaudio.h"

AudioSystem::AudioSystem() {
    engine = ma_engine();
    ma_result result = ma_engine_init(nullptr, &engine);
    if (result != MA_SUCCESS) {
        printf("ma_engine_init failed\n");
    }
}

AudioSystem::~AudioSystem() {
    ma_engine_uninit(&engine);
}

void AudioSystem::PlaySound(const char *soundFilePath) {
    ma_engine_play_sound(&engine, soundFilePath, nullptr);
}

void AudioSystem::PlayRandomFileFromDirectory(const char *directoryPath) {
    namespace fs = std::filesystem;

    std::vector<fs::path> audioFiles;

    // Collect all files in the directory
    try {
        for (const auto &entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
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

    // Play it
    PlaySound(randomFile.string().c_str());
}

