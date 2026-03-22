#pragma once

class RenderingSystem;

class IRenderer {
public:
    virtual ~IRenderer() = default;

    explicit IRenderer(const RenderingSystem* renderer): renderingSys_(renderer) {}

protected:
    const RenderingSystem* renderingSys_;
};
