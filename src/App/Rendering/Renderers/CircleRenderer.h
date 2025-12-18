#pragma once
#include "App/Rendering/RenderingSystem.h"


class CircleRenderer {
public:
    explicit CircleRenderer(const RenderingSystem& renderer);

    void BuildCircleVertices();

    void UploadCircleToGPU();

    void RenderCircle(const SceneObject* obj) const;

private:
    RenderingSystem renderer_;
    int circleSegments_;
    std::vector<glm::vec2> circleVertices_;
    unsigned int circleVao_ = 0;
    unsigned int circleVbo_ = 0;
    unsigned int circleShaderProgram_ = 0;
};
