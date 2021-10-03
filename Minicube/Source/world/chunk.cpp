#include "world/chunk.h"

namespace Minicube
{
    Chunk::Chunk(glm::ivec2 pos)
    {
        m_pos = pos;

        // m_blocks[glm::uvec3(0, 0, 0)] = new Block(glm::uvec3(0, 0, 0));
        // m_blocks[glm::uvec3(0, 0, 1)] = new Block(glm::uvec3(0, 0, 1));

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO.getID());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        m_model = glm::translate(m_model, glm::vec3(pos.x * 16, 0, pos.y * 16));

        for (int x = 0; x < 16; x++)
            for (int y = 0; y < 16; y++)
                addBlock(glm::uvec3(x, 0, y));

        constructVBO();
    }

    void Chunk::draw(const Shader &shader)
    {
        shader.use();
        shader.setMat4("model", m_model);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_VBO.getTrianglesCount());
    }

    void Chunk::addBlock(glm::uvec3 relativeBlockPos)
    {
        m_blocks[relativeBlockPos] = new Block();
    }

    void Chunk::constructVBO()
    {
        for (auto &[pos, block] : m_blocks)
        {
            if (m_blocks.get(glm::ivec3(pos.x + 1, pos.y, pos.z)) == nullptr)
                m_VBO.addData(block->getFace(utils::Side::left, pos));
            if (m_blocks.get(glm::ivec3(pos.x - 1, pos.y, pos.z)) == nullptr)
                m_VBO.addData(block->getFace(utils::Side::right, pos));
            if (m_blocks.get(glm::ivec3(pos.x, pos.y + 1, pos.z)) == nullptr)
                m_VBO.addData(block->getFace(utils::Side::top, pos));
            if (m_blocks.get(glm::ivec3(pos.x, pos.y - 1, pos.z)) == nullptr)
                m_VBO.addData(block->getFace(utils::Side::bottom, pos));
            if (m_blocks.get(glm::ivec3(pos.x, pos.y, pos.z + 1)) == nullptr)
                m_VBO.addData(block->getFace(utils::Side::back, pos));
            if (m_blocks.get(glm::ivec3(pos.x, pos.y, pos.z - 1)) == nullptr)
                m_VBO.addData(block->getFace(utils::Side::front, pos));
        }

        m_VBO.sendData();
    }
} // namespace Minicube
