#include "world/gen/worldGen.h"

namespace Minicube
{
    namespace WorldGen
    {

        HeightMap::HeightMap(int x, int y, PerlinNoiseContext *perlinNoiseContext)
        {
            for (int i = 0; i < 256; i++)
            {
                int _x = (x * 16 + i / 16);
                int _y = (y * 16 + i % 16);
                double height = perlinNoiseContext->heightNoise.noise(_x, _y);
                double humidity = perlinNoiseContext->humidityNoise.noise(_x, _y);
                double temperature = perlinNoiseContext->temperatureNoise.noise(_x, _y);

                data[i].humidity = humidity;
                data[i].temperature = temperature;

                double totalHeight = 0.0;
                double totalWeight = 0.0;

                BIOME mainBiome;
                double maxWeight = 0.0;

                for (int j = 0; j < BIOME_COUNT; j++)
                {
                    BiomeData biome = biomes[j];
                    double weight = 0.0;
                    if (humidity >= biome.startHumidity && humidity <= biome.endHumidity && temperature >= biome.startTemperature && temperature <= biome.endTemperature)
                    {
                        weight = 1.0;
                    }
                    else
                    {
                        double dx = std::min(abs(biome.startHumidity - humidity), abs(biome.endHumidity - humidity));
                        double dy = std::min(abs(biome.startTemperature - temperature), abs(biome.endTemperature - temperature));
                        double distance = std::min(dx, dy);
                        if (distance < 0.03)
                        {
                            distance *= 33.3;
                            distance = 1.0 - distance;
                            weight = 6 * (distance * distance * distance * distance * distance) - 15 * (distance * distance * distance * distance) + 10 * (distance * distance * distance);
                        }
                    }
                    if (weight >= maxWeight)
                    {
                        maxWeight = weight;
                        mainBiome = (BIOME)j;
                    }
                    totalWeight += weight;
                    totalHeight += pow(height, biome.exp) * biome.scale * weight;

                    data[i].biomes[j].biome = (BIOME)j;
                    data[i].biomes[j].weight = weight;
                }

                totalHeight /= totalWeight;
                data[i].height = totalHeight * 80 + 30;
                data[i].blockId = biomes[mainBiome].block;

                if (data[i].height > maxHeight)
                    maxHeight = data[i].height;
            }
        }
    } // namespace WorldGen

} // namespace Minicube
