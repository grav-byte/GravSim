#include "ApplyForceInteractor.h"

#include "App/Layers/EngineLayer.h"
#include "Core/Application.h"

void ApplyForceInteractor::OnClick(Scene &scene, const glm::vec2 mousePos, const bool leftMouse) {
    const auto cam = scene.GetCamera();
    for (const auto obj:scene.GetAllObjects()) {
        auto mouseWorldPos = cam->ScreenToWorld(mousePos);
        const float dist = glm::distance(obj->transform.position, mouseWorldPos);

        if (dist <= radius) {
            const glm::vec2 dir = glm::normalize(obj->transform.position - cam->ScreenToWorld(mousePos));
            const float forceMagnitude = strength * 100 * (1.0f - dist / radius);

            if (leftMouse) {
                obj->ApplyForce(dir * forceMagnitude);
            } else {
                obj->ApplyForce(-dir * forceMagnitude);
            }
        }
    }

    ShaderUniforms uniforms;
    glm::vec2 center = cam->ScreenToUV(mousePos);

    uniforms.floats["uCenterX"] = center.x;
    uniforms.floats["uCenterY"] = center.y;
    uniforms.floats["uRadius"] = radius;
    uniforms.floats["uDirection"] = leftMouse ? 1.0f : -1.0f;
    Core::Application::Get().GetLayer<EngineLayer>()->GetSceneRenderer()->AddTemporaryPostProcessPass(effectShader_, uniforms);
}

void ApplyForceInteractor::OnRelease(Scene &scene, glm::vec2 mousePos, bool leftMouse) {}
