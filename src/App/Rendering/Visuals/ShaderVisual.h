#pragma once

#include <filesystem>

#include "IVisual.h"
#include "App/Rendering/Renderers/ShaderRenderer.h"
#include "cereal/types/base_class.hpp"

enum ShaderType {
    None,
    Sun
};

class ShaderVisual : public IVisual {
public:
    ShaderVisual();
    ~ShaderVisual() override;

    VisualType GetType() override;

    ShaderType shaderType;
    ShaderUniforms shaderData;

    // cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<IVisual>(this), shaderType, shaderData);
    }
};
