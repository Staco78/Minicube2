#include "world/chunk.h"

namespace Minicube
{
    Chunk::Chunk(ChunkMap *chunkMap, glm::ivec3 pos)
    {
        m_chunkMap = chunkMap;
        m_pos = pos;

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO.getID());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        if (pos.x != 0 || pos.y != 0 || pos.z != 0)
            m_model = glm::translate(m_model, glm::vec3(pos.x * 16, pos.y * 16, pos.z * 16));

        m_state = CHUNK_STATE_UNLOADED;
    }

    void Chunk::draw(const Shader &shader)
    {
        if (m_VBO.getSize() == 0)
            return;
        if (m_state == CHUNK_STATE_LOADED)
        {
            m_VBO.sendData();
            m_state = CHUNK_STATE_IDLE;
        }
        if (m_state == CHUNK_STATE_IDLE)
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
        if (m_state == CHUNK_STATE_UNLOADED)
        {
        }
        else if (m_state == CHUNK_STATE_IDLE)
        {
            m_VBO.clear();
        }
        else
            return;
        m_state = CHUNK_STATE_LOADING;
        constructVBO();
        m_state = CHUNK_STATE_LOADED;
    }

    void Chunk::constructVBO()
    {
        std::cout << "constructing VBO " << m_pos.x << " " << m_pos.y << " " << m_pos.z << "\n";
        glm::uvec3 pos;
        for (int i = 0; i < 16 * 16 * 16; i++)
        {
            if (m_blocks[i].id == 0)
                continue;

            int side = 0;
            Block *block;
            pos = getBlockPos(i);

            block = getBlockInWorld(pos.x, pos.y, pos.z + 1);
            if (block == nullptr || block->id == 0)
                side |= BACK;

            block = getBlockInWorld(pos.x, pos.y, pos.z - 1);
            if (block == nullptr || block->id == 0)
                side |= FRONT;

            block = getBlockInWorld(pos.x, pos.y + 1, pos.z);
            if (block == nullptr || block->id == 0)
                side |= TOP;

            block = getBlockInWorld(pos.x, pos.y - 1, pos.z);
            if (block == nullptr || block->id == 0)
                side |= BOTTOM;

            block = getBlockInWorld(pos.x + 1, pos.y, pos.z);
            if (block == nullptr || block->id == 0)
                side |= LEFT;

            block = getBlockInWorld(pos.x - 1, pos.y, pos.z);
            if (block == nullptr || block->id == 0)
                side |= RIGHT;

            if (side != 0)
                get_block_faces(pos, m_VBO, (Side)side, m_blocks[i].id);
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
                        m_blocks[getBlockIndex(x, y, z)].id = 1;
                }
            }
        }

        m_flags |= CHUNK_FLAG_NEED_REBUILD;

        Chunk *chunk = m_chunkMap->get(m_pos + glm::ivec3(1, 0, 0));
        if (chunk != nullptr)
            chunk->setFlags(chunk->getFlags() | CHUNK_FLAG_NEED_REBUILD);

        chunk = m_chunkMap->get(m_pos + glm::ivec3(-1, 0, 0));
        if (chunk != nullptr)
            chunk->setFlags(chunk->getFlags() | CHUNK_FLAG_NEED_REBUILD);

        chunk = m_chunkMap->get(m_pos + glm::ivec3(0, 1, 0));
        if (chunk != nullptr)
            chunk->setFlags(chunk->getFlags() | CHUNK_FLAG_NEED_REBUILD);

        chunk = m_chunkMap->get(m_pos + glm::ivec3(0, -1, 0));
        if (chunk != nullptr)
            chunk->setFlags(chunk->getFlags() | CHUNK_FLAG_NEED_REBUILD);

        chunk = m_chunkMap->get(m_pos + glm::ivec3(0, 0, 1));
        if (chunk != nullptr)
            chunk->setFlags(chunk->getFlags() | CHUNK_FLAG_NEED_REBUILD);

        chunk = m_chunkMap->get(m_pos + glm::ivec3(0, 0, -1));
        if (chunk != nullptr)
            chunk->setFlags(chunk->getFlags() | CHUNK_FLAG_NEED_REBUILD);

        std::cout << "chunk generated " << m_pos.x << " " << m_pos.y << " " << m_pos.z << "\n";
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
