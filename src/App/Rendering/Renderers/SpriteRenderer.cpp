#include "SpriteRenderer.h"

#include <iostream>

#include "App/Rendering/ShaderLoader.h"
#include "App/Rendering/Visuals/SpriteVisual.h"
#include "GL/glew.h"

SpriteRenderer::SpriteRenderer(const RenderingSystem* renderer) : BaseRenderer(renderer) {
    spriteShaderProgram_ = ShaderLoader::LoadShader("sprite.vert", "sprite.frag");
}

void SpriteRenderer::RenderSprite(const SceneObject *obj) const {
    const auto spriteVisual = dynamic_cast<SpriteVisual*>(obj->visual.get());
    const auto activeCamera = renderingSys_->GetActiveCamera();

    if (!spriteShaderProgram_ || !activeCamera) {
        std::cout << "Trying to render sprite without shader program or camera" << std::endl;
        return;
    }
    unsigned int textureId = spriteVisual->GetTextureId();

    if (textureId == 0)
        return;

    glUseProgram(spriteShaderProgram_);

    const auto projection = activeCamera->GetProjectionMatrix();
    glm::mat4 finalTransform = projection * obj->transform.GetMatrix();
    finalTransform = glm::scale(finalTransform, glm::vec3(spriteVisual->GetTextureAspect(), 1.0f, 1.0f));

    GLint transformLoc = glGetUniformLocation(spriteShaderProgram_, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));

    GLint colorLoc = glGetUniformLocation(spriteShaderProgram_, "uColor");
    glUniform4fv(colorLoc, 1, glm::value_ptr(spriteVisual->color));

    GLint texLoc = glGetUniformLocation(spriteShaderProgram_, "uTexture");
    glUniform1i(texLoc, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glBindVertexArray(renderingSys_->quadVao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(renderingSys_->quadVertices.size()));
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}
