#include "ConstraintRenderer.h"

#include <iostream>

#include "App/Engine/Loading/ShaderLoader.h"
#include "GL/glew.h"

ConstraintRenderer::ConstraintRenderer(const RenderingSystem* renderer) : BaseRenderer(renderer) {
    constraintShaderProgram_ = ShaderLoader::LoadShader("sprite.vert", "constraint.frag");
    radialConstShaderProgram_ = ShaderLoader::LoadShader("sprite.vert", "radial_constraint.frag");
}

void ConstraintRenderer::RenderConstraint(const Constraint *constraint, const glm::vec4& color) const {
    const Constraint::ConstraintDirection direction = constraint->direction;
    float threshold = constraint->distance;
    auto activeCamera = renderingSys_->GetActiveCamera();

    if ((!constraintShaderProgram_ && !(direction == Constraint::RADIAL && radialConstShaderProgram_)) || !activeCamera) {
        std::cout << "Trying to render sprite without shader program or camera" << std::endl;
        return;
    }
    if (direction == Constraint::RADIAL) {
        glUseProgram(radialConstShaderProgram_);
    } else {
        glUseProgram(constraintShaderProgram_);
    }

    // framebuffer info
    const float aspect = static_cast<float>(renderingSys_->frameSize.x) / static_cast<float>(renderingSys_->frameSize.y);

    auto finalTransform = glm::mat4(1.0f);
    threshold = threshold * (direction == Constraint::LEFT || direction == Constraint::DOWN ? -1.0f : 1.0f);
    float x = -activeCamera->transform.position.x + (direction == Constraint::LEFT || direction == Constraint::RIGHT ? threshold : 0.0f);
    float y = -activeCamera->transform.position.y + (direction == Constraint::UP || direction == Constraint::DOWN ? threshold : 0.0f);
    x = x * activeCamera->zoom / aspect;
    y = y * activeCamera->zoom;

    if (direction == Constraint::RADIAL)
        finalTransform = glm::translate(finalTransform, glm::vec3(x, y, 0.0f));
    else if (direction == Constraint::LEFT || direction == Constraint::RIGHT)
        finalTransform = glm::translate(finalTransform, glm::vec3(x, 0.0f, 0.0f));
    else
        finalTransform = glm::translate(finalTransform, glm::vec3(0, y, 0.0f));

    const auto shader = direction == Constraint::RADIAL ? radialConstShaderProgram_ : constraintShaderProgram_;

    GLint resLoc = glGetUniformLocation(shader, "uResolution");
    glUniform2f(resLoc, static_cast<float>(renderingSys_->frameSize.x), static_cast<float>(renderingSys_->frameSize.y));

    GLint transformLoc = glGetUniformLocation(shader, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));

    GLint colorLoc = glGetUniformLocation(shader, "uColor");
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));

    if (direction != Constraint::RADIAL) {
        GLint offsetLoc = glGetUniformLocation(shader, "uOffset");
        auto screenPos = activeCamera->WorldToScreen(glm::vec2(0));
        screenPos = glm::vec2(static_cast<int>(screenPos.x) % renderingSys_->frameSize.x, static_cast<int>(screenPos.y) % renderingSys_->frameSize.y);
        glUniform1f(offsetLoc, direction == Constraint::LEFT || direction == Constraint::RIGHT ? -screenPos.y : screenPos.x);

        GLint dirLoc = glGetUniformLocation(shader, "uDirection");
        glUniform1i(dirLoc, static_cast<int>(direction));
    } else {
        GLint radiusLoc = glGetUniformLocation(shader, "uRadius");
        glUniform1f(radiusLoc, threshold * activeCamera->zoom * .5f);
    }

    glBindVertexArray(renderingSys_->quadVao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(renderingSys_->quadVertices.size()));
    glBindVertexArray(0);
}
