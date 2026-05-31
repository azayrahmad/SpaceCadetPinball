# C# to C++ Cheat Sheet (for this codebase)

As a C# developer with 10 years of experience, you'll find the logic familiar, but the "housekeeping" is very different. Here’s what you need to know to start tinkering here.

## 1. Memory Management (The Big One)

**In C#:** The Garbage Collector (GC) handles everything.
**In this Codebase:** Manual management using `new` and `delete`.

- **Pointers (`*`):** Used extensively. If you see `TPinballComponent* comp = new TPinballComponent()`, you **must** call `delete comp` later, or it will leak.
- **Destructors (`~ClassName`):** This is where resources are cleaned up. Look at `TPinballTable::~TPinballTable()` in `TPinballTable.cpp` to see how it deletes its components.
- **`memory.cpp`:** This project has a custom `memory` namespace that wraps `malloc` and `free` for certain allocations, often related to the original game's data structures.

## 2. Objects and Pointers

**In C#:** Classes are always reference types. `MyClass a = new MyClass();` is a reference.
**In this Codebase:**
- `TPinballComponent* comp`: A **pointer** to an object. Access members with `->` (e.g., `comp->Message(1, 0)`).
- `TPinballComponent comp`: An **instance** on the stack. Access members with `.` (e.g., `comp.Message(1, 0)`).
- **Null Checks:** In C#, you check `if (obj != null)`. In C++, you check `if (obj != nullptr)` or simply `if (obj)`.

## 3. The `objlist_class<T>`

You'll see `objlist_class` used everywhere (like `MainTable->ComponentList`).
- Think of this as a simplified `List<T>` from C#.
- `GetCount()` is like `Count`.
- `Get(index)` is like `this[index]`.

## 4. Header Files (`.h`) vs. Source Files (`.cpp`)

**In C#:** Everything is in the `.cs` file.
**In this Codebase:**
- **`.h` (Header):** Declares the "shape" of the class (fields, method signatures).
- **`.cpp` (Source):** Implements the logic.
- **Pro Tip:** If you add a new method to a class, you must add the declaration in the `.h` AND the implementation in the `.cpp`.

## 5. Virtual Functions and Inheritance

- `virtual` in C++ works just like `virtual` or `abstract` in C#.
- `override` is optional in C++11 (but used in this project) to ensure you are actually overriding a base method.
- **Casting:** You'll see `static_cast<T*>` and `dynamic_cast<T*>`.
    - `static_cast` is like `(T)obj` (fast, but unsafe if you're wrong).
    - `dynamic_cast` is like `obj as T` (returns `nullptr` if the type doesn't match).

## 6. Namespaces

- `pb::MainTable` refers to the `MainTable` variable inside the `pb` namespace. This is similar to `SpaceCadetPinball.PB.MainTable` in C#.

## 7. Magic Numbers

Because this is a reverse-engineered project, you will see many magic numbers (like `1024` for messages). Check the header files or the `Message` switch statements to understand what they represent.

## Summary for Tinkering
If you want to change how a bumper behaves:
1.  Find `TBumper.cpp`.
2.  Look at the `Message` function.
3.  Modify the logic (don't forget to recompile!).
4.  If you add fields, add them to `TBumper.h`.
