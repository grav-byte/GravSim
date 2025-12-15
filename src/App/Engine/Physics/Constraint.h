#pragma once


class Constraint {
public:
    enum ConstraintType {
        AXIS_ALIGNED,
        RADIAL
    };

    enum ConstraintDirection {
        UP, RIGHT, DOWN, LEFT
    };

    explicit Constraint(ConstraintType type, float distance, ConstraintDirection direction = UP);

    ConstraintType type;
    ConstraintDirection direction;
    float distance;
};