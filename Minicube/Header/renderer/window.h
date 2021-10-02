#pragma once

#include <iostream>

#include <Glfw/glfw3.h>

namespace Minicube
{
    class Window
    {
    public:
        ~Window();
        void open();
        bool isOpen();
        void swapBuffers();
        GLFWwindow *getWindow()
        {
            return m_window;
        }

    private:
        GLFWwindow *m_window = nullptr;
    };
}