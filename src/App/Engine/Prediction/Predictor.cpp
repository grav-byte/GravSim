#include "Predictor.h"

#include "App/Layers/EngineLayer.h"
#include "Core/AppLayer.h"
#include "Core/AppLayer.h"
#include "Core/Application.h"

std::vector<glm::vec2> Predictor::PredictPath(const PredictionSettings settings, const Scene* scene) {
    // create mock scene
    const Scene mockScene = CreateMockScene(scene);

    const SceneObject* trackedMockObj = mockScene.GetObjById(settings.id);

    auto solver = PhysicsSolver();
    // set new time step
    solver.SetTimeStep(settings.timeStep);

    // propagate and record positions
    std::vector<glm::vec2> predictedPositions;
    float accumulatedTime = 0.0f;
    while (accumulatedTime < settings.timeHorizon) {
        solver.StepPropagation(&mockScene);
        predictedPositions.push_back(trackedMockObj->transform.position);

        accumulatedTime += settings.timeStep;
    }

    return predictedPositions;
}

Scene Predictor::CreateMockScene(const Scene* realScene) {
    // does not include visuals
    Scene mockScene;
    for (const SceneObject* realObj: realScene->GetAllObjects()) {
        // create mock objects
        auto mockObj = std::make_unique<SceneObject>(realObj->id, realObj->name);
        mockObj->id = realObj->id;
        mockObj->transform.position = realObj->transform.position;
        mockObj->transform.rotation = realObj->transform.rotation;
        mockObj->transform.scale = realObj->transform.scale;
        mockObj->velocity = realObj->velocity;
        mockObj->angularVelocity = realObj->angularVelocity;
        mockObj->mass = realObj->mass;
        mockObj->affectedByGravity = realObj->affectedByGravity;
        mockObj->gravitates = realObj->gravitates;

        mockObj->colliders.clear();

        // copy colliders
        for (const auto& collider : realObj->colliders) {
            mockObj->AddCollider(collider->GetType());
            const auto newColl = mockObj->colliders[mockObj->colliders.size()-1].get();
            newColl->friction = collider->friction;
            newColl->localSize = collider->localSize;
            newColl->localPosition = collider->localPosition;
            newColl->elasticity = collider->elasticity;
        }
        mockScene.AddObject(std::move(mockObj));
    }

    mockScene.globalGravity = realScene->globalGravity;
    for (const auto& constraint : realScene->GetConstraints()) {
        mockScene.AddConstraint(std::make_unique<Constraint>(constraint->distance, constraint->direction));
    }
    return mockScene;
}

