cmake_minimum_required(VERSION 4.0)
project(GravSim)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libraries/glfw)
target_link_libraries(MyProject glfw)

add_executable(GravSim main.cpp)