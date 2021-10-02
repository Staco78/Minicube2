#include "renderer/renderer.h"

namespace Minicube
{
    Renderer::Renderer(Camera* camera, World* world)
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

        auto projection = glm::perspective(glm::radians(45.0f), 1080.0f / 720.0f, 0.1f, 300.0f);

        m_shader.setMat4("projection", projection);


        m_camera = camera;
        m_world = world;
    }

    void Renderer::render()
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader.setMat4("view", m_camera->getMatrix());

        m_world->draw(m_shader);

        getWindow()->swapBuffers();
    }

    Window *Renderer::getWindow()
    {
        return &m_window;
    }
}