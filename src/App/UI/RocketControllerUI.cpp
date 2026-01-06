#include "RocketControllerUI.h"
#include "imgui.h"
#include "App/Engine/Loading/Serialiser.h"

RocketControllerUI::RocketControllerUI() : pidSelector_(FileSelector(std::filesystem::path("../assets/pid_parameters"))) {
    controlLayer_ = Core::Application::Get().GetLayer<ControlLayer>();
}

void RocketControllerUI::Draw() {
    if (!controlLayer_)
        return;

    AutonomousControl* autoCtrl = controlLayer_->GetAutoControl();
    if (!autoCtrl) return;

    PIDController* altitudePID = autoCtrl->GetAltitudeController();
    if (!altitudePID) return;

    const bool manual = controlLayer_->manualControlEnabled;

    constexpr ImVec2 sizeManual(200, 90);
    constexpr ImVec2 sizeAuto(445, 250);

    ImGui::SetNextWindowPos(ImVec2(1000, 580), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(manual ? sizeManual : sizeAuto, ImGuiCond_FirstUseEver);

    ImGui::Begin("Rocket Controller", nullptr, ImGuiWindowFlags_NoDocking);

    ImGui::Checkbox("Manual Control Enabled", &controlLayer_->manualControlEnabled);

    ImGui::Separator();

    if (controlLayer_->manualControlEnabled) {
        ImGui::TextWrapped("Manual mode active.");
        ImGui::End();
        return;
    }

    ImGui::TextWrapped("Autonomous mode active.");

    ImGui::Separator();

    ImGui::DragFloat("Target Altitude (m)", &autoCtrl->targetAltitude, 1.0f);

    ImGui::Separator();

    ImGui::Text("Altitude PID");
    ImGui::SliderFloat("Kp", &altitudePID->pidData.pGain, 0.0f, 10.0f, "%.4f");
    ImGui::SliderFloat("Ki", &altitudePID->pidData.iGain, 0.0f, 5.0f, "%.4f");
    ImGui::SliderFloat("Kd", &altitudePID->pidData.dGain, 0.0f, 10.0f, "%.4f");
    static const auto nameBuffer = new char[128];
    ImGui::InputTextWithHint("Name", "New Values", nameBuffer, 128);
    if (ImGui::Button("Save")) {
        Serialiser::SavePIDData(altitudePID->pidData, std::filesystem::path("../assets/pid_parameters") / std::string(nameBuffer));
    }

    DrawPIDLoading(autoCtrl->GetAltitudeController());

    ImGui::Separator();

    ImGui::Checkbox("Visualize", &autoCtrl->visualizePID);
    if (autoCtrl->visualizePID) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0,0,1,1),"P");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1,0,0,1),"I");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0,1,0,1),"D");
    }

    ImGui::End();
}

void RocketControllerUI::DrawPIDLoading(PIDController* pidController) {

    ImGui::SeparatorText("Load PID Values:");
    ImGui::Spacing();


    pidSelector_.Draw();
    if (!pidSelector_.GetSelectedFile().empty()) {
        if (ImGui::Button("Load Value")) {
            pidController->pidData = *Serialiser::LoadPIDData(pidSelector_.GetSelectedFile());
        }
        ImGui::SameLine();

        if (ImGui::Button("Delete Value")) {
            const auto filePath = pidSelector_.GetSelectedFile();
            try {
                if (std::filesystem::exists(filePath)) {
                    std::filesystem::remove(filePath);
                    pidSelector_.RefreshFiles(); // update list
                }
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Failed to delete pid: " << e.what() << std::endl;
            }
        }
    }
}

void RocketControllerUI::OnEvent(Core::Event &event) {
}