#include "RK4Propagator.h"

void RK4Propagator::Propagate(SceneObject &object, std::function<glm::vec2(const SceneObject &)> accelerationFunc,
        float deltaTime) {
        // k1
        glm::vec2 k1_v = accelerationFunc(object);
        glm::vec2 k1_p = object.velocity;

        auto pos = object.transform.position;
        auto vel = object.velocity;

        // k2
        object.transform.position += k1_p * (deltaTime / 2.0f);
        object.velocity += k1_v * (deltaTime / 2.0f);
        glm::vec2 k2_v = accelerationFunc(object);
        glm::vec2 k2_p = object.velocity;

        // reset
        object.transform.position = pos;
        object.velocity = vel;

        // k3
        object.transform.position += k2_p * (deltaTime / 2.0f);
        object.velocity += k2_v * (deltaTime / 2.0f);
        glm::vec2 k3_v = accelerationFunc(object);
        glm::vec2 k3_p = object.velocity;

        // reset
        object.transform.position = pos;
        object.velocity = vel;

        // k4
        object.transform.position += k3_p * deltaTime;
        object.velocity += k3_v * deltaTime;
        glm::vec2 k4_v = accelerationFunc(object);
        glm::vec2 k4_p = object.velocity;

        // reset
        object.transform.position = pos;
        object.velocity = vel;

        // update position and velocity
        object.transform.position += deltaTime / 6.0f * (k1_p + 2.0f * k2_p + 2.0f * k3_p + k4_p);
        object.velocity += deltaTime / 6.0f * (k1_v + 2.0f * k2_v + 2.0f * k3_v + k4_v);
}
