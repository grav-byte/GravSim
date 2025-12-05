//
// Created by Lorenz Saalmann on 01.12.25.
//

#include <GL/glew.h>
#include "EngineLayer.h"
#include <sstream>

#include "Core/Application.h"
#include "Engine/EngineEvents.h"
#include "Rendering/RenderingSystem.h"
#include "Rendering/ShaderLoader.h"

EngineLayer::EngineLayer() : AppLayer() {
    scene_ = nullptr;
}

EngineLayer::~EngineLayer() = default;

void EngineLayer::LoadScene(std::unique_ptr<Scene> scene) {
    scene_ = std::move(scene);
    renderingSystem_->SetActiveCamera(scene_->GetCamera());

    SceneLoadedEvent event(scene_.get());
    Core::Application::Get().RaiseEvent(event);
}

void EngineLayer::OnInit() {
    glewInit();
    auto shader = ShaderLoader::LoadShader("simple.vert", "simple.frag");

    renderingSystem_ = std::make_unique<RenderingSystem>(shader, 64);

    LoadScene(std::make_unique<Scene>());
}

void EngineLayer::OnUpdate(float deltaTime) {
}

void EngineLayer::OnEvent(Core::Event &event) {
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
