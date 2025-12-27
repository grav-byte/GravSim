#include "RipplePass.h"

#include "GL/glew.h"
#include "App/Engine/Loading/ShaderLoader.h"
#include "App/Rendering/RenderingSystem.h"
#include "Core/Application.h"


RipplePass::RipplePass(const RenderingSystem* renderer) : PostProcessPass(renderer) {
    rippleShaderProgram_ = ShaderLoader::LoadShader("sprite.vert", "ripple.frag");
}

void RipplePass::RenderPass(unsigned int inputTexture) const {
    const auto activeCamera = renderingSys_->GetActiveCamera();

    if (!rippleShaderProgram_ || !activeCamera) return;
    glUseProgram(rippleShaderProgram_);

    // identity transform (fullscreen quad is already in NDC)
    GLint transformLoc = glGetUniformLocation(rippleShaderProgram_, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

    // framebuffer size
    GLint resLoc = glGetUniformLocation(rippleShaderProgram_, "uResolution");
    glUniform2f(resLoc, static_cast<float>(renderingSys_->frameSize.x), static_cast<float>(renderingSys_->frameSize.y));

    // time
    GLint timeLoc = glGetUniformLocation(rippleShaderProgram_, "uTime");
    glUniform1f(timeLoc, Core::Application::GetTime());

    // scene texture as input
    GLint texLoc = glGetUniformLocation(rippleShaderProgram_, "uScreenBuffer");
    glUniform1i(texLoc, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexture);

    // draw fullscreen quad
    glBindVertexArray(renderingSys_->quadVao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(renderingSys_->quadVertices.size()));
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}
