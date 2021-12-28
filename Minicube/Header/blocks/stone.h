#pragma once

#include "block.h"

namespace Minicube
{
    namespace Blocks
    {
        class Stone : public Block
        {
        public:
            using Block::Block;
            Textures::TextureId getTexture(utils::Direction direction) override
            {
                return Textures::TextureId::STONE;
            }
        };
    } // namespace Blocks

} // namespace Minicube
