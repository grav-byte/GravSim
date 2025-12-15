#include "Constraint.h"

Constraint::Constraint() : distance(0.0f), direction(UP) {
}

Constraint::Constraint(const float distance, const ConstraintDirection direction)
    : distance(distance), direction(direction) {}

