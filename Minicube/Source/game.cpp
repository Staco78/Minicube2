#include "game.h"

namespace Minicube
{
    Game::Game() : m_camera(m_renderer.getWindow()), m_world(&m_camera), m_renderer(&m_camera, &m_world)
    {
    }

    void Game::start()
    {
        m_world.startThreads();

        while (m_renderer.getWindow()->isOpen())
        {
            // drawing
            m_camera.update();
            m_world.updateVisibleChunks();
            m_renderer.render();
            glfwPollEvents();
        }
    }
}