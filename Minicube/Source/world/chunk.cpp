#include "world/chunk.h"

namespace Minicube
{
    Chunk::Chunk(glm::ivec2 pos)
    {
        m_pos = pos;

        // m_blocks[glm::uvec3(0, 0, 0)] = new Block(glm::uvec3(0, 0, 0));
        // m_blocks[glm::uvec3(0, 0, 1)] = new Block(glm::uvec3(0, 0, 1));

        for (int x = 0; x < 16; x++)
            for (int y = 0; y < 16; y++)
                addBlock(glm::uvec3(x, 0, y));
    }

    void Chunk::draw(const Shader &shader)
    {
        for (auto it = m_blocks.begin(); it != m_blocks.end(); it++)
        {
            it->second->draw(shader);
        }
    }

    void Chunk::addBlock(glm::uvec3 relativeBlockPos)
    {
        m_blocks[relativeBlockPos] = new Block(glm::uvec3(relativeBlockPos.x + m_pos.x * 16, relativeBlockPos.y, relativeBlockPos.z + m_pos.y * 16));
    }
} // namespace Minicube
