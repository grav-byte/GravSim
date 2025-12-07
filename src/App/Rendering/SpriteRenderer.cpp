#include "SpriteRenderer.h"
#include "RenderingSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>
#include <cstdio>

SpriteRenderer::SpriteRenderer(const std::string& path)
    : path_(path)
{
    color = glm::vec4(1.0f); // falls du später z.B. Tint-Farbe nutzen willst
    LoadTexture();
}

SpriteRenderer::~SpriteRenderer() {
    if (textureId_ != 0) {
        glDeleteTextures(1, &textureId_);
    }
}

void SpriteRenderer::LoadTexture() {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path_.c_str(), &width, &height, &channels, 4);
    if (!data) {
        std::printf("Failed to load texture: %s\n", path_.c_str());
        return;
    }

    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_2D, textureId_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void SpriteRenderer::Render(RenderingSystem& rendering, Transform transform) {
    if (textureId_ == 0) return;

    rendering.RenderSprite(textureId_, transform.GetMatrix());
}