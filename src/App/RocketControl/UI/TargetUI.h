//
// Created by kw on 08.01.2026.
//

#pragma once

#include <cstdint>
#include <vector>
#include <glm/vec2.hpp>

class Scene;
class AutonomousPIDRocketController;
class TargetObject;

class TargetUI {
public:
    TargetUI() = default;

    // Main ImGui entry point
    void Draw(Scene* scene, AutonomousPIDRocketController* autoCtrl);

    // Call on scene change; rebuilds internal target list from the scene
    void OnSceneLoaded();

private:
    // ----- Scene sync / ownership -----
    // Raw pointers are non-owning; Scene owns TargetObjects
    void SyncTargetsFromScene(Scene& scene);
    void CleanupMissingTargets(Scene& scene);
    bool IsAlive(Scene& scene, TargetObject* t) const;

    // ----- Target lifecycle -----
    void CreateTarget(Scene& scene);
    void DeleteTargetAt(Scene& scene, int index);
    TargetObject* GetTargetAt(Scene& scene, int index) const;

    // ----- Gameplay logic -----
    void TargetReached(Scene& scene);      // starts completion on current solver target
    void UpdateCompletion(Scene& scene);   // deletes completing target after timer
    void UpdateReachedDetection(Scene& scene);
    void MirrorActiveTargetToController(Scene& scene, AutonomousPIDRocketController* autoCtrl);

    // Which target the controller should fly to (can differ from the completing target)
    int ActiveIndex() const { return solverIndex_; }

    // ----- UI drawing (pure ImGui) -----
    void DrawCreateButtons(Scene& scene);
    void DrawTargetsList(Scene& scene);
    void DrawReachParams();
    static bool DrawFloat2Control(const char* label, glm::vec2* v, float speed = 0.1f);

    // True after a scene load; triggers SyncTargetsFromScene() once in Draw()
    bool needsSync_ = true;

    // Non-owning pointers to TargetObjects currently in the scene
    std::vector<TargetObject*> targets_;

    // Spawn defaults for newly created targets
    glm::vec2 spawnDefaultPos_ = {0.0f, 5.0f};

    // Indices into targets_:
    // - solverIndex_: current target passed to the controller
    // - completingIndex_: target playing the completion effect (deleted after timer)
    int solverIndex_ = -1;
    int completingIndex_ = -1;

    // Completion timing
    bool targetReached_ = false;
    float explodeTimer_ = 0.0f;
    float explodeDuration_ = 5.0f;

    // Reach detection (hold inside tolerance)
    float reachedTimer_ = 0.0f;
    float reachedHoldTime_ = 3.0f;
    float reachedTolerance_ = 1.0f;
};