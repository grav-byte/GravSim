//
// Created by kw on 20.12.2025.
//

#pragma once

#include <string>

#include "IUserInterface.h"
#include "glm/glm.hpp"
#include "imgui.h"
#include "App/UI/InteractionState.h"


class EngineLayer;
struct InteractionState;

class InteractionUI : public IUserInterface {
public:
    explicit InteractionUI(InteractionState* state);
    void Draw() override;
    void OnEvent(Core::Event& event) override {}

    enum class Mode {
        None = 0,
        ClickToPlace,
        ClickToConstraint
    };

private:
    InteractionState* state_;
    EngineLayer* engine_;

    bool clickToPlaceEnabled_ = true;

    Mode activeMode_ = Mode::None;

    ImVec4 btnBgColor_;
    ImVec4 btnTintColor_;
    ImVec4 btnActiveColor_;
    ImVec4 btnDisabledColor_;

    ImVec2 previewScreenPos_;
    bool isPreviewActive_;

    bool ImageToggleButton(const std::string &texturePath, bool selected, const char* tooltip) const;
};