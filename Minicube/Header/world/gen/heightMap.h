#pragma once

#include "noise.h"

namespace Minicube
{
    class HeightMap
    {
    public:
        HeightMap(int x, int y, PerlinNoiseContext *perlinNoiseContext);
        unsigned int data[256];
        unsigned int maxHeight = 0;
    };
} // namespace Minicube
