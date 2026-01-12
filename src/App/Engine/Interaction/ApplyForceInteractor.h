#pragma once
#include "IInteractor.h"
#include "App/Engine/Scene.h"

class ApplyForceInteractor : public IInteractor {
public:
    void Interact(Scene &scene, glm::vec2 mousePos, bool leftMouse) override;
};