cmake_minimum_required(VERSION 4.0)
project(GravSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add GLFW submodule
add_subdirectory(libraries/glfw)

# Add executable
add_executable(GravSim
        src/main.cpp
        libraries/imgui/imgui.cpp
        libraries/imgui/imgui_draw.cpp
        libraries/imgui/imgui_widgets.cpp
        libraries/imgui/imgui_tables.cpp
        libraries/imgui/backends/imgui_impl_glfw.cpp
        libraries/imgui/backends/imgui_impl_opengl3.cpp
)

# Include directories
target_include_directories(GravSim PRIVATE
        src
        libraries/glfw/include
        libraries/imgui
        libraries/imgui/backends
)

# Link libraries
if(WIN32)
    target_link_libraries(GravSim PRIVATE glfw opengl32)
elseif(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(GravSim PRIVATE glfw ${OpenGL_LIBRARY} "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
else()
    target_link_libraries(GravSim PRIVATE glfw GL)
endif()