#include "RenderingSystem.h"

#include <iostream>
#include <ostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/AppLayer.h"
#include "Core/AppLayer.h"
#include "Core/AppLayer.h"
#include "Core/AppLayer.h"
#include "Core/AppLayer.h"
#include "Core/AppLayer.h"
#include "Core/Application.h"

RenderingSystem::RenderingSystem(const unsigned int shaderProgram,
                               const int segments)
    : segments_(std::max(3, segments)),
      vao_(0),
      vbo_(0),
      shaderProgram_(shaderProgram)
{
    BuildVertices();
    UploadToGPU();
}

RenderingSystem::~RenderingSystem() {
    if (vbo_ != 0) {
        glDeleteBuffers(1, &vbo_);
    }
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
    }
}

void RenderingSystem::RenderCircle(const glm::mat4 &transformMatrix, const glm::vec4 &color) const {

    if (!shaderProgram_ || !activeCamera_) {
        std::cout << "Trying to render circle without shader program or camera" << std::endl;
        return;
    }

    glUseProgram(shaderProgram_);

    const auto projection = activeCamera_->GetProjectionMatrix();

    glm::mat4 finalTransform = projection * transformMatrix;

    // set uniforms
    GLint colorLoc = glGetUniformLocation(shaderProgram_, "uColor");
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));

    GLint transformLoc = glGetUniformLocation(shaderProgram_, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));

    // draw
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(vertices_.size()));
    glBindVertexArray(0);
}

void RenderingSystem::BuildVertices() {
    vertices_.clear();
    vertices_.reserve(segments_ + 2);

    // center
    vertices_.emplace_back(0.0f, 0.0f);

    // edges (unit circle, radius = 1)
    for(int i = 0; i <= segments_; ++i)
    {
        const float angle = static_cast<float>(i) / static_cast<float>(segments_) * 2.0f * M_PI;
        float x = cos(angle);
        float y = sin(angle);
        vertices_.emplace_back(x, y);
    }
}

void RenderingSystem::UploadToGPU() {
    // Create VAO and VBO if they don't exist yet
    if (vao_ == 0) {
        glGenVertexArrays(1, &vao_);
    }
    if (vbo_ == 0) {
        glGenBuffers(1, &vbo_);
    }

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    // Upload vertex data
    glBufferData(GL_ARRAY_BUFFER,
                 vertices_.size() * sizeof(glm::vec2),
                 vertices_.data(),
                 GL_STATIC_DRAW);

    // Vertex attribute 0: vec2 position
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