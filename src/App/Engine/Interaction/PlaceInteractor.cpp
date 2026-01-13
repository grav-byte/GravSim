#include "PlaceInteractor.h"

#include "App/Engine/Scene.h"
#include "App/Layers/EngineLayer.h"
#include "Core/Application.h"

PlaceInteractor::PlaceInteractor() {}

void PlaceInteractor::OnClick(Scene &scene, const glm::vec2 mousePos, bool leftMouse) {
    const auto cam = scene.GetCamera();
    lastMousePos = cam->ScreenToWorld(mousePos);
}

void PlaceInteractor::OnRelease(Scene &scene, const glm::vec2 mousePos, bool leftMouse) {
    const auto cam = scene.GetCamera();
    const glm::vec2 world  = cam->ScreenToWorld(mousePos);
    const uint32_t id = scene.CreateObject(world);
    // set scale to radius
    SceneObject* obj = scene.GetObjById(id);
    obj->transform.scale = glm::vec2(radius);
    obj->mass = radius * 2.0f; // mass proportional to size

    // set velocity based on mouse movement if simulating
    if (Core::Application::Get().GetLayer<EngineLayer>()->IsRunningSimulation()) {
        float dt = 1.0f / static_cast<float>(Core::Application::Get().GetFramerate());
        obj->velocity = (world - lastMousePos) / dt;
        obj->lastPosition = lastMousePos;
    }
}
