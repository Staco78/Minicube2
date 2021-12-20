#pragma once

#include <Glew/glew.h>

#include <iostream>
#include <string>

#define TEXTURE_SIZE 16
#define TEXTURE_COUNT 2

namespace Minicube
{
    namespace Textures
    {

        enum TextureId
        {
            GRASS = 1,
            STONE
        };

        void init();
        unsigned int getArrayID();
    } // namespace Texture

} // namespace Minicube
