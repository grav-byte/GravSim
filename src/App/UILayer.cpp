//
// Created by Lorenz Saalmann on 27.11.25.
//

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Core/Application.h"
#include "GLFW/glfw3.h"

#include "UILayer.h"

UILayer::UILayer() {

}

UILayer::~UILayer() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UILayer::OnInit() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // enable docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // enable multi-viewport

    // allow transparent background for platform windows
    io.ConfigViewportsNoTaskBarIcon = false;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_Window->GetHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 150");

    settingsUI = std::make_unique<SettingsUI>();
}

void UILayer::OnUpdate(float deltaTime) {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    settingsUI->Render();

    // --------- TEST UI ----------
    ImGui::Begin("ImGui Working!");
    ImGui::Text("If you see this, ImGui works!");
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

    const ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

}
