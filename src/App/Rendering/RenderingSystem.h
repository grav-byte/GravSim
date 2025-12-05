#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "App/Engine/Camera.h"


class RenderingSystem {
public:
    // shaderProgram: ID of an already compiled & linked OpenGL shader program
    // segments: number of segments used to approximate the circle
    RenderingSystem(unsigned int shaderProgram, int segments);

    ~RenderingSystem();

void SetActiveCamera(const Camera* camera) {
        activeCamera_ = camera;
    }

    void RenderCircle(const glm::mat4 &transformMatrix, const glm::vec4 &color) const;

private:
    void BuildVertices();
    void UploadToGPU();

    const Camera* activeCamera_;

    int segments_;
    std::vector<glm::vec2> vertices_;

    unsigned int vao_;
    unsigned int vbo_;
    unsigned int shaderProgram_;

};
