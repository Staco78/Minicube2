#include "blocks/block.h"

namespace Minicube
{

    std::vector<float> *Block::getFace(utils::Side side, const glm::uvec3 &pos)
    {
        return m_faces.get(side)->getChunkVertices(pos);
    }
} // namespace Minicube
