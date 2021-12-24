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
        PerlinNoise(unsigned int seed);
        double perlin(double x, double y);
        double octavePerlin(double x, double y, int octaves, double persistence);

    private:
        std::vector<int> P;

        void shuffle(std::vector<int> *tab);
        void makePermutation(unsigned int seed);
        glm::vec2 getConstantVector(int v);
        double fade(double t);
        double lerp(double t, double a1, double a2);
    };

    class PerlinNoiseContext
    {
    public:
        PerlinNoiseContext(int seed);

        PerlinNoise heightNoise;
        PerlinNoise expNoise;
        PerlinNoise temperatureNoise;
        PerlinNoise moutainsNoise;
    };

} // namespace Minicube
