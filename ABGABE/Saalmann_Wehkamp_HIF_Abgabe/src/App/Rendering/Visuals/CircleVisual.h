#pragma once
#include "IVisual.h"
#include "cereal/types/base_class.hpp"


class CircleVisual: public IVisual {
public:
    CircleVisual();

    explicit CircleVisual(glm::vec4 color);
    ~CircleVisual() override;

    VisualType GetType() override;

    // Cereal serialization
    template<class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<IVisual>(this), color);
    }

};
