#include "camera.h"

namespace Minicube
{

    Camera::Camera(Window *window)
    {
        mp_window = window;
    }

    glm::mat4 Camera::getMatrix()
    {
        return glm::lookAt(pos, pos - front, up);
    }

    glm::vec3 Camera::getPosition()
    {
        return pos;
    }

    glm::vec3 Camera::getFront()
    {
        return front;
    }

    void Camera::update()
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - Camera::lastFrame;
        Camera::lastFrame = currentFrame;

        double xpos, ypos;
        glfwGetCursorPos(mp_window->getWindow(), &xpos, &ypos);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 4.5f * deltaTime;
        xoffset *= sensitivity;
        yoffset *= -sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);

        glm::vec3 d;
        d.x = cos(glm::radians(yaw));
        d.y = 0;
        d.z = sin(glm::radians(yaw));
        glm::vec3 dir = glm::normalize(d);

        float speed = 150 * deltaTime;
        if (glfwGetKey(mp_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
            pos -= speed * dir;
        if (glfwGetKey(mp_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
            pos += speed * dir;
        if (glfwGetKey(mp_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
            pos += glm::normalize(glm::cross(dir, up)) * speed;
        if (glfwGetKey(mp_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
            pos -= glm::normalize(glm::cross(dir, up)) * speed;
        if (glfwGetKey(mp_window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
            pos.y += speed;
        if (glfwGetKey(mp_window->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            pos.y -= speed;
    }
}