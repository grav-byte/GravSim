//
// Created by kw on 20.12.2025.
//

#pragma once

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
    void OnEvent(Core::Event& event) override {} // nicht nötig, aber Interface verlangt es evtl.

private:
    InteractionState* state_;
    EngineLayer* engine_;

    bool clickToPlaceEnabled_ = true;

    glm::vec2 ScreenToWorld(const ImVec2& mouseScreenPx, const ImVec2& viewportSize) const;
};