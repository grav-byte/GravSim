#include "InteractionUI.h"

#include "imgui.h"
#include "Core/Application.h"
#include "../Layers/EngineLayer.h"

InteractionUI::InteractionUI(InteractionState* state)
    : state_(state)
{
    engine_ = Core::Application::Get().GetLayer<EngineLayer>();
}

void InteractionUI::Draw()
{
    if (!engine_) return;

    auto* scene = engine_->GetScene();
    if (!scene) return;

    auto* cam = scene->GetCamera();
    if (!cam) return;

    ImGui::Begin("InteractionUI");
    ImGui::Checkbox("Click-to-place object", &clickToPlaceEnabled_);
    ImGui::TextDisabled("Left-click on empty space (not UI) to spawn.");
    ImGui::End();

    if (!clickToPlaceEnabled_) return;
    if (!engine_ || !engine_->GetScene()) return;

    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse) return;

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        auto* scene = engine_->GetScene();
        auto* cam   = scene ? scene->GetCamera() : nullptr;
        if (!cam) return;

        const ImGuiViewport* vp = ImGui::GetMainViewport();
        ImVec2 mouse = ImGui::GetMousePos();

        // global -> main-viewport-local
        ImVec2 local(mouse.x - vp->Pos.x, mouse.y - vp->Pos.y);

        glm::vec2 screen(local.x, local.y);
        glm::vec2 world = cam->ScreenToWorld(screen);
        engine_->CreateObjectAt(world);
    }
}

glm::vec2 InteractionUI::ScreenToWorld(const ImVec2& mouseScreenPx, const ImVec2& viewportSize) const
{
    auto* cam = engine_->GetScene()->GetCamera();

    float x = mouseScreenPx.x - viewportSize.x * 0.5f;
    float y = mouseScreenPx.y - viewportSize.y * 0.5f;

    y = -y;

    float pixelsToWorld = 1.0f / cam->zoom;

    glm::vec2 worldDelta(x * pixelsToWorld, y * pixelsToWorld);
    return cam->transform.position + worldDelta;
}