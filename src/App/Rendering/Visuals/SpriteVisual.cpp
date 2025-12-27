#include "SpriteVisual.h"

#include "../../Engine/Loading/TextureLoader.h"

SpriteVisual::SpriteVisual() {
    path_ = "";
}

SpriteVisual::SpriteVisual(const std::filesystem::path& path)
    : path_(path)
{
}

SpriteVisual::~SpriteVisual() = default;

void SpriteVisual::SetPath(const std::filesystem::path &string) {
    path_ = string;
    textureId_ = 0; // reset to force reload
}

VisualType SpriteVisual::GetType() {
    return VisualType::Sprite;
}

unsigned int SpriteVisual::GetTextureId() {
    if (path_ == "")
        return 0;

    if (textureId_ == 0) {
        // texture must be loaded first
        const auto info = TextureLoader::GetTexture(path_);
        textureId_ = info.id;
        aspectRatio_ = static_cast<float>(info.width) / static_cast<float>(info.height);
    }

    return textureId_;
}

float SpriteVisual::GetTextureAspect() const {
    return aspectRatio_;
}
