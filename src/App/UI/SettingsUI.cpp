//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "SettingsUI.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "App/EngineLayer.h"
#include "Core/Application.h"

SettingsUI::SettingsUI() {
    auto engineLayer = Core::Application::Get().GetLayer<EngineLayer>();
    auto audioLayer = Core::Application::Get().GetLayer<AudioLayer>();
    audioSystem_ = audioLayer;
    volume_ = .7f;
    audioSystem_->SetVolume(.7f);
}

SettingsUI::~SettingsUI() = default;

void SettingsUI::Render() {

    ImGui::Begin("Settings");
    ImGui::SeparatorText("Audio");
    if (ImGui::SliderFloat("Volume", &volume_, 0.0f, 1.0f)) {
        audioSystem_->SetVolume(volume_);
    }
    ImGui::TextWrapped("Now Playing: \n%s", audioSystem_->currentSongTitle.c_str());
    if (ImGui::Button("Skip Song")) {
        audioSystem_->NextSong();
    }
    ImGui::End();
}