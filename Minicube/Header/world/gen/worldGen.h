#pragma once

#include "noise.h"
#include "blocks/block.h"

#include <iostream>
#include <cmath>
#include <map>

namespace Minicube
{

#define BIOME_COUNT 7

    namespace WorldGen
    {
        enum BIOME
        {
            DESERT,
            FOREST,
            GRASSLAND,
            JUNGLE,
            MOUNTAINS,
            OCEAN,
            TUNDRA
        };

        static char *biomeNames[BIOME_COUNT] = {
            "Desert",
            "Forest",
            "Grassland",
            "Jungle",
            "Mountains",
            "Ocean",
            "Tundra",
        };

        struct BiomeData
        {
            Blocks::BlockId block;
            double exp;
            double scale;
            double minHeight;
            double minHumidity;
            double minTemperature;
        };

        static BiomeData biomes[BIOME_COUNT] = {
            {Blocks::BlockId::SAND, 0.8, 0.8, 0.2, 0.0, 0.5},  // Desert
            {Blocks::BlockId::GRASS, 1.0, 1.0, 0.2, 0.4, 0.4}, // Forest
            {Blocks::BlockId::GRASS, 1.0, 1.0, 0.2, 0.5, 0.3}, // Grassland
            {Blocks::BlockId::GRASS, 1.2, 1.2, 0.3, 0.5, 0.6}, // Jungle
            {Blocks::BlockId::STONE, 3.0, 3.0, 0.5, 0.0, 0.0}, // Mountains
            {Blocks::BlockId::BLUE, 0.7, 0.7, 0.0, 0.0, 0.0},  // Ocean
            {Blocks::BlockId::GRASS, 1.0, 1.0, 0.2, 0.0, 0.0}, // Tundra
        };

        struct BiomeNear
        {
            BIOME biome;
            double weight;
        };

        struct HeightMapData
        {
            unsigned int height;
            BiomeNear biomes[BIOME_COUNT];
            BIOME biome;
            double temperature;
            double humidity;
            double moutains;
        };

        void init();

        class HeightMap
        {
        public:
            HeightMap(int x, int y, PerlinNoiseContext *perlinNoiseContext);
            HeightMapData data[256];
            unsigned int maxHeight = 0;
        };
    } // namespace Worlgen

} // namespace Minicube
