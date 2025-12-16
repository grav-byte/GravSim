#include "Constraint.h"

Constraint::Constraint() : distance(1.0f), direction(UP) {
}

Constraint::Constraint(const float distance, const ConstraintDirection direction)
    : distance(distance), direction(direction) {}

void Constraint::ApplyConstraint(SceneObject *obj, const float dampingConstant) const {
    glm::vec2 pos = obj->transform.position;
    glm::vec2 vel = obj->velocity;

    switch (direction) {
        case UP:
            if (pos.y > distance - obj->transform.scale.y) {
                pos.y = distance - obj->transform.scale.y;
                if (vel.y > 0) {
                    vel.y *= -dampingConstant;
                }
            }
            break;
        case RIGHT:
            if (pos.x > distance - obj->transform.scale.x) {
                pos.x = distance - obj->transform.scale.x;
                if (vel.x > 0) {
                    vel.x *= -dampingConstant;
                }
            }
            break;
        case LEFT:
            if (pos.x < -distance + obj->transform.scale.x) {
                pos.x = -distance + obj->transform.scale.x;
                if (vel.x < 0) {
                    vel.x *= -dampingConstant;
                }
            }
            break;
        case DOWN:
            if (pos.y < -distance + obj->transform.scale.y) {
                pos.y = -distance + obj->transform.scale.y;
                if (vel.y < 0) {
                    vel.y *= -dampingConstant;
                }
            }
            break;
        case RADIAL: {
            if (pos.x * pos.x + pos.y * pos.y > distance * distance) {
                const glm::vec2 dir = glm::normalize(pos);
                pos = dir * distance;
                // reflect velocity
                vel -= 2.0f * glm::dot(vel, dir) * dir;
            }
            break;
        }
    }

    obj->transform.position = pos;
    obj->SetVelocity(vel);
}
