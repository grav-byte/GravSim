#include <vector>
#include "RocketStateUI.h"
#include "imgui.h"

RocketStateUI::RocketStateUI() {
    controlLayer_ = Core::Application::Get().GetLayer<ControlLayer>();
    engineLayer_ = Core::Application::Get().GetLayer<EngineLayer>();
}

void RocketStateUI::OnEvent(Core::Event &event) {
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
    ImGui::SliderFloat("Thrust", &rocketObj->thrustPercent, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Thrust Angle", &rocketObj->thrustAngle, -20.0f, 20.0f, "%.2f °");

    static int selectedIndex = -1; // -1 = World
    const auto& objects = engineLayer_->GetScene()->GetAllObjects();

    glm::vec2 referenceVelocity;
    glm::vec2 referencePos;
    ComputeReferenceFrame(
        selectedIndex,
        objects,
        rocketObj,
        referenceVelocity,
        referencePos
    );

    ImGui::LabelText("Velocity", "%.2f m/s", glm::length(rocketObj->velocity - referenceVelocity));
    ImGui::LabelText("Altitude", "%.2f m", glm::length(rocketObj->transform.position - referencePos));

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
        const std::string& name = objects[i]->name;

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