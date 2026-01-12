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

bool TargetUI::DrawFloat2Control(const char* label, glm::vec2* v, float speed) {
    float value[2] = { v->x, v->y };
    const bool updated = ImGui::DragFloat2(label, value, speed);
    if (updated) *v = glm::vec2(value[0], value[1]);
    return updated;
}

// ------------------------------------------------------------
// Scene lifecycle / sync
// ------------------------------------------------------------

void TargetUI::OnSceneLoaded(Scene& scene, RocketObject* rocket) {
    cachedRocket_ = rocket;

    // Rebuild immediately (no deferred sync needed)
    SyncTargetsFromScene(scene);

    // Reset runtime state
    targetReached_ = false;
    explodeTimer_  = 0.0f;
    reachedTimer_  = 0.0f;

    spawnDefaultPos_ = glm::vec2(0.0f, 5.0f);

    cachedRocketId_ = kInvalidId;
}

bool TargetUI::IsAlive(Scene& scene, TargetObject* t) const {
    return t && scene.GetObjById(t->id) == t;
}

void TargetUI::SyncTargetsFromScene(Scene& scene) {
    targets_.clear();

    for (auto* obj : scene.GetAllObjects()) {
        if (auto* t = dynamic_cast<TargetObject*>(obj)) {
            targets_.push_back(t);
        }
    }
}

void TargetUI::CleanupMissingTargets(Scene& scene) {
    for (int i = static_cast<int>(targets_.size()) - 1; i >= 0; --i) {
        if (!IsAlive(scene, targets_[i])) {
            targets_.erase(targets_.begin() + i);
        }
    }

    if (targets_.empty()) {
        // No targets left → clear state
        reachedTimer_  = 0.0f;
        targetReached_ = false;
        explodeTimer_  = 0.0f;
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
}

void TargetUI::DeleteTargetAt(Scene& scene, int index) {
    if (index < 0 || index >= (int)targets_.size()) return;

    // If completing target gets deleted, cancel completion state
    if (targetReached_ && index == 0) {
        targetReached_ = false;
        explodeTimer_  = 0.0f;
        reachedTimer_  = 0.0f;
    }

    TargetObject* t = targets_[index];
    if (t && scene.GetObjById(t->id) == t) {
        scene.DeleteObject(t->id);
    }

    targets_.erase(targets_.begin() + index);

    if (targets_.empty()) {
        reachedTimer_  = 0.0f;
        targetReached_ = false;
        explodeTimer_  = 0.0f;
    }
}

TargetObject* TargetUI::GetTargetAt(Scene& scene, int index) const {
    if (index < 0 || index >= (int)targets_.size()) return nullptr;
    TargetObject* t = targets_[index];
    return IsAlive(scene, t) ? t : nullptr;
}

// ------------------------------------------------------------
// Gameplay logic
// ------------------------------------------------------------

void TargetUI::TargetReached(Scene& scene) {
    TargetObject* targetObj = GetTargetAt(scene, 0);
    if (!targetObj) return;

    targetObj->PlayCompletionEffect();

    if (auto* audio = Core::Application::Get().GetLayer<AudioLayer>()) {
        audio->PlaySound("../assets/audio/completed.wav", 5.0);
    }

    targetReached_ = true;
    explodeTimer_  = explodeDuration_;
    reachedTimer_  = 0.0f;
}

void TargetUI::UpdateCompletion(Scene& scene) {
    if (!targetReached_) return;

    explodeTimer_ -= ImGui::GetIO().DeltaTime;

    // After the effect duration, delete the completing target (always index 0)
    DeleteTargetAt(scene, 0);

    targetReached_ = false;
    explodeTimer_  = 0.0f;
    reachedTimer_  = 0.0f;
}

void TargetUI::UpdateReachedDetection(Scene& scene) {
    if (targetReached_) return;

    TargetObject* target = GetTargetAt(scene, 0);
    if (!cachedRocket_ || !target) {
        reachedTimer_ = 0.0f;
        return;
    }

    const float dt = ImGui::GetIO().DeltaTime;
    const float dist = glm::length(cachedRocket_->transform.position - target->transform.position);

    if (dist <= reachedTolerance_) {
        reachedTimer_ += dt;
        if (reachedTimer_ >= reachedHoldTime_) {
            reachedTimer_ = 0.0f;
            TargetReached(scene);
        }
    } else {
        reachedTimer_ = 0.0f;
    }
}

void TargetUI::MirrorActiveTargetToController(Scene& scene, AutonomousPIDRocketController* autoCtrl) {
    if (!autoCtrl) return;

    TargetObject* activeObj = GetTargetAt(scene, 0);
    if (activeObj) {
        autoCtrl->targetPos = activeObj->transform.position;
    }
}

// ------------------------------------------------------------
// UI drawing
// ------------------------------------------------------------

void TargetUI::DrawCreateButtons(Scene& scene) {
    if (ImGui::Button("Create Target")) CreateTarget(scene);
    ImGui::Separator();
}

void TargetUI::DrawTargetsList(Scene& scene) {
    for (int i = 0; i < static_cast<int>(targets_.size()); ++i) {
        // Hide completing target immediately from UI (still exists in scene for the shader)
        if (targetReached_ && i == 0) continue;

        TargetObject* obj = GetTargetAt(scene, i);
        if (!obj) continue;

        ImGui::PushID(i);

        ImGui::Text(i == 0 ? "Target %d (active)" : "Target %d", i + 1);
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
    ImGui::DragFloat("Reach tolerance", &reachedTolerance_, 0.01f, 0.0f, 100.0f, "%.2f");

    ImGui::SetNextItemWidth(kFloatWidth);
    ImGui::DragFloat("Hold time (s)", &reachedHoldTime_, 0.01f, 0.0f, 5.0f, "%.2f");
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

void TargetUI::Draw(Scene* scene, AutonomousPIDRocketController* autoCtrl) {
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

    // Keep list valid
    CleanupMissingTargets(*scene);

    // Completion countdown + delayed delete
    UpdateCompletion(*scene);

    // Targets section only if at least one is visible
    const bool hasVisibleTargets = !targets_.empty() && !(targetReached_ && targets_.size() == 1);
    if (hasVisibleTargets) {
        ImGui::SeparatorText("Targets");
        DrawTargetsList(*scene);
    } else {
        ImGui::TextDisabled("No targets created.");
    }

    // Controller / logic
    MirrorActiveTargetToController(*scene, autoCtrl);
    UpdateReachedDetection(*scene);
}