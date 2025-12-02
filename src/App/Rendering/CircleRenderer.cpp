#include "CircleRenderer.h"

#include <iostream>
#include <ostream>
#include <GL/glew.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

CircleRenderer::CircleRenderer(const float radius,
                               const glm::vec4& color,
                               const GLuint shaderProgram,
                               const int segments)
    : m_radius(radius),
      m_color(color),
      m_position(0.0f, 0.0f),
      m_scale(1.0f, 1.0f),
      m_rotation(0.0f),
      m_segments(std::max(3, segments)),
      m_vao(0),
      m_vbo(0),
      m_shaderProgram(shaderProgram),
      m_transform(glm::mat4(1.0f))
{
    std::cout << "CircleRenderer created, shader=" << shaderProgram << std::endl;

    BuildVertices();
    UploadToGPU();
    UpdateTransform();
}

CircleRenderer::~CircleRenderer() {
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
    }
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
    }
}

void CircleRenderer::SetRadius(float radius) {
    m_radius = radius;
    BuildVertices();
    UploadToGPU();
}

void CircleRenderer::SetColor(const glm::vec4& color) {
    m_color = color;
}

void CircleRenderer::SetPosition(const glm::vec2& position) {
    m_position = position;
    UpdateTransform();
}

void CircleRenderer::SetRotation(float radians) {
    m_rotation = radians;
    UpdateTransform();
}

void CircleRenderer::SetScale(const glm::vec2& scale) {
    m_scale = scale;
    UpdateTransform();
}

void CircleRenderer::Render() {
    if (!m_shaderProgram)
        return;

    glUseProgram(m_shaderProgram);

    // --- Aspect Ratio aus dem aktuellen Viewport holen ---
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    const int vpWidth  = viewport[2];
    const int vpHeight = viewport[3];

    float aspect = 1.0f;
    if (vpHeight > 0) {
        aspect = static_cast<float>(vpWidth) / static_cast<float>(vpHeight);
    }

    // Wir kompensieren die horizontale Streckung: X um 1/aspect skalieren.
    glm::mat4 aspectFix = glm::scale(glm::mat4(1.0f),
                                     glm::vec3(1.0f / aspect, 1.0f, 1.0f));

    glm::mat4 finalTransform = aspectFix * m_transform;

    // --- Uniforms setzen ---
    GLint colorLoc = glGetUniformLocation(m_shaderProgram, "uColor");
    glUniform4fv(colorLoc, 1, glm::value_ptr(m_color));

    GLint transformLoc = glGetUniformLocation(m_shaderProgram, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));

    // --- Zeichnen ---
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(m_vertices.size()));
    glBindVertexArray(0);
}

void CircleRenderer::BuildVertices() {
    m_vertices.clear();
    m_vertices.reserve(m_segments + 2);

    // Mittelpunkt
    m_vertices.emplace_back(0.0f, 0.0f);

    // Randpunkte
    for(int i = 0; i <= m_segments; ++i)
    {
        float angle = (float)i / m_segments * 2.0f * M_PI;
        float x = cos(angle) * m_radius;
        float y = sin(angle) * m_radius;
        m_vertices.emplace_back(x, y);
    }
}

void CircleRenderer::UploadToGPU() {
    // Create VAO and VBO if they don't exist yet
    if (m_vao == 0) {
        glGenVertexArrays(1, &m_vao);
    }
    if (m_vbo == 0) {
        glGenBuffers(1, &m_vbo);
    }

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Upload vertex data
    glBufferData(GL_ARRAY_BUFFER,
                 m_vertices.size() * sizeof(glm::vec2),
                 m_vertices.data(),
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

void CircleRenderer::UpdateTransform() {
    m_transform = glm::mat4(1.0f);

    // Position
    m_transform = glm::translate(m_transform, glm::vec3(m_position, 0.0f));

    // Rotation (um Z-Achse)
    m_transform = glm::rotate(m_transform, m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));

    // Scale
    m_transform = glm::scale(m_transform, glm::vec3(m_scale, 1.0f));
}