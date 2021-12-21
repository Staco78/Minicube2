#include "world/gen/noise.h"

namespace Minicube
{

    PerlinNoiseContext::PerlinNoiseContext()
    {
        makePermutation();
    }

    double PerlinNoiseContext::noise(double x, double y)
    {
        double X = (int)floor(x) & 255;
        double Y = (int)floor(y) & 255;

        double xf = x - (int)floor(x);
        double yf = y - (int)floor(y);

        glm::vec2 topRight(xf - 1.0, yf - 1.0);
        glm::vec2 topLeft(xf, yf - 1.0);
        glm::vec2 bottomRight(xf - 1.0, yf);
        glm::vec2 bottomLeft(xf, yf);

        // Select a value in the array for each of the 4 corners
        int valueTopRight = P[P[X + 1] + Y + 1];
        int valueTopLeft = P[P[X] + Y + 1];
        int valueBottomRight = P[P[X + 1] + Y];
        int valueBottomLeft = P[P[X] + Y];

        double dotTopRight = glm::dot(topRight, getConstantVector(valueTopRight));
        double dotTopLeft = glm::dot(topLeft, getConstantVector(valueTopLeft));
        double dotBottomRight = glm::dot(bottomRight, getConstantVector(valueBottomRight));
        double dotBottomLeft = glm::dot(bottomLeft, getConstantVector(valueBottomLeft));

        double u = fade(xf);
        double v = fade(yf);

        double n = lerp(u, lerp(v, dotBottomLeft, dotTopLeft), lerp(v, dotBottomRight, dotTopRight));

        n += 1.0;
        n /= 2.0;

        return n;
    }

    void PerlinNoiseContext::shuffle(std::vector<int> *tab)
    {
        for (int e = tab->size() - 1; e > 0; e--)
        {
            int index = round((double)rand() / (double)RAND_MAX * (e - 1));
            int temp = tab->at(e);

            tab->operator[](e) = tab->operator[](index);
            tab->operator[](index) = temp;
        }
    }

    void PerlinNoiseContext::makePermutation()
    {
        srand(time(0));
        for (int i = 0; i < 256; i++)
        {
            P.push_back(i);
        }
        shuffle(&P);
        for (int i = 0; i < 256; i++)
        {
            P.push_back(P[i]);
        }
    }

    glm::vec2 PerlinNoiseContext::getConstantVector(int v)
    {
        // v is the value from the permutation table
        int h = v % 4;
        if (h == 0)
            return glm::vec2(1.0, 1.0);
        else if (h == 1)
            return glm::vec2(-1.0, 1.0);
        else if (h == 2)
            return glm::vec2(-1.0, -1.0);
        else
            return glm::vec2(1.0, -1.0);
    }

    double PerlinNoiseContext::fade(double t)
    {
        return ((6 * t - 15) * t + 10) * t * t * t;
    }

    double PerlinNoiseContext::lerp(double t, double a1, double a2)
    {
        return a1 + t * (a2 - a1);
    }

} // namespace Minicube