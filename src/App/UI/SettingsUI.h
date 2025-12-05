//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include <memory>

#include "IUserInterface.h"
#include "App/AudioLayer.h"


class SettingsUI : public IUserInterface {
public:
    SettingsUI();
    ~SettingsUI() override;

    void Render() override;

private:
    AudioLayer* audioSystem_;

    float volume_;
};
