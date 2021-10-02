#include "game.h"

namespace Minicube
{
    Game::Game() : m_camera(m_renderer.getWindow())
    {
    }

    void Game::start()
    {

        Block block;

        while (m_renderer.getWindow()->isOpen())
        {
            m_camera.update();
            m_renderer.render(&m_camera);
            glfwPollEvents();
        }
    }
}