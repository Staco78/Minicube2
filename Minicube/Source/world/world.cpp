#include "world/world.h"

namespace Minicube
{
    World::World()
    {
        for (int x = 0; x < 6; x++)
            for (int y = 0; y < 6; y++)
                m_chunks[glm::vec2(x, y)] = new Chunk(&m_chunks, glm::ivec2(x, y));
    }

    void World::draw(const Shader &shader)
    {
        for (auto it = m_chunks.begin(); it != m_chunks.end(); it++)
        {
            it->second->draw(shader);
        }
    }

    Chunk *World::getChunk(const glm::ivec2 &pos)
    {
        return m_chunks.get(pos);
    }

    Block *World::getBlock(const glm::ivec3 &pos)
    {

        Chunk *chunk = m_chunks.get(glm::ivec2(floor(pos.x / 16.0), floor(pos.z / 16.0)));

        int x;
        int z;

        if (pos.x % 16 == 0)
            x = 0;
        else if (pos.x >= 0)
            x = pos.x % 16;
        else
            x = 16 + (pos.x % 16);

        if (pos.z % 16 == 0)
            z = 0;
        else if (pos.z >= 0)
            z = pos.z % 16;
        else
            z = 16 + (pos.z % 16);

        return chunk->getBlock(glm::uvec3(x, pos.y, z));
    }

} // namespace Minicube
