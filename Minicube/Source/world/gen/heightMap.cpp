#include "world/gen/heightMap.h"

namespace Minicube
{
    HeightMap::HeightMap(int x, int y, PerlinNoiseContext *perlinNoiseContext)
    {
        for (int i = 0; i < 256; i++)
        {
            double _x = (x * 16 + i / 16.0) * 0.0028;
            double _y = (y * 16 + i % 16) * 0.0028;
            data[i] = perlinNoiseContext->octavePerlin(_x, _y, 6, 0.45) * 80 + 30;
            if (data[i] > maxHeight)
                maxHeight = data[i];
        }
    }

} // namespace Minicube
