#pragma once
#include "IUserInterface.h"
#include "App/RocketControl/ControlLayer.h"


class RocketStateUI : public IUserInterface {
public:
    RocketStateUI();

    void OnEvent(Core::Event &event) override;

    void Draw() override;
private:
    static void DrawReferenceCombo(int& selectedIndex,const std::vector<SceneObject*>& objects);

    static void ComputeReferenceFrame(int selectedIndex, const std::vector<SceneObject*>& objects,
                                      const SceneObject* rocketObj,
                                      glm::vec2& outVelocity,
                                      glm::vec2& outPosition
    );
    ControlLayer* controlLayer_;
    EngineLayer * engineLayer_;
};
