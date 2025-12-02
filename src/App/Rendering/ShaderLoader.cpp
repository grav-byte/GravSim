//
// Created by Lorenz Saalmann on 01.12.25.
//

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#if defined(_MSC_VER)
    #include <malloc.h>
    #define alloca _alloca
#else
    #include <alloca.h>
#endif

#include "GL/glew.h"
#include "ShaderLoader.h"


GLuint ShaderLoader::LoadShader(const std::string &vertexPath, const std::string &fragmentPath) {
    // Create shader program and compile & attach shaders
    const GLuint program = glCreateProgram();

    const char* vertexSource = LoadSource(vertexPath);
    const char* fragmentSource = LoadSource(fragmentPath);

    const unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    const unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

char* ShaderLoader::LoadSource(std::string path) {
    // Load shader source from file path

    const auto pathPrefix = "../assets/shaders/";
    path = std::string(pathPrefix) + std::string(path);
    FILE* file = fopen(path.c_str(), "rb");
    if (!file) {
        printf("Failed to open shader file: %s\n", path.c_str());
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    const long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    const auto buffer = new char[length + 1];
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

unsigned int ShaderLoader::CompileShader(const unsigned int type, const char* source) {
    // Create and compile shader
    const unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        const auto message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        const char* shaderType = type == GL_VERTEX_SHADER ? "vertex" : "fragment";
        printf("Failed to compile %s shader!\n%s\n", shaderType, message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}
