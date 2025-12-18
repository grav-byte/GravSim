//
// Created by kw on 01.12.2025.
//

#pragma once
#include "App/Engine/Transform.h"

class RenderingSystem;


enum class VisualType {
    Circle,
    Sprite
};

class IVisual {
public:
    virtual ~IVisual() = default;
    glm::vec4 color;

    virtual void Render(RenderingSystem& rendering, Transform transform) = 0;

    template<class Archive>
    void serialize(Archive& ar) {
        // nothing in base class
    }
};
