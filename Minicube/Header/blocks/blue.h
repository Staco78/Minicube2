#pragma once

#include "block.h"

namespace Minicube
{
    namespace Blocks
    {
        class Blue : public Block
        {
        public:
            using Block::Block;
            Textures::TextureId getTexture(utils::Direction direction) override
            {
                return Textures::TextureId::BLUE;
            }
        };
    } // namespace Blocks

} // namespace Minicube
