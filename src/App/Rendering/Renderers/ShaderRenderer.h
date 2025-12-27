#pragma once
#include <map>
#include <string>
#include <glm/gtc/type_ptr.hpp>

#include "App/Engine/SceneObject.h"

struct ShaderUniforms {
    std::map<std::string, float> floats;
    std::map<std::string, std::string> textures;

    // cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(floats, textures);
    }
};

class ShaderRenderer {
public:
    explicit ShaderRenderer(const RenderingSystem* renderer);

    void Render(const SceneObject* obj, unsigned int shaderProgram,
                const ShaderUniforms& uniforms = ShaderUniforms()) const;

private:
    const RenderingSystem* renderingSys_;
};