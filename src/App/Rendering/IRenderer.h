//
// Created by kw on 01.12.2025.
//

#pragma once
#include "App/Engine/Transform.h"

class RenderingSystem;

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void Render(RenderingSystem& rendering, Transform transform) = 0;
};
