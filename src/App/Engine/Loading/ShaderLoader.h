#pragma once
#include <string>

class ShaderLoader {
public:
    static unsigned int LoadShader(const std::string &vertexPath, const std::string &fragmentPath);
private:
    static unsigned int CompileShader(unsigned int type, const char* source);
    static char *LoadSource(std::string path);
};
