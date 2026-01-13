#include "GL/glew.h"
#include "TextureLoader.h"

#include <filesystem>
#include <iostream>

#include "stb_image.h"

GLuint TextureLoader::whiteTex_ = 0;

TextureLoader::TextureInfo TextureLoader::GetTexture(const std::filesystem::path &path) {
    const auto it = loadedTextures_.find(path);
    if (it != loadedTextures_.end()) {
        return it->second; // already loaded
    }

    TextureInfo tex = LoadTextureFromFile(path);
    loadedTextures_[path] = tex;
    return tex;
}

TextureLoader::TextureInfo TextureLoader::LoadTextureFromFile(const std::filesystem::path &path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 4);
    if (!data) {
        std::cout << "Failed to load texture: " << path.c_str() << std::endl;
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

GLuint TextureLoader::GetWhiteTexture() {
    if (whiteTex_ != 0) {
        return whiteTex_;
    }
    glGenTextures(1, &whiteTex_);
    glBindTexture(GL_TEXTURE_2D, whiteTex_);

    unsigned char whitePixel[4] = { 255, 255, 255, 255 }; // RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);

    // texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
    return whiteTex_;
}

void TextureLoader::Clear() {
    for (auto& [path, tex]: loadedTextures_) {
        glDeleteTextures(1, &tex.id);
    }
    loadedTextures_.clear();
}


