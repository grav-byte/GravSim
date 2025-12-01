//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include "miniaudio.h"

class AudioSystem {
public:
    AudioSystem();
    ~AudioSystem();

    void PlaySound(const char* soundFilePath);
    void PlayRandomFileFromDirectory(const char* directoryPath);

private:
    ma_engine engine;
};
