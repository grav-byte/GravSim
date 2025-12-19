#pragma once
#include "BaseRenderer.h"
#include "App/Rendering/RenderingSystem.h"


class CircleRenderer : public BaseRenderer {
public:
    explicit CircleRenderer(const RenderingSystem* renderer);
    ~CircleRenderer() override = default;


    void BuildCircleVertices();

    void UploadCircleToGPU();

    void RenderCircle(glm::mat4 matrix, glm::vec4 color) const;

private:
    int circleSegments_;
    std::vector<glm::vec2> circleVertices_;
    unsigned int circleVao_ = 0;
    unsigned int circleVbo_ = 0;
    unsigned int circleShaderProgram_ = 0;
};
