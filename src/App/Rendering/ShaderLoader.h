//
// Created by Lorenz Saalmann on 01.12.25.
//

#pragma once
#include "Renderers/RenderingSystem.h"
#include <string>

class ShaderLoader {
public:
    static GLuint LoadShader(const std::string &vertexPath, const std::string &fragmentPath);
private:
    static unsigned int CompileShader(unsigned int type, const char* source);
    static char *LoadSource(std::string path);
};
