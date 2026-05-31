# Build System and Dependencies

This project uses **CMake**, a meta-build system that generates build files (like Makefiles or Visual Studio Solutions) for your specific platform.

## Dependencies

The project relies on three main external libraries:

1.  **SDL2:** Handles the window, input, and basic 2D rendering.
2.  **SDL2_mixer:** Handles sound effects and music (MIDI).
3.  **ImGui:** Handles the "modern" UI overlays. The source for ImGui is included directly in the `SpaceCadetPinball/` folder for ease of compilation.

## Build Process

### General Steps (Linux/macOS)
```bash
mkdir build
cd build
cmake ..
make
```

### Windows (Visual Studio)
1.  Open the project folder in Visual Studio.
2.  Visual Studio should detect the `CMakeLists.txt` file and automatically configure the project.
3.  You will need to have the `SDL2` and `SDL2_mixer` development libraries downloaded and their paths configured (see `CMakeLists.txt` for details on where it looks for them).

## The `bin` Directory

When the project builds, it typically places the executable in a `bin` directory.
- **Important:** The game requires the original game resources (like `PINBALL.DAT`).
- The executable looks for these files in its current working directory or a path relative to it.
- On Windows, you can just drop the compiled `.exe` into your original Pinball game folder.

## Modifying the Build

If you add a **new `.cpp` file** to the project:
1.  Open `CMakeLists.txt` in the root directory.
2.  Find the `add_executable` section.
3.  Add your new file to the list of source files.
4.  Re-run CMake.

## ImGui Integration

ImGui is integrated into the SDL2 rendering loop.
- The `imgui_impl_sdl.cpp` and `imgui_sdl.cpp` files handle the "glue" between ImGui and our SDL renderer.
- If you want to add a new debug window, the best place to start is `winmain::RenderUi()` in `winmain.cpp`.
