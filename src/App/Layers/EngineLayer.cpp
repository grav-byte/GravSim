#include <GL/glew.h>
#include <sstream>

#include "EngineLayer.h"

#include <iostream>

#include "Core/Application.h"
#include "App/Engine/EngineEvents.h"
#include "App/Rendering/RenderingSystem.h"
#include "App/Engine/Loading/Serialiser.h"
#include "App/Rendering/Visuals/ShaderVisual.h"
#include "App/RocketControl/RocketObject.h"

EngineLayer::EngineLayer() : AppLayer() {
    Serialiser::EnsureSceneFolderExists();
    physicsSolver_ = std::make_unique<PhysicsSolver>();
    scene_ = nullptr;
    runningSimulation_ = false;
    pausedSimulation_ = false;
    cameraController_ = CameraController();
}

EngineLayer::~EngineLayer() = default;

void EngineLayer::NewScene() {
    scene_ = std::make_unique<Scene>();

    auto obj = std::make_unique<SceneObject>();
    auto visual = ShaderVisual();
    visual.shaderPath = "completed.frag";
    visual.shaderData.textures["uNoiseTex"] = std::string("../assets/textures/noise_tex.png");
    visual.shaderData.floats["uTimeOffset"] = 5.0f;

    obj->transform.scale = glm::vec2(5.0f, 5.0f);
    obj->visual = std::make_unique<ShaderVisual>(visual);
    scene_->AddObject(std::move(obj));

    OnSceneLoaded();
}

bool EngineLayer::LoadScene(const std::string &filePath) {
    auto loadedScene = Serialiser::LoadScene(filePath);
    if (!loadedScene)
        return false;

    scene_ = std::move(loadedScene);

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
    return Serialiser::SaveScene(*scene_);
}

void EngineLayer::StartSimulation() {
    if (runningSimulation_) {
        // resume simulation
        pausedSimulation_ = false;
        auto event = SimulationResumedEvent();
        Core::Application::Get().RaiseEvent(event);
        return;
    }

    Serialiser::SaveTempScene(*scene_);
    runningSimulation_ = true;

    auto event = SimulationStartedEvent();
    Core::Application::Get().RaiseEvent(event);
}

void EngineLayer::PauseSimulation() {
    pausedSimulation_ = true;
    auto event = SimulationPausedEvent();
    Core::Application::Get().RaiseEvent(event);
}

void EngineLayer::StepSimulation() const {
    if (!runningSimulation_) {
        std::cout << "Simulation is not running!" << std::endl;
        return;
    }
    physicsSolver_->StepPropagation(scene_.get());
    auto event = SimulationSteppedEvent();
    Core::Application::Get().RaiseEvent(event);
}

void EngineLayer::StopSimulation() {
    runningSimulation_ = false;
    pausedSimulation_ = false;
    scene_ = std::move(Serialiser::LoadTempScene());

    auto event = SimulationStoppedEvent();
    Core::Application::Get().RaiseEvent(event);

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

void EngineLayer::OnUpdate(const float deltaTime) {
    if (runningSimulation_ && !pausedSimulation_)
        physicsSolver_->UpdatePhysics(scene_.get(), deltaTime);

    cameraController_.Update();
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

    // clear debug arrows
    for (const auto& obj : scene_->GetAllObjects()) {
        obj->debugArrows.clear();
    }
}

void EngineLayer::CreateObject() const {
    if (!scene_) return;
    scene_->CreateObject();
}

void EngineLayer::CreateObjectAt(const glm::vec2& worldPos) const {
    if (!scene_) return;
    scene_->CreateObject(worldPos);
}