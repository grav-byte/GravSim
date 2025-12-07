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

void EngineLayer::LoadScene(const std::string &filePath) {
    scene_ = std::move(SceneLoader::LoadScene(filePath));
    if (!scene_)
        return;
    OnSceneLoaded();
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

    auto circleShader = ShaderLoader::LoadShader("simple.vert", "simple.frag");
    auto spriteShader = ShaderLoader::LoadShader("sprite.vert", "sprite.frag");

    renderingSystem_ = std::make_unique<RenderingSystem>(circleShader, spriteShader, 64);

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
