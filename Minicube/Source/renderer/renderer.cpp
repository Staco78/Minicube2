#include "renderer/renderer.h"

namespace Minicube
{
    Renderer::Renderer()
    {
        if (!glfwInit())
        {
            std::cout << "Failed to initialize glfw" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        m_window.open();

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            glfwTerminate();
            std::cout << "Failed to initialize glew" << std::endl;
            return;
        }

        std::cout << "Renderer initialized successfuly" << std::endl;

        glfwSwapInterval(1);
        glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(m_window.getWindow(), GLFW_STICKY_KEYS, GL_TRUE);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_CULL_FACE);

        m_shader.load("shaders/shader.vert", "shaders/shader.frag");

        m_shader.use();
    }

    void Renderer::render(Camera *camera)
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto projection = glm::perspective(glm::radians(45.0f), 1080.0f / 720.0f, 0.1f, 100.0f);

        m_shader.setMat4("projection", projection);
        m_shader.setMat4("model", glm::mat4(1.0f));
        m_shader.setMat4("view", camera->getMatrix());

        glDrawArrays(GL_TRIANGLES, 0, 36);

        getWindow()->swapBuffers();
    }

    Window *Renderer::getWindow()
    {
        return &m_window;
    }
}