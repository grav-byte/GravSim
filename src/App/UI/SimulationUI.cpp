//
// Created by Lorenz Saalmann on 08.12.25.
//

#include "SimulationUI.h"

#include "imgui.h"
#include "App/Rendering/TextureLoader.h"

SimulationUI::SimulationUI() {
    btnBgColor_ = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    btnTintColor_ = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    btnDisabledColor_ = ImVec4(1.0f, 1.0f, 1.0f, .4f);
}

void SimulationUI::OnEvent(Core::Event &event) {
}

void SimulationUI::Draw() {
    const float width  = 200.0f;
    const float height = 45.0f;

    const ImGuiViewport* vp = ImGui::GetMainViewport();

    // center horizontally relative to the main window
    ImVec2 pos(
        vp->Pos.x + (vp->Size.x - width) * 0.5f,
        vp->Pos.y + 10.0f
    );

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("Simulation", nullptr, flags);

    ImGui::ArrowButton("##play", ImGuiDir_Right);
    DrawImageBtn("../assets/icons/pause.png", true);
    DrawImageBtn("../assets/icons/stop.png", true);

    ImGui::End();
}

void SimulationUI::DrawImageBtn(const std::string &texturePath, bool disabled) const {
    const auto tex = TextureLoader::GetTexture(texturePath);
    constexpr auto size = ImVec2(16.0f, 16.0f);
    constexpr auto uv0 = ImVec2(0.0f, 0.0f);
    constexpr auto uv1 = ImVec2(1, 1);

    ImGui::BeginDisabled(disabled);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
    ImGui::SameLine();
    ImGui::PushID(static_cast<int>(tex.id));
    ImGui::ImageButton("##btn", tex.id, size, uv0, uv1, btnBgColor_, disabled ? btnDisabledColor_ : btnTintColor_);

    ImGui::EndDisabled();

    ImGui::PopID();    ImGui::PopStyleVar();
}
