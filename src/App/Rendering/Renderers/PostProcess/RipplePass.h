#pragma once
#include "PostProcessPass.h"
#include "App/Rendering/Renderers/BaseRenderer.h"


class RipplePass : public PostProcessPass {
public:
    explicit RipplePass(const RenderingSystem* renderer);

    void RenderPass(unsigned int inputTexture) const override;

private:
    unsigned int rippleShaderProgram_ = 0;
};
