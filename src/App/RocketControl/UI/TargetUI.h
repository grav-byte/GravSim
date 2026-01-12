//
// Created by kw on 08.01.2026.
//

#pragma once

#include <limits>
#include <vector>

#include <glm/vec2.hpp>

#include "App/RocketControl/RocketObject.h"

class Scene;
class AutonomousPIDRocketController;
class RocketObject;
class TargetObject;

class TargetUI {
public:
    TargetUI() = default;

    // ImGui entry point
    void Draw(Scene* scene, AutonomousPIDRocketController* autoCtrl);

    // Call once after a scene was loaded
    void OnSceneLoaded(const Scene& scene, RocketObject* rocket);

    // ----- gameplay -----
    void MirrorActiveTargetToController(AutonomousPIDRocketController *autoCtrl) const;

private:
    static constexpr uint32_t kInvalidId = std::numeric_limits<uint32_t>::max();

    // ----- Scene sync / lookups -----
    void SyncTargetsFromScene(const Scene& scene);

    // ----- Target lifecycle (Scene owns targets) -----
    void CreateTarget(Scene& scene);
    void DeleteTargetAt(Scene& scene, int index);

    // ----- UI helpers -----
    void DrawCreateButtons(Scene& scene);
    static void DrawReachParams();
    void DrawTargetsList(Scene& scene);
    static bool DrawFloat2Control(const char* label, glm::vec2* v, float speed = 0.1f);

    // Non-owning pointers (Scene owns TargetObjects)
    std::vector<TargetObject*> targets_;

    // Default spawn position for new targets
    glm::vec2 spawnDefaultPos_ = {0.0f, 5.0f};

    // Completion state (completed target stays in Scene until timer ends)
    bool targetReached_ = false;
    float explodeTimer_ = 0.0f;
    float explodeDuration_ = 5.0f;

    // Rocket cache (validated via id each time)
    RocketObject* cachedRocket_ = nullptr;
    uint32_t cachedRocketId_ = kInvalidId;
};