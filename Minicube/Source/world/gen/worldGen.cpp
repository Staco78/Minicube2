#include "world/gen/worldGen.h"

namespace Minicube
{
    namespace WorldGen
    {

        BIOME getBiome(int x, int y, PerlinNoiseContext *perlinNoiseContext)
        {

                    double humidity = perlinNoiseContext->humidityNoise.noise(x, y);
            double temperature = perlinNoiseContext->temperatureNoise.noise(x, y);

            BIOME biome;

            if (humidity < 0.5)
            {
                if (temperature < 0.5)
                {
                    biome = TUNDRA;
                }
                else
                {
                    biome = OCEAN;
                }
            }
            else
            {
                if (temperature < 0.5)
                {
                    biome = MOUNTAINS;
                }
                else
                {
                    biome = FOREST;
                }
            }

            return biome;
        }

#define BLEND_RADIUS 8
#define BLEND_ARRAY_LENGTH (BLEND_RADIUS * 2 + 1) * (BLEND_RADIUS * 2 + 1)

        double blurArray[BLEND_ARRAY_LENGTH];

        void init()
        {
            double weightTotal = 0.0;
            for (int iz = 0; iz < BLEND_RADIUS * 2 + 1; iz++)
            {
                int idz = iz - BLEND_RADIUS;
                for (int ix = 0; ix < BLEND_RADIUS * 2 + 1; ix++)
                {
                    int idx = ix - BLEND_RADIUS;
                    float thisWeight = (BLEND_RADIUS - idx - idz);
                    if (thisWeight <= 0)
                        continue; // We only compute for the circle of positive values of the blending function.
                    // thisWeight *= thisWeight; // Make transitions smoother.
                    weightTotal += thisWeight;
                    blurArray[iz * (BLEND_RADIUS * 2 + 1) + ix] = thisWeight;
                }
            }

            // Rescale the weights, so they all add up to 1.
            for (int i = 0; i < BLEND_ARRAY_LENGTH; i++)
                blurArray[i] /= weightTotal;
        }

        void blend(int x, int y, std::vector<BiomeNear> &results, PerlinNoiseContext *perlinNoiseContext)
        {

            for (int iz = 0; iz < BLEND_RADIUS * 2 + 1; iz++)
            {
                int idz = iz - BLEND_RADIUS;
                for (int ix = 0; ix < BLEND_RADIUS * 2 + 1; ix++)
                {
                    int idx = ix - BLEND_RADIUS;

                    // Weight of the blur kernel over this point
                    float thisWeight = blurArray[iz * (BLEND_RADIUS * 2 + 1) + ix];
                    if (thisWeight <= 0)
                        continue; // We can skip when it's zero

                    // Biome at this square within the blending circle
                    BIOME thisBiome = getBiome(x + idx * 3, y + idz * 3, perlinNoiseContext);

                    // If we've already seen this biome at least once, keep adding to its weight
                    bool foundEntry = false;
                    for (auto &entry : results)
                    {
                        if (entry.biome == thisBiome)
                        {
                            entry.weight += thisWeight;
                            foundEntry = true;
                            break;
                        }
                    }

                    // Otherwise create a new results entry.
                    if (!foundEntry)
                    {
                        results.push_back({thisBiome, thisWeight});
                    }
                }
            }
        }

        HeightMap::HeightMap(int x, int y, PerlinNoiseContext *perlinNoiseContext)
        {
            std::vector<BiomeNear> results;
            for (int i = 0; i < 256; i++)
            {
                int _x = (x * 16 + i / 16);
                int _y = (y * 16 + i % 16);
                double height = perlinNoiseContext->heightNoise.noise(_x, _y);
                double humidity = perlinNoiseContext->humidityNoise.noise(_x, _y);
                double temperature = perlinNoiseContext->temperatureNoise.noise(_x, _y);

                data[i].humidity = humidity;
                data[i].temperature = temperature;
                data[i].moutains = 0;

                blend(_x, _y, results, perlinNoiseContext);

                double totalHeight = 0.0;
                double totalWeight = 0.0;

                BIOME mainBiome;
                double mainBiomeWeight = 0.0;

                for (auto &entry : results)
                {
                    totalHeight += pow(height, biomes[entry.biome].exp) * biomes[entry.biome].scale * entry.weight;
                    totalWeight += entry.weight;
                    if (entry.weight > mainBiomeWeight)
                    {
                        mainBiome = entry.biome;
                        mainBiomeWeight = entry.weight;
                    }
                }
                results.clear();

                data[i].height = (totalHeight / totalWeight) * 80 + 30;
                data[i].biome = mainBiome;

                if (data[i].height > maxHeight)
                    maxHeight = data[i].height;
            }
        }
    } // namespace WorldGen

} // namespace Minicube
