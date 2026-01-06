#include "LineRenderer.h"

#include "App/Engine/Loading/TextureLoader.h"
#include "GL/glew.h"

LineRenderer::LineRenderer(const RenderingSystem* renderer): SpriteRenderer(renderer) {
}

void LineRenderer::RenderLine(const glm::vec2& start, const glm::vec2& end, int lineThickPx, const glm::vec4 &color) const {
    auto activeCamera = renderingSys_->GetActiveCamera();

    if (!spriteShaderProgram_ || !activeCamera) return;

    glUseProgram(spriteShaderProgram_);

    // framebuffer info
    float pixelWidth  = 2.0f / renderingSys_->frameSize.x; // NDC per pixel
    float pixelHeight = 2.0f / renderingSys_->frameSize.y;

    // camera projection
    glm::mat4 proj = activeCamera->GetProjectionMatrix();
    glm::mat4 invProj = glm::inverse(proj);

    // line vector
    const glm::vec2 dir = start - end;
    const float length = glm::length(dir);
    const float angle  = std::atan2(dir.y, dir.x);
    const glm::vec2 mid = (start + end) * 0.5f;

    // compute world-space thickness
    const glm::vec2 pixelWorld = glm::vec2(invProj * glm::vec4(pixelWidth, pixelHeight, 0.0f, 0.0f));
    const auto thickness = glm::length(pixelWorld) * static_cast<float>(lineThickPx) * .5f;

    // build transform
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(mid, 0.0f));
    transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(length * .5f, thickness, 1.0f));

    // final transform
    glm::mat4 finalTransform = proj * transform;

    // upload uniforms
    GLint transformLoc = glGetUniformLocation(spriteShaderProgram_, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));

    // color
    GLint colorLoc = glGetUniformLocation(spriteShaderProgram_, "uColor");
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));

    GLint texLoc = glGetUniformLocation(spriteShaderProgram_, "uTexture");
    glUniform1i(texLoc, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureLoader::GetWhiteTexture());

    // draw quad
    glBindVertexArray(renderingSys_->quadVao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(renderingSys_->quadVertices.size()));
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}
