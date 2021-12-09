#include "world/chunk.h"

namespace Minicube
{
    Chunk::Chunk(ChunkMap *chunkMap, glm::ivec2 pos)
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

        if (pos.x != 0 || pos.y != 0)
            m_model = glm::translate(m_model, glm::vec3(pos.x * 16, 0, pos.y * 16));

        std::cout << "chunk constructed\n";
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

    void Chunk::addBlock(glm::uvec3 relativeBlockPos, Block *block)
    {
        m_blocks[relativeBlockPos] = block;
    }

    void Chunk::constructVBO()
    {

        std::cout << "constructing VBO\n";

        std::vector<float> *VBO = m_VBO.getData();
        for (auto &[pos, block] : m_blocks)
        {
            uint8_t side = 0;
            if (getBlockInWorld(pos.x + 1, pos.y, pos.z) == nullptr)
                side |= 0b00010000;
            if (getBlockInWorld(pos.x - 1, pos.y, pos.z) == nullptr)
                side |= 0b00100000;
            if (getBlockInWorld(pos.x, pos.y + 1, pos.z) == nullptr)
                side |= 0b00000100;
            if (getBlockInWorld(pos.x, pos.y - 1, pos.z) == nullptr)
                side |= 0b00001000;
            if (getBlockInWorld(pos.x, pos.y, pos.z + 1) == nullptr)
                side |= 0b00000001;
            if (getBlockInWorld(pos.x, pos.y, pos.z - 1) == nullptr)
                side |= 0b00000010;
            get_block_faces(pos, VBO, side);
        }

        m_VBO.sendData();

        m_isVBOConstructed = true;
    }

    Block *Chunk::getBlock(const glm::uvec3 &pos)
    {
        return m_blocks.get(pos);
    }

    void Chunk::generate()
    {
        Block *blocks = (Block *)malloc(sizeof(Block) * 16 * 16 * 2);
        for (int x = 0; x < 16; x++)
            for (int y = 0; y < 2; y++)
                for (int z = 0; z < 16; z++)
                {
                    blocks[z * 16 * 2 + y * 16 + x].id = 1;
                    addBlock(glm::uvec3(x, y, z), &blocks[z * 16 * 2 + y * 16 + x]);
                }
        std::cout << "chunk generated\n";
    }
} // namespace Minicube
