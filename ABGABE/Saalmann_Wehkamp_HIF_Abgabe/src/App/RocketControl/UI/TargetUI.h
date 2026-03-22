//
// Created by kw on 08.01.2026.
//

#pragma once

#include <vector>

#include <glm/vec2.hpp>

#include "App/RocketControl/ControlLayer.h"
#include "App/RocketControl/RocketObject.h"
#include "App/RocketControl/Controllers/IRocketController.h"

class Scene;
class AutonomousPIDRocketController;
class RocketObject;
class TargetObject;

class TargetUI {
public:
    TargetUI();

    // ImGui entry point
    void Draw(Scene *scene);

    // Call once after a scene was loaded
    void OnSceneLoaded(const Scene &scene);

private:
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

    void SetNextTarget();

    // Non-owning pointers (Scene owns TargetObjects)
    std::vector<TargetObject*> targets_;

    // Default spawn position for new targets
    glm::vec2 spawnDefaultPos_ = {0.0f, 5.0f};

    ControlLayer* controlLayer_ = nullptr;
};