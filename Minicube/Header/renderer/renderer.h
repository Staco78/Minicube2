#pragma once


#include <Glew/glew.h>

#include "window.h"

#include "shader.h"

#include "camera.h"

namespace Minicube
{
    class Renderer
    {
    public:
        Renderer();
        Window* getWindow();
        void render(Camera* camera);
    private:
        Window m_window;
        Shader m_shader;
    };
}