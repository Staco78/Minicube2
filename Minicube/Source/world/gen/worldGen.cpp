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
                double moutains = perlinNoiseContext->moutainsNoise.noise(_x, _y);

                data[i].humidity = humidity;
                data[i].temperature = temperature;
                data[i].moutains = moutains;

                double totalHeight = 0.0;
                double totalWeight = 0.0;

                BIOME mainBiome;
                double maxWeight = 0.0;

                for (int j = 0; j < BIOME_COUNT; j++)
                {
                    double weight = 0.0;
                    if (j == MOUNTAINS)
                    {
                        if (height > 0.5 || (moutains > 0.7 && height > 0.6))
                        {
                            totalWeight = 3.0;
                            totalHeight = pow(height, biomes[MOUNTAINS].exp) * biomes[MOUNTAINS].scale;
                            mainBiome = MOUNTAINS;
                            break;
                        }
                        else
                        {
                            double dx = std::max(0.7 - moutains, 0.0);
                            double dy = std::max(0.5 - height, 0.0);
                            double distance = std::min(dx, dy);
                            if (distance < 0.1)
                            {
                                distance *= 10.0;
                                distance = 1.0 - distance;
                                weight = 6 * (distance * distance * distance * distance * distance) - 15 * (distance * distance * distance * distance) + 10 * (distance * distance * distance);
                                weight *= 3;
                            }
                        }
                        totalWeight += weight;
                        totalHeight += pow(height, biomes[MOUNTAINS].exp) * biomes[MOUNTAINS].scale * weight;
                    }
                    else
                    {
                        BiomeData biome = biomes[j];
                        if (humidity > biome.startHumidity && humidity < biome.endHumidity && temperature > biome.startTemperature && temperature < biome.endTemperature)
                        {
                            weight = 1.0;
                        }
                        else
                        {
                            double dx = std::min(abs(biome.startHumidity - humidity), abs(biome.endHumidity - humidity));
                            double dy = std::min(abs(biome.startTemperature - temperature), abs(biome.endTemperature - temperature));
                            double distance = std::min(dx, dy);
                            if (distance < 0.05)
                            {
                                distance *= 20.0;
                                distance = 1.0 - distance;
                                weight = 6 * (distance * distance * distance * distance * distance) - 15 * (distance * distance * distance * distance) + 10 * (distance * distance * distance);
                            }
                        }
                        totalWeight += weight;
                        totalHeight += pow(height, biome.exp) * biome.scale * weight;
                    }
                    if (weight >= maxWeight)
                    {
                        maxWeight = weight;
                        mainBiome = (BIOME)j;
                    }
                    data[i].biomes[j].biome = (BIOME)j;
                    data[i].biomes[j].weight = weight;
                }

                totalHeight /= totalWeight;
                data[i].height = totalHeight * 80 + 30;
                data[i].biome = mainBiome;

                if (data[i].height > maxHeight)
                    maxHeight = data[i].height;
            }
        }
    } // namespace WorldGen

} // namespace Minicube
