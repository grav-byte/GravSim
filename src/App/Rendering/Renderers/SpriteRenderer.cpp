#include "SpriteRenderer.h"
#include "../RenderingSystem.h"

#include "../TextureLoader.h"

SpriteRenderer::SpriteRenderer() {
    path_ = "";
}

SpriteRenderer::SpriteRenderer(const std::filesystem::path& path)
    : path_(path)
{
}

SpriteRenderer::~SpriteRenderer() = default;

void SpriteRenderer::Render(RenderingSystem& rendering, Transform transform) {
    if (path_ == "")
        return;

    if (textureId_ == 0) {
        // texture must be loaded first
        const auto info = TextureLoader::GetTexture(path_);
        textureId_ = info.id;
        aspectRatio_ = static_cast<float>(info.width) / static_cast<float>(info.height);
    }

    transform.scale.x *= aspectRatio_;

    rendering.RenderSprite(textureId_, transform.GetMatrix());
}

void SpriteRenderer::SetPath(const std::filesystem::path &string) {
    path_ = string;
    textureId_ = 0; // reset to force reload
}
