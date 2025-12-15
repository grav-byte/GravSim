#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "App/Engine/Camera.h"
#include "../Engine/Physics/Constraint.h"


class RenderingSystem {
public:

    RenderingSystem();

    ~RenderingSystem();

    void SetActiveCamera(const Camera* camera) {
        activeCamera_ = camera;
    }

    void RenderCircle(const glm::mat4& transformMatrix,
                            const glm::vec4& color) const;

    void RenderSprite(unsigned int textureId, const glm::mat4& transformMatrix,
                            const glm::vec4& color) const;

    void RenderLine(const glm::vec2& start, const glm::vec2& end, int lineThickPx,
                            const glm::vec4 &color) const;

    void RenderConstraint(Constraint::ConstraintDirection direction, float threshold,
                               const glm::vec4& color) const;

private:
    void BuildCircleVertices();
    void BuildQuadVertices();

    void UploadCircleToGPU();
    void UploadQuadToGPU();

    const Camera* activeCamera_;

    int circleSegments_;
    std::vector<glm::vec2> circleVertices_;
    unsigned int circleVao_ = 0;
    unsigned int circleVbo_ = 0;
    unsigned int circleShaderProgram_ = 0;

    std::vector<glm::vec4> quadVertices_;
    unsigned int quadVao_ = 0;
    unsigned int quadVbo_ = 0;
    unsigned int spriteShaderProgram_ = 0;
    unsigned int constraintShaderProgram = 0;
};
