#pragma once

#include "IVisual.h"
#include "App/Rendering/Renderers/ShaderRenderer.h"
#include "cereal/types/base_class.hpp"

class ShaderVisual : public IVisual {
public:
    ShaderVisual();
    ~ShaderVisual() override;

    VisualType GetType() override;

    std::string shaderPath;
    ShaderUniforms shaderData;

    // cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<IVisual>(this), shaderPath, shaderData);
    }
};
