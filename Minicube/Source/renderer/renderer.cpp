#include "renderer/renderer.h"

namespace Minicube
{
    Renderer::Renderer(Camera *camera, World *world)
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

        auto projection = glm::perspective(glm::radians(50.0f), m_window.getRatio(), 0.1f, 800.0f);

        m_shader.setMat4("projection", projection);

        m_camera = camera;
        m_world = world;

        Textures::init();

        text::init();
        text::setProjection(glm::ortho(0.0f, float(m_window.getSize().x), 0.0f, float(m_window.getSize().y)));
    }

    void Renderer::render()
    {

        double now = glfwGetTime();
        frameTime = now - lastFrame;
        lastFrame = now;

#ifdef _DEBUG
        glfwSetInputMode(getWindow()->getWindow(), GLFW_STICKY_KEYS, GL_FALSE);

        if (lineModeTimer == 0)
        {
            if (glfwGetKey(getWindow()->getWindow(), GLFW_KEY_F2) == GLFW_PRESS)
            {
                lineMode = !lineMode;
                lineModeTimer = 20;
            }

            // std::cout << 1.0f / frameTime << std::endl;
        }

        if (lineModeTimer > 0)
            lineModeTimer--;

        glfwSetInputMode(getWindow()->getWindow(), GLFW_STICKY_KEYS, GL_TRUE);

        if (lineMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#endif

        if (debugTimer == 0)
        {
            fps = int(1.0f / frameTime);
            debugTimer = 20;
        }

        if (debugTimer > 0)
            debugTimer--;

        //
        //
        // drawing
        //
        //

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader.use();
        m_shader.setMat4("view", m_camera->getMatrix());

        m_world->draw(m_shader);

        text::draw2DText(std::to_string(fps), 10, m_window.getSize().y - 26);
        glm::ivec3 playerPos = m_camera->getPosition();
        text::draw2DText(std::to_string(playerPos.x) + " " + std::to_string(playerPos.y) + " " + std::to_string(playerPos.z), 10, m_window.getSize().y - 50);

        glm::ivec3 playerChunkPos = glm::ivec3(playerPos.x / 16, playerPos.y / 16, playerPos.z / 16);
        text::draw2DText(std::to_string(playerChunkPos.x) + " " + std::to_string(playerChunkPos.y) + " " + std::to_string(playerChunkPos.z), 10, m_window.getSize().y - 74);

        text::draw2DText(std::to_string(m_world->getChunkMap()->size()), 10, m_window.getSize().y - 98);

        WorldGen::HeightMapData *heightMap = m_world->getHeightMapData(glm::ivec2(playerPos.x, playerPos.z));
        if (heightMap != nullptr)
        {
            // for (int i = 0; i < BIOME_COUNT; i++)
            //     text::draw2DText(std::string(WorldGen::biomeNames[heightMap->biomes[i].biome]) + " " + std::to_string(int(heightMap->biomes[i].weight * 100)) + "%", 10, m_window.getSize().y - (122 + i * 24));

            text::draw2DText("h: " + std::to_string(heightMap->humidity) + " t: " + std::to_string(heightMap->temperature) + " m: " + std::to_string(heightMap->moutains), 10, m_window.getSize().y - (122 + BIOME_COUNT * 24));

            text::draw2DText(std::string(WorldGen::biomeNames[heightMap->biome]), 10, m_window.getSize().y - (146 + BIOME_COUNT * 24));
        }

        getWindow()->swapBuffers();
    }

    Window *Renderer::getWindow()
    {
        return &m_window;
    }
}