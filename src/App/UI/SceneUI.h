//
// Created by Lorenz Saalmann on 05.12.25.
//

#pragma once

#include "IUserInterface.h"
#include "App/EngineLayer.h"
#include "App/Engine/Scene.h"

class SceneUI : public IUserInterface {
public:
    SceneUI();
    ~SceneUI() override;

    void OnEvent(Core::Event &event) override;
    void Render() override;

private:
    EngineLayer* engine_;
    Scene* scene_;
};
