#pragma once

#include "Glew/glew.h"

#include <Glm/glm.hpp>
#include <Glm/gtc/matrix_transform.hpp>
#include <Glm/gtc/type_ptr.hpp>

#include "shader.h"

namespace Minicube
{
    class Block
    {
    public:
        Block(const glm::vec3 &pos);
        void draw(const Shader& shader);
    private:
        glm::mat4 m_model = glm::mat4(1.0f);
        unsigned int m_VAO;
    };
} // namespace Minicube
