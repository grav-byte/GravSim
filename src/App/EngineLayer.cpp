//
// Created by Lorenz Saalmann on 01.12.25.
//

#include <GL/glew.h>
#include "EngineLayer.h"
#include <sstream>

#include "Rendering/RenderingSystem.h"
#include "Rendering/ShaderLoader.h"

EngineLayer::EngineLayer() : AppLayer() {
    scene_ = std::make_unique<Scene>();
}

EngineLayer::~EngineLayer() = default;

void EngineLayer::OnInit() {
    glewInit();

    auto shader = ShaderLoader::LoadShader("simple.vert", "simple.frag");
    renderingSystem_ = std::make_unique<RenderingSystem>(shader, 64);
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
