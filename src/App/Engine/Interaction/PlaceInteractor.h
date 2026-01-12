#pragma once
#include "IInteractor.h"
#include "App/Engine/Scene.h"

class PlaceInteractor : public IInteractor {
public:
    PlaceInteractor();

    void OnClick(Scene &scene, glm::vec2 mousePos, bool leftMouse) override;
    void OnRelease(Scene &scene, glm::vec2 mousePos, bool leftMouse) override;
private:
    glm::vec2 lastMousePos = glm::vec2(0.0f);
};
