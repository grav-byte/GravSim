#pragma once
#include "App/Engine/SceneObject.h"
#include <functional>
#include <vector>

class IPropagator {
public:
    virtual ~IPropagator() = default;

    struct Sample {
        float t;
        float y;
        float v;
    };
    std::vector<Sample> RunTest(const float timeStep, const float totalTime) {
        float time = 0;
        auto results = std::vector<Sample>();
        SceneObject testObj;
        while (time < totalTime) {
            // changed: pass a callable that returns the acceleration (constant gravity here)
            Propagate(testObj, [](const SceneObject&){ return glm::vec2(0.0f, -9.81f); }, timeStep);
            results.push_back({time, testObj.transform.position.y, testObj.velocity.y});
            time += timeStep;
        }
        return results;
    }

    // accepts a callable that returns acceleration given the object
    virtual void Propagate(SceneObject& object, std::function<glm::vec2(const SceneObject&)> accelerationFunc, float deltaTime) = 0;
};
