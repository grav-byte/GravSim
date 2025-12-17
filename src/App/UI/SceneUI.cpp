//
// Created by Lorenz Saalmann on 05.12.25.
//

#include "SceneUI.h"

#include <iostream>

#include "imgui.h"
#include "../Layers/EngineLayer.h"
#include "App/Engine/Physics/Colliders/CircleCollider.h"
#include "App/Rendering/Renderers/CircleRenderer.h"
#include "App/Rendering/Renderers/SpriteRenderer.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Core/Application.h"

SceneUI::SceneUI() {
    scene_ = nullptr;
    sceneSelector_ = std::make_unique<FileSelector>(Core::Application::GetAppDataFolder() / "scenes");
    spriteSelector_ = std::make_unique<FileSelector>(std::filesystem::path("../assets/sprites"));
    engine_ = Core::Application::Get().GetLayer<EngineLayer>();
    statusMessage_ = "";
    statusTimer_ = 0.0f;
}

#include <filesystem>

SceneUI::~SceneUI() = default;

void SceneUI::ShowStatusMessage(const std::string &msg, float duration = 5) {
    statusMessage_ = msg;
    statusTimer_ = duration;
}

void SceneUI::OnEvent(Core::Event &event) {
    // Listen for scene loaded events to update the scene pointer
    if (event.GetEventType() == Core::SceneLoaded) {
        scene_ = engine_->GetScene();
    }
}

void SceneUI::Draw() {
    if (!scene_)
        return;

    ImGui::Begin("Scene");
    ImGui::Text("Scene");
    if (ImGui::Button("Clear Scene")) {
        engine_ -> NewScene();
        ShowStatusMessage("Scene cleared.", 3.0f);
    }

    if (statusTimer_ > 0.0f) {
        ImGui::Text("%s", statusMessage_.c_str());
        statusTimer_ -= ImGui::GetIO().DeltaTime;
        if (statusTimer_ < 0.0f)
            statusTimer_ = 0.0f;
    } else {
        ImGui::Dummy(ImVec2(-1.0f, ImGui::GetTextLineHeightWithSpacing() * .7f));
    }
    ImGui::Spacing();

    DrawSceneLoading();

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    DrawScene();

    ImGui::End();
}

void SceneUI::DrawSceneLoading() {

    ImGui::SeparatorText("All Scenes");
    ImGui::Spacing();


    sceneSelector_->Draw();
    if (!sceneSelector_->GetSelectedFile().empty()) {
        if (ImGui::Button("Load Scene")) {
            if (engine_->LoadScene(sceneSelector_->GetSelectedFile())) {
                ShowStatusMessage("Scene loaded successfully.");
            } else {
                ShowStatusMessage("Failed to load scene.");
            }
        }
        ImGui::SameLine();

        if (ImGui::Button("Delete Scene")) {
            const auto filePath = sceneSelector_->GetSelectedFile();
            try {
                if (std::filesystem::exists(filePath)) {
                    std::filesystem::remove(filePath);
                    sceneSelector_->RefreshFiles(); // update list
                    ShowStatusMessage("Scene deleted successfully.");
                } else {
                    ShowStatusMessage("Scene file does not exist.");
                }
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Failed to delete scene: " << e.what() << std::endl;
                ShowStatusMessage("Failed to delete scene.");
            }
        }
    }
}

void SceneUI::DrawScene() {
    ImGui::Spacing();
    ImGui::SeparatorText("Current Scene");
    ImGui::Spacing();


    ImGui::InputText("Name", scene_->GetName());
    if (ImGui::Button("Save")) {
        if (engine_->SaveScene()) {
            ShowStatusMessage("Scene saved successfully.");
        } else {
            ShowStatusMessage("Failed to save scene.");
        }
    }

    ImGui::Spacing();
    ImGui::SeparatorText("Objects");
    ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_Header,ImVec4(.5, 0.1629818230867386f, 0.2060086131095886f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered,ImVec4(.55, 0.107295036315918f, 0.1072961091995239f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive,ImVec4(.55, 0.08627451211214066f, 0.1019607856869698f, 1.0f));
    for (SceneObject *obj : scene_->GetAllObjects()) {
        DrawObjectUI(obj);
    }
    ImGui::PopStyleColor(3);

    ImGui::Spacing();

    if (ImGui::Button("Add Object")) {
        scene_->CreateObject();
    }

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::SeparatorText("Camera");
    ImGui::Spacing();

    auto cam = scene_->GetCamera();
    DrawFloat2Control("Position", &cam->transform.position);
    ImGui::DragFloat("Zoom", &cam->zoom, .02f, 0.1f, 20.0f);
    DrawColorControl("Bg Color", &cam->backgroundColor);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SeparatorText("Scene Settings");
    DrawFloat2Control("Global Gravity", &scene_->globalGravity);

    DrawConstraints(scene_);

}

void SceneUI::DrawObjectUI(SceneObject* obj) {
    ImGui::PushID(static_cast<int>(obj->id));

    bool keepAlive = true;
    const std::string text = "[" + std::to_string(obj->id) + "] " + obj->name + "###ObjHeader" + std::to_string(obj->id);
    if (ImGui::CollapsingHeader(text.c_str(), &keepAlive))
    {
        ImGui::Indent();
        ImGui::InputTextWithHint("Name", "Object Name", &obj->name);

        DrawTransform(&obj->transform);

        ImGui::Spacing();

        DrawPhysics(obj);

        ImGui::Spacing();

        DrawCollidersUI(obj);

        ImGui::Spacing();

        DrawRendering(obj);
        ImGui::Unindent();
    }
    if (!keepAlive)
        scene_->DeleteObject(obj->id);

    ImGui::Separator();

    ImGui::PopID();
}

void SceneUI::DrawTransform(Transform* transform) {
    if (!ImGui::TreeNode("Transform"))
        return;

    DrawFloat2Control("Position", &transform->position);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("m");
    }

    // coupled scale
    float scale = transform->scale.x;
    if (ImGui::DragFloat("Scale", &scale, .1f)) {
        transform->scale = glm::vec2(scale, scale);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("m");
    }

    ImGui::DragFloat("Rotation", &transform->rotation, .1f);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("degrees");
    }
    ImGui::TreePop();
}

void SceneUI::DrawPhysics(SceneObject *obj) {
    if (!ImGui::TreeNode("Physics"))
        return;

    ImGui::Checkbox("Gravitates", &obj->gravitates);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Whether this object attracts others");
    }
    ImGui::SameLine();
    ImGui::Checkbox("Affected By Gravity", &obj->affectedByGravity);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Whether this object is attracted by others and global gravity");
    }
    ImGui::DragFloat("Mass", &obj->mass, .1f, 0.0f, 100000.0f);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("kg");
    }
    DrawFloat2Control("Velocity", &obj->velocity);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("m/s");
    }
    ImGui::DragFloat("A. Vel", &obj->angularVelocity, .1f);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("degrees/s");
    }
    ImGui::TreePop();
}

void SceneUI::DrawCollidersUI(SceneObject *obj) {
    if (!ImGui::TreeNode("Colliders"))
        return;

    ImGui::Checkbox("Show", &engine_->showColliders);

    ImGui::SameLine(150.0f);

    if (ImGui::Button("Add")) {
        obj->AddCollider(ColliderType::Circle);
    }

    int idx = 0;
    for (auto &collider : obj->colliders) {
        if (ImGui::TreeNode(collider.get(), "Circle")) {
            ImGui::SetNextItemWidth(150.0f);
            if (ImGui::DragFloat("Size", &collider->size.x, .1f))
                collider->size.y = collider->size.x; // keep circle
            ImGui::SetNextItemWidth(150.0f);
            DrawFloat2Control("Local Pos", &collider->localPosition, .1f);
            ImGui::SetNextItemWidth(150.0f);
            ImGui::DragFloat("Elasticity", &collider->elasticity, .01f, 0.0f, 1.0f);
            if (ImGui::Button("Remove")) {
                obj->RemoveCollider(idx);
            }
            ImGui::TreePop();
        }
        idx++;
    }

    ImGui::TreePop();
}

void SceneUI::DrawRendering(SceneObject *obj) {
    if (!ImGui::TreeNode("Rendering"))
        return;
    // Determine current renderer type
    RendererType currentRenderer = RendererType::Circle;
    if (dynamic_cast<CircleRenderer*>(obj->renderer.get())) currentRenderer = RendererType::Circle;
    else if (dynamic_cast<SpriteRenderer*>(obj->renderer.get())) currentRenderer = RendererType::Sprite;

    static const char* RendererNames[] = { "Circle", "Sprite" };
    int currentRendererInt = static_cast<int>(currentRenderer);
    // Combo box
    if (ImGui::Combo("Renderer", &currentRendererInt, RendererNames, IM_ARRAYSIZE(RendererNames))) {
        currentRenderer = static_cast<RendererType>(currentRendererInt);
        // user changed renderer type
        glm::vec4 color = obj->renderer->color; // preserve color

        switch (currentRenderer) {
            case RendererType::Circle:
                obj->renderer = std::make_unique<CircleRenderer>();
                break;
            case RendererType::Sprite:
                obj->renderer = std::make_unique<SpriteRenderer>();
                break;
            default: ;
        }

        obj->renderer->color = color; // restore color
    }

    ImGui::Spacing();

    if (currentRenderer == RendererType::Sprite) {
        if (auto* sprite = dynamic_cast<SpriteRenderer*>(obj->renderer.get())) {
            ImGui::Text("Select Sprite");
            spriteSelector_->Draw(sprite->GetPath().filename().string().c_str());
            sprite->SetPath(spriteSelector_->GetSelectedFile());
        }
    }

    ImGui::Spacing();

    DrawColorControl("Color", &obj->renderer->color);

    ImGui::TreePop();
}

void SceneUI::DrawConstraints(Scene* scene)
{
    ImGui::Spacing();
    ImGui::Text("Constraints");
    ImGui::Separator();

    struct DirUI { const char* label; Constraint::ConstraintDirection dir; };
    static const DirUI dirs[] = {
        {"UP", Constraint::UP},
        {"DOWN", Constraint::DOWN},
        {"LEFT", Constraint::LEFT},
        {"RIGHT", Constraint::RIGHT},
        {"RADIAL", Constraint::RADIAL}
    };

    for (const auto& d : dirs) {
        bool hasDir = false;
        for (Constraint* c : scene->GetConstraints()) {
            if (c->direction == d.dir) {
                hasDir = true;
                break;
            }
        }
        bool hadDir = hasDir;
        if (ImGui::Checkbox(d.label, &hasDir)) {
            if (!hasDir && hadDir) {
                // remove constraint
                scene->RemoveConstraint(d.dir);
            } else if (hasDir && !hadDir) {
                // add constraint with default distance
                scene->AddConstraint(std::make_unique<Constraint>(1.0f, d.dir));
            }
        }

        ImGui::SameLine(100.0f);
        if (hasDir) {
            ImGui::SetNextItemWidth(150.0f);
            // show distance for this direction (first matching constraint)
            for (Constraint* c : scene->GetConstraints()) {
                if (c->direction == d.dir) {
                    float lowerLimit = d.dir == Constraint::RADIAL ? 0.1f : -1000.0f;
                    ImGui::DragFloat((std::string("Distance##") + d.label).c_str(), &c->distance, 0.1f, lowerLimit, 1000.0f);
                    break;
                }
            }
        } else {
            ImGui::TextDisabled("No constraint");
        }
    }
}

void SceneUI::DrawColorControl(const char *title, glm::vec4 *color) {

    float value[3] = { color->x, color->y, color->z };
    if (ImGui::ColorEdit3(title, value)) {
        // Update
        *color = glm::vec4(value[0], value[1], value[2], color->a);
    }
}

bool SceneUI::DrawFloat2Control(const char *title, glm::vec2 *vec2, float speed) {

    float value[2] = { vec2->x, vec2->y };
    bool updated = ImGui::DragFloat2(title, value, speed);
    if (updated) {
        // Update
        *vec2 = glm::vec2(value[0], value[1]);
    }
    return updated;
}
