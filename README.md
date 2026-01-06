# GravSim
___
This is a university project written in C++ using GLFW, OpenGL and ImGui.

It features a 2D physics simulation engine with the following features:
- Rigid body dynamics
- Collisions
- Friction
- Constraints
- Gravity

Scenes can be built and simulated in real-time using an ImGui based editor. Cereal is used for saving and loading them.

![Screenshot](docs/Screenshot.png)
___
Build on top of this engine a simple autonomous rocket simulation using PID control.

An overview of the PID controllers can be seen below:
![Overview](docs/PIDOverview.png)

___
## Building
To build the project, you can use CMake.
