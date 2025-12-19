//
// Created by Lorenz Saalmann on 05.12.25.
//

#pragma once

#include "FileSelector.h"
#include "IUserInterface.h"
#include "../Layers/EngineLayer.h"
#include "App/Engine/Scene.h"

class SceneUI : public IUserInterface {
public:
    SceneUI();
    ~SceneUI() override;

    void ShowStatusMessage(const std::string &msg, float duration);

    void OnEvent(Core::Event &event) override;

    void Draw() override;


private:
    EngineLayer* engine_;
    Scene* scene_;
    std::unique_ptr<FileSelector> sceneSelector_;
    std::unique_ptr<FileSelector> spriteSelector_;
    std::string statusMessage_;
    float statusTimer_;

    void DrawSceneLoading();
    void DrawScene();

    void DrawObjectUI(SceneObject *obj);
    void DrawTransform(SceneObject *obj);
    void DrawPhysics(SceneObject * obj);

    void DrawCollidersUI(SceneObject *obj);

    void DrawRendering(SceneObject *obj);

    void DrawConstraints(Scene *scene);

    void DrawColorControl(const char *title, glm::vec4 * color);
    bool DrawFloat2Control(const char *title, glm::vec2 * vec2, float speed = .1f);
};
