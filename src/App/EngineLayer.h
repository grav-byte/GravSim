//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include "Core/AppLayer.h"
#include <memory>

class CircleRenderer;

class EngineLayer : public Core::AppLayer{
public:
    EngineLayer();
    ~EngineLayer() override;

    void OnInit() override;

    void OnUpdate(float deltaTime) override;

    void OnEvent(Core::Event &event) override;

    void OnRender() override;

private:
    unsigned int circleShader = 0;                   // Shader-Program-ID
    std::unique_ptr<CircleRenderer> m_Circle;        // unser Kreis-Renderer
};
