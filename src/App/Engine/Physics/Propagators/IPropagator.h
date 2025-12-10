#pragma once
#include "App/Engine/SceneObject.h"

class IPropagator {
public:
    virtual ~IPropagator() = default;

    std::vector<float> RunTest(const float timeStep, const float totalTime) {
        float time = 0;
        auto results = std::vector<float>();
        SceneObject testObj;
        while (time < totalTime) {
            Propagate(testObj, glm::vec2(0.0f, -9.81f), timeStep);
            results.push_back(testObj.transform.position.y);
            time += timeStep;
        }
        return results;
    }

    virtual void Propagate(SceneObject& object, glm::vec2 acceleration, float deltaTime) = 0;
};
