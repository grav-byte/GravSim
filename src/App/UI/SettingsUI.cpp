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
    audioSystem = std::weak_ptr(engineLayer->audioSystem);
    if (auto sys = audioSystem.lock()) {
        // access safely
        volume = sys->GetVolume();
    }
}

SettingsUI::~SettingsUI() = default;

void SettingsUI::Render() {

    ImGui::Begin("Settings");

    ImGui::Text("Audio Settings");
    if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f)) {
        if (auto sys = audioSystem.lock()) {
            // access safely
            sys->SetVolume(volume);
        }
    }
    ImGui::End();
}