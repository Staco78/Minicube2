#pragma once

#include "noise.h"
#include "world/block.h"

#include <iostream>
#include <cmath>

namespace Minicube
{

#define BIOME_COUNT 4

    namespace WorldGen
    {
        enum BIOME
        {
            DESERT,
            SAVANNA,
            PLAINS,
            FOREST
        };

        static char *biomeNames[BIOME_COUNT] = {
            "Desert",
            "Savanna",
            "Plains",
            "Forest",
        };

        struct BiomeData
        {
            BlockId block;
            double exp;
            double scale;
            double startHumidity;
            double endHumidity;
            double startTemperature;
            double endTemperature;
        };

        static BiomeData biomes[BIOME_COUNT] = {
            {BlockId::SAND, 1.0, 2.0, 0.0, 0.5, 0.5, 1.0},
            {BlockId::GRASS, 2.0, 1.0, 0.5, 1.0, 0.5, 1.0},
            {BlockId::STONE, 1.0, 1.0, 0.0, 0.5, 0.0, 0.5},
            {BlockId::BLUE, 0.5, 2.0, 0.5, 1.0, 0.0, 0.5},
        };

        struct BiomeNear
        {
            BIOME biome;
            double weight;
        };

        struct HeightMapData
        {
            unsigned int height;
            BlockId blockId;
            double temperature;
            double humidity;
            BiomeNear biomes[BIOME_COUNT];
        };

        class HeightMap
        {
        public:
            HeightMap(int x, int y, PerlinNoiseContext *perlinNoiseContext);
            HeightMapData data[256];
            unsigned int maxHeight = 0;
        };
    } // namespace Worlgen

} // namespace Minicube
