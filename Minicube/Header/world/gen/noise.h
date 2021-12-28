#pragma once

#include <vector>
#include <chrono>

#include "Glm/vec2.hpp"
#include "Glm/glm.hpp"

namespace Minicube
{
    class PerlinNoise
    {
    public:
        PerlinNoise(unsigned int seed, double multiplier, unsigned int octaves, double persistence);
        double noise(int x, int y);
    private:
        std::vector<int> P;

        double octavePerlin(double x, double y, int octaves, double persistence);
        double perlin(double x, double y);
        void shuffle(std::vector<int> *tab);
        void makePermutation(unsigned int seed);
        glm::vec2 getConstantVector(int v);
        double fade(double t);
        double lerp(double t, double a1, double a2);

        double m_multiplier;
        unsigned int m_octaves;
        double m_persistence;
    };

    class PerlinNoiseContext
    {
    public:
        PerlinNoiseContext(int seed);

        PerlinNoise heightNoise;
        PerlinNoise humidityNoise;
        PerlinNoise temperatureNoise;
        PerlinNoise r;
    };

} // namespace Minicube
