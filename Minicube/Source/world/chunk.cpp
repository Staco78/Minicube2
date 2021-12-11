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

        // std::cout << "chunk constructed\n";
    }

    void Chunk::draw(const Shader &shader, int &VBOConstructed)
    {

        if (!m_isVBOConstructed && VBOConstructed < 2)
        {
            constructVBO();
            VBOConstructed++;
        }

        shader.use();
        shader.setMat4("model", m_model);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_VBO.getTrianglesCount());
    }

    void Chunk::addBlock(int x, int y, int z, Block &block)
    {
        m_blocks[getBlockIndex(x, y, z)] = block;
    }

    void Chunk::constructVBO()
    {

        // std::cout << "constructing VBO\n";

        std::vector<float> *VBO = m_VBO.getData();

        for (int i = 0; i < 16 * 16 * 16; i++)
        {
            uint8_t side = 0;
            glm::uvec3 pos = getBlockPos(i);
            Block *block;
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
            get_block_faces(pos, VBO, side);
        }

        m_VBO.sendData();

        m_isVBOConstructed = true;
    }

    Block *Chunk::getBlock(int x, int y, int z)
    {
        return &m_blocks[getBlockIndex(x, y, z)];
    }

    void Chunk::generate()
    {
        m_blocks = (Block *)calloc(16 * 16 * 16, sizeof(Block));
        for (int i = 0; i < 16 * 16 * 16; i++)
        {
            m_blocks[i].id = 1;
        }
        // std::cout << "chunk generated\n";
    }
} // namespace Minicube
