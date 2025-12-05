//
// Created by Lorenz Saalmann on 05.12.25.
//

#include "SceneUI.h"
#include "imgui.h"
#include "App/EngineLayer.h"
#include "Core/Application.h"

SceneUI::SceneUI() {
    scene_ = nullptr;
    engine_ = Core::Application::Get().GetLayer<EngineLayer>();
}

SceneUI::~SceneUI() = default;

void SceneUI::OnEvent(Core::Event &event) {
    // Listen for scene loaded events to update the scene pointer
    if (event.GetEventType() == Core::SceneLoaded) {
        scene_ = engine_->GetScene();
    }
}

void SceneUI::Render() {
    if (!scene_)
        return;

    ImGui::Begin("Scene");
    ImGui::SeparatorText("Objects");
    for (const SceneObject *obj : scene_->GetAllObjects()) {
        ImGui::TextWrapped("Object ID: %d", obj->id);
    }
    ImGui::End();
}
