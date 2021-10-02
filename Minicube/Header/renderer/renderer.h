#pragma once


#include <Glew/glew.h>

#include "window.h"

namespace Minicube
{
    class Renderer
    {
    public:
        Renderer();
        Window* getWindow();
    private:
        Window m_window;
    };
}