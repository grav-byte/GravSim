#pragma once
#include "App/Engine/SceneObject.h"


class Constraint {
public:
    enum ConstraintDirection {
        UP, RIGHT, DOWN, LEFT, RADIAL
    };

    Constraint();
    explicit Constraint(float distance, ConstraintDirection direction = UP);

    ConstraintDirection direction;
    float distance;

    void ApplyConstraint(SceneObject *obj) const;

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(distance, direction);
    }
};
