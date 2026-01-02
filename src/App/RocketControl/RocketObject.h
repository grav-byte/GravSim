#pragma once
#include "miniaudio.h"
#include "App/Engine/Scene.h"
#include "App/Engine/SceneObject.h"
#include "App/Rendering/Renderers/ShaderRenderer.h"


class RocketObject : public SceneObject {
public:
    explicit RocketObject(Scene& scene);

    float thrustPercent = 0.0f; // thrust level from 0 to 1
    float thrustAngle = 0.0f; // angle of thrust vector in degrees from -20 to 20

    void UpdateVisualisation() const;

private:
    const float maxTrustForce_ = 250.0f; // max thrust force in N

    SceneObject* nozzleObj_;
    SceneObject* exhaustObj_;
    ShaderUniforms* exhaustShaderData_;
    float nozzleOffset = -.8f;
    float exhaustOffset = -.6f;

    ma_sound* rocketSound_;

    glm::vec2 GetThrustVector() const {
        const float angleRad = glm::radians(thrustAngle);
        const glm::vec2 dir = { cos(angleRad), sin(angleRad) };
        const float magnitude = thrustPercent * maxTrustForce_;
        return dir * magnitude;
    }
};
