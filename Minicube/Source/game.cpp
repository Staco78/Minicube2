#include "game.h"

namespace Minicube
{
    Game::Game() : m_camera(m_renderer.getWindow()), m_renderer(&m_camera, &m_world)
    {
    }

    void Game::start()
    {

        while (m_renderer.getWindow()->isOpen())
        {

            // drawing
            m_camera.update();
            m_renderer.render();
            glfwPollEvents();
        }
    }
}