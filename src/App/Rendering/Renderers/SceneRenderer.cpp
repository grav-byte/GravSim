#include "SceneRenderer.h"

void SceneRenderer::RenderScene(RenderingSystem& renderer, const Scene* scene) const {
    gridRenderer_.RenderGrid(renderer);

    for (const Constraint* c : scene->GetConstraints())
        constraintRenderer_.RenderConstraint(renderer, c);

    for (const SceneObject* obj : scene->GetAllObjects()) {
        if (obj->visual) {
            switch (obj->visual->GetType()) {
                case VisualType::Circle:
                    circleRenderer_.RenderCircle(obj);
                    break;
                case VisualType::Sprite:
                    spriteRenderer_.RenderSprite(renderer, obj);
            }
        }
    }

    for (const SceneObject* obj : scene->GetAllObjects()) {
        for (const auto& collider : obj->colliders)
            circleRenderer_.Render(renderer, collider);
    }
}