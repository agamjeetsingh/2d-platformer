# 2D Game Engine

A custom **2D game engine** built in **C++20** with **SFML**.  

Features include a physics engine, an extensible event bus, a flexible callback scheduler, and robust player state management.

---

## Example Game GIF

![Demo](assets/demo.gif)

Demo uses sprites and tiles from Celeste (© Maddy Makes Games), used here for non-commercial demonstration purposes only.

Find details about this game on [this](https://github.com/agamjeetsingh/2d-game-engine/wiki/2D-Platformer) wiki page.

## Features
- **Physics Engine**
    - AABB collision detection, impulse-based resolution,
      and positional correction
    - Uses spatial hashing instead of naive pairwise checking for detecting collisions
    - A contacts handler which can emit events such as `PlayerLanded` and to which you can efficiently query current and previous frames' physics states 
- **Thread-safe Event Bus**
    - Ability to emit events and listeners' callbacks being executed by the Event Bus on certain specified safe points in the program.
    - Listener priority system for fine-grained control over game logic
- **Thread-safe Callback Scheduler**
    - Schedule deferred or repeating callbacks
    - Supports cancellation (even from inside callbacks)
- **Sound Manager and Render System**
    - Thread-safe central audio manager that stores sound buffers and plays them on demand
    - Render class to which you can draw a sprite at will or register a `Drawable` to be drawn each frame
- **Particle System**
    - General particle system with arbitrary velocity and colour functions to create any particle effect
    - Implemented some example effects, such as a snow storm
- **Other miscellaneous features**
    - JSON map loader to load any JSON map from the Tiled map editor
    - Reusable thread pool for parallel task execution
    - General system for 2D sprite management

More detailed explanations, diagrams, and usage guides can be found in the [Wiki](https://github.com/agamjeetsingh/2d-game-engine/wiki)
(Work in Progress).

---
## Getting Started

### Prerequisites
- C++20 compiler (GCC 10+, Clang 10+, or MSVC 2019+)
- CMake 3.31+
- GLFW 3.4+ (required; other dependencies are handled automatically)

### Installing GLFW
**macOS (using Homebrew):**
```bash
brew install glfw
```

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install libglfw3-dev
```

**Windows:**
Download GLFW from https://www.glfw.org/download.html
and follow instructions to install it.

⚠️ SFML 3.0.1+, GLM, and nlohmann_json are automatically downloaded and configured via CMake’s FetchContent. No manual installation required.

### Build & Run
```bash
git clone https://github.com/agamjeetsingh/2d-game-engine.git
cd 2d-platformer
mkdir build && cd build
cmake ..
cmake --build .
./2d-platformer
```

---

## Roadmap
### Towards Testing and Dependency Injection
Currently, the project barely has any tests, which is partially attributed to singleton spam. 
Singletons lead to tightly-coupled and hard to test code. Therefore, I will move away from this singleton pattern and 
use dependency injection. Some classes (CollisionsHandler, SoundManager) have already been converted but there is still 
a lot of work to do.

### The Project as a Library
This project started as a Celeste-inspired platformer built from scratch without using existing game engines. As
development progressed, the engine framework became robust enough to warrant extracting it as a reusable library.
This transition requires significant refactoring: decoupling game-specific code from general engine components,
implementing proper namespace organisation, and designing intuitive APIs. The refactoring process is pushing me to
think critically about software architecture and API design, and it also prompted me to make a GitHub Wiki (Work in 
Progress!).

### Graphics, SFML and OpenGL
I recently started working on a project to make my own [Minecraft clone](https://github.com/agamjeetsingh/3d-voxel)
from scratch, learning OpenGL in the process. This sparked a realisation: if I can build a 3D rendering pipeline,
why rely on SFML for 2D? I'm planning to replace SFML with my own OpenGL-based graphics pipeline to gain complete
control over the rendering process and deepen my understanding of graphics programming.

### Platformer Game
These are some general ideas I had for my platformer game that I haven't implemented yet.
- Enemies with AI including natural movement
- Full lighting system with shaders
- Camera that follows the player naturally
- Pausing the game, general input buffering, freeze frames
