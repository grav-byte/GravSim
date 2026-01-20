#include "Predictor.h"

#include "App/Layers/EngineLayer.h"
#include "Core/Application.h"

std::vector<glm::vec2> Predictor::PredictPath(const PredictionSettings settings, const Scene* scene, PhysicsSolver* solver) {
    // create mock scene
    Scene mockScene = CreateMockScene(scene);

    const SceneObject* trackedMockObj = scene->GetObjById(settings.id);

    const float oldTimeStep = solver->GetTimeStep();

    // set new time step
    solver->SetTimeStep(settings.timeStep);

    // propagate and record positions
    std::vector<glm::vec2> predictedPositions;
    float accumulatedTime = 0.0f;
    while (accumulatedTime < settings.timeHorizon) {
        solver->StepPropagation(&mockScene);
        predictedPositions.push_back(trackedMockObj->transform.position);

        accumulatedTime += settings.timeStep;
    }

    solver->SetTimeStep(oldTimeStep);

    return predictedPositions;
}

Scene Predictor::CreateMockScene(const Scene* realScene) {
    // does not include colliders or visuals
    Scene mockScene;
    for (const SceneObject* realObj: realScene->GetAllObjects()) {
        // create mock objects
        auto mockObj = std::make_unique<SceneObject>(realObj->id, realObj->name);
        mockObj->id = realObj->id;
        mockObj->transform = realObj->transform;
        mockObj->velocity = realObj->velocity;
        mockObj->angularVelocity = realObj->angularVelocity;
        mockObj->mass = realObj->mass;
        mockObj->affectedByGravity = realObj->affectedByGravity;
        mockObj->gravitates = realObj->gravitates;
        mockScene.AddObject(std::move(mockObj));
    }
    return mockScene;
}

