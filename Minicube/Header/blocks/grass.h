#pragma once

#include "block.h"

namespace Minicube
{
    namespace Blocks
    {
        class Grass : public Block
        {
        public:
            using Block::Block;
            Textures::TextureId getTexture(utils::Direction direction) override
            {
                if ((int)direction & (int)utils::Direction::HORIZONTAL)
                    return Textures::TextureId::GRASS_SIDE;
                else if ((int)direction & (int)utils::Direction::Y_AXIS)
                    return Textures::TextureId::GRASS_TOP;
                else
                    return Textures::TextureId::DIRT;
            }
        };
    } // namespace Blocks

} // namespace Minicube
