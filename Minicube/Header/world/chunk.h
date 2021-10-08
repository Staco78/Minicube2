#pragma once

#include <map>
#include <vector>

#include "Glm/vec3.hpp"

#include "chunkMap.h"
#include "blocks/block.h"

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

            for (auto it = this->begin(); it != this->end(); it++)
                delete it->second;
        }
    };

    class DynamicVBO
    {
    public:
        DynamicVBO()
        {
            glGenBuffers(1, &ID);
        }
        void addData(std::vector<float> *data)
        {
            m_data.insert(m_data.end(), data->begin(), data->end());
            delete data;
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

    private:
        unsigned int ID;
        std::vector<float> m_data;
    };

    class Chunk
    {
    public:
        Chunk(ChunkMap *chunkMap, glm::ivec2 pos);
        ~Chunk(){
            std::cout << "chunk destructed\n";
        }
        void draw(const Shader &shader);
        void addBlock(glm::uvec3 relativeBlockPos);
        void constructVBO();
        Block *getBlock(const glm::uvec3 &pos);
        glm::ivec2 getPos()
        {
            return m_pos;
        }

    private:
        Block *getBlockInWorld(const glm::ivec3 &pos);

        BlockMap m_blocks;
        glm::ivec2 m_pos;
        DynamicVBO m_VBO;
        unsigned int m_VAO;
        glm::mat4 m_model = glm::mat4(1.0f);
        ChunkMap *m_chunkMap = nullptr;
        bool m_isVBOConstructed = false;
    };

} // namespace Minicube
