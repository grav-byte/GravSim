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

void AudioLayer::PlaySound(const char* path, const float volume) {
    auto* s = new OneShotSound;

    if (ma_sound_init_from_file(&engine_, path, 0, nullptr, nullptr, &s->sound) != MA_SUCCESS) {
        delete s;
        return;
    }

    ma_sound_set_volume(&s->sound, volume);

    ma_sound_set_end_callback(
        &s->sound,
        [](void* userData, ma_sound*) {
            static_cast<OneShotSound*>(userData)->finished.store(true, std::memory_order_relaxed);
        },
        s
    );

    ma_sound_start(&s->sound);

    pendingCleanup_.push_back(s); // store pointer
}

std::unique_ptr<ma_sound> AudioLayer::PlaySoundRepeating(const char *path) {
    auto sound = std::make_unique<ma_sound>();

    const ma_result result = ma_sound_init_from_file(&engine_, path, MA_SOUND_FLAG_LOOPING, nullptr, nullptr, sound.get());
    if (result != MA_SUCCESS) {
        printf("ma_sound_init looping failed\n");
        return sound;
    }
    ma_sound_start(sound.get());
    return sound;
}

void AudioLayer::AdjustSoundVolume(const float volume, ma_sound* sound) {
    ma_sound_set_volume(sound, volume);
}

void AudioLayer::AdjustSoundPitch(const float pitch, ma_sound* sound) {
    ma_sound_set_pitch(sound, pitch);
}

void AudioLayer::StopSound(std::unique_ptr<ma_sound> sound) {
    if (!sound) return;
    soundsToStop_.push_back(std::move(sound));
}

void AudioLayer::SetGlobalVolume(float volume) {
    ma_engine_set_volume(&engine_, volume);
}

float AudioLayer::GetGlobalVolume() {
    return ma_engine_get_volume(&engine_);
}

void AudioLayer::OnInit() {
    NextSong();
}

void AudioLayer::OnUpdate(float deltaTime) {
    // Stop any sounds scheduled for stopping
    for (auto& s : soundsToStop_) {
        ma_sound_stop(s.get());
        ma_sound_uninit(s.get());
    }
    soundsToStop_.clear();


    // cleanup sounds that are done
    for (auto it = pendingCleanup_.begin(); it != pendingCleanup_.end(); ) {
        if ((*it)->finished.load(std::memory_order_relaxed)) {
            ma_sound_uninit(&(*it)->sound);
            delete *it;
            it = pendingCleanup_.erase(it);
        } else {
            ++it;
        }
    }

    if (shouldPlayNextSong_) {
        shouldPlayNextSong_ = false;
        NextSong();
    }
}

void AudioLayer::PlaySong(const char *soundFilePath, ma_sound_end_proc onEndCallback) {
    ma_sound_stop(&currentSong_);
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
    const fs::path randomFile = audioFiles[dist(gen)];

    currentSongTitle = randomFile.filename().string();

    // Play it
    PlaySong(randomFile.string().c_str(), onEndCallback);
}

void AudioLayer::OnEvent(Core::Event &event) {
}

void AudioLayer::OnRender() {
}