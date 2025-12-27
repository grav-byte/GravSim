#include "CircleRenderer.h"

#include "GL/glew.h"
#include <iostream>

#include "App/Engine/Loading/ShaderLoader.h"
#include "Core/AppLayer.h"


CircleRenderer::CircleRenderer(const RenderingSystem* renderer) : BaseRenderer(renderer), circleSegments_(64) {
    circleShaderProgram_ = ShaderLoader::LoadShader("simple.vert", "simple.frag");

    BuildCircleVertices();
    UploadCircleToGPU();
}

void CircleRenderer::BuildCircleVertices() {
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

void CircleRenderer::UploadCircleToGPU() {
    if (circleVao_ == 0) {
        glGenVertexArrays(1, &circleVao_);
    }
    if (circleVbo_ == 0) {
        glGenBuffers(1, &circleVbo_);
    }

    glBindVertexArray(circleVao_);
    glBindBuffer(GL_ARRAY_BUFFER, circleVbo_);

    glBufferData(GL_ARRAY_BUFFER,circleVertices_.size() * sizeof(glm::vec2),circleVertices_.data(),GL_STATIC_DRAW);

    // layout(location = 0) in vec2 aPos;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(glm::vec2),nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CircleRenderer::RenderCircle(const glm::mat4 matrix, const glm::vec4 color) const {
    const auto activeCamera = renderingSys_->GetActiveCamera();
    if (!circleShaderProgram_ || !activeCamera) {
        std::cout << "Trying to render circle without shader program or camera\n";
        return;
    }

    glUseProgram(circleShaderProgram_);

    const auto projection = activeCamera->GetProjectionMatrix();
    glm::mat4 finalTransform = projection * matrix;

    GLint colorLoc = glGetUniformLocation(circleShaderProgram_, "uColor");
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));

    GLint transformLoc = glGetUniformLocation(circleShaderProgram_, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));

    glBindVertexArray(circleVao_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(circleVertices_.size()));
    glBindVertexArray(0);
}