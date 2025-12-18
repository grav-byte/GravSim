#pragma once
#include "CircleRenderer.h"
#include "GridRenderer.h"
#include "SpriteRenderer.h"
#include "App/Engine/Scene.h"
#include "App/Rendering/RenderingSystem.h"


class SceneRenderer {
public:
    void RenderScene(RenderingSystem& renderer, const Scene* scene) const;

private:
    SpriteRenderer spriteRenderer_;
    CircleRenderer circleRenderer_;
    ConstraintRenderer constraintRenderer_;
    GridRenderer gridRenderer_;
};
