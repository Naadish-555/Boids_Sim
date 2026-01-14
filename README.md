# Optimized Multi-Agent Boids Simulation (WIP)

![C++](https://img.shields.io/badge/Language-C++-00599C?style=for-the-badge&logo=c%2B%2B)
![SFML](https://img.shields.io/badge/Graphics-SFML-8A2BE2?style=for-the-badge&logo=sfml)

> A high-performance C++ implementation of Craig Reynolds' Flocking Algorithm, currently optimized with spatial partitioning.

## Overview
This project explores Emergent Behavior and Optimization Algorithms in game development. It implements the classic "Boids" rules (Alignment, Cohesion, Separation) using SFML for rendering.

* **Objective:** To optimize the simulation from a naive $O(N^2)$ approach to a scalable solution capable of handling 10,000+ agents at 60 FPS.

## Key Features
Core Boids Logic: Complete implementation of Alignment, Cohesion, and Separation forces.
Spatial Optimization: Custom Uniform Grid (Spatial Hashing) to reduce neighbor search complexity to approx $O(1)$.
Batch Rendering: Replaced standard OOP rendering with Vertex Arrays to reduce draw calls from $N$ to 1 per frame.
ImGui Integration: Real-time parameter tuning for velocity, grid, and debugging visualization.

## Performance Goals
Target performance metrics for the optimization phase:

| Method | Complexity | Agent Count (Target) | Status |
| :--- | :--- | :--- | :--- |
| **Naive Approach** | O(N^2) | ~300 | Implemented |
| **Grid Optimization** | ~O(N) | 2,500 | Implemented |
| **Grid + Vertex Arrays** | **Optimization** | **10,000+** | **Achieved** |

### Prerequisites
* [CMake](https://cmake.org/download/) (3.16 or newer)
* A C++17 compatible compiler (MSVC, GCC, or Clang)

### 1. Windows (Visual Studio)
1.  Open Visual Studio.
2.  Select **"Open a Local Folder"** and choose this project directory.
3.  Wait for CMake to configure (check the Output window).
4.  **Important:** Switch the build configuration dropdown (top bar) to **Release** or **RelWithDebInfo**.
    * *Note: Debug mode includes safety checks that significantly reduce performance.*
5.  Select `mygame.exe` from the run dropdown menu.
6.  Press **Ctrl + F5** to build and run.

### 2. macOS & Linux (Command Line)

**Prerequisites:**
You do NOT need to install SFML manually (it is included in this project). 
However, you need the system tools (compilers and dependencies) to build it from source.

- Ubuntu/Debian:
  sudo apt update
  sudo apt install cmake build-essential libxrandr-dev libxcursor-dev libudev-dev libopenal-dev libflac-dev libvorbis-dev libgl1-mesa-dev libfreetype6-dev

- Fedora:
  sudo dnf install cmake make gcc-c++ systemd-devel libX11-devel libXrandr-devel libXcursor-devel libXi-devel libXinerama-devel mesa-libGL-devel alsa-lib-devel

- macOS:
  brew install cmake

**Build Steps:**

1. Open a terminal in the project directory.

2. Create a build folder:
   mkdir build && cd build

3. Configure with CMake (Release mode is critical for simulation performance):
   cmake -DCMAKE_BUILD_TYPE=Release ..

4. Build the project:
   make

5. Run the executable:
   ./Boids_Sim

## Credits
* **Template:** SFMLCmakeSetup by [Meemknight](https://github.com/meemknight).
* **Reference:** Reynolds, C. W. (1987). "Flocks, Herds, and Schools: A Distributed Behavioral Model."