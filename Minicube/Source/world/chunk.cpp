#include "world/chunk.h"

namespace Minicube
{
    Chunk::Chunk(ChunkMap *chunkMap, glm::ivec3 pos)
    {
        m_chunkMap = chunkMap;
        m_pos = pos;

        if (pos.x != 0 || pos.y != 0 || pos.z != 0)
            m_model = glm::translate(m_model, glm::vec3(pos.x * 16, pos.y * 16, pos.z * 16));
    }

    // must be called in main thread before drawing
    void Chunk::init()
    {
        m_VBO.init();

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO.getID());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);

        m_flags &= ~CHUNK_FLAG_NEED_INIT;
    }

    void Chunk::draw(const Shader &shader)
    {

        if (m_flags & CHUNK_FLAG_NEED_DELETE)
            return;
        if (m_flags & CHUNK_FLAG_NEED_SEND_VBO)
        {
            m_VBO.sendData();
            m_flags &= ~CHUNK_FLAG_NEED_SEND_VBO;
        }
        if (m_VBO.getSize() == 0)
            return;
        if (!(m_flags & CHUNK_FLAG_NEED_INIT) && m_flags & CHUNK_FLAG_IS_GENERATED)
        {
            shader.use();
            shader.setMat4("model", m_model);
            glBindVertexArray(m_VAO);
            glBindTexture(GL_TEXTURE_2D_ARRAY, Textures::getArrayID());
            glDrawArrays(GL_TRIANGLES, 0, m_VBO.getTrianglesCount());
        }
    }

    inline void Chunk::addBlock(unsigned int x, unsigned int y, unsigned int z, Blocks::BlockId id)
    {
        void *addr = (void *)&m_blocks[getBlockIndex(x, y, z)];
        switch (id)
        {
        case Blocks::GRASS:
            new (addr) Blocks::Grass(id);
            break;
        case Blocks::DIRT:
            new (addr) Blocks::Dirt(id);
            break;
        case Blocks::STONE:
            new (addr) Blocks::Stone(id);
            break;
        case Blocks::SAND:
            new (addr) Blocks::Sand(id);
            break;
        case Blocks::BLUE:
            new (addr) Blocks::Blue(id);
            break;
        default:
            assert(false);
            break;
        }
    }

    void Chunk::build()
    {
        m_flags |= CHUNK_FLAG_IS_BUILDING;
        if (m_flags & CHUNK_FLAG_NEED_REBUILD && m_flags & CHUNK_FLAG_IS_GENERATED && !(m_flags & CHUNK_FLAG_NEED_DELETE))
        {
            m_flags &= ~CHUNK_FLAG_NEED_REBUILD;
            m_VBO.clear();
            constructVBO();
            m_flags |= CHUNK_FLAG_NEED_SEND_VBO;
        }
        m_flags &= ~CHUNK_FLAG_IS_BUILDING;
    }

    void Chunk::constructVBO()
    {
        m_isConstructing_mutex.lock();
        glm::uvec3 pos;
        for (int i = 0; i < 16 * 16 * 16; i++)
        {
            if (m_blocks[i].id == 0)
                continue;

            Block *block;
            pos = getBlockPos(i);

            block = getBlockInWorld(pos.x, pos.y, pos.z + 1);
            if (block == nullptr || block->id == 0)
            {
                for (unsigned int y = 0; y < Minicube::Vertices::cube::south.size(); y += 5)
                {
                    m_VBO.push_back(Minicube::Vertices::cube::south.at(y) + pos.x);
                    m_VBO.push_back(Minicube::Vertices::cube::south.at(y + 1) + pos.y);
                    m_VBO.push_back(Minicube::Vertices::cube::south.at(y + 2) + pos.z);
                    m_VBO.push_back(Minicube::Vertices::cube::south.at(y + 3));
                    m_VBO.push_back(Minicube::Vertices::cube::south.at(y + 4));
                    m_VBO.push_back((float)m_blocks[i].getTexture(utils::Direction::SOUTH));
                    m_VBO.push_back(0.0f);
                }
            }

            block = getBlockInWorld(pos.x, pos.y, pos.z - 1);
            if (block == nullptr || block->id == 0)
            {
                for (unsigned int y = 0; y < Minicube::Vertices::cube::north.size(); y += 5)
                {
                    m_VBO.push_back(Minicube::Vertices::cube::north.at(y) + pos.x);
                    m_VBO.push_back(Minicube::Vertices::cube::north.at(y + 1) + pos.y);
                    m_VBO.push_back(Minicube::Vertices::cube::north.at(y + 2) + pos.z);
                    m_VBO.push_back(Minicube::Vertices::cube::north.at(y + 3));
                    m_VBO.push_back(Minicube::Vertices::cube::north.at(y + 4));
                    m_VBO.push_back((float)m_blocks[i].getTexture(utils::Direction::NORTH));
                    m_VBO.push_back(1.0f);
                }
            }

            block = getBlockInWorld(pos.x, pos.y + 1, pos.z);
            if (block == nullptr || block->id == 0)
            {
                for (unsigned int y = 0; y < Minicube::Vertices::cube::up.size(); y += 5)
                {
                    m_VBO.push_back(Minicube::Vertices::cube::up.at(y) + pos.x);
                    m_VBO.push_back(Minicube::Vertices::cube::up.at(y + 1) + pos.y);
                    m_VBO.push_back(Minicube::Vertices::cube::up.at(y + 2) + pos.z);
                    m_VBO.push_back(Minicube::Vertices::cube::up.at(y + 3));
                    m_VBO.push_back(Minicube::Vertices::cube::up.at(y + 4));
                    m_VBO.push_back((float)m_blocks[i].getTexture(utils::Direction::UP));
                    m_VBO.push_back(4.0f);
                }
            }

            block = getBlockInWorld(pos.x, pos.y - 1, pos.z);
            if (block == nullptr || block->id == 0)
            {
                for (unsigned int y = 0; y < Minicube::Vertices::cube::down.size(); y += 5)
                {
                    m_VBO.push_back(Minicube::Vertices::cube::down.at(y) + pos.x);
                    m_VBO.push_back(Minicube::Vertices::cube::down.at(y + 1) + pos.y);
                    m_VBO.push_back(Minicube::Vertices::cube::down.at(y + 2) + pos.z);
                    m_VBO.push_back(Minicube::Vertices::cube::down.at(y + 3));
                    m_VBO.push_back(Minicube::Vertices::cube::down.at(y + 4));
                    m_VBO.push_back((float)m_blocks[i].getTexture(utils::Direction::DOWN));
                    m_VBO.push_back(5.0f);
                }
            }

            block = getBlockInWorld(pos.x + 1, pos.y, pos.z);
            if (block == nullptr || block->id == 0)
            {
                for (unsigned int y = 0; y < Minicube::Vertices::cube::east.size(); y += 5)
                {
                    m_VBO.push_back(Minicube::Vertices::cube::east.at(y) + pos.x);
                    m_VBO.push_back(Minicube::Vertices::cube::east.at(y + 1) + pos.y);
                    m_VBO.push_back(Minicube::Vertices::cube::east.at(y + 2) + pos.z);
                    m_VBO.push_back(Minicube::Vertices::cube::east.at(y + 3));
                    m_VBO.push_back(Minicube::Vertices::cube::east.at(y + 4));
                    m_VBO.push_back((float)m_blocks[i].getTexture(utils::Direction::EAST));
                    m_VBO.push_back(2.0f);
                }
            }

            block = getBlockInWorld(pos.x - 1, pos.y, pos.z);
            if (block == nullptr || block->id == 0)
            {
                for (unsigned int y = 0; y < Minicube::Vertices::cube::west.size(); y += 5)
                {
                    m_VBO.push_back(Minicube::Vertices::cube::west.at(y) + pos.x);
                    m_VBO.push_back(Minicube::Vertices::cube::west.at(y + 1) + pos.y);
                    m_VBO.push_back(Minicube::Vertices::cube::west.at(y + 2) + pos.z);
                    m_VBO.push_back(Minicube::Vertices::cube::west.at(y + 3));
                    m_VBO.push_back(Minicube::Vertices::cube::west.at(y + 4));
                    m_VBO.push_back((float)m_blocks[i].getTexture(utils::Direction::WEST));
                    m_VBO.push_back(3.0f);
                }
            }
        }

        m_isConstructing_mutex.unlock();
    }

    void Chunk::generate(WorldGen::HeightMap *heightMap)
    {
        if (m_flags & CHUNK_FLAG_IS_GENERATED)
        {
            return;
        }

        m_heightMap = heightMap;
        m_blocks = (Block *)calloc(16 * 16 * 16, sizeof(Block));

        for (int x = 0; x < 16; x++)
        {
            for (int z = 0; z < 16; z++)
            {
                WorldGen::HeightMapData data = heightMap->data[x * 16 + z];
                for (int y = m_pos.y * 16; y < (m_pos.y + 1) * 16; y++)
                {
                    int diff = data.height - y;
                    if (diff < 0)
                        break;
                    if (diff == 0)
                        addBlock(x, y - m_pos.y * 16, z, WorldGen::biomes[data.biome].block);
                    else if (diff < 3)
                        addBlock(x, y - m_pos.y * 16, z, Blocks::DIRT);
                    else
                        addBlock(x, y - m_pos.y * 16, z, Blocks::STONE);
                }
            }
        }

        m_flags |= CHUNK_FLAG_IS_GENERATED;
        m_flags |= CHUNK_FLAG_NEED_REBUILD;

        Chunk *chunk = m_chunkMap->get(m_pos + glm::ivec3(1, 0, 0));
        if (chunk != nullptr)
            chunk->addFlag(CHUNK_FLAG_NEED_REBUILD);

        chunk = m_chunkMap->get(m_pos + glm::ivec3(-1, 0, 0));
        if (chunk != nullptr)
            chunk->addFlag(CHUNK_FLAG_NEED_REBUILD);

        chunk = m_chunkMap->get(m_pos + glm::ivec3(0, 1, 0));
        if (chunk != nullptr)
            chunk->addFlag(CHUNK_FLAG_NEED_REBUILD);

        chunk = m_chunkMap->get(m_pos + glm::ivec3(0, -1, 0));
        if (chunk != nullptr)
            chunk->addFlag(CHUNK_FLAG_NEED_REBUILD);

        chunk = m_chunkMap->get(m_pos + glm::ivec3(0, 0, 1));
        if (chunk != nullptr)
            chunk->addFlag(CHUNK_FLAG_NEED_REBUILD);

        chunk = m_chunkMap->get(m_pos + glm::ivec3(0, 0, -1));
        if (chunk != nullptr)
            chunk->addFlag(CHUNK_FLAG_NEED_REBUILD);

        // std::cout << "chunk generated " << m_pos.x << " " << m_pos.y << " " << m_pos.z << "\n";
    }

    Block *Chunk::getBlockInWorld(int x, int y, int z)
    {

        if (!(x < 0 || y < 0 || z < 0 || x > 15 || y > 15 || z > 15))
            return getBlock(x, y, z);

        int _x = x + m_pos.x * 16;
        int _y = y + m_pos.y * 16;
        int _z = z + m_pos.z * 16;

        Chunk *chunk = m_chunkMap->get(glm::ivec3(floor(_x / 16.0), floor(_y / 16.0), floor(_z / 16.0)));

        if (chunk == nullptr)
            return nullptr;

        glm::uvec3 blockPos = utils::getBlockOffset(_x, _y, _z);

        return chunk->getBlock(blockPos.x, blockPos.y, blockPos.z);
    }
} // namespace Minicube
