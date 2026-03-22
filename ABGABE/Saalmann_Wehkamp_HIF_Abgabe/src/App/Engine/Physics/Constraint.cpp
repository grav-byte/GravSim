#include "Constraint.h"
#include <iostream>

#include "Colliders/CircleCollider.h"

Constraint::Constraint() : distance(1.0f), direction(UP) {
}

Constraint::Constraint(const float distance, const ConstraintDirection direction)
    : direction(direction), distance(distance) {}


void Constraint::ApplyConstraint(SceneObject *obj) const {
    for (const auto &collider : obj->colliders) {
        if (collider->GetType() != ColliderType::Circle) {
            std::cout << "Only circle colliders are supported for constraints." << std::endl;
            continue;
        }

        glm::vec2 collPos = collider->GetWorldPosition();
        const float radius = collider->GetWorldSize().x;
        auto normal = glm::vec2(0, 0);
        auto wallPos = glm::vec2(0, 0);
        float penetration = 0.0f;

        switch (direction) {
            case UP: {
                penetration = collPos.y + radius - distance;
                normal = glm::vec2(0, -1);
                wallPos = glm::vec2(collPos.x, distance);
                break;
            }
            case DOWN: {
                penetration = -distance - (collPos.y - radius);
                wallPos = glm::vec2(collPos.x, -distance);
                normal = glm::vec2(0, 1);
                break;
            }
            case RIGHT: {
                penetration = collPos.x + radius - distance;
                wallPos = glm::vec2(distance, collPos.y);
                normal = glm::vec2(-1, 0);
                break;
            }
            case LEFT: {
                penetration = -distance - (collPos.x - radius);
                wallPos = glm::vec2(-distance, collPos.y);
                normal = glm::vec2(1, 0);
                break;
            }
            case RADIAL: {
                const float dist = glm::length(collPos);
                normal = -collPos / dist;
                wallPos = -normal * distance;
                penetration = dist - distance + radius;
                break;
            }
        }

        if (penetration < 0.0f) {
            continue; // no collision
        }

        auto contact = ContactPoint(wallPos, normal, penetration, collider.get());
        obj->AddContactPoint(contact);
    }
}
