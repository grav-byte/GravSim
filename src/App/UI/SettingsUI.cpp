#include "SettingsUI.h"

#include "imgui.h"
#include "Core/Application.h"

class EngineLayer;

SettingsUI::SettingsUI() {
    audioLayer_ = Core::Application::Get().GetLayer<AudioLayer>();
    engineLayer_ = Core::Application::Get().GetLayer<EngineLayer>();
    volume_ = .25f;
    zoomToMouse_ = true;
    showGrid_ = true;
    engineLayer_->GetCameraController()->SetZoomToMouse(zoomToMouse_);
    audioLayer_->SetGlobalVolume(.5f);
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

    ImGui::SeparatorText("Grid");
    auto renderer = engineLayer_->GetSceneRenderer();
    if (ImGui::Checkbox("Show Grid", &showGrid_)) {
        renderer->showGrid = showGrid_;
    }

    // powers of 2
    int logSpacing = std::log2(renderer->gridSpacing_);
    if (ImGui::Button("-")) {
        logSpacing = std::max(-5, logSpacing - 1);
    }
    ImGui::SameLine();
    if (ImGui::Button("+")) {
        logSpacing = std::min(3, logSpacing + 1);
    }
    ImGui::SameLine();
    ImGui::Text("Grid Spacing");
    renderer->gridSpacing_ = std::pow(2, logSpacing);

    float value[4] = { renderer->gridColor_.x, renderer->gridColor_.y, renderer->gridColor_.z, renderer->gridColor_.a };
    if (ImGui::ColorEdit4("Grid Color", value, 0)) {
        renderer->gridColor_ = glm::vec4(value[0], value[1], value[2], value[3]);
    }

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::SeparatorText("Audio");
    if (ImGui::SliderFloat("Volume", &volume_, 0.0f, 1.0f)) {
        audioLayer_->SetGlobalVolume(volume_);
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
