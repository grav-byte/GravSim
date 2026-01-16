# GravSim  <img src="resources/app_icon.png" width="50" style="margin-left: 5px;margin-bottom: -10px"/>
___
This is a university project written in C++ using GLFW, OpenGL and ImGui.

It features a 2D physics simulation engine with the following features:
- Rigid body dynamics
- Collisions
- Friction
- Constraints
- Gravity

Scenes can be built and simulated in real-time using an ImGui based editor. Cereal is used for saving and loading them.

A screenshot of the editor can be seen below:
![Screenshot](docs/Screenshot.png)
___
Build on top of this engine a simple autonomous rocket simulation using PID control.

An overview of the PID controllers can be seen below:
![Overview](docs/PIDOverview.png)

___
## Building
To build the project, you can use CMake

or Visual Studio 2022 by running the following command in the root folder:

```bash
cmake -S . -B build_vs -G "Visual Studio 17 2022"
```
Then open the generated solution file in the `build_vs` folder.

