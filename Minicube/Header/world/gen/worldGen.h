#pragma once

#include "noise.h"
#include "blocks/block.h"

#include <iostream>
#include <cmath>

namespace Minicube
{

#define BIOME_COUNT 14

    namespace WorldGen
    {
        enum BIOME
        {
            OCEAN,
            RIVER,
            BEACH,
            DESERT,
            SAVANNA,
            JUNGLE,
            GRASSLAND,
            WOODLAND,
            FOREST,
            RAINFOREST,
            TAIGA,
            TUNDRA,
            ICE,
            MOUNTAIN
        };

        static char *biomeNames[BIOME_COUNT] = {
            "Ocean",
            "River",
            "Beach",
            "Desert",
            "Savanna",
            "Jungle",
            "Grassland",
            "Woodland",
            "Forest",
            "Rainforest",
            "Taiga",
            "Tundra",
            "Ice",
            "Mountain",
        };

        struct BiomeData
        {
            Blocks::BlockId block;
            double exp;
            double scale;
            double startHumidity;
            double endHumidity;
            double startTemperature;
            double endTemperature;
        };

        static BiomeData biomes[BIOME_COUNT] = {
            { Blocks::SAND, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 }, // OCEAN
            { Blocks::SAND, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 }, // RIVER
            { Blocks::SAND, 1.3, 0.8, 0.0, 0.0, 0.0, 0.0 }, // BEACH
            { Blocks::SAND, 1.2, 0.6, 0.0, 0.3, 0.6, 1.0 }, // DESERT
            { Blocks::GRASS, 1.0, 1.0, 0.3, 0.6, 0.6, 1.0 }, // SAVANNA
            { Blocks::GRASS, 1.0, 1.0, 0.6, 1.0, 0.6, 1.0 }, // JUNGLE
            { Blocks::GRASS, 1.0, 1.0, 0.0, 0.3, 0.3, 0.6 }, // GRASSLAND
            { Blocks::GRASS, 1.0, 1.0, 0.3, 0.6, 0.3, 0.6 }, // WOODLAND
            { Blocks::GRASS, 1.0, 1.0, 0.6, 0.7, 0.5, 0.6 }, // FOREST
            { Blocks::GRASS, 1.0, 1.0, 0.7, 1.0, 0.5, 0.6 }, // RAINFOREST
            { Blocks::GRASS, 1.0, 1.0, 0.5, 1.0, 0.18, 0.4 }, // TAIGA
            { Blocks::GRASS, 1.0, 1.0, 0.5, 1.0, 0.05, 0.18 }, // TUNDRA
            { Blocks::GRASS, 1.0, 1.0, 0.0, 1.0, 0.0, 0.05 }, // ICE
            { Blocks::STONE, 3.0, 1.2, 0.0, 0.0, 0.0, 0.0 }, // MOUNTAIN
        };

        struct BiomeNear
        {
            BIOME biome;
            double weight;
        };

        struct HeightMapData
        {
            unsigned int height;
            // BiomeNear biomes[BIOME_COUNT];
            BIOME biome;
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
