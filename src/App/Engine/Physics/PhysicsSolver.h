//
// Created by Lorenz Saalmann on 10.12.25.
//

#pragma once
#include "Propagators/IPropagator.h"
#include "App/Engine/Scene.h"


class PhysicsSolver {
public:
    PhysicsSolver();
    void SetActivePropagator(const std::string &name);
    IPropagator* GetActivePropagator() const;

    void SetTimeStep(float timeStep);

    float GetTimeStep() const;

    void StepPropagation(Scene *scene);

    void ResolveContacts(SceneObject &object);

    void UpdatePhysics(Scene *scene, float deltaTime);

    void ApplyCollisionImpulse(SceneObject &obj, const ContactPoint &contact);

    glm::vec2 GetAccelerationForObject(const SceneObject &object) const;

    struct PropagatorEntry {
        std::string name;
        std::function<std::unique_ptr<IPropagator>()> factory;
    };

    static std::vector<const char *> GetPropagatorNames();

    static std::vector<PropagatorEntry> propagators;

private:
    Scene* currentScene_;
    std::unique_ptr<IPropagator> activePropagator_;
    float timeAccumulator_;
    float timeStep_;
};
