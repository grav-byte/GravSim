#pragma once


class Constraint {
public:
    enum ConstraintDirection {
        UP, RIGHT, DOWN, LEFT, RADIAL
    };

    Constraint();
    explicit Constraint(float distance, ConstraintDirection direction = UP);

    ConstraintDirection direction;
    float distance;

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(distance, direction);
    }
};