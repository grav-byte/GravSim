#include "FollowingUI.h"

#include "imgui.h"
#include "App/Layers/EngineLayer.h"
#include "Core/Application.h"

FollowingUI::FollowingUI() {
    cameraController_ = Core::Application::Get().GetLayer<EngineLayer>()->GetCameraController();
}

void FollowingUI::OnEvent(Core::Event &event) {
}

void FollowingUI::Draw() {
    const ImGuiViewport* vp = ImGui::GetMainViewport();

    const auto obj = cameraController_->GetFollowingObject();

    if (obj == nullptr) {
        return;
    }

    float textWidth = ImGui::CalcTextSize(obj->name.c_str()).x;
    float labelWidth = ImGui::CalcTextSize("Following ").x;
    float buttonWidth = 60.0f; // size of your "Stop" button
    float padding = ImGui::GetStyle().WindowPadding.x * 2;

    float width = labelWidth + textWidth + buttonWidth + padding;

    // center horizontally relative to the main window
    ImVec2 pos(
        vp->Pos.x + (vp->Size.x - width) * 0.5f,
        vp->Pos.y - 100.0f + vp->Size.y
    );

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, 0), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;


    ImGui::Begin("Following", nullptr, flags);
    ImGui::Text("Following %s", obj->name.c_str());
    ImGui::SameLine();
    if (ImGui::Button("Stop")) {
        cameraController_->StopFollowing();
    }
    ImGui::End();
}
