#pragma once
#include "miniaudio.h"
#include "App/Engine/Scene.h"
#include "App/Engine/SceneObject.h"
#include "App/Layers/EngineLayer.h"
#include "App/Rendering/Renderers/ShaderRenderer.h"
#include "cereal/types/base_class.hpp"
#include "Core/Application.h"


class RocketObject : public SceneObject {
public:
    RocketObject();
    explicit RocketObject(Scene& scene);

    void StartSound();

    void StopSound();

    ~RocketObject() override;

    float thrustPercent = 0.0f; // thrust level from 0 to 1
    float thrustAngle = 0.0f; // angle of thrust vector in degrees from -20 to 20

    void UpdateVisualisation() const;
    glm::vec2 GetThrustPosition() const;
    glm::vec2 GetThrustVector() const;

    void RelinkObjects(const Scene& scene);

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<SceneObject>(this), thrustPercent, thrustAngle, nozzleId_, exhaustId_);
    }

private:
    const float maxTrustForce_ = 250.0f; // max thrust force in N
    const float nozzleOffset = -.8f;
    const float exhaustOffset = -.6f;

    SceneObject* nozzleObj_;
    SceneObject* exhaustObj_;
    ShaderUniforms* exhaustShaderData_;

    uint32_t exhaustId_;
    uint32_t nozzleId_;

    std::unique_ptr<ma_sound> rocketSound_;
};
