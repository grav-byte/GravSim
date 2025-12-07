//
// Created by Lorenz Saalmann on 07.12.25.
//

#pragma once
#include <string>
#include <unordered_map>
#define STB_IMAGE_IMPLEMENTATION

class TextureLoader {
public:
    struct TextureInfo {
        unsigned int id;
        int width;
        int height;
    };

    static TextureInfo GetTexture(const std::string &path);

    static TextureInfo LoadTextureFromFile(const std::string &path);

    static void Clear();

private:
    inline static std::unordered_map<std::string, TextureInfo> textures_;
};
