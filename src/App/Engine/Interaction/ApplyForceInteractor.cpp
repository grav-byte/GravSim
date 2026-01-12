#include "ApplyForceInteractor.h"

#include "imgui.h"

void ApplyForceInteractor::Interact(Scene &scene, const glm::vec2 mousePos, const bool leftMouse) {
    const auto cam = scene.GetCamera();
    for (const auto obj:scene.GetAllObjects()) {
        auto mouseWorldPos = cam->ScreenToWorld(mousePos);
        const float dist = glm::distance(obj->transform.position, mouseWorldPos);

        if (dist <= radius) {
            const glm::vec2 dir = glm::normalize(obj->transform.position - cam->ScreenToWorld(mousePos));
            const float forceMagnitude = 500.0f * (1.0f - dist / radius);

            if (leftMouse) {
                obj->ApplyForce(dir * forceMagnitude);
            } else {
                obj->ApplyForce(-dir * forceMagnitude);
            }
        }
    }
}
