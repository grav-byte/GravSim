#include "InteractionUI.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Core/Application.h"
#include "App/Layers/EngineLayer.h"
#include "App/Engine/Loading/TextureLoader.h"
#include <iostream>

#include "Core/InputEvents.h"

InteractionUI::InteractionUI()
{
    engine_ = Core::Application::Get().GetLayer<EngineLayer>();
    activeMode_ = Mode::None;
    btnBgColor_ = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    btnTintColor_ = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    btnActiveColor_ = ImVec4(0.2f, 0.6f, 0.9f, 0.9f);
    btnDisabledColor_ = ImVec4(1.0f, 1.0f, 1.0f, .4f);
    isPreviewActive_ = false;
}

bool InteractionUI::ImageToggleButton(const std::string &texturePath, bool selected, bool disabled, const char *tooltip) const {
    const auto tex = TextureLoader::GetTexture(texturePath);
    constexpr auto size = ImVec2(16, 16);
    constexpr auto uv0 = ImVec2(0, 1);
    constexpr auto uv1 = ImVec2(1, 0);

    ImGui::PushStyleColor(ImGuiCol_Button, selected ? ImVec4(0.25f,0.25f,0.25f,1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 6.0f));
    const bool pressed = ImGui::ImageButton(("##btn"+std::to_string(tex.id)).c_str(), tex.id, size, uv0, uv1, btnBgColor_, disabled ? btnDisabledColor_ : btnTintColor_);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", tooltip);
    }

    return pressed;
}


void InteractionUI::Draw()
{
    constexpr float width  = 100.0f;
    constexpr float height = 165.0f;
    constexpr float margin = 10.0f;

    const ImGuiViewport* vp = ImGui::GetMainViewport();

    ImVec2 pos(
        vp->Pos.x + vp->Size.x - width - margin,
        vp->Pos.y + (vp->Size.y - height - margin) * 0.5f
    );

    if (const ImGuiWindow* settingsWin = ImGui::FindWindowByName("Settings")) {
        if (!settingsWin->Collapsed) {
            const float leftEdgeOfSettings = settingsWin->Pos.x;
            const float topOfSettings      = settingsWin->Pos.y;

            pos.x = leftEdgeOfSettings - width - margin;
            pos.y = topOfSettings + (vp->Size.y - height - margin) * 0.5f;
        }
    }

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);

    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("Interaction", nullptr, flags);

    ImGui::Text("Interaction");
    ImGui::Separator();

    const std::string iconPlaceObj    = "../assets/icons/place.png";
    const std::string iconConstraint  = "../assets/icons/force.png";
    const std::string iconNone        = "../assets/icons/arrow.png";

    if (ImageToggleButton(iconNone, activeMode_ == Mode::None, false, "Drag camera")) {
        activeMode_ = Mode::None;
    }
    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Mouse");

    ImGui::Dummy(ImVec2(0, 6));

    if (ImageToggleButton(iconPlaceObj, activeMode_ == Mode::ClickToPlace, false, "Place object")) {
        activeMode_ = activeMode_ == Mode::ClickToPlace ? Mode::None : Mode::ClickToPlace;
    }
    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Object");

    const bool disabled = !engine_->IsRunningSimulation();
    ImGui::BeginDisabled(disabled);
    ImGui::Dummy(ImVec2(0, 6));

    const auto tooltip = disabled ? "Only available during simulation" : "Apply push (left click) or pull (right click) force";

    if (ImageToggleButton(iconConstraint, activeMode_ == Mode::ClickToConstraint, disabled, tooltip)) {
        activeMode_ = activeMode_ == Mode::ClickToConstraint ? Mode::None : Mode::ClickToConstraint;
    }
    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    ImGui::PushStyleColor(ImGuiCol_Text, disabled ? ImVec4(0.5f,0.5f,0.5f,1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Text));
    ImGui::Text("Force");
    ImGui::PopStyleColor();

    ImGui::EndDisabled();

    ImGui::End();

    if (ImGui::GetIO().WantCaptureMouse)
        return;

    if (activeMode_ != Mode::ClickToPlace) {
        isPreviewActive_ = false;
        return;
    }

    if (!engine_) engine_ = Core::Application::Get().GetLayer<EngineLayer>();
    if (!engine_) return;

    const auto* scene = engine_->GetScene();
    if (!scene) return;

    const auto* cam = scene->GetCamera();
    if (!cam) return;

    const ImVec2 mouse = ImGui::GetMousePos();
    previewScreenPos_ = mouse;
    isPreviewActive_ = true;

    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    constexpr float radius = 12.0f;

    constexpr ImU32 color = IM_COL32(50, 150, 230, 180);
    drawList->AddCircleFilled(previewScreenPos_, radius, color);
    drawList->AddCircle(previewScreenPos_, radius, IM_COL32(255,255,255,120), 0, 2.0f);

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        activeMode_ = Mode::None;
        isPreviewActive_ = false;
        return;
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        const glm::vec2 world  = cam->ScreenToWorld(mouse_position_);

        engine_->GetScene()->CreateObject(world);
    }
}

void InteractionUI::OnEvent(Core::Event &event) {
    if (event.GetEventType() == Core::MouseMoved) {
        const auto& mouseEvent = dynamic_cast<Core::MouseMovedEvent&>(event);
        mouse_position_ = glm::vec2(mouseEvent.GetX(), mouseEvent.GetY());
    }
}
