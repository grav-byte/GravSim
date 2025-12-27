#include "ShaderRenderer.h"

#include "GL/glew.h"
#include "App/Rendering/RenderingSystem.h"
#include "App/Engine/Loading/TextureLoader.h"
#include "App/Engine/Loading/ShaderLoader.h"
#include "Core/Application.h"

ShaderRenderer::ShaderRenderer(const RenderingSystem *renderer): renderingSys_(renderer) {}

void ShaderRenderer::Render(const SceneObject *obj, const std::string &fragPath, const ShaderUniforms &uniforms) const {
    const auto shaderProgram = ShaderLoader::LoadShader("sprite.vert", fragPath);
    if(!shaderProgram || !obj) return;

    glUseProgram(shaderProgram);

    // upload transform
    const auto activeCamera = renderingSys_->GetActiveCamera();
    if(activeCamera) {
        glm::mat4 finalTransform = activeCamera->GetProjectionMatrix() * obj->transform.GetMatrix();
        GLint transformLoc = glGetUniformLocation(shaderProgram, "uTransform");
        if(transformLoc != -1)
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));
    }

    // time
    const GLint loc = glGetUniformLocation(shaderProgram, "uTime");
    if(loc != -1) glUniform1f(loc, Core::Application::GetTime());

    // upload standard uniforms
    for(const auto& [name, value] : uniforms.floats) {
        const GLint loc = glGetUniformLocation(shaderProgram, name.c_str());
        if(loc != -1) glUniform1f(loc, value);
    }

    // bind textures
    int texUnit = 0;
    for(const auto& [name, texPath] : uniforms.textures) {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureLoader::LoadTextureFromFile(texPath).id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        GLint loc = glGetUniformLocation(shaderProgram, name.c_str());
        if(loc != -1) glUniform1i(loc, texUnit);
        texUnit++;
    }

    // draw the quad
    glBindVertexArray(renderingSys_->quadVao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(renderingSys_->quadVertices.size()));
    glBindVertexArray(0);

    // unbind textures
    for(int i = 0; i < texUnit; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
