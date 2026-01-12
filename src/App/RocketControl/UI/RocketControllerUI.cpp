#include "RocketControllerUI.h"
#include "imgui.h"
#include "App/Engine/Loading/Serialiser.h"
#include "App/Engine/Scene.h"
#include "App/Rendering/Visuals/ShaderVisual.h"
#include "App/Layers/EngineLayer.h"
#include "App/RocketControl/Controllers/AutonomousPIDRocketController.h"
#include "misc/cpp/imgui_stdlib.h"


RocketControllerUI::RocketControllerUI() : pidFileSelector_(FileSelector(std::filesystem::path("../assets/pid_parameters"))) {
    controlLayer_ = Core::Application::Get().GetLayer<ControlLayer>();
    engineLayer_  = Core::Application::Get().GetLayer<EngineLayer>();
}

void RocketControllerUI::Draw() {
    if (!controlLayer_ || !engineLayer_)
        return;

    Scene* scene = engineLayer_->GetScene();
    if (!scene)
        return;

    AutonomousPIDRocketController* autoCtrl = controlLayer_->GetAutoControl();
    auto rocket = controlLayer_->GetRocketObject();
    if (!autoCtrl || !rocket) return;

    constexpr ImVec2 sizeAuto(445, 300);

    // Controller / logic
    targetUI_.MirrorActiveTargetToController(autoCtrl);

    ImGui::SetNextWindowPos(ImVec2(1000, 580), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(sizeAuto, ImGuiCond_FirstUseEver);

    ImGui::Begin("Rocket Controller", nullptr, ImGuiWindowFlags_NoDocking);

    if (ImGui::BeginTabBar("RocketControllerTabs", ImGuiTabBarFlags_DrawSelectedOverline)) {
        if (ImGui::BeginTabItem("Control")) {
            ImGui::BeginChild("ControlContent", ImVec2(0, 0), false);
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Checkbox("Enable User Control", &controlLayer_->manualControlEnabled);

            if (controlLayer_->manualControlEnabled) {
                ImGui::TextWrapped("Manual mode active.");
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Text(" Controls:     +     |    -   \n "
                                 "Thrust:     Shift   |   Ctrl \n"
                                 " Angle:        A     |    D");

                ImGui::EndChild();
                ImGui::EndTabItem();
                ImGui::EndTabBar();
                ImGui::End();
                return;
            }

            ImGui::TextWrapped("Autonomous mode active.");
            ImGui::Separator();

            ImGui::Spacing();
            ImGui::Spacing();

            DrawPIDSettings(autoCtrl);
            ImGui::EndChild();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Target")) {
            ImGui::Spacing();
            ImGui::Spacing();

            targetUI_.Draw(scene, autoCtrl);

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void RocketControllerUI::DrawPIDSettings(AutonomousPIDRocketController *autoCtrl) {
    // array of PID controllers
    PIDController* pids[] = {
        autoCtrl->GetVerticalController(),
        autoCtrl->GetHorizontalController(),
        autoCtrl->GetAttitudeController()
    };

    DrawPIDLoading(pids, autoCtrl->maxSteeringAngle);

    ImGui::SeparatorText("Set PID Parameters");
    ImGui::Spacing();
    ImGui::TextLinkOpenURL("(Click for overview)", "https://github.com/grav-byte/GravSim/blob/master/docs/PIDOverview.png");
    ImGui::SameLine(250);
    ImGui::TextLinkOpenURL("(Click for implementation)", "https://github.com/grav-byte/GravSim/blob/master/docs/PIDImplementation.png");
    ImGui::Spacing();
    ImGui::SetNextItemWidth(100);
    ImGui::DragFloat("Max Steering Angle (°)", &autoCtrl->maxSteeringAngle, 0.1f, 0.f, 90.f);
    ImGui::Spacing();

    activePID = pids[0];

    // start table with 4 columns: label + 3 PIDs
    if (ImGui::BeginTable("PIDTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        const char* pidNames[] = { "Vertical", "Horizontal", "Attitude" };
        // header row
        ImGui::TableSetupColumn("Parameter");
        ImGui::TableSetupColumn(pidNames[0]);
        ImGui::TableSetupColumn(pidNames[1]);
        ImGui::TableSetupColumn(pidNames[2]);
        ImGui::TableHeadersRow();

        // Helper lambda to draw each gain row
        auto drawGainRow = [&](const char* label,
                       float* v0, float* v1, float* v2,
                       const float speed = 0.005f, const float min = 0.f, const float max = 1.f)
        {
            ImGui::PushID(label);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", label);

            auto drawCell = [&](int column, float* value) {
                ImGui::TableSetColumnIndex(column);
                if (!value) return;

                // set background color based on value
                const auto intensity = *value * 2.0f;
                const auto bgCol = ImVec4( intensity+ .2f,  .2f, .2f, 0.3f);

                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(bgCol));
                ImGui::PushID(column);
                ImGui::DragFloat("##v", value, speed, min, max);
                ImGui::PopID();
            };

            drawCell(1, v0);
            drawCell(2, v1);
            drawCell(3, v2);

            ImGui::PopID();
        };

        // Kp row
        drawGainRow("Kp",
                    &pids[0]->pidData.pGain,
                    &pids[1]->pidData.pGain,
                    &pids[2]->pidData.pGain);

        // Ki row
        drawGainRow("Ki",
                    &pids[0]->pidData.iGain,
                    &pids[1]->pidData.iGain,
                    &pids[2]->pidData.iGain);

        // Kd row
        drawGainRow("Kd",
                    &pids[0]->pidData.dGain,
                    &pids[1]->pidData.dGain,
                    &pids[2]->pidData.dGain);
        // Bias row
        drawGainRow("Bias",
            &pids[0]->pidData.bias,
            &pids[1]->pidData.bias,
            &pids[2]->pidData.bias);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Visualise");
        ImGui::TextColored(ImVec4(0,0,1,1),"P");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1,0,0,1),"I");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0,1,0,1),"D");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0,.4,.5,1),"B");


        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.05f, 0.05f, 0.05f, 1.0f)));
        ImGui::TableSetColumnIndex(1);
        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.05f, 0.05f, 0.05f, 1.0f)));
        ImGui::Checkbox("##vis0", &autoCtrl->visualizePID.x);
        ImGui::TableSetColumnIndex(2);
        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.05f, 0.05f, 0.05f, 1.0f)));
        ImGui::Checkbox("##vis1", &autoCtrl->visualizePID.y);
        ImGui::TableSetColumnIndex(3);
        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.05f, 0.05f, 0.05f, 1.0f)));
        ImGui::Checkbox("##vis2", &autoCtrl->visualizePID.z);

        ImGui::EndTable();
    }

    ImGui::Spacing();

    ImGui::InputText("Save Name", &saveName);
    if (ImGui::Button("Save")) {
        SaveConfig(autoCtrl);
    }
}

void RocketControllerUI::SaveConfig(const AutonomousPIDRocketController* autoCtrl) const {
    std::array<PIDData,3> pidDataArray;
    pidDataArray[0] = autoCtrl->GetVerticalController()->pidData;
    pidDataArray[1] = autoCtrl->GetHorizontalController()->pidData;
    pidDataArray[2] = autoCtrl->GetAttitudeController()->pidData;
    auto config = PIDConfig(pidDataArray, autoCtrl->maxSteeringAngle);
    Serialiser::SavePIDConfig(config, std::filesystem::path("../assets/pid_parameters") / saveName);
}

void RocketControllerUI::DrawPIDLoading(PIDController *(&pids)[3], float& steeringAngle) {

    ImGui::SeparatorText("Load PID Values:");
    ImGui::Spacing();

    pidFileSelector_.Draw();
    if (!pidFileSelector_.GetSelectedFile().empty()) {
        if (ImGui::Button("Load")) {
            const std::unique_ptr<PIDConfig> result = Serialiser::LoadPIDConfig(pidFileSelector_.GetSelectedFile());
            if (result != nullptr) {
                pids[0]->pidData = result->GetPIDData(0);
                pids[1]->pidData = result->GetPIDData(1);
                pids[2]->pidData = result->GetPIDData(2);
                steeringAngle = result->GetSteeringAngle();
            }
            // Set saveName to filename without path and without .json extension
            const std::filesystem::path selectedPath = pidFileSelector_.GetSelectedFile();
            saveName = selectedPath.stem().string();
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
            const auto filePath = pidFileSelector_.GetSelectedFile();
            try {
                if (std::filesystem::exists(filePath)) {
                    std::filesystem::remove(filePath);
                    pidFileSelector_.RefreshFiles(); // update list
                }
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Failed to delete pid: " << e.what() << std::endl;
            }
        }
    }
}

void RocketControllerUI::OnEvent(Core::Event &event) {
    if (event.GetEventType() == Core::SceneLoaded) {
        Scene *scene = dynamic_cast<SceneLoadedEvent &>(event).GetScene();
        targetUI_.OnSceneLoaded(*scene, controlLayer_->GetRocketObject());
    }
}