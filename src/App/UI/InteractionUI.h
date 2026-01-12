#pragma once

#include <string>

#include "IUserInterface.h"
#include "glm/glm.hpp"
#include "imgui.h"

class EngineLayer;
struct InteractionState;

class InteractionUI : public IUserInterface {
public:
    explicit InteractionUI();
    void Draw() override;
    void OnEvent(Core::Event& event) override;

    enum class Mode {
        None = 0,
        ClickToPlace,
        ClickToConstraint
    };

private:
    EngineLayer* engine_;

    Mode activeMode_ = Mode::None;
    glm::vec2 mouse_position_ = glm::vec2(0.0f);

    ImVec4 btnBgColor_;
    ImVec4 btnTintColor_;
    ImVec4 btnActiveColor_;
    ImVec4 btnDisabledColor_;

    ImVec2 previewScreenPos_;
    bool isPreviewActive_;

    bool ImageToggleButton(const std::string &texturePath, bool selected, bool disabled, const char *tooltip) const;
};