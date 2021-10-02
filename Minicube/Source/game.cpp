#include "game.h"

namespace Minicube
{
    Game::Game()
    {
    }

    void Game::start()
    {

        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f};

        unsigned int VBO;

        glGenBuffers(1, &VBO);

        unsigned int VAO;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        while (m_renderer.getWindow()->isOpen())
        {
           m_renderer.render();
            glfwPollEvents();
        }
    }
}