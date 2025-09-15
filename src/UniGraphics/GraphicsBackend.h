#pragma once

#include <memory>

#include "IGraphicsBackend.h"

class GraphicsBackend : public IGraphicsBackend {
   public:
    GraphicsBackend();
    ~GraphicsBackend() override;

    IWindow*   GetWindow() override { return m_Window.get(); }
    IInput*    GetInput() override { return m_Input.get(); }
    IRenderer* GetRenderer() override { return m_Renderer.get(); }

   protected:
    std::unique_ptr<IWindow>   m_Window;
    std::unique_ptr<IInput>    m_Input;
    std::unique_ptr<IRenderer> m_Renderer;
};