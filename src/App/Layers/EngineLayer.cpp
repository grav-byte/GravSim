//
// Created by Lorenz Saalmann on 01.12.25.
//

#include <GL/glew.h>
#include <sstream>

#include "EngineLayer.h"

#include <iostream>

#include "Core/Application.h"
#include "../Engine/EngineEvents.h"
#include "../Rendering/RenderingSystem.h"
#include "../Rendering/ShaderLoader.h"
#include "App/Engine/SceneLoader.h"

EngineLayer::EngineLayer() : AppLayer() {
    SceneLoader::EnsureSceneFolderExists();
    physicsSolver_ = std::make_unique<PhysicsSolver>();
    scene_ = nullptr;
    runningSimulation_ = false;
    pausedSimulation_ = false;
    cameraController_ = CameraController();
}

EngineLayer::~EngineLayer() = default;

void EngineLayer::NewScene() {
    scene_ = std::make_unique<Scene>();
    OnSceneLoaded();
}

bool EngineLayer::LoadScene(const std::string &filePath) {
    auto loadedScene = SceneLoader::LoadScene(filePath);
    if (!loadedScene)
        return false;

    scene_ = std::move(loadedScene);
    if (!scene_)
        return false;
    OnSceneLoaded();
    return true;
}


CameraController * EngineLayer::GetCameraController() {
    return &cameraController_;
}

void EngineLayer::OnSceneLoaded() const {
    renderingSystem_->SetActiveCamera(scene_->GetCamera());

    SceneLoadedEvent event(scene_.get());
    Core::Application::Get().RaiseEvent(event);
}

bool EngineLayer::SaveScene() const {
    return SceneLoader::SaveScene(*scene_);
}

void EngineLayer::StartSimulation() {
    if (runningSimulation_) {
        pausedSimulation_ = false;
        return;
    }
    SceneLoader::SaveTempScene(*scene_);
    runningSimulation_ = true;
}

void EngineLayer::PauseSimulation() {
    pausedSimulation_ = true;
}

void EngineLayer::StepSimulation() const {
    if (!runningSimulation_) {
        std::cout << "Simulation is not running!" << std::endl;
        return;
    }
    physicsSolver_->UpdatePhysics(scene_.get(), 1.0f / 60.0f);
}

void EngineLayer::StopSimulation() {
    runningSimulation_ = false;
    pausedSimulation_ = false;
    scene_ = std::move(SceneLoader::LoadTempScene());
    OnSceneLoaded();
}

bool EngineLayer::IsRunningSimulation() const {
    return runningSimulation_;
}

bool EngineLayer::IsSimulationPaused() const {
    return pausedSimulation_;
}

void EngineLayer::OnInit() {
    glewInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto circleShader = ShaderLoader::LoadShader("simple.vert", "simple.frag");
    auto spriteShader = ShaderLoader::LoadShader("sprite.vert", "sprite.frag");

    renderingSystem_ = std::make_unique<RenderingSystem>(circleShader, spriteShader, 64);

    NewScene();
}

void EngineLayer::OnUpdate(float deltaTime) {
    if (runningSimulation_ && !pausedSimulation_)
        physicsSolver_->UpdatePhysics(scene_.get(), deltaTime);
}

void EngineLayer::OnEvent(Core::Event &event) {
    cameraController_.OnEvent(event);
}

void EngineLayer::OnRender() {
    auto backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (scene_)
        backgroundColor = scene_->GetCamera()->backgroundColor;
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const SceneObject* obj : scene_->GetAllObjects()) {
        if (obj->renderer) {
            obj->renderer->Render(*renderingSystem_, obj->transform);
        }
    }
}
