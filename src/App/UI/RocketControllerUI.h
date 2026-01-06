#pragma once
#include "FileSelector.h"
#include "IUserInterface.h"
#include "App/RocketControl/ControlLayer.h"
#include "App/Layers/EngineLayer.h"


class RocketControllerUI : public IUserInterface {
public:
    RocketControllerUI();

    void Draw() override;

    void DrawPIDLoading();

    void OnEvent(Core::Event &event) override;

private:
    ControlLayer* controlLayer_ = nullptr;
    EngineLayer* engineLayer_ = nullptr;
    PIDController* activePID;

    FileSelector pidFileSelector_;

    uint32_t targetId_ = 0;
    bool targetCreated_ = false;
    glm::vec2 targetPos_ = glm::vec2(0.0f, 0.0f);

    void CreateTarget(Scene& scene);
    void DeleteTarget(Scene& scene);
    void DrawPIDSettings(AutonomousControl *autoCtrl);

    void DrawTargetSettings(Scene *scene);

    SceneObject* GetTarget(Scene& scene) const;
};