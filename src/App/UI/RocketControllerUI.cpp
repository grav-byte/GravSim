#include "RocketControllerUI.h"
#include "imgui.h"

RocketControllerUI::RocketControllerUI() {
    controlLayer_ = Core::Application::Get().GetLayer<ControlLayer>();
}

void RocketControllerUI::Draw() {
    if (!controlLayer_)
        return;

    AutonomousControl* autoCtrl = controlLayer_->GetAutoControl();

    PIDController* altitudePID = autoCtrl->GetAltitudeController();

    ImGui::SetNextWindowPos(ImVec2(1000, 580), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(419, 160), ImGuiCond_FirstUseEver);

    ImGui::Begin("Rocket Controller", nullptr, ImGuiWindowFlags_NoDocking);

    ImGui::Checkbox("Manual Control Enabled", &controlLayer_->manualControlEnabled);

    ImGui::Separator();

    if (controlLayer_->manualControlEnabled) {
        ImGui::TextWrapped(
            "Manual mode active."
        );


        ImGui::End();
        return;
    } else {
        ImGui::TextWrapped(
            "Autonomous mode active."
        );
    }

    ImGui::Separator();

    ImGui::DragFloat("Target Altitude (m)", &autoCtrl->targetAltitude, 1.0f);

    ImGui::Separator();

    ImGui::Text("Altitude PID");
    ImGui::SliderFloat("Kp", &altitudePID->pGain, 0.0f, 10.0f, "%.4f");
    ImGui::SliderFloat("Ki", &altitudePID->iGain, 0.0f, 5.0f, "%.4f");
    ImGui::SliderFloat("Kd", &altitudePID->dGain, 0.0f, 10.0f, "%.4f");

    ImGui::Separator();

    if (ImGui::Button("Reset PID Integral")) {
        altitudePID->Reset();
    }

    ImGui::End();
}

void RocketControllerUI::OnEvent(Core::Event &event) {
}