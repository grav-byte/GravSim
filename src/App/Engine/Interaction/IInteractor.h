#pragma once
#include "imgui.h"
#include "App/Engine/Scene.h"

class IInteractor {
public:
    virtual ~IInteractor() = default;

    virtual void DrawPreview(const Scene &scene, glm::vec2 mousePos) {
        // default preview: draw circle at mouse position with radius
        constexpr int segments = 64;
        ImDrawList* drawList = ImGui::GetForegroundDrawList();
        const auto cam = scene.GetCamera();
        const glm::vec2 origin = cam->WorldToScreen(glm::vec2(0.0f));
        const glm::vec2 radiusWorld = cam->WorldToScreen(glm::vec2(0.0f, radius));
        const float screenRadius = glm::distance(origin, radiusWorld);
        drawList->AddCircle(ImVec2(mousePos.x, mousePos.y), screenRadius, IM_COL32(255, 255, 255, 255), segments, 2.0f);
    }

    virtual void Interact(Scene &scene, glm::vec2 mousePos, bool leftMouse) = 0;

    float radius = 1.0f;
    bool continuous = true;
};
