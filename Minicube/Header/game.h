#pragma once

#include "renderer/renderer.h"
#include "world/world.h"
#include "camera.h"

namespace Minicube
{
    class Game
    {
    public:
        Game();
        void start();
    private:
        Renderer m_renderer;
        Camera m_camera;
        World m_world;
    };
}