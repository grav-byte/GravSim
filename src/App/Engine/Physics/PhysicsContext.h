#pragma once
#include "glm/vec2.hpp"


class PhysicsSolver;
class SceneObject;
class Scene;

struct PhysicsContext {
    const Scene& scene;
    const PhysicsSolver& solver;

    glm::vec2 GetAcceleration(const SceneObject& obj) const;
};
