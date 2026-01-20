#include "PhysicsContext.h"
#include "PhysicsSolver.h"

glm::vec2 PhysicsContext::GetAcceleration(const SceneObject& obj) const {
     return solver.GetAccelerationForObject(scene, obj);
}