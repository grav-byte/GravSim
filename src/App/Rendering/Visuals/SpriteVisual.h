#pragma once

#include <filesystem>

#include "IVisual.h"
#include "cereal/types/base_class.hpp"

class SpriteVisual : public IVisual {
public:
    SpriteVisual();
    explicit SpriteVisual(const std::filesystem::path& path);
    ~SpriteVisual() override;

    void Render(RenderingSystem& rendering, Transform transform) override;

    const std::filesystem::path& GetPath() const { return path_; }

    void SetPath(const std::filesystem::path& string);

    // cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        std::string pathStr = path_.string();
        ar(cereal::base_class<IVisual>(this), pathStr, color);
        if constexpr (Archive::is_loading::value) {
            path_ = pathStr;
        }
    }

private:
    unsigned int textureId_ = 0;
    std::filesystem::path path_;

    float aspectRatio_ = 1.0f;
};
