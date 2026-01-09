#pragma once
#include "App/UI/FileSelector.h"
#include "App/RocketControl/PID/PIDController.h"
#include "App/UI/IUserInterface.h"
#include "App/Layers/EngineLayer.h"
#include <limits>

#include "App/RocketControl/ControlLayer.h"
#include "App/RocketControl/Controllers/AutonomousPIDRocketController.h"


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
    void DrawPIDSettings(AutonomousPIDRocketController *autoCtrl);

    void SaveConfig(const AutonomousPIDRocketController *autoCtrl) const;

    void DrawTargetSettings(Scene *scene, AutonomousPIDRocketController* autoCtrl);

    SceneObject* GetTarget(Scene& scene) const;
};
