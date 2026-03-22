#pragma once
#include <filesystem>
#include <unordered_map>
#define STB_IMAGE_IMPLEMENTATION

class TextureLoader {
public:
    struct TextureInfo {
        unsigned int id;
        int width;
        int height;
    };

    static TextureInfo GetTexture(const std::filesystem::path &path);

    static TextureInfo LoadTextureFromFile(const std::filesystem::path &path);

    static unsigned int GetWhiteTexture();

    static void Clear();

private:
    inline static std::unordered_map<std::filesystem::path, TextureInfo> loadedTextures_;
    static unsigned int whiteTex_;
};
