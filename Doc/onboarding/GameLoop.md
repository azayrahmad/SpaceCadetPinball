# The Game Loop & Physics

Understanding how the game progresses from one millisecond to the next is crucial for tinkering with physics or animations.

## The High-Level Loop

The loop lives in `winmain.cpp` inside the `WinMain` function (or the `run_loop` lambda).

1.  **Input:** `ProcessWindowMessages()` collects keyboard/mouse events and sends them to `pb::keydown` or `pb::keyup`.
2.  **Logic Update (`pb::frame`):**
    - Calculates how much time has passed (`dtMilliSec`).
    - Calls `pb::timed_frame`.
    - Updates timers and the UI.
3.  **Physics Update (`pb::timed_frame`):**
    - This is the "crunchy" part.
    - It applies gravity and "field effects" to the balls.
    - It calls `pb::collide` to see if the ball hit anything.
4.  **Rendering:**
    - `render::update()` and `render::PresentVScreen()` update the game's internal bitmap.
    - `RenderUi()` draws the ImGui layer.
    - `SDL_RenderPresent()` pushes the final image to your monitor.

## Collision Detection

Collision isn't done with a modern physics engine like Box2D or Unity's PhysX. Instead, it uses a custom ray-casting approach:

1.  **Ray Casting:** For each ball, the game projects a ray from its current position along its velocity vector.
2.  **Edge Manager:** The `TEdgeManager` checks this ray against all "edges" (lines and circles) on the table.
3.  **Collision Response:** If a collision is found:
    - The ball's position is moved to the point of impact.
    - The `EdgeCollision` method of the hit object is called.
    - The object handles the response (e.g., a flipper imparts force, a bumper bounces the ball back).

## Timing

The game is designed to run at a specific "ticks per second" rate (defaulting to 100 UPS - Updates Per Second).
- `pb::time_now` tracks the absolute game time in seconds.
- `dtMicroSec` (delta time) is used to ensure movement is smooth regardless of your monitor's refresh rate (though the physics is still stepped).

## Tinkering with Physics

If you want to change the gravity or ball behavior:
- **Gravity:** Look in `TPinballTable::TPinballTable` (in `TPinballTable.cpp`) where `GravityAngleX` and `GravityAnglY` are initialized.
- **Ball Speed:** Look at `pb::ball_speed_limit` in `pb.cpp`.
- **Flipper Strength:** Check `TFlipper.cpp`.
