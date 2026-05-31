# Project Overview: Space Cadet Pinball

Welcome to the Space Cadet Pinball codebase! This project is a reverse-engineered port of the classic `3D Pinball for Windows – Space Cadet`.

## What is this?

This is a decompilation and port of the original game's executable (`pinball.exe` from Windows XP and `CADET.EXE` from Full Tilt! Pinball). The original game logic was written in C++, and this project aims to provide a modern, cross-platform version of that logic while staying faithful to the original mechanics.

## Core Technologies

*   **Language:** C++11.
*   **Multimedia:** [SDL2](https://www.libsdl.org/) and [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/) handle window management, input, rendering, and audio.
*   **UI:** [ImGui](https://github.com/ocornut/imgui) is used for the menus, dialogs, and debug tools.
*   **Build System:** CMake.

## High-Level Architecture

The codebase is organized into several key areas:

1.  **Entry Point (`winmain.cpp`):** Handles the main loop, SDL initialization, and ImGui rendering.
2.  **Game Engine (`pb.cpp`):** The "Pinball" (pb) namespace contains the high-level game state management, timing, and collision coordination.
3.  **Table Objects (`TPinballComponent`):** A large hierarchy of classes representing everything on the table (flippers, bumpers, lights, etc.).
4.  **Physics & Math (`maths.cpp`, `TEdgeManager.cpp`):** Handles ball-to-edge collisions and field effects.
5.  **Resources (`loader.cpp`, `partman.cpp`):** Loads game assets (graphics, sounds, table layout) from the original `.dat` files.

## Tinkering Goals

Since you are coming from a C# background, you'll find the game logic is very object-oriented, but uses some patterns typical of mid-90s C++ code (like a message-passing system for component communication).

Check out `CSharpToCpp.md` for a guide on how to translate your C# knowledge to this codebase.
