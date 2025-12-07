#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "App/Engine/Camera.h"


struct TextureInfo;

class RenderingSystem {
public:
    // circleShaderProgram: ID of an already compiled & linked OpenGL shader program
    // spriteShaderProgram: ID of an already compiled & linked OpenGL shader program
    // segments: number of segments used to approximate the circle
    RenderingSystem(unsigned int circleShaderProgram,
                    unsigned int spriteShaderProgram,
                    int segments);

    ~RenderingSystem();

    void SetActiveCamera(const Camera* camera) {
        activeCamera_ = camera;
    }

    void RenderCircle(const glm::mat4& transformMatrix,
                          const glm::vec4& color) const;

    void RenderSprite(unsigned int textureId,
                      const glm::mat4& transformMatrix) const;


private:
    void BuildCircleVertices();
    void UploadCircleToGPU();

    void BuildQuadVertices();
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
};
