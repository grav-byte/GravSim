#pragma once
#include "ContactSolver.h"
#include "Propagators/IPropagator.h"
#include "App/Engine/Scene.h"


class PhysicsSolver {
public:
    PhysicsSolver();
    void SetActivePropagator(const std::string &name);
    IPropagator* GetActivePropagator() const;

    void SetTimeStep(float timeStep);
    float GetTimeStep() const;

    void UpdatePhysics(const Scene *scene, float deltaTime) const;

    void DrawContactPoints();

    void StepPropagation(const Scene *scene) const;
    glm::vec2 GetAccelerationForObject(const Scene& scene, const SceneObject &object) const;

    struct PropagatorEntry {
        std::string name;
        std::function<std::unique_ptr<IPropagator>()> factory;
    };
    static std::vector<const char *> GetPropagatorNames();
    static std::vector<PropagatorEntry> propagators;

private:
    std::unique_ptr<IPropagator> activePropagator_;
    float timeStep_;

};
