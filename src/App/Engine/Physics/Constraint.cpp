#include "Constraint.h"

#include <iostream>

#include "Colliders/CircleCollider.h"

Constraint::Constraint() : distance(1.0f), direction(UP) {
}

Constraint::Constraint(const float distance, const ConstraintDirection direction)
    : direction(direction), distance(distance) {}


void Constraint::ApplyConstraint(SceneObject *obj, const float deltaTime) const {
    for (const auto &collider : obj->colliders) {
        if (collider->GetType() != ColliderType::Circle) {
            std::cout << "Only circle colliders are supported for constraints." << std::endl;
            continue;
        }

        glm::vec2 collPos = collider->GetWorldPosition();
        glm::vec2 colliderOffset = collPos - obj->transform.position;
        const float radius = collider->size.x;

        switch (direction) {
            case UP: {
                float penetration = (collPos.y + radius) - distance;
                if (penetration > 0.0f) {
                    glm::vec2 wallPos = collPos - glm::vec2(0.0f, penetration);
                    obj->ApplyCollisionImpulse(colliderOffset, wallPos, glm::vec2(0, -1), collider->elasticity);
                }
                break;
            }
            case DOWN: {
                float penetration = -distance - (collPos.y - radius);
                if (penetration > 0.0f) {
                    glm::vec2 wallPos = collPos + glm::vec2(0.0f, penetration);
                    obj->ApplyCollisionImpulse(colliderOffset, wallPos, glm::vec2(0, 1), collider->elasticity);
                }
                break;
            }
            case RIGHT: {
                float penetration = (collPos.x + radius) - distance;
                if (penetration > 0.0f) {
                    glm::vec2 wallPos = collPos - glm::vec2(penetration, 0.0f);
                    obj->ApplyCollisionImpulse(colliderOffset, wallPos, glm::vec2(-1, 0), collider->elasticity);
                }
                break;
            }
            case LEFT: {
                float penetration = -distance - (collPos.x - radius);
                if (penetration > 0.0f) {
                    glm::vec2 wallPos = collPos + glm::vec2(penetration, 0.0f);
                    obj->ApplyCollisionImpulse(colliderOffset, wallPos, glm::vec2(1, 0), collider->elasticity);
                }
                break;
            }
            case RADIAL: {
                float R = distance - radius;
                float dist = glm::length(collPos);
                if (dist > R) {
                    glm::vec2 dir = collPos / dist;
                    glm::vec2 wallPos = dir * R;
                    obj->ApplyCollisionImpulse(colliderOffset, wallPos, -dir, collider->elasticity);
                }
                break;
            }
        }
    }
}
