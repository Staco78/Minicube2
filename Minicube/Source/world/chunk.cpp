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

        m_state.store(1);
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

        std::cout << "constructing VBO " << m_pos.x << " " << m_pos.y << " " << m_pos.z << "\n";
        for (int i = 0; i < 16 * 16 * 16; i++)
        {
            uint8_t side = 0;
            glm::uvec3 pos = getBlockPos(i);
            Block *block;

            // std::cout << "1\n";
            block = getBlockInWorld(pos.x + 1, pos.y, pos.z);
            // std::cout << "2\n";
            if (block == nullptr)
                side |= 0b00010000;
            // std::cout << "3\n";

            // std::cout << "4\n";
            block = getBlockInWorld(pos.x - 1, pos.y, pos.z);
            // std::cout << "5\n";
            if (block == nullptr)
                side |= 0b00100000;
            // std::cout << "6\n";

            // std::cout << "7\n";
            block = getBlockInWorld(pos.x, pos.y + 1, pos.z);
            // std::cout << "8\n";
            if (block == nullptr)
                side |= 0b00000100;
            // std::cout << "9\n";

            // std::cout << "10\n";
            block = getBlockInWorld(pos.x, pos.y - 1, pos.z);
            // std::cout << "11\n";
            if (block == nullptr)
                side |= 0b00001000;
            // std::cout << "12\n";

            // std::cout << "13\n";
            block = getBlockInWorld(pos.x, pos.y, pos.z + 1);
            // std::cout << "14\n";
            if (block == nullptr)
                side |= 0b00000001;
            // std::cout << "15\n";

            // std::cout << "16\n";
            block = getBlockInWorld(pos.x, pos.y, pos.z - 1);
            // std::cout << "17\n";
            if (block == nullptr)
                side |= 0b00000010;
            // std::cout << "18\n";

            // std::cout << "19\n";
            get_block_faces(pos, m_VBO, side);
            // std::cout << "20\n";
        }

        // std::cout << "VBO constructed\n";

        // m_VBO.sendData();

        // std::cout << "VBO sended\n";

        m_state.store(3);
    }

    Block *Chunk::getBlock(int x, int y, int z)
    {
        std::lock_guard<std::mutex> lock(m_blocks_mutex);
        return &m_blocks[getBlockIndex(x, y, z)];
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
        // std::cout << "chunk generated\n";
    }
} // namespace Minicube
