#pragma once

#include <Glew/glew.h>

#include <iostream>
#include <string>

#define TEXTURE_SIZE 16
#define TEXTURE_COUNT 4

namespace Minicube
{
    namespace Textures
    {

        enum class TextureId
        {
            GRASS = 1,
            STONE,
            SAND,
            BLUE
        };

        void init();
        unsigned int getArrayID();
    } // namespace Texture

} // namespace Minicube
