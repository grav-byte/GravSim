#include "SceneRenderer.h"

#include "App/Rendering/Visuals/ShaderVisual.h"
#include "GL/glew.h"
#include "Core/Application.h"

SceneRenderer::SceneRenderer(RenderingSystem* system) :
    spriteRenderer_(system),
    circleRenderer_(system),
    constraintRenderer_(system),
    gridRenderer_(LineRenderer(system), system),
    shaderRenderer_(system),
    colliderColor_(glm::vec4(1.0f, 0.0f, 0.0f, 0.5f)),
    renderer_(system)
{
    showGrid = true;
    passes_.push_back(std::make_unique<PostProcessPass>(renderer_, "bloom.frag"));
    //passes_.push_back(std::make_unique<PostProcessPass>(renderer_, "ripple.frag"));
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
            case VisualType::Shader:
                const auto visual = dynamic_cast<ShaderVisual*>(obj->visual.get());
                shaderRenderer_.Render(obj, visual->shaderPath, visual->shaderData);
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
