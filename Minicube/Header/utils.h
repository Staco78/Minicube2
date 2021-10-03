#include <vector>

namespace Minicube
{
    namespace utils
    {
        enum class Side
        {
            back,
            front,
            left,
            right,
            bottom,
            top
        };
    } // namespace Utils

    namespace Vertexs
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
    } // namespace Vertexs

} // namespace Minicube
