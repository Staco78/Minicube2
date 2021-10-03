#pragma once

#include <iostream>

#include <Glfw/glfw3.h>

#include "Glm/vec2.hpp"

namespace Minicube
{
    class Window
    {
    public:
        ~Window();
        void open();
        bool isOpen();
        void swapBuffers();
        glm::ivec2 getSize();
        float getRatio();
        GLFWwindow *getWindow()
        {
            return m_window;
        }

    private:
        GLFWwindow *m_window = nullptr;
    };
}