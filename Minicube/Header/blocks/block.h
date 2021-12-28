#pragma once

#include "renderer/textures.h"
#include "utils.h"

namespace Minicube
{

    namespace Blocks
    {
        enum BlockId
        {
            AIR,
            GRASS,
            DIRT,
            STONE,
            SAND,
            BLUE
        };
    }

    class Block
    {
    public:
        explicit inline Block(Blocks::BlockId id)
        {
            Block::id = id;
        }
        virtual Textures::TextureId getTexture(utils::Direction direction) = 0;
        Blocks::BlockId id;
    };
}