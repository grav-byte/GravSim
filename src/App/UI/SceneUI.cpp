//
// Created by Lorenz Saalmann on 05.12.25.
//

#include "SceneUI.h"

#include <iostream>

#include "imgui.h"
#include "../Layers/EngineLayer.h"
#include "Core/AppLayer.h"
#include "tinyfiledialogs.h"
#include "Core/AppLayer.h"
#include "misc/cpp/imgui_stdlib.h"
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

void SceneUI::Draw() {
    if (!scene_)
        return;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File"))
        {
            FileMenu();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Scene");
    ImGui::SeparatorText("Objects");
    if (ImGui::Button("Add Object")) {
        scene_->CreateObject();
    }
    for (SceneObject *obj : scene_->GetAllObjects()) {
        DrawObjectUI(obj);
    }

    ImGui::SeparatorText("Camera");
    auto cam = scene_->GetCamera();
    DrawFloat2Control("Position", &cam->transform.position);
    ImGui::DragFloat("Zoom", &cam->zoom, .02f, 0.1f, 20.0f);

    if (ImGui::Button("Save")) {
        engine_->SaveScene("scene.json");
    }

    if (ImGui::Button("Load")) {
        engine_->LoadScene("scene.json");
    }

    ImGui::End();
}

void SceneUI::FileMenu() {
    if (ImGui::MenuItem("New Scene")) {
        engine_->NewScene();
    }
    if (ImGui::MenuItem("Open Scene")) {
        engine_->LoadScene(OpenFileDialog());
    }
    if (ImGui::MenuItem("Save Scene")) {

    }
}

const char* SceneUI::OpenFileDialog() {
    const char* filters[] = { "*.public.json" };
    const char* filename = tinyfd_openFileDialog(
        "Open Scene",
        "",
        1,           // number of filters
        filters,
        "Scene files",
        0             // allow multiple selections?
    );

    return filename;
}

void SceneUI::DrawObjectUI(SceneObject* obj) {
    ImGui::PushID(static_cast<int>(obj->id));

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.25f, 0.25f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.3f, 0.3f, 0.35f, 1.0f));

    bool keepAlive = true;
    const std::string text = "[" + std::to_string(obj->id) + "] " + obj->name + "###ObjHeader" + std::to_string(obj->id);
    if (ImGui::CollapsingHeader(text.c_str(), &keepAlive))
    {
        ImGui::Spacing();
        ImGui::InputTextWithHint("Name", "Object Name", &obj->name);
        ImGui::Text("Transform");
        DrawFloat2Control("Position", &obj->transform.position);
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("m");
        }

        // coupled scale
        float scale = obj->transform.scale.x;
        if (ImGui::DragFloat("Scale", &scale, .1f)) {
            obj->transform.scale = glm::vec2(scale, scale);
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("m");
        }

        ImGui::DragFloat("Rotation", &obj->transform.rotation, .1f);
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("degrees");
        }

        ImGui::Text("Physics");
        DrawFloat2Control("Velocity", &obj->velocity);
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("m/s");
        }
        ImGui::DragFloat("A. Vel", &obj->angularVelocity, .1f);
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("degrees/s");
        }

        ImGui::Text("Rendering");
        DrawColorControl("Color", &obj->renderer->color);

        ImGui::Spacing();
    }
    if (!keepAlive)
        scene_->DeleteObject(obj->id);

    ImGui::PopStyleColor(3);
    ImGui::PopID();
}

void SceneUI::DrawColorControl(const char *title, glm::vec4 *color) {

    float value[3] = { color->x, color->y, color->z };
    if (ImGui::ColorEdit3(title, value)) {
        // Update
        *color = glm::vec4(value[0], value[1], value[2], color->a);
    }
}

void SceneUI::DrawFloat2Control(const char *title, glm::vec2 *vec2, float speed) {

    float value[2] = { vec2->x, vec2->y };
    if (ImGui::DragFloat2(title, value, speed)) {
        // Update
        *vec2 = glm::vec2(value[0], value[1]);
    }
}
