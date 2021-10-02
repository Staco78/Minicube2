#pragma once


#include <Glew/glew.h>

#include "window.h"

#include "shader.h"

#include "camera.h"

#include "world/world.h"

#include "text.h"

namespace Minicube
{
    class Renderer
    {
    public:
        Renderer(Camera* camera, World* world);
        Window* getWindow();
        void render();
    private:
        Window m_window;
        Shader m_shader;
        Camera* m_camera;
        World* m_world;
        double frameTime;
        double lastFrame = glfwGetTime();
    };
}