//
// Created by kw on 08.01.2026.
//

#include "App/RocketControl/UI/TargetUI.h"

#include "imgui.h"
#include "App/Engine/Scene.h"
#include "App/RocketControl/Controllers/AutonomousPIDRocketController.h"
#include "App/RocketControl/RocketObject.h"
#include "App/RocketControl/TargetObject.h"
#include "App/Layers/AudioLayer.h"

namespace {
    constexpr float kLabelX      = 160.0f; // label->control spacing
    constexpr float kFloat2Width = 160.0f; // DragFloat2 width
    constexpr float kFloatWidth  = 160.0f; // DragFloat width
}

// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------

bool TargetUI::DrawFloat2Control(const char* label, glm::vec2* v, const float speed) {
    float value[2] = { v->x, v->y };
    const bool updated = ImGui::DragFloat2(label, value, speed);
    if (updated) *v = glm::vec2(value[0], value[1]);
    return updated;
}

// ------------------------------------------------------------
// Scene lifecycle / sync
// ------------------------------------------------------------

void TargetUI::SetNextTarget() {
    if (targets_.empty())
        return;

    targets_.erase(
        std::remove_if(
            targets_.begin(),
            targets_.end(),
            [](TargetObject* t) { return t->IsReached(); }
        ),
        targets_.end()
    );

    if (targets_.empty())
        return;

    autoCtrl_->SetActiveTarget(
        targets_[0],
        [this] {
            this->SetNextTarget();
        }
    );
}

void TargetUI::OnSceneLoaded(const Scene& scene, RocketObject* rocket, AutonomousPIDRocketController* autoCtrl) {
    cachedRocket_ = rocket;
    autoCtrl_ = autoCtrl;

    // Rebuild immediately (no deferred sync needed)
    SyncTargetsFromScene(scene);

    SetNextTarget();

    spawnDefaultPos_ = glm::vec2(0.0f, 5.0f);
}

void TargetUI::SyncTargetsFromScene(const Scene& scene) {
    targets_.clear();

    for (auto* obj : scene.GetAllObjects()) {
        if (auto* t = dynamic_cast<TargetObject*>(obj)) {
            targets_.push_back(t);
        }
    }
}

// ------------------------------------------------------------
// Target creation / deletion
// ------------------------------------------------------------

void TargetUI::CreateTarget(Scene& scene) {
    auto targetUq = std::make_unique<TargetObject>();
    targetUq->transform.position = spawnDefaultPos_;
    targetUq->lastPosition       = spawnDefaultPos_;

    const uint32_t id = scene.AddObject(std::move(targetUq));

    auto* obj = scene.GetObjById(id);
    auto* tgt = dynamic_cast<TargetObject*>(obj);
    if (!tgt) return;

    targets_.push_back(tgt);
    if (targets_.size() == 1)
        SetNextTarget();
}

void TargetUI::DeleteTargetAt(Scene& scene, const int index) {
    if (index < 0 || index >= static_cast<int>(targets_.size())) return;

    TargetObject* t = targets_[index];
    if (t && scene.GetObjById(t->id) == t) {
        scene.DeleteObject(t->id);
    }

    targets_.erase(targets_.begin() + index);
}

// ------------------------------------------------------------
// Gameplay logic
// ------------------------------------------------------------

// ------------------------------------------------------------
// UI drawing
// ------------------------------------------------------------

void TargetUI::DrawCreateButtons(Scene& scene) {
    if (ImGui::Button("Create Target")) CreateTarget(scene);
    ImGui::Separator();
}

void TargetUI::DrawTargetsList(Scene& scene) {
    int displayIdx = 0; // counts only visible targets

    for (int i = 0; i < static_cast<int>(targets_.size()); ++i) {
        TargetObject* obj = targets_[i];
        if (!obj) continue;

        if (obj->IsReached() && i == 0) continue;

        ++displayIdx;

        ImGui::PushID(i);

        ImGui::Text(i == 0 ? "Target %d (active)" : "Target %d", displayIdx);
        ImGui::SameLine(kLabelX);

        ImGui::SetNextItemWidth(kFloat2Width);
        glm::vec2 pos = obj->transform.position;
        if (DrawFloat2Control("##Position", &pos, 0.1f)) {
            obj->transform.position = pos;
            obj->lastPosition       = pos;
        }

        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
            DeleteTargetAt(scene, i);
            ImGui::PopID();
            break; // vector changed
        }

        ImGui::PopID();
    }

    ImGui::Separator();
}

void TargetUI::DrawReachParams() {
    ImGui::SetNextItemWidth(kFloatWidth);
    ImGui::DragFloat("Reach tolerance", &TargetObject::reachRadius, 0.01f, 0.0f, 100.0f, "%.2f");

    ImGui::SetNextItemWidth(kFloatWidth);
    ImGui::DragFloat("Hold time (s)", &TargetObject::reachTime, 0.01f, 0.0f, 5.0f, "%.2f");
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

void TargetUI::Draw(Scene *scene) {
    ImGui::SeparatorText("Target Settings");

    if (!scene) {
        ImGui::TextDisabled("No scene loaded.");
        return;
    }

    // Always visible settings
    DrawReachParams();
    ImGui::Separator();

    // Creation
    DrawCreateButtons(*scene);

    // Targets section only if at least one is visible
    const bool hasVisibleTargets = !targets_.empty();
    if (hasVisibleTargets) {
        ImGui::SeparatorText("Targets");
        DrawTargetsList(*scene);
    } else {
        ImGui::TextDisabled("No targets created.");
    }
}