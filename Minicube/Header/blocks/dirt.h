#pragma once

#include "block.h"

namespace Minicube
{
    namespace Blocks
    {
        class Dirt : public Block
        {
        public:
            using Block::Block;
            Textures::TextureId getTexture(utils::Direction direction) override
            {
                return Textures::TextureId::DIRT;
            }
        };
    } // namespace Blocks

} // namespace Minicube
