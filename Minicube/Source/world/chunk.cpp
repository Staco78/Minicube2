#include "world/chunk.h"

namespace Minicube
{
    Chunk::Chunk(ChunkMap *chunkMap, glm::ivec2 pos)
    {
        m_chunkMap = chunkMap;
        m_pos = pos;

        std::cout << sizeof(Chunk) << std::endl;

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
            // for (int y = 0; y < 6; y++)
            for (int z = 0; z < 16; z++)
                addBlock(glm::uvec3(x, 0, z));

        std::cout << "chunk constructed\n";
    }

    void Chunk::draw(const Shader &shader)
    {
        if (!m_isVBOConstructed)
            constructVBO();

        shader.use();
        shader.setMat4("model", m_model);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_VBO.getTrianglesCount());
    }

    void Chunk::addBlock(glm::uvec3 relativeBlockPos)
    {
        m_blocks[relativeBlockPos] = new Block();
    }

    Block *Chunk::getBlockInWorld(const glm::ivec3 &relativePos)
    {

        int _x = relativePos.x + m_pos.x * 16;
        int _z = relativePos.z + m_pos.y * 16;

        Chunk *chunk = m_chunkMap->get(glm::ivec2(floor(_x / 16.0), floor(_z / 16.0)));

        if (chunk == nullptr)
            return nullptr;

        int x;
        int z;

        if (_x % 16 == 0)
            x = 0;
        else if (_x >= 0)
            x = _x % 16;
        else
            x = 16 + (_x % 16);

        if (_z % 16 == 0)
            z = 0;
        else if (_z >= 0)
            z = _z % 16;
        else
            z = 16 + (_z % 16);

        return chunk->getBlock(glm::uvec3(x, relativePos.y, z));
    }

    void Chunk::constructVBO()
    {
        for (auto &[pos, block] : m_blocks)
        {
            if (getBlockInWorld(glm::ivec3(pos.x + 1, pos.y, pos.z)) == nullptr)
                m_VBO.addData(block->getFace(utils::Side::left, pos));
            if (getBlockInWorld(glm::ivec3(pos.x - 1, pos.y, pos.z)) == nullptr)
                m_VBO.addData(block->getFace(utils::Side::right, pos));
            if (getBlockInWorld(glm::ivec3(pos.x, pos.y + 1, pos.z)) == nullptr)
                m_VBO.addData(block->getFace(utils::Side::top, pos));
            if (getBlockInWorld(glm::ivec3(pos.x, pos.y - 1, pos.z)) == nullptr)
                m_VBO.addData(block->getFace(utils::Side::bottom, pos));
            if (getBlockInWorld(glm::ivec3(pos.x, pos.y, pos.z + 1)) == nullptr)
                m_VBO.addData(block->getFace(utils::Side::back, pos));
            if (getBlockInWorld(glm::ivec3(pos.x, pos.y, pos.z - 1)) == nullptr)
                m_VBO.addData(block->getFace(utils::Side::front, pos));
        }

        m_VBO.sendData();

        m_isVBOConstructed = true;
    }

    Block *Chunk::getBlock(const glm::uvec3 &pos)
    {
        return m_blocks.get(pos);
    }
} // namespace Minicube
