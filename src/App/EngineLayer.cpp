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

    std::cout << "ENTER OnInit()" << std::endl;
    circleShader = ShaderLoader::LoadShader("circle.vert", "circle.frag");
    std::cout << "circleShader = " << circleShader << std::endl;

    m_Circle = std::make_unique<CircleRenderer>(
        0.1f,
        glm::vec4(1, 0, 0, 1),
        circleShader,
        64                      // optional: Segmente
    );
    std::cout << "After CircleRenderer creation" << std::endl;

}

void EngineLayer::OnUpdate(float deltaTime) {
}

void EngineLayer::OnEvent(Core::Event &event) {
}

void EngineLayer::OnRender() {
    std::cout << "Rendering..." << std::endl;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (m_Circle) {
        m_Circle->Render();
    }
}
