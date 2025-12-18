#include "RenderingSystem.h"

#include <iostream>
#include <ostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderLoader.h"
#include "App/Rendering/TextureLoader.h"
#include "Core/Application.h"

RenderingSystem::RenderingSystem()    : activeCamera_(nullptr), circleSegments_(64) {
    circleShaderProgram_ = ShaderLoader::LoadShader("simple.vert", "simple.frag");
    spriteShaderProgram_ = ShaderLoader::LoadShader("sprite.vert", "sprite.frag");
    constraintShaderProgram_ = ShaderLoader::LoadShader("sprite.vert", "constraint.frag");
    radialConstShaderProgram_ = ShaderLoader::LoadShader("sprite.vert", "radial_constraint.frag");
    rippleShaderProgram_ = ShaderLoader::LoadShader("sprite.vert", "ripple.frag");

    BuildCircleVertices();
    UploadCircleToGPU();

    BuildQuadVertices();
    UploadQuadToGPU();
}

RenderingSystem::~RenderingSystem() {
    if (circleVbo_ != 0) glDeleteBuffers(1, &circleVbo_);
    if (circleVao_ != 0) glDeleteVertexArrays(1, &circleVao_);
    if (quadVbo_ != 0)   glDeleteBuffers(1, &quadVbo_);
    if (quadVao_ != 0)   glDeleteVertexArrays(1, &quadVao_);
    TextureLoader::Clear();
}

void RenderingSystem::BuildCircleVertices() {
    circleVertices_.clear();
    circleVertices_.reserve(circleSegments_ + 2);

    // center
    circleVertices_.emplace_back(0.0f, 0.0f);

    // edges (unit circle, radius = 1)
    const float twoPi = 2.0f * glm::pi<float>();
    for (int i = 0; i <= circleSegments_; ++i) {
        float angle = static_cast<float>(i) / static_cast<float>(circleSegments_) * twoPi;
        float x = std::cos(angle);
        float y = std::sin(angle);
        circleVertices_.emplace_back(x, y);
    }
}

void RenderingSystem::BuildQuadVertices() {
    quadVertices_.clear();
    quadVertices_.reserve(6);

    // Full-screen quad in NDC coordinates [-1,1] (2 triangles)
    quadVertices_.emplace_back(-1.0f, -1.0f, 0.0f, 0.0f);
    quadVertices_.emplace_back( 1.0f, -1.0f, 1.0f, 0.0f);
    quadVertices_.emplace_back( 1.0f,  1.0f, 1.0f, 1.0f);

    quadVertices_.emplace_back(-1.0f, -1.0f, 0.0f, 0.0f);
    quadVertices_.emplace_back( 1.0f,  1.0f, 1.0f, 1.0f);
    quadVertices_.emplace_back(-1.0f,  1.0f, 0.0f, 1.0f);
}

void RenderingSystem::UploadCircleToGPU() {
    if (circleVao_ == 0) {
        glGenVertexArrays(1, &circleVao_);
    }
    if (circleVbo_ == 0) {
        glGenBuffers(1, &circleVbo_);
    }

    glBindVertexArray(circleVao_);
    glBindBuffer(GL_ARRAY_BUFFER, circleVbo_);

    glBufferData(GL_ARRAY_BUFFER,
                 circleVertices_.size() * sizeof(glm::vec2),
                 circleVertices_.data(),
                 GL_STATIC_DRAW);

    // layout(location = 0) in vec2 aPos;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec2),
        reinterpret_cast<void*>(0)
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderingSystem::UploadQuadToGPU() {
    if (quadVao_ == 0) glGenVertexArrays(1, &quadVao_);
    if (quadVbo_ == 0) glGenBuffers(1, &quadVbo_);

    glBindVertexArray(quadVao_);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo_);

    glBufferData(GL_ARRAY_BUFFER,
                 quadVertices_.size() * sizeof(glm::vec4),
                 quadVertices_.data(),
                 GL_STATIC_DRAW);

    // layout(location = 0) vec2 aPos;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(glm::vec4), (void*)0);

    // layout(location = 1) vec2 aTex;
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(glm::vec4), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void RenderingSystem::RenderLine(const glm::vec2& start, const glm::vec2& end, int lineThickPx,
                                 const glm::vec4 &color) const {
    if (!spriteShaderProgram_ || !activeCamera_) return;

    glUseProgram(spriteShaderProgram_);

    // framebuffer info
    float pixelWidth  = 2.0f / frameSize_.x; // NDC per pixel
    float pixelHeight = 2.0f / frameSize_.y;

    // camera projection
    glm::mat4 proj = activeCamera_->GetProjectionMatrix();
    glm::mat4 invProj = glm::inverse(proj);

    // convert start/end to NDC
    glm::vec4 startNDC4 = proj * glm::vec4(start, 0.0f, 1.0f);
    glm::vec4 endNDC4   = proj * glm::vec4(end, 0.0f, 1.0f);

    glm::vec2 startNDC = glm::vec2(startNDC4) / startNDC4.w;
    glm::vec2 endNDC   = glm::vec2(endNDC4) / endNDC4.w;

    // snap endpoints to pixel grid
    startNDC = glm::round(startNDC / glm::vec2(pixelWidth, pixelHeight)) * glm::vec2(pixelWidth, pixelHeight);
    endNDC   = glm::round(endNDC / glm::vec2(pixelWidth, pixelHeight)) * glm::vec2(pixelWidth, pixelHeight);

    // back to world space
    glm::vec2 snappedStart = glm::vec2(invProj * glm::vec4(startNDC, 0.0f, 1.0f));
    glm::vec2 snappedEnd   = glm::vec2(invProj * glm::vec4(endNDC, 0.0f, 1.0f));

    // line vector
    glm::vec2 dir = snappedEnd - snappedStart;
    float length = glm::length(dir);
    float angle  = std::atan2(dir.y, dir.x);
    glm::vec2 mid = (snappedStart + snappedEnd) * 0.5f;

    // compute world-space thickness
    glm::vec2 pixelWorld = glm::vec2(invProj * glm::vec4(pixelWidth, pixelHeight, 0.0f, 0.0f));
    auto thickness = glm::length(pixelWorld) * lineThickPx * .5f;

    // build transform
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(mid, 0.0f));
    transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(length, thickness, 1.0f));

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
    glBindVertexArray(quadVao_);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(quadVertices_.size()));
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}


void RenderingSystem::RenderCircle(const glm::mat4 &transformMatrix,
                                   const glm::vec4 &color) const {

    if (!circleShaderProgram_ || !activeCamera_) {
        std::cout << "Trying to render circle without shader program or camera\n";
        return;
    }

    glUseProgram(circleShaderProgram_);

    const auto projection = activeCamera_->GetProjectionMatrix();
    glm::mat4 finalTransform = projection * transformMatrix;

    GLint colorLoc = glGetUniformLocation(circleShaderProgram_, "uColor");
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));

    GLint transformLoc = glGetUniformLocation(circleShaderProgram_, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));

    glBindVertexArray(circleVao_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(circleVertices_.size()));
    glBindVertexArray(0);
}


void RenderingSystem::RenderSprite(unsigned int textureId, const glm::mat4& transformMatrix, const glm::vec4& color) const {

    if (!spriteShaderProgram_ || !activeCamera_) {
        std::cout << "Trying to render sprite without shader program or camera" << std::endl;
        return;
    }
    glUseProgram(spriteShaderProgram_);

    const auto projection = activeCamera_->GetProjectionMatrix();
    glm::mat4 finalTransform = projection * transformMatrix;

    GLint transformLoc = glGetUniformLocation(spriteShaderProgram_, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));

    GLint colorLoc = glGetUniformLocation(spriteShaderProgram_, "uColor");
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));

    GLint texLoc = glGetUniformLocation(spriteShaderProgram_, "uTexture");
    glUniform1i(texLoc, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glBindVertexArray(quadVao_);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(quadVertices_.size()));
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingSystem::RenderConstraint(Constraint::ConstraintDirection direction, float threshold, const glm::vec4& color) const {
    if ((!constraintShaderProgram_ && !(direction == Constraint::RADIAL && radialConstShaderProgram_)) || !activeCamera_) {
        std::cout << "Trying to render sprite without shader program or camera" << std::endl;
        return;
    }
    if (direction == Constraint::RADIAL) {
        glUseProgram(radialConstShaderProgram_);
    } else {
        glUseProgram(constraintShaderProgram_);
    }

    // framebuffer info
    float aspect = static_cast<float>(frameSize_.x) / static_cast<float>(frameSize_.y);

    glm::mat4 finalTransform = glm::mat4(1.0f);
    threshold = threshold * (direction == Constraint::LEFT || direction == Constraint::DOWN ? -1.0f : 1.0f);
    float x = -activeCamera_->transform.position.x + (direction == Constraint::LEFT || direction == Constraint::RIGHT ? threshold : 0.0f);
    float y = -activeCamera_->transform.position.y + (direction == Constraint::UP || direction == Constraint::DOWN ? threshold : 0.0f);
    x = x * activeCamera_->zoom / aspect;
    y = y * activeCamera_->zoom;

    if (direction == Constraint::RADIAL)
        finalTransform = glm::translate(finalTransform, glm::vec3(x, y, 0.0f));
    else if (direction == Constraint::LEFT || direction == Constraint::RIGHT)
        finalTransform = glm::translate(finalTransform, glm::vec3(x, 0.0f, 0.0f));
    else
        finalTransform = glm::translate(finalTransform, glm::vec3(0, y, 0.0f));

    const auto shader = direction == Constraint::RADIAL ? radialConstShaderProgram_ : constraintShaderProgram_;

    GLint resLoc = glGetUniformLocation(shader, "uResolution");
    glUniform2f(resLoc, static_cast<float>(frameSize_.x), static_cast<float>(frameSize_.y));

    GLint transformLoc = glGetUniformLocation(shader, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTransform));

    GLint colorLoc = glGetUniformLocation(shader, "uColor");
    glUniform4fv(colorLoc, 1, glm::value_ptr(color));

    if (direction != Constraint::RADIAL) {
        GLint offsetLoc = glGetUniformLocation(shader, "uOffset");
        auto screenPos = activeCamera_->WorldToScreen(glm::vec2(0));
        screenPos = glm::vec2(static_cast<int>(screenPos.x) % frameSize_.x, static_cast<int>(screenPos.y) % frameSize_.y);
        glUniform1f(offsetLoc, direction == Constraint::LEFT || direction == Constraint::RIGHT ? -screenPos.y : screenPos.x);

        GLint dirLoc = glGetUniformLocation(shader, "uDirection");
        glUniform1i(dirLoc, static_cast<int>(direction));
    } else {
        GLint radiusLoc = glGetUniformLocation(shader, "uRadius");
        glUniform1f(radiusLoc, threshold * activeCamera_->zoom * .5f);
    }

    glBindVertexArray(quadVao_);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(quadVertices_.size()));
    glBindVertexArray(0);
}

void RenderingSystem::RenderRipple() {
    if (!rippleShaderProgram_ || !activeCamera_) return;

    glBindFramebuffer(GL_FRAMEBUFFER, postFBO_);
    glViewport(0, 0, frameSize_.x, frameSize_.y);

    glUseProgram(rippleShaderProgram_);

    // identity transform (fullscreen quad is already in NDC)
    GLint transformLoc = glGetUniformLocation(rippleShaderProgram_, "uTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

    // framebuffer size
    GLint resLoc = glGetUniformLocation(rippleShaderProgram_, "uResolution");
    glUniform2f(resLoc, static_cast<float>(frameSize_.x), static_cast<float>(frameSize_.y));

    // time
    GLint timeLoc = glGetUniformLocation(rippleShaderProgram_, "uTime");
    glUniform1f(timeLoc, static_cast<float>(glfwGetTime()));

    // scene texture as input
    GLint texLoc = glGetUniformLocation(rippleShaderProgram_, "uScreenBuffer");
    glUniform1i(texLoc, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sceneTexture_);

    // draw fullscreen quad
    glBindVertexArray(quadVao_);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(quadVertices_.size()));
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

    // swap buffers
    std::swap(sceneFBO_, postFBO_);
    std::swap(sceneTexture_, postTexture_);
}

void RenderingSystem::CreateSceneFramebuffer() {
    // Delete previous if exists
    if (sceneFBO_ != 0) glDeleteFramebuffers(1, &sceneFBO_);
    if (sceneTexture_ != 0) glDeleteTextures(1, &sceneTexture_);
    if (sceneDepthRBO_ != 0) glDeleteRenderbuffers(1, &sceneDepthRBO_);

    glGenFramebuffers(1, &sceneFBO_);
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO_);

    glGenTextures(1, &sceneTexture_);
    glBindTexture(GL_TEXTURE_2D, sceneTexture_);

    frameSize_ = Core::Application::Get().GetWindow()->GetFramebufferSize();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, frameSize_.x, frameSize_.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTexture_, 0);

    glGenRenderbuffers(1, &sceneDepthRBO_);
    glBindRenderbuffer(GL_RENDERBUFFER, sceneDepthRBO_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameSize_.x, frameSize_.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, sceneDepthRBO_);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Failed to create scene framebuffer!" << std::endl;
    }

    // post-process framebuffer
    if (postFBO_ != 0) glDeleteFramebuffers(1, &postFBO_);
    if (postTexture_ != 0) glDeleteTextures(1, &postTexture_);

    glGenFramebuffers(1, &postFBO_);
    glBindFramebuffer(GL_FRAMEBUFFER, postFBO_);

    glGenTextures(1, &postTexture_);
    glBindTexture(GL_TEXTURE_2D, postTexture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,frameSize_.x, frameSize_.y,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postTexture_, 0);
}

void RenderingSystem::StartFrame(const glm::vec4 backgroundColor) const {
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO_);
    glViewport(0, 0, frameSize_.x, frameSize_.y);

    glClearColor(backgroundColor.r, backgroundColor.g,backgroundColor.b, backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderingSystem::OutputFrameToScreen() const {
    // scene rendering done - output to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, frameSize_.x, frameSize_.y);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, sceneFBO_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(
        0, 0, frameSize_.x, frameSize_.y, 0, 0, frameSize_.x, frameSize_.y,
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
}
