#pragma once

#include "renderer/renderer.h"
#include "blocks/block.h"
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
    };
}