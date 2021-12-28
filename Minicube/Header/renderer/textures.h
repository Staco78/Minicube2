#pragma once

#include <Glew/glew.h>

#include <iostream>
#include <string>

#define TEXTURE_SIZE 16

namespace Minicube
{
    namespace Textures
    {

#define TEXTURE_COUNT 6
        enum class TextureId
        {
            GRASS_TOP = 1,
            GRASS_SIDE,
            STONE,
            SAND,
            BLUE,
            DIRT,
            SNOW,
        };

        void init();
        unsigned int getArrayID();
    } // namespace Texture

} // namespace Minicube
