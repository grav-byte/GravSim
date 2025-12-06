//
// Created by Lorenz Saalmann on 05.12.25.
//

#pragma once
#include "IRenderer.h"
#include "cereal/types/base_class.hpp"


class CircleRenderer: public IRenderer {
public:
    CircleRenderer();
    CircleRenderer(glm::vec4 color);
    ~CircleRenderer() override;
    void Render(RenderingSystem& rendering, Transform transform) override;

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<IRenderer>(this), color);
    }
};
