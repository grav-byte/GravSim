#include "InteractionUI.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Core/Application.h"
#include "App/Layers/EngineLayer.h"
#include "App/Engine/Loading/TextureLoader.h"
#include <iostream>

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

bool InteractionUI::ImageToggleButton(const std::string &texturePath, bool selected, const char *tooltip) const {
    const auto tex = TextureLoader::GetTexture(texturePath);
    constexpr auto size = ImVec2(16, 16);
    constexpr auto uv0 = ImVec2(0, 1);
    constexpr auto uv1 = ImVec2(1, 0);

    ImGui::PushStyleColor(ImGuiCol_Button, selected ? ImVec4(0.2f,0.6f,0.9f,0.3f) : ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 6.0f));
    bool pressed = ImGui::ImageButton(("##btn"+std::to_string(tex.id)).c_str(), tex.id, size, uv0, uv1, btnBgColor_, selected ? btnActiveColor_ : btnTintColor_);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s", tooltip);
    }

    return pressed;
}


void InteractionUI::Draw()
{
    const float width  = 100.0f;
    const float height = 165.0f;
    const float margin = 10.0f;

    const ImGuiViewport* vp = ImGui::GetMainViewport();

    ImVec2 pos(
        vp->Pos.x + vp->Size.x - width - margin,
        vp->Pos.y + (vp->Size.y - height - margin) * 0.5f
    );

    if (ImGuiWindow* settingsWin = ImGui::FindWindowByName("Settings")) {
        if (!settingsWin->Collapsed) {
            float leftEdgeOfSettings = settingsWin->Pos.x;
            float topOfSettings      = settingsWin->Pos.y;

            pos.x = leftEdgeOfSettings - width - margin;
            pos.y = topOfSettings + (vp->Size.y - height - margin) * 0.5f;
        }
    }

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);

    ImGuiWindowFlags flags =
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

    if (ImageToggleButton(iconPlaceObj, activeMode_ == Mode::ClickToPlace, "Place object")) {
        activeMode_ = (activeMode_ == Mode::ClickToPlace) ? Mode::None : Mode::ClickToPlace;
    }
    ImGui::SameLine();
    ImGui::Text("Object");

    ImGui::Dummy(ImVec2(0, 6));

    if (ImageToggleButton(iconConstraint, activeMode_ == Mode::ClickToConstraint, "Place constraint")) {
        activeMode_ = (activeMode_ == Mode::ClickToConstraint) ? Mode::None : Mode::ClickToConstraint;
    }
    ImGui::SameLine();
    ImGui::Text("Force");

    ImGui::Dummy(ImVec2(0, 8));

    if (ImageToggleButton(iconNone, activeMode_ == Mode::None, "Keine Aktion")) {
        activeMode_ = Mode::None;
    }
    ImGui::SameLine();
    ImGui::Text("Mouse");
    ImGui::End();

    if (activeMode_ != Mode::ClickToPlace) {
        isPreviewActive_ = false;
        return;
    }

    if (!engine_) engine_ = Core::Application::Get().GetLayer<EngineLayer>();
    if (!engine_) return;

    auto* scene = engine_->GetScene();
    if (!scene) return;

    auto* cam = scene->GetCamera();
    if (!cam) return;

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) return;

    ImVec2 mouse = ImGui::GetMousePos();
    previewScreenPos_ = mouse;
    isPreviewActive_ = true;

    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    const float radius = 12.0f;

    ImU32 color = IM_COL32(50, 150, 230, 180);
    drawList->AddCircleFilled(previewScreenPos_, radius, color);
    drawList->AddCircle(previewScreenPos_, radius, IM_COL32(255,255,255,120), 0, 2.0f);

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        activeMode_ = Mode::None;
        isPreviewActive_ = false;
        return;
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        const ImGuiViewport* mvp = ImGui::GetMainViewport();
        ImVec2 mouse = ImGui::GetMousePos();
        ImVec2 local(mouse.x - mvp->Pos.x, mouse.y - mvp->Pos.y);

        glm::vec2 screen(local.x, local.y);
        glm::vec2 world  = cam->ScreenToWorld(screen);

        engine_->CreateObjectAt(world);
    }
}