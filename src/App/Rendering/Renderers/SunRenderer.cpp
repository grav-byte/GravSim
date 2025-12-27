#include "SunRenderer.h"
#include "GL/glew.h"

#include <iostream>

#include "App/Rendering/ShaderLoader.h"
#include "App/Rendering/TextureLoader.h"
#include "App/Rendering/Visuals/SpriteVisual.h"
#include "Core/Application.h"

SunRenderer::SunRenderer(const RenderingSystem* renderer) : BaseRenderer(renderer) {
    sunShaderProgram_ = ShaderLoader::LoadShader("sprite.vert", "sun.frag");
}

void SunRenderer::RenderSun(const SceneObject *obj) const {
    const auto activeCamera = renderingSys_->GetActiveCamera();

    if (!sunShaderProgram_ || !activeCamera) {
        std::cout << "Trying to render sun without shader program or camera" << std::endl;
        return;
    }

    const auto textureId = TextureLoader::GetTexture("../assets/textures/sun.png").id;
    glUseProgram(sunShaderProgram_);

    const auto projection = activeCamera->GetProjectionMatrix();
    glm::mat4 finalTransform = projection * obj->transform.GetMatrix();
    GLint transformLoc = glGetUniformLocation(sunShaderProgram_, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));

    // time
    GLint timeLoc = glGetUniformLocation(sunShaderProgram_, "uTime");
    glUniform1f(timeLoc, Core::Application::GetTime());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLint texLoc = glGetUniformLocation(sunShaderProgram_, "uTexture");
    glUniform1i(texLoc, 0);

    glBindVertexArray(renderingSys_->quadVao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(renderingSys_->quadVertices.size()));
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}
