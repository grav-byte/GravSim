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

    void OnEvent(Core::Event &event) override;
    void Draw() override;

    void FileMenu();

    static const char *OpenFileDialog();

private:
    EngineLayer* engine_;
    Scene* scene_;
    std::unique_ptr<FileSelector> sceneSelector_;

    void DrawObjectUI(SceneObject *obj);
    void DrawColorControl(const char *title, glm::vec4 * color);
    void DrawFloat2Control(const char *title, glm::vec2 * vec2, float speed = .1f);
};
