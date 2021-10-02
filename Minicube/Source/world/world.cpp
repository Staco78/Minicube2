#include "world/world.h"

namespace Minicube
{
    World::World()
    {
        for (int x = 0; x < 6; x++)
            for (int y = 0; y < 6; y++)
                m_chunks[glm::vec2(x, y)] = new Chunk(glm::ivec2(x, y));
    }

    void World::draw(const Shader &shader)
    {
        for (auto it = m_chunks.begin(); it != m_chunks.end(); it++)
        {
            it->second->draw(shader);
        }
    }
} // namespace Minicube
