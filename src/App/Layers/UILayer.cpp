//
// Created by Lorenz Saalmann on 27.11.25.
//

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "UILayer.h"

#include "imgui_internal.h"
#include "implot.h"
#include "../UI/SceneUI.h"
#include "App/UI/CustomImGuiStyle.h"
#include "Core/Application.h"

UILayer::UILayer() {
    settingsUI_ = std::make_unique<SettingsUI>();
    sceneUI_ = std::make_unique<SceneUI>();
    simulationUI_ = std::make_unique<SimulationUI>();
    showDemo_ = false;
    io_ = nullptr;
}

UILayer::~UILayer() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void UILayer::OnInit() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    io_ = &ImGui::GetIO();
    io_->Fonts->AddFontDefault();

    io_->ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // enable docking
    io_->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // enable multi-viewport

    // allow transparent background for platform windows
    io_->ConfigViewportsNoTaskBarIcon = false;

    CustomImGuiStyle::ApplyStyle();

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

void UILayer::DrawFPSCounter() {
    const float width  = 70.0f;
    const float height = 45.0f;

    const ImGuiViewport* vp = ImGui::GetMainViewport();

    ImVec2 pos(
        vp->Pos.x + (vp->Size.x - width) * 0.5f - 230.0f,
        vp->Pos.y + 10.0f
    );

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground;

    ImGui::Begin("FPS", nullptr, flags);
    ImGui::Text("FPS: %i", Core::Application::Get().GetFramerate());
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Framerate of the application");
    }
    ImGui::End();
}

void UILayer::OnUpdate(float deltaTime) {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    // make background transparent
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0, 0, 0, 0));

    ImGui::NewFrame();

    ImGuiID mainId = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar);

    settingsUI_->Draw();
    sceneUI_->Draw();
    simulationUI_->Draw();

    DrawFPSCounter();

    DockWindowsFirstFrame(mainId);

    // --------- TEST UI ----------
    ImGui::Begin("ImGui Working!");
    ImGui::Text("If you see this, ImGui works!");
    ImGui::Checkbox("Show Demo Window", &showDemo_);
    ImGui::End();

    ImGui::PopStyleColor(1);

    if (showDemo_)
        ImGui::ShowDemoWindow(&showDemo_);
}

void UILayer::OnEvent(Core::Event &event) {
    settingsUI_->OnEvent(event);
    sceneUI_->OnEvent(event);

    if (!io_)
       return;

    // Stop mouse event propagation if the mouse is over any ImGui window and ImGui wants to capture the mouse
    if (io_->WantCaptureMouse) {
        const Core::EventType type = event.GetEventType();
        if (type == Core::MouseButtonPressed || type == Core::MouseButtonReleased || type == Core::MouseMoved || type == Core::MouseScrolled) {
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
