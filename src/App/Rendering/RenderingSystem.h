#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "App/Engine/Camera.h"

class RenderingSystem {
public:

    RenderingSystem();

    ~RenderingSystem();

    void SetActiveCamera(const Camera* camera);

    void CreateSceneFramebuffer();
    void StartFrame(glm::vec4 backgroundColor) const;
    void OutputFrameToScreen() const;

    Camera* GetActiveCamera() const;

    std::vector<glm::vec4> quadVertices;
    unsigned int quadVao = 0;
    unsigned int quadVbo = 0;

    glm::ivec2 frameSize{};

    unsigned int sceneFBO = 0;
    unsigned int sceneTexture = 0;
    unsigned int sceneDepthRBO = 0;

    unsigned int postFBO = 0;
    unsigned int postTexture = 0;

private:
    void BuildQuadVertices();

    void UploadQuadToGPU();

    const Camera* activeCamera_;
};
