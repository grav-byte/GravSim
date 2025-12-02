#pragma once

#include "IRenderer.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class CircleRenderer {
public:
    // shaderProgram: ID of an already compiled & linked OpenGL shader program
    // color: RGBA color
    // segments: number of segments used to approximate the circle
    CircleRenderer(unsigned int shaderProgram, int segments);

    ~CircleRenderer();

    void RenderCircle(const glm::vec2 &position,
                                 float rotation,
                                 const glm::vec2 &scale,
                                 const glm::vec4 &color,
                                 float radius) const;

private:
    void BuildVertices();
    void UploadToGPU();

    int m_segments;
    std::vector<glm::vec2> m_vertices;

    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_shaderProgram;

};