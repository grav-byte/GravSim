#pragma once
#include "implot.h"
#include "IUserInterface.h"
#include "App/RocketControl/ControlLayer.h"


class RocketStateUI : public IUserInterface {
public:
    RocketStateUI();

    void OnEvent(Core::Event &event) override;

    void RecordHistory(RocketObject *rocketObj, float rocketVelocityRel, float rocketPosRel);

    void Draw() override;
private:
    static void DrawReferenceCombo(int& selectedIndex,const std::vector<SceneObject*>& objects);

    static void ComputeReferenceFrame(int selectedIndex, const std::vector<SceneObject*>& objects,
                                      const SceneObject* rocketObj,
                                      glm::vec2& outVelocity,
                                      glm::vec2& outPosition
    );

    static void ShowCurrentState(RocketObject *rocketObj, float rocketVelocityRel, float rocketPosRel);

    void PlotHistory(const std::vector<float> &data, glm::vec2 yLimits, const char* title) const;

    void ShowHistoryGraph();

    const int maxHistorySize_ = 100;
    std::vector<float> thrustHistory_;
    std::vector<float> angleHistory_;
    std::vector<float> velocityHistory_;
    std::vector<float> altitudeHistory_;

    const ImPlotFlags flags = ImPlotFlags_NoTitle | ImPlotFlags_NoMouseText;
    const ImPlotAxisFlags axisFlags = ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels;
    const float spacing = 10.0f;
    float availableWidth;
    float availableHeight;

    ControlLayer* controlLayer_;
    EngineLayer * engineLayer_;
};
