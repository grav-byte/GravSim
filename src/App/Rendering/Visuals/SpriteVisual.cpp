#include "SpriteVisual.h"
#include "../RenderingSystem.h"

#include "../TextureLoader.h"

SpriteVisual::SpriteVisual() {
    path_ = "";
}

SpriteVisual::SpriteVisual(const std::filesystem::path& path)
    : path_(path)
{
}

SpriteVisual::~SpriteVisual() = default;

void SpriteVisual::Render(RenderingSystem& rendering, Transform transform) {
    if (path_ == "")
        return;

    if (textureId_ == 0) {
        // texture must be loaded first
        const auto info = TextureLoader::GetTexture(path_);
        textureId_ = info.id;
        aspectRatio_ = static_cast<float>(info.width) / static_cast<float>(info.height);
    }

    transform.scale.x *= aspectRatio_;

    rendering.RenderSprite(textureId_, transform.GetMatrix(), color);
}

void SpriteVisual::SetPath(const std::filesystem::path &string) {
    path_ = string;
    textureId_ = 0; // reset to force reload
}
