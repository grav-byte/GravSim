#pragma once
#include "BaseRenderer.h"
#include "App/Engine/SceneObject.h"
#include "App/Rendering/RenderingSystem.h"


class SpriteRenderer: public BaseRenderer {
public:
    explicit SpriteRenderer(const RenderingSystem* renderer);

    void RenderSprite(const SceneObject *obj) const;

    ~SpriteRenderer() override;

protected:
    unsigned int spriteShaderProgram_ = 0;
};
