#include "RocketControllerUI.h"
#include "imgui.h"
#include "App/Engine/Loading/Serialiser.h"
#include "App/Engine/Scene.h"
#include "App/Engine/SceneObject.h"
#include "App/Rendering/Visuals/ShaderVisual.h"
#include "App/Layers/EngineLayer.h"
#include "App/RocketControl/TargetObject.h"
#include "App/RocketControl/Controllers/AutonomousPIDRocketController.h"
#include "misc/cpp/imgui_stdlib.h"

RocketControllerUI::RocketControllerUI() : pidFileSelector_(FileSelector(std::filesystem::path("../assets/pid_parameters"))) {
    controlLayer_ = Core::Application::Get().GetLayer<ControlLayer>();
    engineLayer_  = Core::Application::Get().GetLayer<EngineLayer>();
}

void RocketControllerUI::CreateTarget(Scene& scene) {
    auto targetUq = std::make_unique<TargetObject>();

    targetUq->transform.position = glm::vec2(0.0f, 0.0f);

    targetPos_ = targetUq->transform.position;

    // Save ID after adding to scene
    targetId_ = scene.AddObject(std::move(targetUq));
    targetCreated_ = true;
}

void RocketControllerUI::DeleteTarget(Scene& scene) {
    if (!targetCreated_ || targetId_ == InvalidId) return;

    scene.DeleteObject(targetId_);
    targetCreated_ = false;
    targetId_ = InvalidId;
}

SceneObject* RocketControllerUI::GetTarget(Scene& scene) const {
    if (!targetCreated_ || targetId_ == InvalidId)
        return nullptr;

    return scene.GetObjById(targetId_);
}

static bool DrawFloat2Control(const char* label, glm::vec2* v, float speed = 0.1f) {
    float value[2] = { v->x, v->y };
    const bool updated = ImGui::DragFloat2(label, value, speed);
    if (updated) {
        *v = glm::vec2(value[0], value[1]);
    }
    return updated;
}

void RocketControllerUI::Draw() {
    if (!controlLayer_ || !engineLayer_)
        return;

    Scene* scene = engineLayer_->GetScene();
    if (!scene)
        return;

    AutonomousPIDRocketController* autoCtrl = controlLayer_->GetAutoControl();
    if (!autoCtrl) return;

    constexpr ImVec2 sizeAuto(445, 300);

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
            ImGui::BeginChild("TargetContent", ImVec2(0, 0), false);

            ImGui::Spacing();
            ImGui::Spacing();

            DrawTargetSettings(scene, autoCtrl);

            ImGui::EndChild();
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

    DrawPIDLoading(pids);

    ImGui::Spacing();

    ImGui::SeparatorText("Set PID Parameters");
    ImGui::Spacing();
    ImGui::TextLinkOpenURL("(Click for overview)", "https://github.com/grav-byte/GravSim/blob/d9fad45460bd06021d731f6bce4695e4bb6d9295/docs/PIDOverview.png");
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
                const auto bgCol = ImVec4( intensity+ .2f,  .2f, intensity+.2f, 0.2f);

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

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Visualise");
        ImGui::TextColored(ImVec4(0,0,1,1),"P");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1,0,0,1),"I");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0,1,0,1),"D");
        ImGui::TableSetColumnIndex(1);
        ImGui::Checkbox("##vis0", &autoCtrl->visualizePID.x);
        ImGui::TableSetColumnIndex(2);
        ImGui::Checkbox("##vis1", &autoCtrl->visualizePID.y);
        ImGui::TableSetColumnIndex(3);
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
    auto config = PIDConfig(pidDataArray);
    Serialiser::SavePIDConfig(config, std::filesystem::path("../assets/pid_parameters") / saveName);
}

void RocketControllerUI::DrawTargetSettings(Scene *scene, AutonomousPIDRocketController* autoCtrl) {
    ImGui::SeparatorText("Target Settings");

    // Create/Delete target scene object (rendered by SceneRenderer)
    if (!targetCreated_) {
        if (ImGui::Button("Create Target")) {
            CreateTarget(*scene);
        }
    } else {
        if (ImGui::Button("Delete Target")) {
            DeleteTarget(*scene);
        }
    }

    ImGui::Separator();

    if (SceneObject* targetObj = GetTarget(*scene)) {

        // Edit targetPos_ via UI
        DrawFloat2Control("Position", &targetPos_, 0.1f);

        // Apply to scene object
        targetObj->transform.position = targetPos_;
        targetObj->lastPosition       = targetPos_;

        if (autoCtrl) {
            autoCtrl->targetPos      = targetPos_;
        }
    }
}

void RocketControllerUI::DrawPIDLoading(PIDController *(&pids)[3]) {

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
        targetCreated_ = false;
        targetId_ = InvalidId;
    }
}