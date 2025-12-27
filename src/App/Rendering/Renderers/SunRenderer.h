#pragma once
#include "BaseRenderer.h"
#include "App/Engine/SceneObject.h"
#include "App/Rendering/RenderingSystem.h"


class SunRenderer: public BaseRenderer {
public:
    explicit SunRenderer(const RenderingSystem* renderer);

    void RenderSun(const SceneObject *obj) const;

    ~SunRenderer() override = default;

protected:
    unsigned int sunShaderProgram_ = 0;
};
