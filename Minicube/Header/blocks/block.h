#pragma once

#include "Glew/glew.h"

#include <Glm/glm.hpp>
#include <Glm/gtc/matrix_transform.hpp>
#include <Glm/gtc/type_ptr.hpp>

#include "shader.h"

#include "utils.h"

namespace Minicube
{

    class Face
    {
    public:
        Face(utils::Side side)
        {
            if (side == utils::Side::front)
                m_vertices = &Vertexs::cube::front;
            else if (side == utils::Side::back)
                m_vertices = &Vertexs::cube::back;
            else if (side == utils::Side::left)
                m_vertices = &Vertexs::cube::left;
            else if (side == utils::Side::right)
                m_vertices = &Vertexs::cube::right;
            else if (side == utils::Side::bottom)
                m_vertices = &Vertexs::cube::bottom;
            else if (side == utils::Side::top)
                m_vertices = &Vertexs::cube::top;
        }
        std::vector<float> *getVertices() { return m_vertices; }
        std::vector<float> *getChunkVertices(const glm::uvec3 &pos)
        {
            std::vector<float> *r = new std::vector<float>;

            for (unsigned int i = 0; i < m_vertices->size(); i += 5)
            {
                r->push_back(m_vertices->at(i) + pos.x);
                /*if (pos.x == 15)
					std::cout << r[i] << std::endl;*/
                r->push_back(m_vertices->at(i + 1) + pos.y);
                r->push_back(m_vertices->at(i + 2) + pos.z);
                r->push_back(m_vertices->at(i + 3));
                r->push_back(m_vertices->at(i + 4));

                /*if (m_textureID == 1)
					assert(false);*/

                //std::cout << pos.x << ":" << pos.y << ":" << pos.z << std::endl;
            }

            return r;
        }

    private:
        std::vector<float> *m_vertices;
    };

    class Faces
    {
    public:
        Faces() : front(utils::Side::front), back(utils::Side::back), left(utils::Side::left), right(utils::Side::right), bottom(utils::Side::bottom), top(utils::Side::top)
        {
        }
        Face *get(utils::Side side)
        {
            if (side == utils::Side::front)
                return &front;
            else if (side == utils::Side::back)
                return &back;
            else if (side == utils::Side::left)
                return &left;
            else if (side == utils::Side::right)
                return &right;
            else if (side == utils::Side::bottom)
                return &bottom;
            else if (side == utils::Side::top)
                return &top;
        }

    private:
        Face front;
        Face back;
        Face left;
        Face right;
        Face bottom;
        Face top;
    };

    class Block
    {
    public:
        std::vector<float> *getFace(utils::Side side, const glm::uvec3 &pos);

    private:
        Faces m_faces;
    };
} // namespace Minicube
