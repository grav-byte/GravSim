#include "RocketControllerUI.h"
#include "imgui.h"
#include "App/Engine/Loading/Serialiser.h"
#include "App/Engine/Scene.h"
#include "App/Engine/SceneObject.h"
#include "App/Engine/Loading/TextureLoader.h"
#include "App/Rendering/Visuals/ShaderVisual.h"
#include "App/Layers/EngineLayer.h"
#include "App/RocketControl/TargetObject.h"
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
    scene.DeleteObject(targetId_); // adapt if API differs
    targetCreated_ = false;
    targetId_ = 0;
}

SceneObject* RocketControllerUI::GetTarget(Scene& scene) const {
    if (!targetCreated_ || targetId_ == 0)
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

    AutonomousControl* autoCtrl = controlLayer_->GetAutoControl();
    if (!autoCtrl) return;

    constexpr ImVec2 sizeAuto(445, 270);

    ImGui::SetNextWindowPos(ImVec2(1000, 580), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(sizeAuto, ImGuiCond_FirstUseEver);

    ImGui::Begin("Rocket Controller", nullptr, ImGuiWindowFlags_NoDocking);

    if (ImGui::BeginTabBar("RocketControllerTabs", ImGuiTabBarFlags_DrawSelectedOverline)) {
        if (ImGui::BeginTabItem("Control")) {
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

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Target")) {
            ImGui::Spacing();
            ImGui::Spacing();

            DrawTargetSettings(scene);

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void RocketControllerUI::DrawPIDSettings(AutonomousControl *autoCtrl) {
    DrawPIDLoading();

    ImGui::SeparatorText("Set PID Values");
    ImGui::Spacing();

    // PID selection dropdown
    static int pidSelection = 0;
    const char* pidNames[] = { "Vertical", "Horizontal", "Attitude" };

    ImGui::Text("Selected PID:");
    ImGui::SameLine();
    ImGui::TextLinkOpenURL("(Click for overview)", "https://github.com/grav-byte/GravSim/blob/d9fad45460bd06021d731f6bce4695e4bb6d9295/docs/PIDOverview.png");
    ImGui::Combo("PID", &pidSelection, pidNames, IM_ARRAYSIZE(pidNames));

    // select active PID controller
    switch (pidSelection) {
        case 0: activePID = autoCtrl->GetVerticalController();   break;
        case 1: activePID = autoCtrl->GetHorizontalController(); break;
        case 2: activePID = autoCtrl->GetAttitudeController();   break;
    }
    if (!activePID)
        return;

    ImGui::Spacing();
    ImGui::Checkbox("Visualize", &autoCtrl->visualizePID);
    if (autoCtrl->visualizePID) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0,0,1,1),"P");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1,0,0,1),"I");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0,1,0,1),"D");
    }

    ImGui::DragFloat("Kp", &activePID->pidData.pGain, .02f, 0.0f, 5.0f);
    ImGui::DragFloat("Ki", &activePID->pidData.iGain, .02f, 0.0f, 5.0f);
    ImGui::DragFloat("Kd", &activePID->pidData.dGain, .02f, 0.0f, 5.0f);

    ImGui::InputText("Name", &activePID->pidData.name);
    if (ImGui::Button("Save")) {
        Serialiser::SavePIDData(activePID->pidData, std::filesystem::path("../assets/pid_parameters") / activePID->pidData.name);
    }
}

void RocketControllerUI::DrawTargetSettings(Scene *scene) {
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

        if (DrawFloat2Control("Position", &targetPos_, 0.1f)) {
            targetObj->transform.position = targetPos_;

            targetObj->lastPosition = targetObj->transform.position;
        } else {
            targetObj->transform.position = targetPos_;
        }
    }
}

void RocketControllerUI::DrawPIDLoading() {

    ImGui::SeparatorText("Load PID Values:");
    ImGui::Spacing();


    pidFileSelector_.Draw();
    if (!pidFileSelector_.GetSelectedFile().empty()) {
        if (ImGui::Button("Load Value")) {
            std::unique_ptr<PIDData> result = Serialiser::LoadPIDData(pidFileSelector_.GetSelectedFile());
            if (result != nullptr)
                activePID->pidData = *result;
        }
        ImGui::SameLine();

        if (ImGui::Button("Delete Value")) {
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
        targetId_ = 0;
    }
}