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
    void RenderRipple();

    void CreateSceneFramebuffer();
    void StartFrame(glm::vec4 backgroundColor) const;
    void OutputFrameToScreen() const;

    Camera* GetActiveCamera() const;

private:
    void BuildCircleVertices();
    void BuildQuadVertices();

    void UploadCircleToGPU();
    void UploadQuadToGPU();

    const Camera* activeCamera_;
    glm::ivec2 frameSize_;

    std::vector<glm::vec4> quadVertices_;
    unsigned int quadVao_ = 0;
    unsigned int quadVbo_ = 0;
    unsigned int spriteShaderProgram_ = 0;
    unsigned int constraintShaderProgram_ = 0;
    unsigned int radialConstShaderProgram_ = 0;
    unsigned int rippleShaderProgram_ = 0;

    unsigned int sceneFBO_ = 0;
    unsigned int sceneTexture_ = 0;
    unsigned int sceneDepthRBO_ = 0;

    unsigned int postFBO_ = 0;
    unsigned int postTexture_ = 0;
};
