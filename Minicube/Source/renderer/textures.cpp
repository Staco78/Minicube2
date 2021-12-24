#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "renderer/textures.h"

namespace Minicube
{
    namespace
    {
        unsigned int ID;
    }
    namespace Textures
    {

        void load(std::string path, TextureId index)
        {
            int width, height, nrChannels;
            unsigned char *data = stbi_load(std::string("assets/textures/" + path).c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                if (nrChannels == 3)
                {
                    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                                    0, 0, (int)index,
                                    width, height, 1,
                                    GL_RGB, GL_UNSIGNED_BYTE, data);
                }
                else if (nrChannels == 4)
                {
                    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                                    0, 0, (int)index,
                                    width, height, 1,
                                    GL_RGBA, GL_UNSIGNED_BYTE, data);
                }
                else
                {
                    std::cout << "Texture " << path << " has wrong number of channels: " << nrChannels << std::endl;
                }
            }
            else
            {
                std::cout << "Failed to load texture " << path << std::endl;
            }
            stbi_image_free(data);
        }

        void init()
        {
            glGenTextures(1, &ID);
            glBindTexture(GL_TEXTURE_2D_ARRAY, ID);

            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8,
                         TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_COUNT + 1, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

            load("grass.png", TextureId::GRASS);
            load("stone.png", TextureId::STONE);
            load("sand.png", TextureId::SAND);
            load("blue.png", TextureId::BLUE);

            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        }

        unsigned int getArrayID()
        {
            return ID;
        }
    } // namespace Textures

} // namespace Minicube
