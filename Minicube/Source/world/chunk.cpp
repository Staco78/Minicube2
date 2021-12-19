#include "world/chunk.h"

namespace Minicube
{
    Chunk::Chunk(ChunkMap *chunkMap, glm::ivec3 pos)
    {
        m_chunkMap = chunkMap;
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

        if (pos.x != 0 || pos.y != 0 || pos.z != 0)
            m_model = glm::translate(m_model, glm::vec3(pos.x * 16, pos.y * 16, pos.z * 16));

        m_state = 1;
        // std::cout << "chunk constructed\n";
    }

    void Chunk::draw(const Shader &shader)
    {
        // std::cout << "try to draw chunk\n";

        if (m_state == 3)
        {
            m_VBO.sendData();
            m_state = 4;
        }
        if (m_state == 4)
        {
            shader.use();
            // std::cout << "draw chunk " << m_pos.x << " " << m_pos.y << " " << m_pos.z << "\n";
            shader.setMat4("model", m_model);
            // std::cout << "set model\n";
            glBindVertexArray(m_VAO);
            glDrawArrays(GL_TRIANGLES, 0, m_VBO.getTrianglesCount());
            // std::cout << "chunk drawed\n";
        }
    }

    void Chunk::addBlock(int x, int y, int z, Block &block)
    {
        m_blocks_mutex.lock();
        m_blocks[getBlockIndex(x, y, z)] = block;
        m_blocks_mutex.unlock();
    }

    void Chunk::constructVBO()
    {
        if (m_state != 1)
            return;

        m_state = 2;

        glm::uvec3 pos;

        std::cout << "constructing VBO " << m_pos.x << " " << m_pos.y << " " << m_pos.z << "\n";
        for (int i = 0; i < 16 * 16 * 16; i++)
        {
            uint8_t side = 0;
            Block *block;
            pos = getBlockPos(i);

            block = getBlockInWorld(pos.x + 1, pos.y, pos.z);
            if (block == nullptr || block->id == 0)
                side |= 0b00010000;

            block = getBlockInWorld(pos.x - 1, pos.y, pos.z);
            if (block == nullptr || block->id == 0)
                side |= 0b00100000;

            block = getBlockInWorld(pos.x, pos.y + 1, pos.z);
            if (block == nullptr || block->id == 0)
                side |= 0b00000100;

            block = getBlockInWorld(pos.x, pos.y - 1, pos.z);
            if (block == nullptr || block->id == 0)
                side |= 0b00001000;

            block = getBlockInWorld(pos.x, pos.y, pos.z + 1);
            if (block == nullptr || block->id == 0)
                side |= 0b00000001;

            block = getBlockInWorld(pos.x, pos.y, pos.z - 1);
            if (block == nullptr || block->id == 0)
                side |= 0b00000010;

            if (side != 0)
                get_block_faces(pos, m_VBO, side);
        }

        m_state = 3;
    }

    Block *Chunk::getBlock(int x, int y, int z)
    {
        m_blocks_mutex.lock();
        Block *ret = &m_blocks[getBlockIndex(x, y, z)];
        m_blocks_mutex.unlock();
        return ret;
    }

    void Chunk::generate()
    {
        m_blocks = (Block *)malloc(16 * 16 * 16 * sizeof(Block));
        m_blocks_mutex.lock();
        for (int i = 0; i < 16 * 16 * 16; i++)
        {
            m_blocks[i].id = 1;
        }
        m_blocks_mutex.unlock();
        std::cout << "chunk generated\n";
    }
} // namespace Minicube
