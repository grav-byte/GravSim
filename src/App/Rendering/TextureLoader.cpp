//
// Created by Lorenz Saalmann on 07.12.25.
//

#include "GL/glew.h"
#include "TextureLoader.h"

#include <filesystem>

#include "stb_image.h"


TextureLoader::TextureInfo TextureLoader::GetTexture(const std::filesystem::path &path) {
    auto it = textures_.find(path);
    if (it != textures_.end()) {
        return it->second; // already loaded
    }

    TextureInfo tex = LoadTextureFromFile(path);
    textures_[path] = tex;
    return tex;
}

TextureLoader::TextureInfo TextureLoader::LoadTextureFromFile(const std::filesystem::path &path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 4);
    if (!data) {
        std::printf("Failed to load texture: %s\n", path.c_str());
        return {0, 0, 0};
    }

    unsigned int textureId = 0;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

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

    return {textureId, width, height};
}

void TextureLoader::Clear() {
    for (auto& [path, tex]: textures_) {
        glDeleteTextures(1, &tex.id);
    }
    textures_.clear();
}


