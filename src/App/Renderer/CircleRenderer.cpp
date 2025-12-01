#include "CircleRenderer.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

// -----------------------------------------------------
// Constructor / Destructor
// -----------------------------------------------------

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
      m_shaderProgram(shaderProgram)
{
    BuildVertices();
    UploadToGPU();
}

CircleRenderer::~CircleRenderer() {
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
    }
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
    }
}

// -----------------------------------------------------
// Configuration
// -----------------------------------------------------

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
}

void CircleRenderer::SetRotation(float radians) {
    m_rotation = radians;
}

void CircleRenderer::SetScale(const glm::vec2& scale) {
    m_scale = scale;
}

// -----------------------------------------------------
// Rendering
// -----------------------------------------------------

void CircleRenderer::Render() {
    // Build model matrix from position, rotation, and scale
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(m_position, 0.0f));
    model = glm::rotate(model, m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(
        model,
        glm::vec3(
            m_scale.x * 2.0f * m_radius,
            m_scale.y * 2.0f * m_radius,
            1.0f
        )
    );

    // Activate shader program
    glUseProgram(m_shaderProgram);

    // Set model matrix uniform (must exist in the shader as "u_Model")
    GLint locModel = glGetUniformLocation(m_shaderProgram, "u_Model");
    if (locModel >= 0) {
        glUniformMatrix4fv(locModel, 1, GL_FALSE, &model[0][0]);
    }

    // Set color uniform (must exist in the shader as "u_Color")
    GLint locColor = glGetUniformLocation(m_shaderProgram, "u_Color");
    if (locColor >= 0) {
        glUniform4fv(locColor, 1, &m_color[0]);
    }

    // Bind VAO and draw the circle using triangle fan
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(m_vertices.size()));
    glBindVertexArray(0);

    // Optional: reset program
    // glUseProgram(0);
}

// -----------------------------------------------------
// Internal helpers
// -----------------------------------------------------

void CircleRenderer::BuildVertices() {
    m_vertices.clear();

    // Center vertex of the triangle fan
    m_vertices.emplace_back(0.0f, 0.0f);

    constexpr auto twoPi = glm::two_pi<float>();
    for (int i = 0; i <= m_segments; ++i) {
        const float t   = static_cast<float>(i) / static_cast<float>(m_segments);
        const float ang = t * twoPi;
        float x   = std::cos(ang) * m_radius;
        float y   = std::sin(ang) * m_radius;
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