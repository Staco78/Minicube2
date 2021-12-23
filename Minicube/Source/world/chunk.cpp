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

        if (m_flags & CHUNK_FLAG_NEED_SEND_VBO)
        {
            m_VBO.sendData();
            m_flags &= ~CHUNK_FLAG_NEED_SEND_VBO;
        }
        if (m_VBO.getSize() == 0)
            return;
        if (!(m_flags & CHUNK_FLAG_NEED_INIT & ~CHUNK_FLAG_IS_GENERATED & CHUNK_FLAG_NEED_DELETE))
        {
            shader.use();
            shader.setMat4("model", m_model);
            glBindVertexArray(m_VAO);
            glBindTexture(GL_TEXTURE_2D_ARRAY, Textures::getArrayID());
            glDrawArrays(GL_TRIANGLES, 0, m_VBO.getTrianglesCount());
        }
    }

    inline void Chunk::addBlock(int x, int y, int z, Block &block)
    {
        m_blocks[getBlockIndex(x, y, z)] = block;
    }

    void Chunk::build()
    {
        if (m_flags & CHUNK_FLAG_NEED_REBUILD && m_flags & CHUNK_FLAG_IS_GENERATED && !(m_flags & CHUNK_FLAG_NEED_DELETE))
        {
            m_flags &= ~CHUNK_FLAG_NEED_REBUILD;
            m_flags |= CHUNK_FLAG_IS_BUILDING;
            constructVBO();
            m_flags &= ~CHUNK_FLAG_IS_BUILDING;
            m_flags |= CHUNK_FLAG_NEED_SEND_VBO;
        }
    }

    void Chunk::constructVBO()
    {
        // std::cout << "constructing VBO " << m_pos.x << " " << m_pos.y << " " << m_pos.z << "\n";
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
                for (unsigned int y = 0; y < Minicube::Vertices::cube::back.size(); y += 5)
                {
                    m_VBO.push_back(Minicube::Vertices::cube::back.at(y) + pos.x);
                    m_VBO.push_back(Minicube::Vertices::cube::back.at(y + 1) + pos.y);
                    m_VBO.push_back(Minicube::Vertices::cube::back.at(y + 2) + pos.z);
                    m_VBO.push_back(Minicube::Vertices::cube::back.at(y + 3));
                    m_VBO.push_back(Minicube::Vertices::cube::back.at(y + 4));
                    m_VBO.push_back(m_blocks[i].id);
                    m_VBO.push_back(0.0f);
                }
            }

            block = getBlockInWorld(pos.x, pos.y, pos.z - 1);
            if (block == nullptr || block->id == 0)
            {
                for (unsigned int y = 0; y < Minicube::Vertices::cube::front.size(); y += 5)
                {
                    m_VBO.push_back(Minicube::Vertices::cube::front.at(y) + pos.x);
                    m_VBO.push_back(Minicube::Vertices::cube::front.at(y + 1) + pos.y);
                    m_VBO.push_back(Minicube::Vertices::cube::front.at(y + 2) + pos.z);
                    m_VBO.push_back(Minicube::Vertices::cube::front.at(y + 3));
                    m_VBO.push_back(Minicube::Vertices::cube::front.at(y + 4));
                    m_VBO.push_back(m_blocks[i].id);
                    m_VBO.push_back(1.0f);
                }
            }

            block = getBlockInWorld(pos.x, pos.y + 1, pos.z);
            if (block == nullptr || block->id == 0)
            {
                for (unsigned int y = 0; y < Minicube::Vertices::cube::top.size(); y += 5)
                {
                    m_VBO.push_back(Minicube::Vertices::cube::top.at(y) + pos.x);
                    m_VBO.push_back(Minicube::Vertices::cube::top.at(y + 1) + pos.y);
                    m_VBO.push_back(Minicube::Vertices::cube::top.at(y + 2) + pos.z);
                    m_VBO.push_back(Minicube::Vertices::cube::top.at(y + 3));
                    m_VBO.push_back(Minicube::Vertices::cube::top.at(y + 4));
                    m_VBO.push_back(m_blocks[i].id);
                    m_VBO.push_back(4.0f);
                }
            }

            block = getBlockInWorld(pos.x, pos.y - 1, pos.z);
            if (block == nullptr || block->id == 0)
            {
                for (unsigned int y = 0; y < Minicube::Vertices::cube::bottom.size(); y += 5)
                {
                    m_VBO.push_back(Minicube::Vertices::cube::bottom.at(y) + pos.x);
                    m_VBO.push_back(Minicube::Vertices::cube::bottom.at(y + 1) + pos.y);
                    m_VBO.push_back(Minicube::Vertices::cube::bottom.at(y + 2) + pos.z);
                    m_VBO.push_back(Minicube::Vertices::cube::bottom.at(y + 3));
                    m_VBO.push_back(Minicube::Vertices::cube::bottom.at(y + 4));
                    m_VBO.push_back(m_blocks[i].id);
                    m_VBO.push_back(5.0f);
                }
            }

            block = getBlockInWorld(pos.x + 1, pos.y, pos.z);
            if (block == nullptr || block->id == 0)
            {
                for (unsigned int y = 0; y < Minicube::Vertices::cube::left.size(); y += 5)
                {
                    m_VBO.push_back(Minicube::Vertices::cube::left.at(y) + pos.x);
                    m_VBO.push_back(Minicube::Vertices::cube::left.at(y + 1) + pos.y);
                    m_VBO.push_back(Minicube::Vertices::cube::left.at(y + 2) + pos.z);
                    m_VBO.push_back(Minicube::Vertices::cube::left.at(y + 3));
                    m_VBO.push_back(Minicube::Vertices::cube::left.at(y + 4));
                    m_VBO.push_back(m_blocks[i].id);
                    m_VBO.push_back(2.0f);
                }
            }

            block = getBlockInWorld(pos.x - 1, pos.y, pos.z);
            if (block == nullptr || block->id == 0)
            {
                for (unsigned int y = 0; y < Minicube::Vertices::cube::right.size(); y += 5)
                {
                    m_VBO.push_back(Minicube::Vertices::cube::right.at(y) + pos.x);
                    m_VBO.push_back(Minicube::Vertices::cube::right.at(y + 1) + pos.y);
                    m_VBO.push_back(Minicube::Vertices::cube::right.at(y + 2) + pos.z);
                    m_VBO.push_back(Minicube::Vertices::cube::right.at(y + 3));
                    m_VBO.push_back(Minicube::Vertices::cube::right.at(y + 4));
                    m_VBO.push_back(m_blocks[i].id);
                    m_VBO.push_back(3.0f);
                }
            }
        }

        m_flags &= ~CHUNK_FLAG_NEED_REBUILD;
    }

    Block *Chunk::getBlock(int x, int y, int z)
    {
        Block *ret = &m_blocks[getBlockIndex(x, y, z)];
        return ret;
    }

    void Chunk::generate(HeightMap *heightMap)
    {
        if (m_flags & CHUNK_FLAG_IS_GENERATED)
        {
            return;
        }

        m_blocks = (Block *)calloc(16 * 16 * 16, sizeof(Block));
        for (int x = 0; x < 16; x++)
        {
            for (int z = 0; z < 16; z++)
            {
                int height = heightMap->data[x * 16 + z];
                if (height > (m_pos.y + 1) * 16)
                    for (int y = 0; y < 16; y++)
                        m_blocks[getBlockIndex(x, y, z)].id = 2;
                else
                {
                    for (int y = 0; y < height - m_pos.y * 16; y++)
                        m_blocks[getBlockIndex(x, y, z)].id = 2;
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

        if (_x % 16 == 0)
            x = 0;
        else if (_x >= 0)
            x = _x % 16;
        else
            x = 16 + (_x % 16);

        if (_y % 16 == 0)
            y = 0;
        else if (_y >= 0)
            y = _y % 16;
        else
            y = 16 + (_y % 16);

        if (_z % 16 == 0)
            z = 0;
        else if (_z >= 0)
            z = _z % 16;
        else
            z = 16 + (_z % 16);

        return chunk->getBlock(x, y, z);
    }
} // namespace Minicube
