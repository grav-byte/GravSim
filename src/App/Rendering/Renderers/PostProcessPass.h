#pragma once
#include "App/Rendering/Renderers/ShaderRenderer.h"

class PostProcessPass: public ShaderRenderer {
public:
    explicit PostProcessPass(const RenderingSystem* renderer, const std::string &fragPath): ShaderRenderer(renderer), fragPath_(fragPath){}

    void RenderPass(unsigned int inputTexture);
    ShaderUniforms uniforms;

private:
    std::string fragPath_;
};

inline void PostProcessPass::RenderPass(const unsigned int inputTexture) {
    uniforms.resolution = glm::vec2(static_cast<float>(renderingSys_->frameSize.x), static_cast<float>(renderingSys_->frameSize.y));
    uniforms.screenBufferTex = inputTexture;

    Render(nullptr, fragPath_, uniforms);
}
