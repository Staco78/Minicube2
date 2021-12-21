#pragma once

#include <vector>
#include <chrono>

#include "Glm/vec2.hpp"
#include "Glm/glm.hpp"

namespace Minicube
{
    class PerlinNoiseContext
    {
    public:
        PerlinNoiseContext();
        double noise(double x, double y);

    private:
        std::vector<int> P;

        void shuffle(std::vector<int> *tab);
        void makePermutation();
        glm::vec2 getConstantVector(int v);
        double fade(double t);
        double lerp(double t, double a1, double a2);
    };

} // namespace Minicube
