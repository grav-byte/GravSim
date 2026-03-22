#include "PredictionUI.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "App/Engine/EngineEvents.h"
#include "Core/AppLayer.h"

struct ImGuiWindow;

PredictionUI::PredictionUI() : scene_(nullptr), color_(glm::vec4(.5f, 0.7f, 0.2f, .8f)) {}

void PredictionUI::OnEvent(Core::Event &event) {
    if (event.GetEventType() == Core::SceneLoaded) {
        scene_ = dynamic_cast<SceneLoadedEvent &>(event).GetScene();
    }
}

void PredictionUI::Draw() {
    if (!scene_)
        return;

    float width  = 370.0f;
    const float height = 200.0f;

    const ImGuiViewport* vp = ImGui::GetMainViewport();
    if (const ImGuiWindow* settingsWin = ImGui::FindWindowByName("Settings")) {
        width = settingsWin->Size.x;
    }

    // center horizontally relative to the main window
    ImVec2 pos(
        vp->Pos.x + vp->Size.x - width,
        vp->Pos.y + vp->Size.y - (height + 10.0f)
    );

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, -1), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("Prediction", nullptr, flags);
    ImGui::Text("Prediction");

    static int index = -1;
    auto objs = scene_->GetAllObjects();
    DrawCombo(index, objs);
    if (objs.size() > 0 && index >= 0) {
        settings.id = objs[index]->id;
        QueueDrawPrediction(objs[index]);
    }

    ImGui::DragFloat("Time Horizon", &settings.timeHorizon, 0.1f, 0.1f, 60.0f, "%.1f s");
    ImGui::DragFloat("Time Step", &settings.timeStep, 0.01f, 0.002f, .5f, "%.3f s");
    float value[4] = { color_.x, color_.y, color_.z, color_.a };
    if (ImGui::ColorEdit4("Color", value)) {
        // Update
        color_ = glm::vec4(value[0], value[1], value[2], value[3]);
    }
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::End();
}


void PredictionUI::QueueDrawPrediction(SceneObject *obj) const {
    const auto path = Predictor::PredictPath(settings, scene_);
    glm::vec2 prevPoint = obj->transform.position;
    for (auto current : path) {
        auto arrow = std::make_unique<SceneObject::DebugArrow>(prevPoint, current - prevPoint, color_);
        arrow->hasArrow = true;
        arrow->width = 1;
        arrow->arrowHeadLength = .05;
        arrow->dashed = false;
        obj->debugArrows.push_back(std::move(arrow));
        prevPoint = current;
    }
}

void PredictionUI::DrawCombo(int& selectedIndex, const std::vector<SceneObject*>& objects) {
    const char* preview = selectedIndex == -1
        ? "None"
        : objects[selectedIndex]->name.c_str();

    if (!ImGui::BeginCombo("Object", preview))
        return;

    {
        const bool isSelected = (selectedIndex == -1);
        if (ImGui::Selectable("None", isSelected))
            selectedIndex = -1;

        if (isSelected)
            ImGui::SetItemDefaultFocus();
    }

    for (int i = 0; i < objects.size(); ++i) {
        const std::string& name = objects[i]->name + "##" + std::to_string(objects[i]->id);

        const bool isSelected = (selectedIndex == i);
        if (ImGui::Selectable(name.c_str(), isSelected))
            selectedIndex = i;

        if (isSelected)
            ImGui::SetItemDefaultFocus();
    }

    ImGui::EndCombo();
}
