#pragma once

#include <filesystem>

#include "IRenderer.h"
#include "cereal/types/base_class.hpp"

class SpriteRenderer : public IRenderer {
public:
    SpriteRenderer();
    explicit SpriteRenderer(const std::filesystem::path& path);
    ~SpriteRenderer() override;

    void Render(RenderingSystem& rendering, Transform transform) override;

    const std::filesystem::path& GetPath() const { return path_; }

    void SetPath(const std::filesystem::path& string);

    // cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        std::string pathStr = path_.string();
        ar(cereal::base_class<IRenderer>(this), pathStr, color);
        if constexpr (Archive::is_loading::value) {
            path_ = pathStr;
        }
    }

private:
    unsigned int textureId_ = 0;
    std::filesystem::path path_;

    float aspectRatio_ = 1.0f;
};
