#pragma once
#include "RocketObject.h"
#include <memory>
#include "App/Layers/EngineLayer.h"
#include "Controllers/AutonomousPIDRocketController.h"
#include "Core/AppLayer.h"

class ControlLayer : public Core::AppLayer {
public:
    ControlLayer();

    void OnInit() override;

    void OnUpdate(float deltaTime) override;

    void FindRocket(Core::Event &event);

    void OnEvent(Core::Event &event) override;

    RocketObject* GetRocketObject() const;

    template<typename T>
    void SetActiveControl() {
        activeControl_ = std::make_unique<T>(targetManager_.get());
    }

    IRocketController* GetActiveControl() const;

    void OnRender() override;


private:
    void CreateRocket(Scene *scene);

    std::unique_ptr<IRocketController> activeControl_;
    std::unique_ptr<TargetManager> targetManager_;

    RocketObject* rocketObj_;
    EngineLayer * engine_;
};
