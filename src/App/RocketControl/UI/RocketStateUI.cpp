#include <vector>
#include "RocketStateUI.h"
#include "imgui.h"
#include "implot.h"

RocketStateUI::RocketStateUI() : availableWidth(0), availableHeight(0) {
    controlLayer_ = Core::Application::Get().GetLayer<ControlLayer>();
    engineLayer_ = Core::Application::Get().GetLayer<EngineLayer>();
}

void RocketStateUI::OnEvent(Core::Event &event) {
    if (event.GetEventType() == Core::SceneLoaded) {
        // clear history on scene load
        thrustHistory_.clear();
        angleHistory_.clear();
        velocityHistory_.clear();
        altitudeHistory_.clear();
    }
}

void RocketStateUI::RecordHistory(RocketObject * const rocketObj, const float rocketVelocityRel, const float rocketPosRel) {
    // record history
    thrustHistory_.push_back(rocketObj->thrustPercent);
    angleHistory_.push_back(rocketObj->thrustAngle);
    velocityHistory_.push_back(rocketVelocityRel);
    altitudeHistory_.push_back(rocketPosRel);

    if (thrustHistory_.size() > maxHistorySize_) {
        // remove oldest data points
        thrustHistory_.erase(thrustHistory_.begin());
        angleHistory_.erase(angleHistory_.begin());
        velocityHistory_.erase(velocityHistory_.begin());
        altitudeHistory_.erase(altitudeHistory_.begin());
    }
}

void RocketStateUI::Draw() {
    if (!engineLayer_->IsRunningSimulation())
        return;
    const auto rocketObj = controlLayer_->GetRocketObject();
    if (!rocketObj)
        return;

    ImGui::SetNextWindowPos(ImVec2(1000, 750), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(419, 163), ImGuiCond_FirstUseEver);

    ImGui::Begin("Rocket State",nullptr, ImGuiWindowFlags_NoDocking);
    static int selectedIndex = -1; // -1 = World
    const auto& objects = engineLayer_->GetScene()->GetAllObjects();

    glm::vec2 referenceVelocity;
    glm::vec2 referencePos;
    ComputeReferenceFrame(selectedIndex, objects, rocketObj, referenceVelocity,referencePos);

    const auto rocketVelocityRel = glm::length(rocketObj->velocity - referenceVelocity);
    const auto rocketPosRel = glm::length(rocketObj->transform.position - referencePos);
    if (ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_DrawSelectedOverline)) {
        if (ImGui::BeginTabItem("Current"))
        {
            ShowCurrentState(rocketObj, rocketVelocityRel, rocketPosRel);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Graph"))
        {
            ShowHistoryGraph();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    RecordHistory(rocketObj, rocketVelocityRel, rocketPosRel);

    DrawReferenceCombo(selectedIndex, objects);

    ImGui::End();
}

void RocketStateUI::ComputeReferenceFrame(const int selectedIndex, const std::vector<SceneObject*>& objects,
    const SceneObject* rocketObj,
    glm::vec2& outVelocity,
    glm::vec2& outPosition
) {
    if (selectedIndex == -1) {
        outVelocity = glm::vec2(0.0f);
        outPosition = glm::vec2(rocketObj->transform.position.x, 0.0f);
        return;
    }

    outVelocity = objects[selectedIndex]->velocity;
    outPosition = objects[selectedIndex]->transform.position;
}

void RocketStateUI::DrawReferenceCombo(int& selectedIndex,const std::vector<SceneObject*>& objects) {
    const char* preview = (selectedIndex == -1)
        ? "World"
        : objects[selectedIndex]->name.c_str();

    if (!ImGui::BeginCombo("Reference", preview))
        return;

    {
        const bool isSelected = (selectedIndex == -1);
        if (ImGui::Selectable("World", isSelected))
            selectedIndex = -1;

        if (isSelected)
            ImGui::SetItemDefaultFocus();
    }

    for (int i = 0; i < objects.size(); ++i) {
        const std::string& name = objects[i]->name + "##" + std::to_string(objects[i]->id);

        if (name.rfind("Rocket", 0) == 0)
            continue;

        const bool isSelected = (selectedIndex == i);
        if (ImGui::Selectable(name.c_str(), isSelected))
            selectedIndex = i;

        if (isSelected)
            ImGui::SetItemDefaultFocus();
    }

    ImGui::EndCombo();
}

void RocketStateUI::ShowCurrentState(RocketObject* const rocketObj, const float rocketVelocityRel, const float rocketPosRel) {
    ImGui::SliderFloat("Thrust", &rocketObj->thrustPercent, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Thrust Angle", &rocketObj->thrustAngle, -20.0f, 20.0f, "%.2f °");
    ImGui::LabelText("Velocity", "%.2f m/s", rocketVelocityRel);
    ImGui::LabelText("Altitude", "%.2f m", rocketPosRel);
}

void RocketStateUI::PlotHistory(const std::vector<float> &data, const glm::vec2 yLimits, const char* title) const {
    ImPlot::SetNextAxisLimits(ImAxis_Y1, yLimits.x, yLimits.y, ImPlotCond_Always);
    if (ImPlot::BeginPlot(title, ImVec2(availableWidth * .5f, availableHeight * .5f), flags))
    {
        ImPlot::SetupAxis(ImAxis_X1, nullptr, axisFlags);
        ImPlot::PlotLine(title, data.data(), static_cast<int>(data.size()));
        ImPlot::EndPlot();
    }
}

void RocketStateUI::ShowHistoryGraph() {
    ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));
    ImPlot::PushStyleVar(ImPlotStyleVar_PlotBorderSize, 0.0f);
    ImPlot::PushStyleVar(ImPlotStyleVar_PlotMinSize, ImVec2(0, 0));

    availableWidth = ImGui::GetWindowWidth() - spacing - ImGui::GetStyle().WindowPadding.x * 2.0f;
    availableHeight = ImGui::GetWindowHeight() - spacing - ImGui::GetStyle().WindowPadding.y * 2.0f - 75.0f;

    // Plot Thrust (red)
    ImPlot::PushStyleColor(ImPlotCol_Line, IM_COL32(255, 0, 0, 255));
    PlotHistory(thrustHistory_, {-.1, 1.1}, "Thrust");
    ImPlot::PopStyleColor();

    ImGui::SameLine(availableWidth * .5f + spacing);

    // Plot Angle (blue)
    PlotHistory(angleHistory_, {-21, 21}, "Angle in °");

    // find min and max for velocity and altitude to set y axis limits
    const auto lastIdx = velocityHistory_.size()-1;
    float velMin = velocityHistory_[lastIdx] - 1.0f;
    float velMax = velocityHistory_[lastIdx] + 1.0f;
    float altMin = altitudeHistory_[lastIdx] - 1.0f;
    float altMax = altitudeHistory_[lastIdx] + 1.0f;
    for (const auto& v : velocityHistory_) {
        if (v < velMin) velMin = v;
        if (v > velMax) velMax = v;
    }
    for (const auto& a : altitudeHistory_) {
        if (a < altMin) altMin = a;
        if (a > altMax) altMax = a;
    }

    // Plot Velocity (green)
    ImPlot::PushStyleColor(ImPlotCol_Line, IM_COL32(0, 255, 0, 255));
    PlotHistory(velocityHistory_, {velMin, velMax}, "Velocity in m/s");
    ImPlot::PopStyleColor();

    ImGui::SameLine(availableWidth * .5f + spacing);

    // Plot Altitude (orange)
    ImPlot::PushStyleColor(ImPlotCol_Line, IM_COL32(255, 165, 0, 255));
    PlotHistory(altitudeHistory_, {altMin, altMax}, "Altitude in m");
    ImPlot::PopStyleColor();

    ImPlot::PopStyleVar(3);
}
