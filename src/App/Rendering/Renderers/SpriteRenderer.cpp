#include "SpriteRenderer.h"
#include "../RenderingSystem.h"

#include "../TextureLoader.h"

SpriteRenderer::SpriteRenderer(const std::string& path)
    : path_(path)
{
}

SpriteRenderer::~SpriteRenderer() = default;

void SpriteRenderer::Render(RenderingSystem& rendering, Transform transform) {
    if (textureId_ == 0) {
        const auto info = TextureLoader::GetTexture(path_);
        textureId_ = info.id;
        aspectRatio_ = static_cast<float>(info.width) / static_cast<float>(info.height);
    }

    transform.scale.x *= aspectRatio_;

    rendering.RenderSprite(textureId_, transform.GetMatrix());
}