#pragma once

#include <map>
#include <vector>

#include "Glew/glew.h"

#include <Glm/glm.hpp>
#include "Glm/vec3.hpp"

#include "chunkMap.h"
#include "shader.h"
#include "utils.h"

namespace std
{
    template <>
    struct less<glm::uvec3>
    {
        bool operator()(const glm::uvec3 &left, const glm::uvec3 &right) const
        {
            if (left.y == right.y && left.x == right.x)
                return left.z < right.z;
            else if (left.x == right.x)
                return left.y < right.y;
            else
                return left.x < right.x;
        }
    };
}

typedef struct
{
    uint16_t id;
} Block;

namespace
{

    /*
    side:
    0b00000001 = back
    0b00000010 = front
    0b00000100 = top
    0b00001000 = bottom
    0b00010000 = left
    0b00100000 = right */

    void get_block_faces(const glm::uvec3 &pos, std::vector<float> *vertices, uint8_t side)
    {
        if (side & 0b00000001)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::back.size(); i += 5)
            {
                vertices->push_back(Minicube::Vertices::cube::back.at(i) + pos.x);
                /*if (pos.x == 15)
                    std::cout << r[i] << std::endl;*/
                vertices->push_back(Minicube::Vertices::cube::back.at(i + 1) + pos.y);
                vertices->push_back(Minicube::Vertices::cube::back.at(i + 2) + pos.z);
                vertices->push_back(Minicube::Vertices::cube::back.at(i + 3));
                vertices->push_back(Minicube::Vertices::cube::back.at(i + 4));
            }
        }
        if (side & 0b00000010)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::front.size(); i += 5)
            {
                vertices->push_back(Minicube::Vertices::cube::front.at(i) + pos.x);
                /*if (pos.x == 15)
                    std::cout << r[i] << std::endl;*/
                vertices->push_back(Minicube::Vertices::cube::front.at(i + 1) + pos.y);
                vertices->push_back(Minicube::Vertices::cube::front.at(i + 2) + pos.z);
                vertices->push_back(Minicube::Vertices::cube::front.at(i + 3));
                vertices->push_back(Minicube::Vertices::cube::front.at(i + 4));
            }
        }
        if (side & 0b00000100)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::top.size(); i += 5)
            {
                vertices->push_back(Minicube::Vertices::cube::top.at(i) + pos.x);
                /*if (pos.x == 15)
                    std::cout << r[i] << std::endl;*/
                vertices->push_back(Minicube::Vertices::cube::top.at(i + 1) + pos.y);
                vertices->push_back(Minicube::Vertices::cube::top.at(i + 2) + pos.z);
                vertices->push_back(Minicube::Vertices::cube::top.at(i + 3));
                vertices->push_back(Minicube::Vertices::cube::top.at(i + 4));
            }
        }
        if (side & 0b00001000)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::bottom.size(); i += 5)
            {
                vertices->push_back(Minicube::Vertices::cube::bottom.at(i) + pos.x);
                /*if (pos.x == 15)
                    std::cout << r[i] << std::endl;*/
                vertices->push_back(Minicube::Vertices::cube::bottom.at(i + 1) + pos.y);
                vertices->push_back(Minicube::Vertices::cube::bottom.at(i + 2) + pos.z);
                vertices->push_back(Minicube::Vertices::cube::bottom.at(i + 3));
                vertices->push_back(Minicube::Vertices::cube::bottom.at(i + 4));
            }
        }
        if (side & 0b00010000)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::left.size(); i += 5)
            {
                vertices->push_back(Minicube::Vertices::cube::left.at(i) + pos.x);
                /*if (pos.x == 15)
                    std::cout << r[i] << std::endl;*/
                vertices->push_back(Minicube::Vertices::cube::left.at(i + 1) + pos.y);
                vertices->push_back(Minicube::Vertices::cube::left.at(i + 2) + pos.z);
                vertices->push_back(Minicube::Vertices::cube::left.at(i + 3));
                vertices->push_back(Minicube::Vertices::cube::left.at(i + 4));
            }
        }
        if (side & 0b00100000)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::right.size(); i += 5)
            {
                vertices->push_back(Minicube::Vertices::cube::right.at(i) + pos.x);
                /*if (pos.x == 15)
                    std::cout << r[i] << std::endl;*/
                vertices->push_back(Minicube::Vertices::cube::right.at(i + 1) + pos.y);
                vertices->push_back(Minicube::Vertices::cube::right.at(i + 2) + pos.z);
                vertices->push_back(Minicube::Vertices::cube::right.at(i + 3));
                vertices->push_back(Minicube::Vertices::cube::right.at(i + 4));
            }
        }
    }
}

namespace Minicube
{

    class BlockMap : public std::map<glm::uvec3, Block *>
    {
    public:
        Block *get(glm::ivec3 pos)
        {

            if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x > 15 || pos.z > 15)
                return nullptr;

            auto it = find(pos);
            if (it == end())
                return nullptr;
            return it->second;
        }

        ~BlockMap()
        {

            // for (auto it = this->begin(); it != this->end(); it++)
            //     free(it->second);
        }
    };

    class DynamicVBO
    {
    public:
        DynamicVBO()
        {
            glGenBuffers(1, &ID);
        }
        void sendData()
        {
            glBindBuffer(GL_ARRAY_BUFFER, ID);
            glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(float), m_data.data(), GL_DYNAMIC_DRAW);
        }
        int getSize()
        {
            return m_data.size();
        }
        unsigned int getTrianglesCount() { return (getSize() / 5); }
        unsigned int getID() { return ID; }
        std::vector<float> *getData() { return &m_data; }

    private:
        unsigned int ID;
        std::vector<float> m_data;
    };

    class Chunk
    {
    public:
        Chunk(ChunkMap *chunkMap, glm::ivec2 pos);
        ~Chunk()
        {
            std::cout << "chunk destructed\n";
        }
        void draw(const Shader &shader, int &isVBOConstructable);
        void addBlock(glm::uvec3 relativeBlockPos, Block *block);
        void constructVBO();
        Block *getBlock(const glm::uvec3 &pos);
        glm::ivec2 getPos()
        {
            return m_pos;
        }

        void generate();

    private:
        Block *getBlockInWorld(int x, int y, int z)
        {

            int _x = x + m_pos.x * 16;
            int _z = z + m_pos.y * 16;

            Chunk *chunk = m_chunkMap->get(glm::ivec2(floor(_x / 16.0), floor(_z / 16.0)));

            if (chunk == nullptr)
                return nullptr;

            if (_x % 16 == 0)
                x = 0;
            else if (_x >= 0)
                x = _x % 16;
            else
                x = 16 + (_x % 16);

            if (_z % 16 == 0)
                z = 0;
            else if (_z >= 0)
                z = _z % 16;
            else
                z = 16 + (_z % 16);

            return chunk->getBlock(glm::uvec3(x, y, z));
        }

        BlockMap m_blocks;
        glm::ivec2 m_pos;
        DynamicVBO m_VBO;
        unsigned int m_VAO;
        glm::mat4 m_model = glm::mat4(1.0f);
        ChunkMap *m_chunkMap = nullptr;
        bool m_isVBOConstructed = false;
    };

} // namespace Minicube
