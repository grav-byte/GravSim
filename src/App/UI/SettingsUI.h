//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once

#include "IUserInterface.h"
#include "../Layers/AudioLayer.h"
#include "App/Layers/EngineLayer.h"


class SettingsUI : public IUserInterface {
public:
    SettingsUI();
    ~SettingsUI() override;

    void Draw() override;
    void OnEvent(Core::Event &event) override;

private:
    AudioLayer* audioLayer_;
    EngineLayer* engineLayer_;
    float volume_;
    bool zoomToMouse_;
};
