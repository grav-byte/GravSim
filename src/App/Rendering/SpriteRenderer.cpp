#include "SpriteRenderer.h"
#include "Renderers/RenderingSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>

#include "TextureLoader.h"

SpriteRenderer::SpriteRenderer(const std::string& path)
    : path_(path)
{
}

SpriteRenderer::~SpriteRenderer() {
    if (textureId_ != 0) {
        glDeleteTextures(1, &textureId_);
    }
}



void SpriteRenderer::Render(RenderingSystem& rendering, Transform transform) {
    if (textureId_ == 0) {
        const auto info = TextureLoader::GetTexture(path_);
        textureId_ = info.id;
        aspectRatio_ = static_cast<float>(info.width) / static_cast<float>(info.height);
    }

    transform.scale.x *= aspectRatio_;

    rendering.RenderSprite(textureId_, transform.GetMatrix());
}