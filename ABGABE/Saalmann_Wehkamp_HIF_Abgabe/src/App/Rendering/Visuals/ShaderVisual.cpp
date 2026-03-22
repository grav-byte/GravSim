#include "ShaderVisual.h"

ShaderVisual::ShaderVisual() : shaderData({}) {
}
ShaderVisual::~ShaderVisual() = default;

VisualType ShaderVisual::GetType() {
    return VisualType::Shader;
}
