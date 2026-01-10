#pragma once
#include "ArrowRenderer.h"
#include "CircleRenderer.h"
#include "ConstraintRenderer.h"
#include "GridRenderer.h"
#include "ShaderRenderer.h"
#include "SpriteRenderer.h"
#include "App/Engine/Scene.h"
#include "App/Rendering/RenderingSystem.h"
#include "PostProcess/PostProcessPass.h"


class SceneRenderer {
public:
    explicit SceneRenderer(RenderingSystem* system);

    void RenderScene(const Scene *scene, bool showColliders) const;

    void ApplyPostProcess() const;

    bool showGrid;
    float gridSpacing_ = 2.0f;
private:
    SpriteRenderer spriteRenderer_;
    CircleRenderer circleRenderer_;
    LineRenderer lineRenderer_;
    ConstraintRenderer constraintRenderer_;
    ShaderRenderer shaderRenderer_;
    GridRenderer gridRenderer_;
    ArrowRenderer arrowRenderer;

    glm::vec4 colliderColor_;
    RenderingSystem* renderer_;
    std::vector<std::unique_ptr<PostProcessPass>> passes_;
};
