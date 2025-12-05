//
// Created by Lorenz Saalmann on 27.11.25.
//

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Core/Application.h"
#include "GLFW/glfw3.h"

#include "UILayer.h"

#include "UI/SceneUI.h"

UILayer::UILayer() {
    settingsUI_ = std::make_unique<SettingsUI>();
    sceneUI_ = std::make_unique<SceneUI>();
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
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.FrameRounding = 2.0f;

    ImGui_ImplGlfw_InitForOpenGL(window_->GetHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void UILayer::OnUpdate(float deltaTime) {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    // make background transparent
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,0));

    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);

    settingsUI_->Render();
    sceneUI_->Render();

    // --------- TEST UI ----------
    ImGui::Begin("ImGui Working!");
    ImGui::Text("If you see this, ImGui works!");
    ImGui::Checkbox("Show Demo Window", &showDemo_);
    ImGui::End();

    ImGui::PopStyleColor(2);

    if (showDemo_)
        ImGui::ShowDemoWindow(&showDemo_);
}

void UILayer::OnEvent(Core::Event &event) {
    settingsUI_->OnEvent(event);
    sceneUI_->OnEvent(event);
}

void UILayer::OnRender() {
    const auto size = window_->GetFramebufferSize();
    glViewport(0, 0, size.x, size.y);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup);
    }
}
