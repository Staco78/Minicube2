#include <vector>

namespace Minicube
{
    namespace utils
    {
        inline glm::uvec3 getBlockOffset(int x, int y, int z)
        {
            if (x % 16 == 0)
                x = 0;
            else if (x >= 0)
                x = x % 16;
            else
                x = 16 + (x % 16);

            if (y % 16 == 0)
                y = 0;
            else if (y >= 0)
                y = y % 16;
            else
                y = 16 + (y % 16);

            if (z % 16 == 0)
                z = 0;
            else if (z >= 0)
                z = z % 16;
            else
                z = 16 + (z % 16);

            return glm::uvec3(x, y, z);
        }

    } // namespace utils

    namespace Vertices
    {
        namespace cube
        {

            static std::vector<float> back = {
                // Back face
                -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, // Bottom-left
                0.5f, -0.5f, 0.5f, 0.0f, 1.0f,  // bottom-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f,   // top-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f,   // top-right
                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,  // top-left
                -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, // bottom-left
            };
            static std::vector<float> front = {
                // Front face
                -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // bottom-left
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f,   // top-right
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f,   // top-right
                -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // bottom-left
                -0.5f, 0.5f, -0.5f, 1.0f, 0.0f,  // top-left
            };
            static std::vector<float> left = {
                // Left face
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f,   // top-right
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // bottom-left
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f,  // top-left
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // bottom-left
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f,   // top-right
                0.5f, -0.5f, 0.5f, 0.0f, 1.0f,  // bottom-right
            };
            static std::vector<float> right = {
                // Right face
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,  // top-right
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top-left
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
                -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,  // bottom-left
                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top-left
            };
            static std::vector<float> bottom = {
                // Bottom face
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // top-left
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
            };
            static std::vector<float> top = {
                // Top face
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-right
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f   // bottom-left
            };
        }
    } // namespace Vertices

} // namespace Minicube
