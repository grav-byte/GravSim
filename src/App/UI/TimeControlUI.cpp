#include "TimeControlUI.h"

#include "Core/Application.h"

TimeControlUI::TimeControlUI() {
    engine_ = Core::Application::Get().GetLayer<EngineLayer>();
    btnActiveColor_ = ImVec4(0.80f, 0.39f, 0.15f, 1.0f); // orange accent
}

void TimeControlUI::OnEvent(Core::Event &event) {
}

void TimeControlUI::Draw() {
    if (!engine_->IsRunningSimulation())
        return;

    static float activeScale = 1.0f; // .5x, 1x, 2x, 4x

    constexpr float width  = 150.0f;

    const ImGuiViewport* vp = ImGui::GetMainViewport();

    const ImVec2 pos(
        vp->Pos.x + (vp->Size.x - width) * 0.5f + 280.0f,
        vp->Pos.y + 10.0f
    );

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(-1, -1), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("Time Control", nullptr, flags);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Time Scale:");
    ImGui::SameLine();
    ImGui::SetWindowFontScale(.9f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4)); // X, Y padding
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, btnActiveColor_);
    constexpr auto size = ImVec2(30, 20);

    auto drawToggle = [&](const char* label, const float value)
    {
        const bool active = activeScale == value;

        if (active)
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);

        if (ImGui::Button(label, size)) {
            activeScale = value;
            Core::Application::Get().SetTimeScale(activeScale);
        }

        if (active)
            ImGui::PopStyleColor();
    };
    drawToggle(".5x", 0.5f);
    ImGui::SameLine();
    drawToggle("1x", 1);
    ImGui::SameLine();
    drawToggle("2x", 2);
    ImGui::SameLine();
    drawToggle("4x", 4);

    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::End();
}
