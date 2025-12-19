#pragma once
#include "App/Rendering/RenderingSystem.h"

class BaseRenderer {
public:
    virtual ~BaseRenderer() = default;

    explicit BaseRenderer(const RenderingSystem* renderer): renderingSys_(renderer) {}

protected:
    const RenderingSystem* renderingSys_;
};
