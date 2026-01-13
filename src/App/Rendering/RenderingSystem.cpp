#include "RenderingSystem.h"

#include <iostream>
#include <ostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "App/Engine/Loading/TextureLoader.h"
#include "Core/Application.h"

RenderingSystem::RenderingSystem(): activeCamera_(nullptr) {
    frameSize = Core::Application::Get().GetWindow()->GetFramebufferSize();

    BuildQuadVertices();
    UploadQuadToGPU();
}

RenderingSystem::~RenderingSystem() {
    if (quadVbo != 0)   glDeleteBuffers(1, &quadVbo);
    if (quadVao != 0)   glDeleteVertexArrays(1, &quadVao);
    TextureLoader::Clear();
}

void RenderingSystem::SetActiveCamera(const Camera *camera) {
    activeCamera_ = camera;
}


void RenderingSystem::BuildQuadVertices() {
    quadVertices.clear();
    quadVertices.reserve(6);

    // Full-screen quad in NDC coordinates [-1,1] (2 triangles)
    quadVertices.emplace_back(-1.0f, -1.0f, 0.0f, 0.0f);
    quadVertices.emplace_back( 1.0f, -1.0f, 1.0f, 0.0f);
    quadVertices.emplace_back( 1.0f,  1.0f, 1.0f, 1.0f);

    quadVertices.emplace_back(-1.0f, -1.0f, 0.0f, 0.0f);
    quadVertices.emplace_back( 1.0f,  1.0f, 1.0f, 1.0f);
    quadVertices.emplace_back(-1.0f,  1.0f, 0.0f, 1.0f);
}


void RenderingSystem::UploadQuadToGPU() {
    if (quadVao == 0) glGenVertexArrays(1, &quadVao);
    if (quadVbo == 0) glGenBuffers(1, &quadVbo);

    glBindVertexArray(quadVao);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo);

    glBufferData(GL_ARRAY_BUFFER,quadVertices.size() * sizeof(glm::vec4), quadVertices.data(), GL_STATIC_DRAW);

    // layout(location = 0) vec2 aPos;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(glm::vec4), (void*)0);

    // layout(location = 1) vec2 aTex;
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,sizeof(glm::vec4), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void RenderingSystem::CreateSceneFramebuffer() {
    // Delete previous if exists
    if (sceneFBO != 0) glDeleteFramebuffers(1, &sceneFBO);
    if (sceneTexture != 0) glDeleteTextures(1, &sceneTexture);
    if (sceneDepthRBO != 0) glDeleteRenderbuffers(1, &sceneDepthRBO);

    glGenFramebuffers(1, &sceneFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

    glGenTextures(1, &sceneTexture);
    glBindTexture(GL_TEXTURE_2D, sceneTexture);

    frameSize = Core::Application::Get().GetWindow()->GetFramebufferSize();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, frameSize.x, frameSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTexture, 0);

    glGenRenderbuffers(1, &sceneDepthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, sceneDepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameSize.x, frameSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, sceneDepthRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Failed to create scene framebuffer!" << std::endl;
    }

    // post-process framebuffer
    if (postFBO != 0) glDeleteFramebuffers(1, &postFBO);
    if (postTexture != 0) glDeleteTextures(1, &postTexture);

    glGenFramebuffers(1, &postFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, postFBO);

    glGenTextures(1, &postTexture);
    glBindTexture(GL_TEXTURE_2D, postTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,frameSize.x, frameSize.y,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postTexture, 0);
}

void RenderingSystem::StartFrame(const glm::vec4 backgroundColor) const {
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
    glViewport(0, 0, frameSize.x, frameSize.y);

    glClearColor(backgroundColor.r, backgroundColor.g,backgroundColor.b, backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderingSystem::OutputFrameToScreen() const {
    // scene rendering done - output to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, frameSize.x, frameSize.y);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, finalFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(
        0, 0, frameSize.x, frameSize.y, 0, 0, frameSize.x, frameSize.y,
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
}

Camera * RenderingSystem::GetActiveCamera() const { return const_cast<Camera*>(activeCamera_); }
