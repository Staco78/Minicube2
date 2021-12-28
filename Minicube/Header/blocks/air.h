#pragma once

#include "block.h"

namespace Minicube
{
    namespace Blocks
    {
        class Air : public Block
        {
        public:
            using Block::Block;
            Textures::TextureId getTexture(utils::Direction direction) override
            {
                assert(false);
                return (Textures::TextureId)0;
            }
        };
    } // namespace Blocks

} // namespace Minicube
