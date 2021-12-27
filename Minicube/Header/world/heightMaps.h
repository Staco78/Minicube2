#pragma once

#include <map>

#include "gen/worldGen.h"

namespace std
{
    template <>
    struct less<glm::ivec2>
    {
        bool operator()(const glm::ivec2 &left, const glm::ivec2 &right) const
        {
            if (left.x == right.x)
                return left.y < right.y;
            else
                return left.x < right.x;
        }
    };
}

namespace Minicube
{
    class HeightMaps
    {
    public:
        WorldGen::HeightMap *add(const glm::ivec2 &pos, PerlinNoiseContext *perlinNoiseContext)
        {
            m_data.insert(std::pair<glm::ivec2, std::pair<WorldGen::HeightMap, unsigned int>>(pos, std::pair<WorldGen::HeightMap, unsigned int>(WorldGen::HeightMap(pos.x, pos.y, perlinNoiseContext), 0)));
            return &m_data.at(pos).first;
        }

        void setUses(const glm::ivec2 &pos, unsigned int uses)
        {
            auto find = m_data.find(pos);
            if (find == m_data.end())
                throw std::runtime_error("HeightMaps::erase: not found");

            assert(uses > 0);

            find->second.second = uses;
        }

        void erase(const glm::ivec2 &pos)
        {
            auto find = m_data.find(pos);
            if (find == m_data.end())
                throw std::runtime_error("HeightMaps::erase: not found");

            assert(find->second.second > 0);
            find->second.second--;
            if (find->second.second == 0)
                m_data.erase(find);
        }

    private:
        std::map<glm::ivec2, std::pair<WorldGen::HeightMap, unsigned int>> m_data;
    };
} // namespace Minicube
