#include "InteractionUI.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Core/Application.h"
#include "App/Layers/EngineLayer.h"
#include "App/Engine/Loading/TextureLoader.h"

#include "CustomImGuiStyle.h"
#include "App/Engine/Interaction/ApplyForceInteractor.h"
#include "App/Engine/Interaction/PlaceInteractor.h"
#include "Core/InputEvents.h"

InteractionUI::InteractionUI()
{
    engine_ = Core::Application::Get().GetLayer<EngineLayer>();
    activeMode_ = Mode::None;
    btnBgColor_ = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    interactors_.emplace_back(std::make_unique<PlaceInteractor>());
    interactors_.emplace_back(std::make_unique<ApplyForceInteractor>());
}

bool InteractionUI::ImageToggleButton(const std::string &texturePath, bool selected, bool disabled, const char *tooltip) const {
    const auto tex = TextureLoader::GetTexture(texturePath);
    constexpr auto size = ImVec2(16, 16);
    constexpr auto uv0 = ImVec2(0, 1);
    constexpr auto uv1 = ImVec2(1, 0);

    ImGui::PushStyleColor(ImGuiCol_Button, selected ? CustomImGuiStyle::AccentColor : ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 6.0f));
    ImVec4 tint = CustomImGuiStyle::ContrastColor;
    tint.w = disabled ? .4f : 1.0f;
    const bool pressed = ImGui::ImageButton(("##btn"+std::to_string(tex.id)).c_str(), tex.id, size, uv0, uv1, btnBgColor_, tint);
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
    constexpr float height = 230.0f;
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
        activeInteractorIdx_ = -1;
    }
    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Mouse");

    ImGui::Dummy(ImVec2(0, 6));

    if (ImageToggleButton(iconPlaceObj, activeMode_ == Mode::ClickToPlace, false, "Place object")) {
        activeMode_ = Mode::ClickToPlace;
        activeInteractorIdx_ = 0;
    }
    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Object");

    const bool disabled = !engine_->IsRunningSimulation();
    ImGui::BeginDisabled(disabled);
    ImGui::Dummy(ImVec2(0, 6));

    const auto tooltip = disabled ? "Only available during simulation" : "Apply push (left click) or pull (right click) force";

    if (ImageToggleButton(iconConstraint, activeMode_ == Mode::ApplyForce, disabled, tooltip)) {
        activeMode_ = Mode::ApplyForce;
        activeInteractorIdx_ = 1;
    }

    ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    ImGui::PushStyleColor(ImGuiCol_Text, disabled ? ImVec4(0.5f,0.5f,0.5f,1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Text));
    ImGui::Text("Force");
    ImGui::PopStyleColor();

    ImGui::Text("Strength");
    ImGui::DragFloat("##Strength", &interactors_[1]->strength, 0.1f, 0.0f, 100.0f, "%.1f");

    ImGui::EndDisabled();

    ImGui::End();

    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        activeMode_ = Mode::None;
        activeInteractorIdx_ = -1;
        interacting_ = false;
        return;
    }

    if (ImGui::GetIO().WantCaptureMouse || activeInteractorIdx_ == -1)
        return;

    interactors_[activeInteractorIdx_]->DrawPreview(*engine_->GetScene(), mousePosition_);

    if (interacting_) {
        interactors_[activeInteractorIdx_]->OnClick(*engine_->GetScene(), mousePosition_, usingLeftMouse_);
    }

}

void InteractionUI::OnEvent(Core::Event &event) {
    if (event.GetEventType() == Core::MouseMoved) {
        const auto& mouseEvent = dynamic_cast<Core::MouseMovedEvent&>(event);
        mousePosition_ = glm::vec2(mouseEvent.GetX(), mouseEvent.GetY());
    }

    if (event.GetEventType() == Core::MouseButtonPressed) {
        if (ImGui::GetIO().WantCaptureMouse || activeInteractorIdx_ == -1) {
            interacting_ = false;
            return;
        }
        event.Handled = true;
        interacting_ = true;
        usingLeftMouse_ = dynamic_cast<Core::MouseButtonPressedEvent&>(event).GetMouseButton() == 0;
    }

    if (event.GetEventType() == Core::MouseButtonReleased) {
        interacting_ = false;
        if (ImGui::GetIO().WantCaptureMouse || activeInteractorIdx_ == -1)
            return;

        interactors_[activeInteractorIdx_]->OnRelease(*engine_->GetScene(), mousePosition_, usingLeftMouse_);
    }

    if (event.GetEventType() == Core::SimulationStopped) {
        // reset interactor on simulation stop
        activeMode_ = Mode::None;
        activeInteractorIdx_ = -1;
        interacting_ = false;
    }

    if (event.GetEventType() == Core::MouseScrolled) {
        if (ImGui::GetIO().WantCaptureMouse || activeInteractorIdx_ == -1)
            return;

        const auto amount = dynamic_cast<Core::MouseScrolledEvent&>(event).GetAmount();
        interactors_[activeInteractorIdx_]->radius += amount * .02f;
        // clamp radius to >= .1
        interactors_[activeInteractorIdx_]->radius = glm::max(interactors_[activeInteractorIdx_]->radius, 0.1f);

        // dont propagate scroll event
        event.Handled = true;
    }
}
