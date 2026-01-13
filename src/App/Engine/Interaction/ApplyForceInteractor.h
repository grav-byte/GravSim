#pragma once
#include "IInteractor.h"
#include "App/Engine/Scene.h"

class ApplyForceInteractor : public IInteractor {
public:
    void OnClick(Scene &scene, glm::vec2 mousePos, bool leftMouse) override;
    void OnRelease(Scene &scene, glm::vec2 mousePos, bool leftMouse) override;

private:
    std::string effectShader_ = "ripple.frag";
};
