#pragma once
#include <string>
#include <unordered_map>

class ShaderLoader {
public:
    static unsigned int LoadShader(const std::string &vertexPath, const std::string &fragmentPath);

    static void UnloadShader(unsigned int sprite_shader_program);

private:
    static unsigned int CompileShader(unsigned int type, const char* source);
    static char *LoadSource(std::string path);

    inline static std::unordered_map<std::string, unsigned int> loadedShaders_;
};
