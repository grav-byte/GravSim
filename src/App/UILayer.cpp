//
// Created by Lorenz Saalmann on 27.11.25.
//

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Core/Application.h"
#include "GLFW/glfw3.h"

#include "UILayer.h"

void UILayer::OnInit() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    const ImGuiIO &io = ImGui::GetIO();
    (void) io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_Window->GetHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void UILayer::OnUpdate(float deltaTime) {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // --------- TEST UI ----------
    ImGui::Begin("ImGui Working!");
    ImGui::Text("✅ If you see this, ImGui works!");
    ImGui::Checkbox("Show Demo Window", &m_ShowDemo);
    ImGui::End();

    if (m_ShowDemo)
        ImGui::ShowDemoWindow(&m_ShowDemo);
}

void UILayer::OnEvent(Core::Event &event) {
}

void UILayer::OnRender() {
    ImGui::Render();
    const auto size = m_Window->GetFramebufferSize();
    glViewport(0, 0, size.x, size.y);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UILayer::~UILayer() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}