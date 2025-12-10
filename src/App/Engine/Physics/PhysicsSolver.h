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

    void StepPropagation(const Scene *scene) const;

    void UpdatePhysics(Scene *scene, float deltaTime);

    struct PropagatorEntry {
        std::string name;
        std::function<std::unique_ptr<IPropagator>()> factory;
    };

    static std::vector<const char *> GetPropagatorNames();

    static std::vector<PropagatorEntry> propagators;

private:
    glm::vec2 globalGravity_;
    std::unique_ptr<IPropagator> activePropagator_;
    float timeAccumulator_;
    float timeStep_;
};
