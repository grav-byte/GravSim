#pragma once

#include "Renderers/IRenderer.h"
#include "cereal/types/base_class.hpp"
#include <string>

class SpriteRenderer : public IRenderer {
public:
    SpriteRenderer() = default;
    explicit SpriteRenderer(const std::string& path);
    ~SpriteRenderer() override;

    void Render(RenderingSystem& rendering, Transform transform) override;

    const std::string& GetPath() const { return path_; }

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<IRenderer>(this), path_, color);
    }

private:
    unsigned int textureId_ = 0;
    std::string path_;

    float aspectRatio_ = 1.0f;
};