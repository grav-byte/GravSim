//
// Created by kw on 08.01.2026.
//

#include "App/RocketControl/UI/TargetUI.h"

#include <algorithm>

#include "imgui.h"
#include "App/Engine/Scene.h"
#include "App/RocketControl/Controllers/AutonomousPIDRocketController.h"
#include "App/RocketControl/RocketObject.h"
#include "App/RocketControl/TargetObject.h"

// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------

bool TargetUI::DrawFloat2Control(const char* label, glm::vec2* v, float speed) {
    float value[2] = { v->x, v->y };
    const bool updated = ImGui::DragFloat2(label, value, speed);
    if (updated) *v = glm::vec2(value[0], value[1]);
    return updated;
}

static RocketObject* FindRocket(Scene& scene) {
    // TODO pass from parent and cache instead of searching every time

    for (auto* obj : scene.GetAllObjects()) {
        if (!obj) continue;
        if (obj->name == "Rocket") {
            if (auto* rocket = dynamic_cast<RocketObject*>(obj)) return rocket;
        }
    }
    return nullptr;
}

// ------------------------------------------------------------
// Scene lifecycle / sync
// ------------------------------------------------------------

void TargetUI::OnSceneLoaded() {
    // Rebuild targets from the scene on next Draw()

    // TODO: why not immediately here? pass scene as argument: scene_ = dynamic_cast<SceneLoadedEvent&>(event).GetScene();

    needsSync_ = true;

    targetReached_ = false;
    explodeTimer_ = 0.0f;
    reachedTimer_ = 0.0f;

    solverIndex_ = -1;
    completingIndex_ = -1;

    spawnDefaultPos_ = glm::vec2(0.0f, 5.0f);
}

bool TargetUI::IsAlive(Scene& scene, TargetObject* t) const {
    if (!t) return false;
    return scene.GetObjById(t->id) == t;
}

void TargetUI::SyncTargetsFromScene(Scene& scene) {
    targets_.clear();

    for (auto* obj : scene.GetAllObjects()) {
        if (auto* t = dynamic_cast<TargetObject*>(obj)) {
            targets_.push_back(t);
        }
    }

    // TODO they will be in order - dont worry
    // Stable order so "oldest" is deterministic
    std::sort(targets_.begin(), targets_.end(),
              [](TargetObject* a, TargetObject* b) { return a->id < b->id; });

    solverIndex_ = targets_.empty() ? -1 : 0;
    completingIndex_ = -1;

    targetReached_ = false;
    explodeTimer_ = 0.0f;
    reachedTimer_ = 0.0f;
}

void TargetUI::CleanupMissingTargets(Scene& scene) {
    // TODO optimize by tracking deletions only

    for (int i = (int)targets_.size() - 1; i >= 0; --i) {
        if (!IsAlive(scene, targets_[i])) {
            targets_.erase(targets_.begin() + i);
        }
    }

    if (targets_.empty()) {
        solverIndex_ = -1;
        completingIndex_ = -1;
        reachedTimer_ = 0.0f;
        targetReached_ = false;
        explodeTimer_ = 0.0f;
    } else {
        if (solverIndex_ < 0) solverIndex_ = 0;
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

    if (solverIndex_ < 0) solverIndex_ = 0;
}

void TargetUI::DeleteTargetAt(Scene& scene, int index) {
    if (index < 0 || index >= (int)targets_.size()) return;

    TargetObject* t = targets_[index];
    if (t && scene.GetObjById(t->id) == t) {
        scene.DeleteObject(t->id);
    }

    targets_.erase(targets_.begin() + index);

    // TODO why are there indeces at all?
    // Fix indices after erase
    auto fixIndex = [index](int& idx) {
        if (idx < 0) return;
        if (idx == index) idx = -1;
        else if (idx > index) idx -= 1;
    };
    fixIndex(solverIndex_);
    fixIndex(completingIndex_);

    if (targets_.empty()) {
        solverIndex_ = -1;
        completingIndex_ = -1;
        reachedTimer_ = 0.0f;
        targetReached_ = false;
        explodeTimer_ = 0.0f;
        return;
    }

    if (solverIndex_ < 0) solverIndex_ = 0;
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
    const int active = ActiveIndex();
    if (active < 0) return;

    TargetObject* targetObj = GetTargetAt(scene, active);
    if (!targetObj) return;

    targetObj->PlayCompletionEffect();

    completingIndex_ = active;

    targetReached_ = true;
    explodeTimer_  = explodeDuration_;
    reachedTimer_  = 0.0f;

    // TODO just remove first target from the list and always fly to the first target if there is one?

    // Switch solver immediately to the next target
    if ((int)targets_.size() >= 2) {
        solverIndex_ = (active + 1 < (int)targets_.size()) ? (active + 1) : -1;
    } else {
        solverIndex_ = -1;
    }
}

void TargetUI::UpdateCompletion(Scene& scene) {
    if (!targetReached_) return;

    explodeTimer_ -= ImGui::GetIO().DeltaTime;

    if (explodeTimer_ > 0.0f) {
        ImGui::Text("Target completed... %.2fs", explodeTimer_);
        ImGui::Separator();
        return;
    }

    // TODO shouldn't completing index always be 0 since its the first target we fly to?
    // Delete the completing target when the timer is over
    if (completingIndex_ >= 0) {
        DeleteTargetAt(scene, completingIndex_);
    }

    completingIndex_ = -1;
    targetReached_ = false;
    explodeTimer_ = 0.0f;
    reachedTimer_ = 0.0f;

    // If solverIndex_ is invalid but targets exist, start with first
    if (solverIndex_ < 0 && !targets_.empty()) solverIndex_ = 0;
}

void TargetUI::UpdateReachedDetection(Scene& scene) {
    if (targetReached_) return;

    const int active = ActiveIndex();
    if (active < 0) return;

    // TODO cache rocket
    RocketObject* rocket = FindRocket(scene);
    TargetObject* target = GetTargetAt(scene, active);
    if (!rocket || !target) {
        reachedTimer_ = 0.0f;
        return;
    }

    const float dt = ImGui::GetIO().DeltaTime;
    const float dist = glm::length(rocket->transform.position - target->transform.position);

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

    TargetObject* activeObj = GetTargetAt(scene, ActiveIndex());
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
    const int active = ActiveIndex();

    for (int i = 0; i < (int)targets_.size(); ++i) {
        TargetObject* obj = GetTargetAt(scene, i);
        if (!obj) continue;

        ImGui::PushID(i);

        ImGui::Text(i == active ? "Target %d (active)" : "Target %d", i + 1);
        ImGui::SameLine(160.0f);

        const bool lock = (targetReached_ && i == completingIndex_);
        if (lock) ImGui::BeginDisabled();

        glm::vec2 pos = obj->transform.position;
        if (DrawFloat2Control("##Position", &pos, 0.1f)) {
            obj->transform.position = pos;
            obj->lastPosition       = pos;
        }

        if (lock) ImGui::EndDisabled();

        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
            // If we manually delete the completing target, cancel completion
            if (i == completingIndex_ && targetReached_) {
                targetReached_ = false;
                explodeTimer_ = 0.0f;
                reachedTimer_ = 0.0f;
                completingIndex_ = -1;
            }

            DeleteTargetAt(scene, i);
            ImGui::PopID();
            break; // targets_ changed
        }

        ImGui::PopID();
    }

    ImGui::Separator();
}

void TargetUI::DrawReachParams() {
    ImGui::DragFloat("Reach tolerance", &reachedTolerance_, 0.01f, 0.0f, 100.0f, "%.2f");
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

    if (needsSync_) {
        SyncTargetsFromScene(*scene);
        needsSync_ = false;
    }

    DrawCreateButtons(*scene);

    CleanupMissingTargets(*scene);

    if (targets_.empty()) {
        ImGui::TextDisabled("No targets created.");
        return;
    }

    UpdateCompletion(*scene);
    DrawTargetsList(*scene);
    DrawReachParams();
    UpdateReachedDetection(*scene);
    MirrorActiveTargetToController(*scene, autoCtrl);
}