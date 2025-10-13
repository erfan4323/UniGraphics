# UniGraphics

An abstraction layer that unifies multiple graphics libraries.

## Overview

UniGraphics provides a unified interface for handling graphics windows, input, and rendering, abstracting over multiple graphics backends such as SDL and Raylib. This allows you to write graphics code independent of the specific library used for backend rendering or window/input handling.

**Main features:**
- Cross-platform graphics backend abstraction (e.g., SDL, Raylib)
- Unified API for window management, input, and rendering
- Easily switch between different graphics backends

If you enjoyed this project, consider giving it a star — it really helps! ⭐

## Prerequisites

- MinGw gcc & g++ (for building the builder and the project)
- [SDL2](https://www.libsdl.org/) and/or [Raylib](https://www.raylib.com/) development libraries installed
- (Optional) Python 3.x for running Python Library wrapper

## Building the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/erfan4323/UniGraphics.git
   cd UniGraphics
   ```

2. Build the nob.c to have your builder/runner tool:
   ```bash
   gcc -o nob nob.c
   ```

   - Make sure SDL2 and/or Raylib development headers and libraries are available on your system.

3. Now run the nob executable:
   ```bash
   ./nob
   ```
   - Every time you make a change in the codebase, just run this command — made possible by [Nob.h](https://github.com/tsoding/nob.h)

## How to Use the Library

To use UniGraphics in your project:

1. Include the main header:
   ```cpp
   #include "UniGraphics/UniGraphics.h"
   ```

2. Create a graphics backend:
   ```cpp
   std::unique_ptr<ugfx::IGraphicsBackend> backend = ugfx::CreateBackend(ugfx::BackendType::SDL);
   ```

   - Supported backend types may include `SDL`, `Raylib`, etc.

3. Use the unified interfaces for window, input, and rendering:
   ```cpp
   ugfx::IWindow* window = backend->GetWindow();
   ugfx::IInput* input = backend->GetInput();
   ugfx::IRenderer* renderer = backend->GetRenderer();
   ```

## Simple Example

Below is a minimal example using the SDL backend:

```cpp
#include "UniGraphics/UniGraphics.h"

int main() {
    // Create a backend (change BackendType to Raylib to switch)
    auto backend = ugfx::CreateBackend(ugfx::BackendType::SDL);
    auto* window = backend->GetWindow();
    auto* input = backend->GetInput();
    auto* renderer = backend->GetRenderer();

    window->Create("UniGraphics Showcase", 800, 600, ugfx::WindowFlags::Resizable);

    ugfx::Vector2 rectPos{400, 300};
    const float speed = 5.0f;

    while (!window->ShouldClose()) {
        window->PollEvents();

        // Move rectangle with arrow keys
        if (input->IsKeyDown(ugfx::Key::right)) rectPos.x += speed;
        if (input->IsKeyDown(ugfx::Key::left))  rectPos.x -= speed;
        if (input->IsKeyDown(ugfx::Key::down))  rectPos.y += speed;
        if (input->IsKeyDown(ugfx::Key::up))    rectPos.y -= speed;

        renderer->BeginDrawing();
        renderer->Clear({30, 30, 40, 255}); // dark background

        // Draw a moving rectangle
        renderer->DrawRectangle({rectPos.x, rectPos.y, 80, 60}, {0, 200, 180, 255});

        // Draw text
        renderer->DrawText("Use arrow keys to move the box. Press ESC to quit.",
                          {20, 20}, 20, {255, 255, 255, 255});

        renderer->EndDrawing();

        if (input->IsKeyPressed(ugfx::Key::escape))
            break;
    }

    window->Shutdown();
    return 0;
}
```

## API Guide

### IGraphicsBackend

```cpp
class IGraphicsBackend {
 public:
    virtual ~IGraphicsBackend() = default;
    virtual IWindow* GetWindow() = 0;
    virtual IInput* GetInput() = 0;
    virtual IRenderer* GetRenderer() = 0;
    virtual BackendType GetBackendType() const = 0;
};
```

### IWindow

```cpp
class IWindow {
 public:
    virtual ~IWindow() = default;
    virtual bool Create(const std::string& title, int width, int height, WindowFlags flags) = 0;
    virtual void SetTitle(const std::string& title) = 0;
    virtual std::pair<int, int> GetSize() const = 0;
    virtual bool  ShouldClose() const = 0;
    virtual void PollEvents() = 0;
    virtual void Shutdown() = 0;
    virtual void SetTargetFPS(int fps) = 0;
    virtual float GetDeltaTime() const = 0;
    virtual uint32_t GetTicks() const = 0;
    virtual void* GetHandle() const = 0;
};
```

### IInput

```cpp
class IInput {
 public:
    virtual ~IInput() = default;
    using EventCallback = std::function<void(void* event)>;
    virtual void ProcessEvents(void* event) = 0;
    virtual void BeginFrame() = 0;
    virtual void RegisterEventCallback(EventCallback callback) = 0;
    virtual bool IsKeyPressed(Key key) const = 0;
    virtual bool IsKeyDown(Key key) const = 0;
    virtual bool IsKeyReleased(Key key) const = 0;
    virtual bool IsKeyUp(Key key) const = 0;
    virtual void* GetHandle() const = 0;
};
```

### IRenderer

```cpp
    class IRenderer : public IShapeRenderer, public IImageRenderer, public ITextRenderer {
       public:
        virtual ~IRenderer() = default;

        virtual void BeginDrawing() = 0;
        virtual void EndDrawing() = 0;
        virtual void Clear(Color color) = 0;
        virtual void ReleaseAllResources() = 0;
        virtual void* GetHandle() const = 0;
    };
```
*(Refer to `src/UniGraphics/IRenderer.h` for the full set of renderer methods.)*


## More Information

- For backend-specific details, see the files in [`src/UniGraphics/backends/`](https://github.com/erfan4323/UniGraphics/tree/main/src/UniGraphics/backends).
- For advanced usage, explore the interfaces in [`src/UniGraphics/`](https://github.com/erfan4323/UniGraphics/tree/main/src/UniGraphics).

## Limitations
I’ve developed this library as part of my game engine, but it’s still a work in progress. My goal is to eventually include full functionality.
Currently, it only compiles on Windows due to MinGW, and doesn’t yet support macOS or Linux. I plan to add cross-platform support soon.
For now, the project isn’t ready for contributions, but any help with making it cross-platform compatible would be appreciated.

## TODO
- [ ] Adding More Backends
   - - [ ] OpenGl  
   - - [ ] Software Renderer  
   - - [ ] Sokol  
   - - [ ] SFML
- [ ] Better Support and compatibility for fonts.
- [ ] Converting MinGw to Clang for enabling this project too run in other platforms.
- [ ] Closer functionality in all backends.