//
// Created by Lorenz Saalmann on 01.12.25.
//

#include <GL/glew.h>
#include "EngineLayer.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include "Rendering/CircleRenderer.h"
#include "Rendering/ShaderLoader.h"

static std::string LoadFile(const std::string &path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

EngineLayer::EngineLayer() : AppLayer() {
}

EngineLayer::~EngineLayer() {
}

void EngineLayer::OnInit() {
    glewInit();

    circleShader = ShaderLoader::LoadShader("simple.vert", "simple.frag");

    m_Circle = std::make_unique<CircleRenderer>(circleShader, 64);
}

void EngineLayer::OnUpdate(float deltaTime) {
}

void EngineLayer::OnEvent(Core::Event &event) {
}

void EngineLayer::OnRender() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (m_Circle) {
        m_Circle->RenderCircle(
            glm::vec2(0.0f, 0.0f),    // position
            0.0f,                    // rotation
            glm::vec2(0.5f, 0.5f),   // scale
            glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // color (green)
            0.5f                     // radius
        );
        m_Circle->RenderCircle(
            glm::vec2(1.0f, 0.0f),    // position
            0.0f,                    // rotation
            glm::vec2(0.5f, 0.5f),   // scale
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // color (red)
            0.5f                     // radius
        );
    }
}
