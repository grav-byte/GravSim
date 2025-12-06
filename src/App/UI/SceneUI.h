//
// Created by Lorenz Saalmann on 05.12.25.
//

#pragma once

#include "IUserInterface.h"
#include "../Layers/EngineLayer.h"
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

    void DrawObjectUI(SceneObject *obj);
    void DrawColorControl(const char *title, glm::vec4 * color);
    void DrawFloat2Control(const char *title, glm::vec2 * vec2, float speed = .1f);
};
