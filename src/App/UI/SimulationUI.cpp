//
// Created by Lorenz Saalmann on 08.12.25.
//

#include "SimulationUI.h"

#include "imgui.h"

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

    ImGui::End();
}
