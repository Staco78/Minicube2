#pragma once


#include <Glew/glew.h>

#include "window.h"

#include "shader.h"

namespace Minicube
{
    class Renderer
    {
    public:
        Renderer();
        Window* getWindow();
        void render();
    private:
        Window m_window;
        Shader m_shader;
    };
}