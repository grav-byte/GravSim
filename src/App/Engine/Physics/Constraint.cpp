#include "Constraint.h"

#include <iostream>

#include "Colliders/CircleCollider.h"

Constraint::Constraint() : distance(1.0f), direction(UP) {
}

Constraint::Constraint(const float distance, const ConstraintDirection direction)
    : distance(distance), direction(direction) {}

void Constraint::ApplyConstraint(SceneObject *obj) const {
    for (auto &collider : obj->colliders) {
        if (collider->GetType() != ColliderType::Circle) {
            // only circle colliders supported for now
            std::cout << "Only circle colliders are supported for constraints." << std::endl;
            continue;
        }

        glm::vec2 collPos = collider->GetWorldPosition();
        glm::vec2 colliderOffset = collPos - obj->transform.position;
        float radius = collider->size.x;
        glm::vec2 vel = obj->velocity;

        switch (direction) {
            case UP:
                if (collPos.y > distance - radius) {
                    obj->transform.position.y = distance - radius + colliderOffset.y;
                    if (vel.y > 0) {
                        vel.y *= -collider->elasticity;
                    }
                }
                break;
            case RIGHT:
                if (collPos.x > distance - radius) {
                    obj->transform.position.x = distance - radius + colliderOffset.x;
                    if (vel.x > 0) {
                        vel.x *= -collider->elasticity;
                    }
                }
                break;
            case LEFT:
                if (collPos.x < -distance + radius) {
                    obj->transform.position.x = -distance + radius + colliderOffset.x;
                    if (vel.x < 0) {
                        vel.x *= -collider->elasticity;
                    }
                }
                break;
            case DOWN:
                if (collPos.y < -distance + radius) {
                    obj->transform.position.y = -distance + radius + colliderOffset.y;
                    if (vel.y < 0) {
                        vel.y *= -collider->elasticity;
                    }
                }
                break;
            case RADIAL: {
                const float R = distance - radius;
                const float distSqrd = glm::dot(collPos, collPos);

                if (distSqrd > R * R) {
                    const glm::vec2 dir = glm::normalize(collPos);
                    collPos = dir * R;
                    // reflect velocity
                    const float vn = glm::dot(vel, dir);
                    if (vn > 0.0f) {
                        vel -= 2.0f * vn * dir;
                        vel *= 1;
                    }
                }
                break;
            }
        }
        // TODO - apply force instead
        obj->velocity = vel;
    }
}
