#pragma once

#include "IRenderer.h"

#include <vector>
#include <glm/glm.hpp>

// Avoid hard dependency on GL headers in the header file.
// GLuint is an unsigned 32-bit integer in OpenGL.
using GLuint = std::uint32_t;

class CircleRenderer : public IRenderer {
public:
    // shaderProgram: ID of an already compiled & linked OpenGL shader program
    // color: RGBA color
    // segments: number of segments used to approximate the circle
    CircleRenderer(float radius,
                   const glm::vec4& color,
                   GLuint shaderProgram,
                   int segments = 16);

    ~CircleRenderer() override;

    // --- Configuration ---
    void SetRadius(float radius);
    void SetColor(const glm::vec4& color);

    void SetPosition(const glm::vec2& position);
    void SetRotation(float radians);
    void SetScale(const glm::vec2& scale);

    // --- RenderingInterface ---
    void Render() override;

private:
    void BuildVertices();
    void UploadToGPU();

private:
    float                  m_radius;
    glm::vec4              m_color;

    glm::vec2              m_position;
    glm::vec2              m_scale;
    float                  m_rotation;    // in radians

    int                    m_segments;
    std::vector<glm::vec2> m_vertices;

    GLuint                 m_vao;
    GLuint                 m_vbo;
    GLuint                 m_shaderProgram;
};