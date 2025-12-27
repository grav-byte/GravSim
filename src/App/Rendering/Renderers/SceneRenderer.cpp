#include "SceneRenderer.h"

#include "GL/glew.h"
#include "PostProcess/RipplePass.h"

SceneRenderer::SceneRenderer(RenderingSystem* system) :
    spriteRenderer_(system),
    circleRenderer_(system),
    constraintRenderer_(system),
    gridRenderer_(LineRenderer(system), system),
    colliderColor_(glm::vec4(1.0f, 0.0f, 0.0f, 0.5f)),
    renderer_(system)
{
    showGrid = true;
    //passes_.push_back(std::make_unique<RipplePass>(renderer_));
}

void SceneRenderer::RenderScene(const Scene* scene, const bool showColliders) const {
    // render grid
    gridRenderer_.RenderGrid(gridColor_, gridSpacing_);

    // render constraints
    for (const Constraint* c : scene->GetConstraints())
        constraintRenderer_.RenderConstraint(c, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // render scene objs
    for (const SceneObject* obj : scene->GetAllObjects()) {
        if (!obj->visual) continue;

        switch (obj->visual->GetType()) {
            case VisualType::Circle:
                circleRenderer_.RenderCircle(obj->transform.GetMatrix(), obj->visual->color);
                break;
            case VisualType::Sprite:
                spriteRenderer_.RenderSprite(obj);
                break;
        }
    }

    // render colliders and debug info
    if (showColliders) {
        for (const SceneObject* obj : scene->GetAllObjects()) {
            for (const auto& collider : obj->colliders)
                circleRenderer_.RenderCircle(collider->GetTransformMatrix(), colliderColor_);
            for (const auto& contact : obj->contactPoints) {
                // render contact point
                auto transform = glm::mat4(1.0f);
                // position
                transform = glm::translate(transform, glm::vec3(contact.point, 0.0f));
                // scale
                transform = glm::scale(transform, glm::vec3(0.1f));

                circleRenderer_.RenderCircle(transform, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            }
        }
    }
}

void SceneRenderer::ApplyPostProcess() const {
    unsigned int readFBO = renderer_->sceneFBO;
    unsigned int writeFBO = renderer_->postFBO;

    for (auto& pass : passes_) {

        glBindFramebuffer(GL_FRAMEBUFFER, writeFBO);
        glViewport(0, 0, renderer_->frameSize.x, renderer_->frameSize.y);

        pass->RenderPass(readFBO);
        std::swap(readFBO, writeFBO);
    }

    // final output is always in readFBO
    renderer_->sceneFBO = readFBO;
}
