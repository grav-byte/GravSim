//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include "Core/AppLayer.h"
#include <memory>

#include "Engine/Scene.h"

class RenderingSystem;

class EngineLayer : public Core::AppLayer{
public:
    EngineLayer();
    ~EngineLayer() override;

    void OnInit() override;

    void OnUpdate(float deltaTime) override;

    void OnEvent(Core::Event &event) override;

    void OnRender() override;

    Scene* GetScene() const { return scene_.get(); }

private:
    std::unique_ptr<RenderingSystem> renderingSystem_;
    std::unique_ptr<Scene> scene_;
};
