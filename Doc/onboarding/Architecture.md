# Architecture Map

The Space Cadet Pinball codebase follows a structure largely dictated by its origin as a 1990s C++ application. Understanding the flow of data and the "Message" system is key to tinkering.

## Key Components

### 1. The Main Loop (`winmain.cpp`)
This is the heart of the application.
- **Initialization:** Sets up SDL2, ImGui, and the sound engine.
- **`run_loop`:** Executes the game frame.
- **`ProcessWindowMessages`:** Polls SDL events (keyboard, mouse, window resize) and passes them to the game engine.
- **`RenderUi`:** Draws the ImGui menus and dialogs.

### 2. The Game Engine (`pb` namespace)
Found in `pb.cpp` and `pb.h`, this is the "manager" of the pinball game.
- **`pb::init` / `pb::uninit`:** Lifecycle of the game session.
- **`pb::frame`:** Calculates the time delta and updates the game state.
- **`pb::timed_frame`:** The core physics update. It iterates through all active balls and handles collisions.
- **`pb::collide`:** Coordinates collision detection using the `TEdgeManager`.

### 3. The Table (`TPinballTable`)
The `TPinballTable` class (derived from `TPinballComponent`) owns everything on the table.
- **`ComponentList`:** A list of every flipper, bumper, light, and mission target.
- **`BallList`:** Tracks the balls in play.
- **Score Management:** Tracks player scores, ball counts, and extra balls.

### 4. Table Components (`TPinballComponent` and subclasses)
Almost everything you see on the screen is a `TPinballComponent`.
- **Hierarchical:** Classes like `TBumper`, `TFlipper`, and `TLight` all inherit from `TPinballComponent`.
- **Data-Driven:** Most components are initialized from data loaded by `loader.cpp`.

## The "Message" System

Instead of a modern event bus, the game uses a `Message` system. Every `TPinballComponent` has a virtual function:
```cpp
virtual int Message(int code, float value);
```

- **Communication:** Components talk to each other by calling `Message`. For example, when a ball hits a bumper, the bumper might send a message to a light to turn on, or to the table to add score.
- **Lifecycle:** Standard codes (found in `TPinballComponent.h` or as magic numbers in the code) are used for:
    - `1024`: Reset component.
    - `1000-1005`: Flipper and Plunger control.
    - `1011`: Table Tilt.
    - `1014`: New Game / Setup.

## Data Loading (`loader.cpp` and `partman.cpp`)
The game does not hardcode the table layout. Instead, it reads a `.dat` file.
- **`partman.cpp`:** Parses the record-based file format of the original game.
- **`loader.cpp`:** Acts as a factory that takes data records and turns them into C++ objects (like `TLight` or `TBall`).

## Rendering (`render.cpp` and `gdrv.cpp`)
- **`render.cpp`:** Manages the "vscreen" (virtual screen), which is a 8-bit bitmap representing the table.
- **`gdrv.cpp`:** Handles the low-level drawing of bitmaps and palette management.
- **ImGui:** The modern UI is layered on top of the SDL renderer in `winmain.cpp`.
