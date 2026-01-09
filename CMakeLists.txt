cmake_minimum_required(VERSION 3.16)
project(GravSim)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT GravSim)

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
        libraries/implot/implot.h
        libraries/implot/implot_items.cpp
        libraries/implot/implot.cpp
        libraries/implot/implot_internal.h
        libraries/miniaudio/miniaudio.c
        src/core/Application.cpp
        src/core/Window.cpp
        src/App/Layers/UILayer.cpp
        src/App/Layers/AudioLayer.cpp
        src/App/Layers/EngineLayer.cpp
        src/App/UI/SettingsUI.cpp
        src/App/Rendering/RenderingSystem.cpp
        src/App/Rendering/RenderingSystem.h
        src/App/Engine/Scene.cpp
        src/App/Engine/Scene.h
        src/App/Engine/SceneObject.cpp
        src/App/Engine/SceneObject.h
        src/App/Engine/Physics/Colliders/ColliderBase.h
        src/App/Engine/Loading/ShaderLoader.cpp
        src/App/Engine/Loading/ShaderLoader.h
        src/App/Engine/Camera.cpp
        src/App/Engine/Camera.h
        src/App/Rendering/Visuals/CircleVisual.cpp
        src/App/Rendering/Visuals/CircleVisual.h
        src/App/Engine/Transform.h
        src/App/UI/SceneUI.cpp
        src/App/UI/SceneUI.h
        src/App/Engine/EngineEvents.h
        src/App/Engine/CameraController.cpp
        src/App/Engine/CameraController.h
        src/App/Engine/Loading/Serialiser.cpp
        src/App/Engine/Loading/Serialiser.h
        src/App/UI/FileSelector.cpp
        src/App/UI/FileSelector.h
        src/App/Rendering/Visuals/SpriteVisual.cpp
        src/App/Rendering/Visuals/SpriteVisual.h
        src/App/Engine/Loading/TextureLoader.cpp
        src/App/Engine/Loading/TextureLoader.h
        src/App/UI/CustomImGuiStyle.h
        src/App/UI/SimulationUI.cpp
        src/App/UI/SimulationUI.h
        src/App/Engine/Physics/PhysicsSolver.cpp
        src/App/Engine/Physics/PhysicsSolver.h
        src/App/Engine/Physics/Propagators/IPropagator.h
        src/App/Engine/Physics/Propagators/EulerPropagator.cpp
        src/App/Engine/Physics/Propagators/EulerPropagator.h
        src/App/Engine/Physics/Propagators/SemiImplicitEulerPropagator.cpp
        src/App/Engine/Physics/Propagators/SemiImplicitEulerPropagator.h
        src/App/Engine/Physics/Propagators/VerletPropagator.cpp
        src/App/Engine/Physics/Propagators/VerletPropagator.h
        src/App/Engine/Physics/Propagators/RK4Propagator.cpp
        src/App/Engine/Physics/Propagators/RK4Propagator.h
        src/App/Engine/Physics/Propagators/VelocityVerletPropagator.cpp
        src/App/Engine/Physics/Propagators/VelocityVerletPropagator.h
        src/App/UI/FollowingUI.cpp
        src/App/UI/FollowingUI.h
        src/App/Rendering/Renderers/GridRenderer.cpp
        src/App/Rendering/Renderers/GridRenderer.h
        src/App/Engine/Physics/Constraint.cpp
        src/App/Engine/Physics/Constraint.h
        src/App/Engine/Physics/Constraint.h
        src/App/Engine/Physics/Colliders/CircleCollider.cpp
        src/App/Engine/Physics/Colliders/CircleCollider.h
        src/App/Engine/Physics/Colliders/ColliderBase.cpp
        src/App/Rendering/Renderers/SceneRenderer.cpp
        src/App/Rendering/Renderers/SceneRenderer.h
        src/App/Rendering/Renderers/SpriteRenderer.cpp
        src/App/Rendering/Renderers/SpriteRenderer.h
        src/App/Rendering/Renderers/CircleRenderer.cpp
        src/App/Rendering/Renderers/CircleRenderer.h
        src/App/Rendering/Renderers/ConstraintRenderer.cpp
        src/App/Rendering/Renderers/ConstraintRenderer.h
        src/App/Rendering/Renderers/LineRenderer.cpp
        src/App/Rendering/Renderers/LineRenderer.h
        src/App/Rendering/Renderers/BaseRenderer.h
        src/App/Rendering/Renderers/PostProcess/PostProcessPass.h
        src/App/UI/InteractionUI.cpp
        src/App/UI/InteractionUI.h
        src/App/UI/InteractionState.h
        src/App/Engine/Physics/ContactPoint.h
        src/App/Engine/Physics/ContactSolver.cpp
        src/App/Engine/Physics/ContactSolver.h
        src/App/Rendering/Renderers/ShaderRenderer.cpp
        src/App/Rendering/Renderers/ShaderRenderer.h
        src/App/Rendering/Visuals/ShaderVisual.h
        src/App/Rendering/Visuals/ShaderVisual.cpp
        src/App/Engine/Loading/CerealHelpers.h
        src/App/RocketControl/RocketObject.cpp
        src/App/RocketControl/RocketObject.h
        src/App/RocketControl/ControlLayer.cpp
        src/App/RocketControl/ControlLayer.h
        src/App/RocketControl/UI/RocketStateUI.cpp
        src/App/RocketControl/UI/RocketStateUI.h
        src/App/RocketControl/Controllers/ManualRocketController.cpp
        src/App/RocketControl/Controllers/ManualRocketController.h
        src/App/RocketControl/PID/PIDController.cpp
        src/App/RocketControl/PID/PIDController.h
        src/App/RocketControl/UI/RocketControllerUI.cpp
        src/App/RocketControl/UI/RocketControllerUI.h
        src/App/RocketControl/Controllers/AutonomousPIDRocketController.cpp
        src/App/RocketControl/Controllers/AutonomousPIDRocketController.h
        src/App/RocketControl/PID/PIDData.h
        src/App/Rendering/Renderers/ArrowRenderer.cpp
        src/App/Rendering/Renderers/ArrowRenderer.h
        src/App/RocketControl/TargetObject.cpp
        src/App/RocketControl/TargetObject.h
        src/App/RocketControl/PID/PIDVisualizer.cpp
        src/App/RocketControl/PID/PIDVisualizer.h
        src/App/RocketControl/Controllers/IRocketController.h
)

# Include directories
target_include_directories(GravSim PRIVATE
        src
        libraries/glfw/include
        libraries/imgui
        libraries/imgui/backends
        libraries/implot
        libraries/implot/backends
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