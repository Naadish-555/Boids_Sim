# Optimized Multi-Agent Boids Simulation (WIP)

![C++](https://img.shields.io/badge/Language-C++-00599C?style=for-the-badge&logo=c%2B%2B)
![SFML](https://img.shields.io/badge/Graphics-SFML-8A2BE2?style=for-the-badge&logo=sfml)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

> A high-performance C++ implementation of Craig Reynolds' Flocking Algorithm, currently being optimized with spatial partitioning.

## Overview
This project explores Emergent Behavior and Optimization Algorithms in game development. It implements the classic "Boids" rules (Alignment, Cohesion, Separation) using SFML for rendering.

* **Foundation:** Built upon the SFML-CMake template by Meemknight.
* **Objective:** To optimize the simulation from a naive O(N^2) approach to a near O(N) solution using Uniform Grid Spatial Partitioning.

## Key Features
* **Core Boids Logic:** Complete implementation of Alignment, Cohesion, and Separation forces.
* **SFML Rendering:** Hardware-accelerated 2D rendering.
* **Spatial Optimization:** Custom Grid class to partition space (Currently in development).
* **Multi-threading:** Parallel processing for agent updates (Planned).

## Performance Goals
Target performance metrics for the optimization phase:

| Method | Complexity | Agent Count (Target) | Status |
| :--- | :--- | :--- | :--- |
| **Naive Approach** | O(N^2) | ~500 | Implemented |
| **Grid Optimization** | ~O(N) | 2,000+ | Work in Progress |

## Build Instructions
This project uses CMake and is designed for Visual Studio.

1.  Open Visual Studio.
2.  Select "Open a Local Folder" and choose this project directory.
3.  Wait for CMake to configure (check the Output window).
4.  Select `mygame.exe` from the run dropdown menu at the top.
5.  Press **Ctrl + F5** to build and run.

## Roadmap
This project serves as the foundation for my Master's research proposal:

* [x] Basic Flocking Rules
* [ ] Implement Grid Spatial Partitioning
* [ ] Benchmarking and Data Collection
* [ ] Application: Procedural Level Generation

## Credits
* **Template:** SFMLCmakeSetup by [Meemknight](https://github.com/meemknight).
* **Reference:** Reynolds, C. W. (1987). "Flocks, Herds, and Schools: A Distributed Behavioral Model."