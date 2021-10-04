#include "renderer/window.h"

namespace Minicube
{

    Window::~Window()
    {
        m_window = nullptr;
        std::cout << "Window released" << std::endl;
        glfwTerminate();
    }

    bool Window::isOpen()
    {
        return !glfwWindowShouldClose(m_window);
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(m_window);
    }

    void Window::open()
    {
        m_window = glfwCreateWindow(1080, 720, "Minicube", nullptr, nullptr);

        if (!m_window)
        {
            glfwTerminate();
            std::cout << "Failed to open the window" << std::endl;
            return;
        }

        glfwMakeContextCurrent(m_window);
        // glfwSetFramebufferSizeCallback(m_window, callbackFrameBufferSize);
        // glViewport(0, 0, getWindowSize().x, getWindowSize().y);

        std::cout << "Window opened successfuly" << std::endl;
    }

    glm::ivec2 Window::getSize()
    {
        int x, y;
        glfwGetWindowSize(m_window, &x, &y);
        return glm::vec2(x, y);
    }

    float Window::getRatio()
    {
        int x, y;
        glfwGetWindowSize(m_window, &x, &y);
        return float(x) / float(y);
    }
}