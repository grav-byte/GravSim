//
// Created by Lorenz Saalmann on 01.12.25.
//

#include "SettingsUI.h"

#include "imgui.h"
#include "Core/Application.h"

class EngineLayer;

SettingsUI::SettingsUI() {
    audioLayer_ = Core::Application::Get().GetLayer<AudioLayer>();
    engineLayer_ = Core::Application::Get().GetLayer<EngineLayer>();
    volume_ = .25f;
    zoomToMouse_ = true;
    engineLayer_->GetCameraController()->SetZoomToMouse(zoomToMouse_);
    audioLayer_->SetVolume(.5f);
}

SettingsUI::~SettingsUI() = default;

void SettingsUI::Draw() {

    ImGui::Begin("Settings");
    ImGui::Text("Settings");

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::SeparatorText("Camera");
    if (ImGui::Checkbox("Zoom to Mouse", &zoomToMouse_)) {
        engineLayer_->GetCameraController()->SetZoomToMouse(zoomToMouse_);
    }

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::SeparatorText("Audio");
    if (ImGui::SliderFloat("Volume", &volume_, 0.0f, 1.0f)) {
        audioLayer_->SetVolume(volume_);
    }
    ImGui::TextWrapped("Now Playing: \n%s", audioLayer_->currentSongTitle.c_str());
    if (ImGui::Button("Skip Song")) {
        audioLayer_->NextSong();
    }
    ImGui::End();
}

void SettingsUI::OnEvent(Core::Event &event) {
    // No event handling needed for settings currently

}
