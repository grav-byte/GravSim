#include "RenderingSystem.h"

#include <iostream>
#include <ostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

RenderingSystem::RenderingSystem(const unsigned int circleShaderProgram,
                                 const unsigned int spriteShaderProgram,
                                 const int segments)
    : activeCamera_(nullptr),
      circleSegments_(std::max(3, segments)),
      circleVao_(0),
      circleVbo_(0),
      circleShaderProgram_(circleShaderProgram),
      quadVao_(0),
      quadVbo_(0),
      spriteShaderProgram_(spriteShaderProgram)
{
    BuildCircleVertices();
    UploadCircleToGPU();

    BuildQuadVertices();
    UploadQuadToGPU();
}

RenderingSystem::~RenderingSystem() {
    if (circleVbo_ != 0) glDeleteBuffers(1, &circleVbo_);
    if (circleVao_ != 0) glDeleteVertexArrays(1, &circleVao_);
    if (quadVbo_ != 0)   glDeleteBuffers(1, &quadVbo_);
    if (quadVao_ != 0)   glDeleteVertexArrays(1, &quadVao_);
}

void RenderingSystem::RenderCircle(const glm::mat4 &transformMatrix,
                                   const glm::vec4 &color) const {

    if (!circleShaderProgram_ || !activeCamera_) {
        std::cout << "Trying to render circle without shader program or camera\n";
        return;
    }

    glUseProgram(circleShaderProgram_);

    const auto projection = activeCamera_->GetProjectionMatrix();
    glm::mat4 finalTransform = projection * transformMatrix;

    GLint colorLoc = glGetUniformLocation(circleShaderProgram_, "uColor");
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));

    GLint transformLoc = glGetUniformLocation(circleShaderProgram_, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));

    glBindVertexArray(circleVao_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(circleVertices_.size()));
    glBindVertexArray(0);
}

void RenderingSystem::BuildCircleVertices() {
    circleVertices_.clear();
    circleVertices_.reserve(circleSegments_ + 2);

    // center
    circleVertices_.emplace_back(0.0f, 0.0f);

    // edges (unit circle, radius = 1)
    const float twoPi = 2.0f * glm::pi<float>();
    for (int i = 0; i <= circleSegments_; ++i) {
        float angle = static_cast<float>(i) / static_cast<float>(circleSegments_) * twoPi;
        float x = std::cos(angle);
        float y = std::sin(angle);
        circleVertices_.emplace_back(x, y);
    }
}

void RenderingSystem::UploadCircleToGPU() {
    if (circleVao_ == 0) {
        glGenVertexArrays(1, &circleVao_);
    }
    if (circleVbo_ == 0) {
        glGenBuffers(1, &circleVbo_);
    }

    glBindVertexArray(circleVao_);
    glBindBuffer(GL_ARRAY_BUFFER, circleVbo_);

    glBufferData(GL_ARRAY_BUFFER,
                 circleVertices_.size() * sizeof(glm::vec2),
                 circleVertices_.data(),
                 GL_STATIC_DRAW);

    // layout(location = 0) in vec2 aPos;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec2),
        reinterpret_cast<void*>(0)
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderingSystem::BuildQuadVertices() {
    quadVertices_.clear();
    quadVertices_.reserve(6);

    // Zwei Dreiecke, centered (-0.5..0.5) um die Origin
    quadVertices_.emplace_back(-0.5f, -0.5f, 0.0f, 0.0f);
    quadVertices_.emplace_back( 0.5f, -0.5f, 1.0f, 0.0f);
    quadVertices_.emplace_back( 0.5f,  0.5f, 1.0f, 1.0f);

    quadVertices_.emplace_back(-0.5f, -0.5f, 0.0f, 0.0f);
    quadVertices_.emplace_back( 0.5f,  0.5f, 1.0f, 1.0f);
    quadVertices_.emplace_back(-0.5f,  0.5f, 0.0f, 1.0f);
}

void RenderingSystem::UploadQuadToGPU() {
    if (quadVao_ == 0) glGenVertexArrays(1, &quadVao_);
    if (quadVbo_ == 0) glGenBuffers(1, &quadVbo_);

    glBindVertexArray(quadVao_);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo_);

    glBufferData(GL_ARRAY_BUFFER,
                 quadVertices_.size() * sizeof(glm::vec4),
                 quadVertices_.data(),
                 GL_STATIC_DRAW);

    // layout(location = 0) vec2 aPos;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(glm::vec4), (void*)0);

    // layout(location = 1) vec2 aTex;
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(glm::vec4), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderingSystem::RenderSprite(unsigned int textureId,
                                   const glm::mat4& transformMatrix) const {

    if (!spriteShaderProgram_ || !activeCamera_) {
        std::cout << "Trying to render sprite without shader program or camera" << std::endl;
        return;
    }

    glUseProgram(spriteShaderProgram_);

    const auto projection = activeCamera_->GetProjectionMatrix();
    glm::mat4 finalTransform = projection * transformMatrix;

    GLint transformLoc = glGetUniformLocation(spriteShaderProgram_, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));

    GLint texLoc = glGetUniformLocation(spriteShaderProgram_, "uTexture");
    glUniform1i(texLoc, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glBindVertexArray(quadVao_);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(quadVertices_.size()));
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}