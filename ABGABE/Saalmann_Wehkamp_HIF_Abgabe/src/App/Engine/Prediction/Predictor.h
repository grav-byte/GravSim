#pragma once
#include <vector>

#include "App/Engine/SceneObject.h"
#include "App/Engine/Physics/PhysicsSolver.h"
#include "glm/vec2.hpp"


class Predictor {
public:
    struct PredictionSettings {
        uint32_t id = 0;   // SceneObject id to predict
        float timeHorizon = 4.0f; // seconds
        float timeStep = 0.01f;    // seconds
    };
    static std::vector<glm::vec2> PredictPath(PredictionSettings settings, const Scene *scene);


private:
    static Scene CreateMockScene(const Scene *realScene);
};
