#include "world/gen/heightMap.h"

namespace Minicube
{
    HeightMap::HeightMap(int x, int y, PerlinNoiseContext *perlinNoiseContext)
    {
        for (int i = 0; i < 256; i++)
        {
            data[i] = perlinNoiseContext->noise((x * 16 + i / 16.0) * 0.005, (y * 16 + i % 16) * 0.005) * 60;
            if (data[i] > maxHeight)
                maxHeight = data[i];
        }
    }

} // namespace Minicube
