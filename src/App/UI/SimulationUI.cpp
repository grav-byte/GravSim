//
// Created by Lorenz Saalmann on 08.12.25.
//

#include "SimulationUI.h"

#include <iostream>

#include "imgui.h"
#include "implot.h"
#include "App/Layers/EngineLayer.h"
#include "App/Rendering/TextureLoader.h"
#include "Core/Application.h"

SimulationUI::SimulationUI() {
    engine_ = Core::Application::Get().GetLayer<EngineLayer>();
    stepsPerSec_ = 120;
    btnBgColor_ = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    btnTintColor_ = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    btnDisabledColor_ = ImVec4(1.0f, 1.0f, 1.0f, .4f);
    showPlot_ = false;
    plotHideTime_ = 0.0f;

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
    ImGui::SetNextWindowSize(ImVec2(width, -1), ImGuiCond_Always);

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

    const auto names = PhysicsSolver::GetPropagatorNames();

    ImGui::SetNextItemWidth(100.0f);
    if (ImGui::BeginCombo("Solver", names[activePropagatorIdx_])) {
        for (int i = 0; i < names.size(); ++i) {
            bool selected = (i == activePropagatorIdx_);
            if (ImGui::Selectable(names[i], selected))
                activePropagatorIdx_ = i;
            if (selected)
                ImGui::SetItemDefaultFocus();
        }

        // combo popup is open — keep plot visible
        plotHideTime_ = Core::Application::GetTime() + 0.2f;
        ImGui::EndCombo();
    }

    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Integration method");
        plotHideTime_ = Core::Application::GetTime() + .2f;
    }

    ImGui::SameLine();

    ImGui::SetNextItemWidth(50.0f);
    if (ImGui::InputInt("steps/s", &stepsPerSec_, 0, 0)) {
        engine_->SetTimeStep(1.0f / stepsPerSec_);
    }

    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Time step: %.2f ms", 1000.0f / stepsPerSec_);
    }

    float totalTime = 10.0f;
    auto results = engine_->GetActivePropagator()->RunTest(1.0f / stepsPerSec_, totalTime);
    int count = static_cast<int>(results.size());
    float trueValues[count];
    float timeValues[count];
    for (int i = 0; i < count; ++i) {
        const auto t = i * (1.0f / stepsPerSec_);
        timeValues[i] = t;
        trueValues[i] = 0.5f * -9.81f * std::pow(t, 2);
    }

    showPlot_ = Core::Application::GetTime() < plotHideTime_;

    if (showPlot_ && ImPlot::BeginPlot("Object from y=0 with gravity -9.81 m/s²", "t in s", "y in m", ImVec2(-1,0), ImPlotFlags_NoInputs)) {
        ImPlot::SetupLegend(ImPlotLocation_NorthEast);

        ImPlot::PushStyleVar(ImPlotStyleVar_Marker, ImPlotMarker_Cross);
        ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 4.0f);
        ImPlot::PlotScatter(names[activePropagatorIdx_], timeValues, results.data(), count);
        ImPlot::PopStyleVar(2);
        ImPlot::PlotLine("True Values", timeValues, trueValues, count);
        ImPlot::EndPlot();

        if (ImGui::IsItemHovered()) {
            plotHideTime_ = Core::Application::GetTime() + .1f;
        }
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
