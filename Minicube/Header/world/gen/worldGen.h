#pragma once

#include "noise.h"

#include <iostream>
#include <cmath>

namespace Minicube
{
    enum BIOME
    {
        OCEAN,
        BEACH,
        DESERT,
        GRASSLAND,
        FOREST,
        TAIGA,
        SNOW,
        TUNDRA,
        MOUNTAIN,
        HILLS,
        SWAMP,
        RIVER,
        LAKE,
        VOID
    };

    struct HeightMapData
    {
        unsigned int height;
        BIOME biome;
    };

    BIOME getBiome(double height, double humidity, double temperature, double moutains);

    class HeightMap
    {
    public:
        HeightMap(int x, int y, PerlinNoiseContext *perlinNoiseContext);
        HeightMapData data[256];
        unsigned int maxHeight = 0;
    };
} // namespace Minicube
