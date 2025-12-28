#pragma once
#include <map>
#include <string>
#include <glm/gtc/type_ptr.hpp>

#include "App/Engine/SceneObject.h"

struct ShaderUniforms {
    std::map<std::string, float> floats;
    std::map<std::string, std::string> textures;

    glm::vec2 resolution;
    unsigned int screenBufferTex = 0;

    // cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(floats, textures);
    }
};

class ShaderRenderer {
public:
    explicit ShaderRenderer(const RenderingSystem* renderer);

    void Render(const SceneObject *obj,
                const std::string &fragPath, const ShaderUniforms &uniforms = ShaderUniforms()) const;

protected:
    const RenderingSystem* renderingSys_;
};