#include "PlaceInteractor.h"

#include "imgui.h"
#include "App/Engine/Scene.h"
#include "Core/AppLayer.h"

PlaceInteractor::PlaceInteractor() {
    continuous = false;
}

void PlaceInteractor::Interact(Scene &scene, const glm::vec2 mousePos, bool leftMouse) {
    const auto cam = scene.GetCamera();
    const glm::vec2 world  = cam->ScreenToWorld(mousePos);
    uint32_t id = scene.CreateObject(world);
    // set scale to radius
    scene.GetObjById(id)->transform.scale = glm::vec2(radius);
}
