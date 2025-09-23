# 2D Platformer Engine (C++ / SFML)

A custom **2D physics-based platformer game engine** built in **C++20** with **SFML**.  

Features include a physics engine, an extensible event bus, a flexible callback scheduler, and robust player state management.

---

## Demo GIF

![Demo](assets/demo.gif)

## Features
- **Physics Engine**
    - Swept AABB collision detection, impulse-based resolution,
      and positional correction
    - Uses spatial hashing instead of naive pairwise checking for detecting collisions
    - A contacts handler which emits events such as `PlayerLanded` and to which you can efficiently query current and previous frames' physics states 
    - Supports fake platformer physics nuances
- **Event Bus**
    - Ability to emit events and listeners' callbacks being executed by the Event Bus on certain specified safe points in the program.
    - Listener priority system for fine-grained control over game logic
- **Callback Scheduler**
    - Schedule deferred or repeating callbacks
    - Supports cancellation (even from inside callbacks)
- **Player State Management**
    - Unified way to handle all abilities and their complex states
    - Effortlessly handles features such as Coyote Jumping or Jump Buffering
- **Audio and Render Systems**
    - Central audio manager that stores sound buffers and plays them on demand
    - Render class to which you can draw a sprite at will or register a `Drawable` to be drawn each frame

---

## Controls and Usage

### Basic Controls
- **A/D** - Move left/right
- **Space** - Jump
- **L + WASD** - Dash in 8 directions (up, down, left, right, and diagonals)

### Mechanics
- **Dash cooldown** - Resets when touching ground
- **Coyote jumping** - Brief window to jump mid-air after leaving a platform
- **Jump buffering** - Jump input remembered for a few frames even if you cannot jump immediately

## Design Notes
- Chose spatial hashing for collisions leading to O(n) detection instead of O(n²).
- Event bus runs callbacks at specific **safe points** to avoid inconsistent state.
- Unified player ability system making features like Coyote Jumping trivial to implement.

## Future Plans
- Implement a level parser to create a map from a JSON file
- Saving and loading data, such as custom key-binds
- Enemies with AI including natural movement
- Pausing the game, general input buffering, screen shake, freeze frames
- Other UI elements like a pause menu, particle effects, shaders

## Tech Stack
- **C++20**
- **CMake** (build system)
- **SFML** (rendering, input, window management)

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

Windows:
Download GLFW from https://www.glfw.org/download.html
and follow instructions to install it.

⚠️ SFML 3.0.1+, GLM, and nlohmann_json are automatically downloaded and configured via CMake’s FetchContent. No manual installation required.

### Build & Run
```bash
git clone https://github.com/agamjeetsingh/2d-game-engine.git
cd 2d-platformer
mkdir build && cd build
cmake ..
cmake --build .  # or 'make' if using Makefiles
./2d-platformer
```