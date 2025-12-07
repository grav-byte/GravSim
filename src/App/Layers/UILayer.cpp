//
// Created by Lorenz Saalmann on 27.11.25.
//

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "UILayer.h"

#include "imgui_internal.h"
#include "../UI/SceneUI.h"

UILayer::UILayer() {
    settingsUI_ = std::make_unique<SettingsUI>();
    sceneUI_ = std::make_unique<SceneUI>();
    showDemo_ = false;
    io_ = nullptr;
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
    io_ = &ImGui::GetIO();

    io_->ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // enable docking
    io_->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // enable multi-viewport

    // allow transparent background for platform windows
    io_->ConfigViewportsNoTaskBarIcon = false;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.FrameRounding = 2.0f;

    ImGui_ImplGlfw_InitForOpenGL(window_->GetHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void UILayer::DockWindowsFirstFrame(ImGuiID mainId) {
    // dock windows once
    static bool firstFrame = true;
    if (firstFrame) {
        firstFrame = false;

        ImGuiID dock_main_id = mainId;

        ImGuiID dock_id_left  = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
        ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);

        ImGui::DockBuilderDockWindow("Settings", dock_id_right);
        ImGui::DockBuilderDockWindow("Scene", dock_id_left);

        ImGui::DockBuilderFinish(mainId);
    }
}

void UILayer::OnUpdate(float deltaTime) {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    // make background transparent
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,0));

    ImGui::NewFrame();
    auto mainId = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);

    settingsUI_->Draw();
    sceneUI_->Draw();

    DockWindowsFirstFrame(mainId);

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

    if (!io_)
       return;

    // Stop mouse event propagation if the mouse is over any ImGui window and ImGui wants to capture the mouse
    if (io_->WantCaptureMouse  && ImGui::IsAnyItemHovered()) {
        const Core::EventType type = event.GetEventType();
        if (type == Core::MouseButtonPressed || type == Core::MouseButtonReleased || type == Core::MouseMoved) {
            event.Handled = true;
        }
    }
}


void UILayer::OnRender() {
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup);
    }
}
