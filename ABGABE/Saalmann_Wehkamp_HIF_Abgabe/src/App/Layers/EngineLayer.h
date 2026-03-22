#pragma once

#include "Core/AppLayer.h"
#include <memory>

#include "App/Engine/Scene.h"
#include "App/Engine/CameraController.h"
#include "App/Engine/Physics/PhysicsSolver.h"
#include "App/Rendering/Renderers/SceneRenderer.h"

class RenderingSystem;

class EngineLayer : public Core::AppLayer{
public:
    EngineLayer();
    ~EngineLayer() override;

    void NewScene();

    bool LoadScene(const std::string &filePath);

    void OnInit() override;

    void OnUpdate(float deltaTime) override;

    void OnEvent(Core::Event &event) override;

    void OnRender() override;

    bool SaveScene() const;

    void StartSimulation();
    void PauseSimulation();
    void StepSimulation() const;
    void StopSimulation();

    void SetSolverType(const char *typeName) const;
    IPropagator *GetActivePropagator() const;
    PhysicsContext GetPhysicsContext() const;

    void SetTimeStep(float timeStep) const;
    float GetTimeStep() const;

    bool IsRunningSimulation() const;
    bool IsSimulationPaused() const;

    Scene* GetScene() const { return scene_.get(); }
    CameraController* GetCameraController();
    SceneRenderer *GetSceneRenderer() const;

    void Schedule(const std::function<void()>& func, float delaySeconds);

    bool showColliders = false;

private:
    void OnSceneLoaded() const;

    struct ScheduledTask {
        float executeAt;                   // global time in seconds
        std::function<void()> callback;
    };

    std::vector<ScheduledTask> scheduledTasks;

    bool runningSimulation_;
    bool pausedSimulation_;

    std::unique_ptr<Scene> scene_;
    CameraController cameraController_;

    std::unique_ptr<PhysicsSolver> physicsSolver_;

    std::unique_ptr<SceneRenderer> sceneRenderer_;
    std::unique_ptr<RenderingSystem> renderingSystem_;

};
