//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include "Core/AppLayer.h"
#include <memory>

#include "../Engine/Scene.h"
#include "App/Engine/CameraController.h"

class RenderingSystem;

class EngineLayer : public Core::AppLayer{
public:
    EngineLayer();
    ~EngineLayer() override;

    void NewScene();

    bool LoadScene(const std::string &filePath);

    void OnInit() override;

    void OnUpdate(float deltaTime) override;

    void OnEvent(Core::Event &event) override;

    void OnRender() override;

    bool SaveScene() const;

    Scene* GetScene() const { return scene_.get(); }

    CameraController* GetCameraController();

private:
    void OnSceneLoaded() const;

    std::unique_ptr<RenderingSystem> renderingSystem_;
    std::unique_ptr<Scene> scene_;
    CameraController cameraController_;
};
