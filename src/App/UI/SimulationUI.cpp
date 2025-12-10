//
// Created by Lorenz Saalmann on 08.12.25.
//

#include "SimulationUI.h"

#include "imgui.h"
#include "App/Layers/EngineLayer.h"
#include "App/Rendering/TextureLoader.h"
#include "Core/Application.h"

SimulationUI::SimulationUI() {
    engine_ = Core::Application::Get().GetLayer<EngineLayer>();
    stepsPerSec_ = 120;
    btnBgColor_ = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    btnTintColor_ = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    btnDisabledColor_ = ImVec4(1.0f, 1.0f, 1.0f, .4f);

    activePropagatorIdx_ = 0;
}

void SimulationUI::OnEvent(Core::Event &event) {
}

void SimulationUI::Draw() {
    const float width  = 370.0f;
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
    bool isRunning = engine_->IsRunningSimulation();
    bool isPaused = engine_->IsSimulationPaused();

    if (ImageBtn("../assets/icons/play.png", isRunning && !isPaused, "Start simulation")) {
        engine_->StartSimulation();
    }

    if (!isPaused) {
        if (ImageBtn("../assets/icons/pause.png", !isRunning, "Pause simulation")) {
            engine_->PauseSimulation();
        }
    } else {
        if (ImageBtn("../assets/icons/skip.png", false, "Step simulation")) {
            engine_->StepSimulation();
        }
    }

    if (ImageBtn("../assets/icons/reset.png", !isRunning, "Stop and reset simulation")) {
        engine_->StopSimulation();
    }

    ImGui::SameLine();

    auto names = PhysicsSolver::GetPropagatorNames();

    ImGui::SetNextItemWidth(100.0f);
    if (ImGui::Combo("Solver", &activePropagatorIdx_, names.data(), names.size())) {
        engine_->SetTimeStep(stepsPerSec_);
    }

    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Integration method");
    }

    ImGui::SameLine();

    ImGui::SetNextItemWidth(50.0f);
    if (ImGui::InputInt("steps/s", &stepsPerSec_, 0, 0)) {
        engine_->SetTimeStep(1.0f / stepsPerSec_);
    }

    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Time step: %.2f ms", 1000.0f / stepsPerSec_);
    }

    ImGui::End();
}

bool SimulationUI::ImageBtn(const std::string &texturePath, bool disabled, const char* tooltip) const {
    const auto tex = TextureLoader::GetTexture(texturePath);
    constexpr auto size = ImVec2(16, 16);
    constexpr auto uv0 = ImVec2(0, 1);
    constexpr auto uv1 = ImVec2(1, 0);

    ImGui::BeginDisabled(disabled);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
    ImGui::SameLine();
    ImGui::PushID(static_cast<int>(tex.id));
    bool pressed = ImGui::ImageButton("##btn", tex.id, size, uv0, uv1, btnBgColor_, disabled ? btnDisabledColor_ : btnTintColor_);

    ImGui::EndDisabled();

    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", tooltip);
    }

    ImGui::PopID();
    ImGui::PopStyleVar();
    return pressed;
}
