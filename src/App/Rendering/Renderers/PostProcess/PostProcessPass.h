#pragma once
#include "App/Rendering/Renderers/BaseRenderer.h"

class PostProcessPass: public BaseRenderer {
public:
    explicit PostProcessPass(const RenderingSystem* renderer): BaseRenderer(renderer) {}
    ~PostProcessPass() override = default;

    virtual void RenderPass(unsigned int inputTexture) const = 0;
};

