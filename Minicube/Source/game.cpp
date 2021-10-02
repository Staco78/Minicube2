#include "game.h"

namespace Minicube
{
    Game::Game() : m_camera(m_renderer.getWindow()), m_renderer(&m_camera, &m_world)
    {
        text::init();
        text::setProjection(glm::ortho(0.0f, 1080.0f, 0.0f, 720.0f));
    }

    void Game::start()
    {

        int debugTimer = 0;
        bool lineMode = false;

        double frameTime;
        double lastFrame = glfwGetTime();

        while (m_renderer.getWindow()->isOpen())
        {

            //debug
            glfwSetInputMode(m_renderer.getWindow()->getWindow(), GLFW_STICKY_KEYS, GL_FALSE);

            if (debugTimer == 0)
            {
                if (glfwGetKey(m_renderer.getWindow()->getWindow(), GLFW_KEY_F2) == GLFW_PRESS)
                {
                    lineMode = !lineMode;
                    debugTimer = 20;
                }
            }

            if (debugTimer > 0)
                debugTimer--;

            glfwSetInputMode(m_renderer.getWindow()->getWindow(), GLFW_STICKY_KEYS, GL_TRUE);

            if (lineMode)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            // drawing
            m_camera.update();
            m_renderer.render();
            glfwPollEvents();
        }
    }
}