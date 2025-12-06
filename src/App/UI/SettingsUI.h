//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once

#include "IUserInterface.h"
#include "../Layers/AudioLayer.h"


class SettingsUI : public IUserInterface {
public:
    SettingsUI();
    ~SettingsUI() override;

    void Render() override;
    void OnEvent(Core::Event &event) override;

private:
    AudioLayer* audioSystem_;
    float volume_;
};
