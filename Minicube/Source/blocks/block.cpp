#include "blocks/block.h"

static float cube[] = {
    // Back face
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // bottom-right
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // top-left
    // Front face
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // bottom-right
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-right
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-right
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // top-left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top-right
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-left
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top-right
                                     // Right face
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-left
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-left
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // bottom-left
    // Bottom face
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // top-left
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
    // Top face
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-right
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f   // bottom-left
};

namespace Minicube
{
    Block::Block(const glm::vec3 &pos)
    {

        unsigned int VBO;

        glGenBuffers(1, &VBO);

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

        m_model = glm::translate(m_model, pos);
    }

    void Block::draw(const Shader &shader)
    {
        shader.setMat4("model", m_model);
        glBindVertexArray(m_VAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
} // namespace Minicube
