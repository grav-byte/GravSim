//
// Created by Lorenz Saalmann on 01.12.25.
//

#include <GL/glew.h>
#include <sstream>

#include "EngineLayer.h"
#include "Core/Application.h"
#include "../Engine/EngineEvents.h"
#include "../Rendering/RenderingSystem.h"
#include "../Rendering/ShaderLoader.h"
#include "App/Engine/SceneLoader.h"

EngineLayer::EngineLayer() : AppLayer() {
    SceneLoader::EnsureSceneFolderExists();
    scene_ = nullptr;
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

void EngineLayer::OnSceneLoaded() const {
    renderingSystem_->SetActiveCamera(scene_->GetCamera());

    SceneLoadedEvent event(scene_.get());
    Core::Application::Get().RaiseEvent(event);
}

bool EngineLayer::SaveScene() const {
    return SceneLoader::SaveScene(*scene_);
}

void EngineLayer::OnInit() {
    glewInit();
    auto shader = ShaderLoader::LoadShader("simple.vert", "simple.frag");

    renderingSystem_ = std::make_unique<RenderingSystem>(shader, 64);

    NewScene();
}

void EngineLayer::OnUpdate(float deltaTime) {
}

void EngineLayer::OnEvent(Core::Event &event) {
    cameraController_.OnEvent(event);
}

void EngineLayer::OnRender() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const SceneObject* obj : scene_->GetAllObjects()) {
        if (obj->renderer) {
            obj->renderer->Render(*renderingSystem_, obj->transform);
        }
    }
}
