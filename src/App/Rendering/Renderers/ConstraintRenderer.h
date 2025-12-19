#pragma once
#include "BaseRenderer.h"
#include "App/Engine/Physics/Constraint.h"
#include "App/Rendering/RenderingSystem.h"

class ConstraintRenderer: public BaseRenderer {
public:
    explicit ConstraintRenderer(const RenderingSystem* renderer);
    ~ConstraintRenderer() override = default;


    void RenderConstraint(const Constraint *constraint, const glm::vec4& color) const;
private:

    unsigned int constraintShaderProgram_ = 0;
    unsigned int radialConstShaderProgram_ = 0;
};
