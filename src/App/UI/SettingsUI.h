//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include <memory>

#include "IUserInterfaceWindow.h"
#include "App/AudioSystem.h"


class SettingsUI : public IUserInterfaceWindow {
public:
    SettingsUI();
    ~SettingsUI() override;

    void Render() override;

private:
    std::weak_ptr<AudioSystem> audioSystem;

    float volume;
};
