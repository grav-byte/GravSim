#pragma once

#include <string>

#include "IUserInterface.h"
#include "glm/glm.hpp"
#include "imgui.h"
#include "App/Engine/Camera.h"
#include "App/Engine/Interaction/IInteractor.h"

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
        ApplyForce
    };

private:
    EngineLayer* engine_;

    Mode activeMode_ = Mode::None;
    glm::vec2 mousePosition_ = glm::vec2(0.0f);

    bool interacting_ = false;
    bool usingLeftMouse_ = false;

    ImVec4 btnBgColor_;

    std::pmr::vector<std::unique_ptr<IInteractor>> interactors_;
    int activeInteractorIdx_ = -1;

    bool ImageToggleButton(const std::string &texturePath, bool selected, bool disabled, const char *tooltip) const;

    void HandleInteract(const Camera *cam) const;

    bool DrawPreview(const Camera *&cam);
};