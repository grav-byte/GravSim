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
    audioSystem = audioLayer;
    volume = audioSystem->GetVolume();
}

SettingsUI::~SettingsUI() = default;

void SettingsUI::Render() {

    ImGui::Begin("Settings");
    ImGui::SeparatorText("Audio");
    if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f)) {
        audioSystem->SetVolume(volume);
    }
    ImGui::Text("Now Playing: %s", audioSystem->currentSongTitle.c_str());
    if (ImGui::Button("Skip Song")) {
        audioSystem->NextSong();
    }
    ImGui::End();
}