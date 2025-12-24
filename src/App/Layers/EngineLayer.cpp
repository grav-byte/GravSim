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

SceneRenderer * EngineLayer::GetSceneRenderer() const {
    return sceneRenderer_.get();
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
    physicsSolver_->StepPropagation(scene_.get());
}

void EngineLayer::StopSimulation() {
    runningSimulation_ = false;
    pausedSimulation_ = false;
    scene_ = std::move(SceneLoader::LoadTempScene());
    OnSceneLoaded();
}

void EngineLayer::SetSolverType(const char* typeName) const {
    physicsSolver_->SetActivePropagator(typeName);
}

IPropagator* EngineLayer::GetActivePropagator() const {
    return physicsSolver_->GetActivePropagator();
}

void EngineLayer::SetTimeStep(const float timeStep) const {
    physicsSolver_->SetTimeStep(timeStep);
}

float EngineLayer::GetTimeStep() const {
    return physicsSolver_->GetTimeStep();
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

    // setup rendering system
    renderingSystem_ = std::make_unique<RenderingSystem>();
    renderingSystem_->CreateSceneFramebuffer();
    sceneRenderer_ = std::make_unique<SceneRenderer>(renderingSystem_.get());

    NewScene();
}

void EngineLayer::OnUpdate(float deltaTime) {
    if (runningSimulation_ && !pausedSimulation_)
        physicsSolver_->UpdatePhysics(scene_.get(), deltaTime);

    cameraController_.Update(deltaTime);
}

void EngineLayer::OnEvent(Core::Event &event) {
    if (event.GetEventType() == Core::WindowResized) {
        renderingSystem_->CreateSceneFramebuffer();
    }

    cameraController_.OnEvent(event);
}

void EngineLayer::OnRender() {
    auto backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    if (scene_)
        backgroundColor = scene_->GetCamera()->backgroundColor;

    // render scene to framebuffer
    renderingSystem_->StartFrame(backgroundColor);
    sceneRenderer_->RenderScene(scene_.get(), showColliders);

    // apply post-processing effects
    sceneRenderer_->ApplyPostProcess();

    // render framebuffer to screen
    renderingSystem_->OutputFrameToScreen();
}

void EngineLayer::CreateObject() const {
    if (!scene_) return;
    scene_->CreateObject();
}

void EngineLayer::CreateObjectAt(const glm::vec2& worldPos) const {
    if (!scene_) return;
    scene_->CreateObject(worldPos);
}