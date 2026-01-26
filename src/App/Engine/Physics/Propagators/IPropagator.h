#pragma once
#include "App/Engine/SceneObject.h"
#include "App/Engine/Scene.h"
#include <vector>
#include "App/Engine/Physics/PhysicsContext.h"


class IPropagator {
public:
    virtual ~IPropagator() = default;

    struct Sample {
        float t;
        float y;
        float v;
    };
    std::vector<Sample> RunTest(const float timeStep, const float totalTime, const PhysicsContext context) {
        float time = 0;
        auto results = std::vector<Sample>();

        SceneObject testObj;
        while (time < totalTime) {
            Propagate(testObj, context, timeStep);
            results.push_back({time, testObj.transform.position.y, testObj.velocity.y});
            time += timeStep;
        }
        return results;
    }
    
    // accepts a PhysicsContext
    virtual void Propagate(SceneObject& object, const PhysicsContext& context, float deltaTime) = 0;

    virtual bool UsePreviousState() const { return false; }
};
