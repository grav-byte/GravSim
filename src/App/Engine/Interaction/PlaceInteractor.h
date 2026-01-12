#pragma once
#include "IInteractor.h"
#include "App/Engine/Scene.h"

class PlaceInteractor : public IInteractor {
public:
    PlaceInteractor();

    void Interact(Scene &scene, glm::vec2 mousePos, bool leftMouse) override;
};
