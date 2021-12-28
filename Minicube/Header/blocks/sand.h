#pragma once

#include "block.h"

namespace Minicube
{
    namespace Blocks
    {
        class Sand : public Block
        {
        public:
            using Block::Block;
            Textures::TextureId getTexture(utils::Direction direction) override
            {
                return Textures::TextureId::SAND;
            }
        };
    } // namespace Blocks

} // namespace Minicube
