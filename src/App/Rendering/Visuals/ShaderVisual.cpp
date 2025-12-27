#include "ShaderVisual.h"

ShaderVisual::ShaderVisual() : shaderType(None) {
}
ShaderVisual::~ShaderVisual() = default;

VisualType ShaderVisual::GetType() {
    return VisualType::Shader;
}
