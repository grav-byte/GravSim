#pragma once
#include "Physics/Colliders/ColliderBase.h"
#include "glm/vec2.hpp"
#include <memory>
#include <vector>
#include <string>

#include "Transform.h"
#include "App/Rendering/Visuals/IVisual.h"
#include "Physics/ContactPoint.h"

class SceneObject {
public:
    SceneObject();

    SceneObject(uint32_t objectId, const std::string &objectName);
    virtual ~SceneObject() = default;

    // --- id, name ---
    uint32_t id;
    std::string name;

    Transform transform;

    // --- physics ---
    float mass;

    glm::vec2 velocity;
    float angularVelocity;

    bool gravitates;
    bool affectedByGravity;

    glm::vec2 accelerationAccumulated;
    float angularAccelerationAccumulated;

    // --- visual ---
    std::unique_ptr<IVisual> visual;
    bool renderTrail = false;
    // --- colliders ---
    std::vector<std::unique_ptr<ColliderBase>> colliders;
    std::vector<ContactPoint> contactPoints;

    // debug arrows
    struct DebugArrow {
        glm::vec2 origin;
        glm::vec2 direction;
        glm::vec4 color;

        float arrowHeadLength = .1f;
        int width = 2;

        bool hasArrow = true;
        bool dashed = false;
        DebugArrow(const glm::vec2 &orig, const glm::vec2 &dir, const glm::vec4 &col)
        : origin(orig), direction(dir), color(col) {}
    };
    std::vector<std::unique_ptr<DebugArrow>> debugArrows;

    // deprecated - kept for loading old scenes with cereal
    glm::vec2 lastPosition;
    float lastRotation;

    // camera focus flag
    bool canFocusCamera = true;

    void AddContactPoint(const ContactPoint &point);
    void AddCollider(ColliderType type);
    void RemoveCollider(int idx);

    virtual float GetInertia() const;
    void ApplyForce(const glm::vec2 & force, const glm::vec2 & atPoint = glm::vec2(0, 0));
    void ResetAccumulatedForces();

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(id, name, transform, lastPosition, mass, velocity, colliders, lastRotation, angularVelocity, visual, gravitates, affectedByGravity, renderTrail);
        if constexpr (Archive::is_loading::value) {
            // re-link parents after loading
            for (const auto& collider : colliders) {
                collider->parentObject = this;
            }
        }
    }

};


