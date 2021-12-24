#include "world/gen/worldGen.h"

namespace Minicube
{
    BIOME biomeMap[1][1] = {
        {OCEAN}};

    BIOME getBiome(double height, double humidity, double temperature)
    {
        return VOID;
    }

    HeightMap::HeightMap(int x, int y, PerlinNoiseContext *perlinNoiseContext)
    {
        for (int i = 0; i < 256; i++)
        {
            double _x = (x * 16 + i / 16.0);
            double _y = (y * 16 + i % 16);
            double height = perlinNoiseContext->heightNoise.octavePerlin(_x * 0.0028, _y * 0.0028, 6, 0.45);
            double exp = perlinNoiseContext->expNoise.octavePerlin(_x * 0.001, _y * 0.001, 2, 0.5);

            height = pow(height * 1.2, exp + 1);

            data[i].biome = VOID;

            if (isnan(height))
            {
                height = 0;
            }

            data[i].height = height * 100 + 30;

            assert(data[i].height >= 30);

            if (data[i].height > maxHeight)
                maxHeight = data[i].height;
        }

        // std::cout << maxHeight << std::endl;
    }
} // namespace Minicube
