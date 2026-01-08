#pragma once
#include "FileSelector.h"
#include "IUserInterface.h"
#include "App/RocketControl/ControlLayer.h"
#include "App/Layers/EngineLayer.h"
#include <limits>


class RocketControllerUI : public IUserInterface {
public:
    RocketControllerUI();

    void Draw() override;

    void DrawPIDLoading(PIDController *(&pids)[3]);

    void OnEvent(Core::Event &event) override;

private:
    static constexpr uint32_t InvalidId =
        std::numeric_limits<uint32_t>::max();

    ControlLayer* controlLayer_ = nullptr;
    EngineLayer* engineLayer_ = nullptr;
    PIDController* activePID;

    FileSelector pidFileSelector_;

    bool targetCreated_ = false;
    uint32_t targetId_ = InvalidId;
    glm::vec2 targetPos_ = glm::vec2(0.0f, 0.0f);
    std::string saveName;

    void CreateTarget(Scene& scene);
    void DeleteTarget(Scene& scene);
    void DrawPIDSettings(AutonomousControl *autoCtrl);

    void SaveConfig(const AutonomousControl *autoCtrl) const;

    void DrawTargetSettings(Scene *scene, AutonomousControl* autoCtrl);

    SceneObject* GetTarget(Scene& scene) const;
};