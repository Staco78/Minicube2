#pragma once

#include "renderer/renderer.h"

namespace Minicube
{
    class Game
    {
    public:
        Game();
        void start();
    private:
        Renderer m_renderer;
    };
}