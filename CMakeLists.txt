cmake_minimum_required(VERSION 3.16)
project(GravSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Silence OpenGL deprecation warnings on macOS
if(APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

# Add GLFW submodule
add_subdirectory(libraries/glfw)

# Add GLEW STATIC lib
add_library(glew STATIC
        libraries/glew/src/glew.c
)
target_include_directories(glew PUBLIC
        ${CMAKE_SOURCE_DIR}/libraries/glew/include
)
target_compile_definitions(glew PUBLIC GLEW_STATIC)

# Add executable
add_executable(GravSim
        src/App/main.cpp
        libraries/imgui/imgui.cpp
        libraries/imgui/imgui_draw.cpp
        libraries/imgui/imgui_widgets.cpp
        libraries/imgui/imgui_tables.cpp
        libraries/imgui/imgui_demo.cpp
        libraries/imgui/backends/imgui_impl_glfw.cpp
        libraries/imgui/backends/imgui_impl_opengl3.cpp
        libraries/imgui/misc/cpp/imgui_stdlib.cpp
        libraries/miniaudio/miniaudio.c
        libraries/tinyfiledialogs/tinyfiledialogs.c
        src/core/Application.cpp
        src/core/Window.cpp
        src/App/Layers/UILayer.cpp
        src/App/Layers/AudioLayer.cpp
        src/App/Layers/EngineLayer.cpp
        src/App/UI/SettingsUI.cpp
        src/App/Rendering/RenderingSystem.cpp
        src/App/Engine/Scene.cpp
        src/App/Engine/Scene.h
        src/App/Engine/SceneObject.cpp
        src/App/Engine/SceneObject.h
        src/App/Engine/ICollider.h
        src/App/Rendering/ShaderLoader.cpp
        src/App/Rendering/ShaderLoader.h
        src/App/Engine/Camera.cpp
        src/App/Engine/Camera.h
        src/App/Rendering/Renderers/CircleRenderer.cpp
        src/App/Rendering/Renderers/CircleRenderer.h
        src/App/Engine/Transform.h
        src/App/UI/SceneUI.cpp
        src/App/UI/SceneUI.h
        src/App/Engine/EngineEvents.h
        src/App/Engine/CameraController.cpp
        src/App/Engine/CameraController.h
        src/App/Engine/SceneLoader.cpp
        src/App/Engine/SceneLoader.h
        src/App/UI/FileSelector.cpp
        src/App/UI/FileSelector.h
        src/App/Rendering/Renderers/SpriteRenderer.cpp
        src/App/Rendering/Renderers/SpriteRenderer.h
        src/App/Rendering/TextureLoader.cpp
        src/App/Rendering/TextureLoader.h
        src/App/UI/CustomImGuiStyle.h
        src/App/UI/SimulationUI.cpp
        src/App/UI/SimulationUI.h
        src/App/Engine/Physics/PhysicsSolver.cpp
        src/App/Engine/Physics/PhysicsSolver.h
        src/App/Engine/Physics/IPropagator.h
)

# Include directories
target_include_directories(GravSim PRIVATE
        src
        libraries/glfw/include
        libraries/imgui
        libraries/imgui/backends
        libraries/cereal/include
        libraries/tinyfiledialogs
        libraries/glm
        libraries/miniaudio
        libraries/glew/include
        ${CMAKE_SOURCE_DIR}/libraries/stb
)

if(WIN32)
    target_link_libraries(GravSim PRIVATE glfw glew opengl32)
elseif(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(GravSim PRIVATE glfw glew ${OpenGL_LIBRARY} "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
else()
    target_link_libraries(GravSim PRIVATE glfw glew GL)
endif()