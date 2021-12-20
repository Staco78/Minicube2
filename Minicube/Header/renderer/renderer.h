#pragma once

#include <Glew/glew.h>

#include "window.h"
#include "shader.h"
#include "camera.h"
#include "world/world.h"
#include "text.h"
#include "renderer/textures.h"

namespace Minicube
{
    class Renderer
    {
    public:
        Renderer(Camera *camera, World *world);
        Window *getWindow();
        void render();

    private:
        Window m_window;
        Shader m_shader;
        Camera *m_camera;
        World *m_world;

        double frameTime;
        double lastFrame = glfwGetTime();
        int debugTimer = 0;
        int fps = 0;
#ifdef _DEBUG
        int lineModeTimer = 0;
        bool lineMode = true;
#endif
    };
}