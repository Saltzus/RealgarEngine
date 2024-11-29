#include "Texture.h"

namespace RED
{
    Texture::Texture(const char* filePath)
    {
        if (Renderer::GetGraphicsApi() == GraphicsApis::OpenGL)
        {
            this->impl = new RED::Opengl::OpenglTexture(filePath);
        }
    }
    Texture::Texture(unsigned char* data, int format, int width, int height)
    {
        if (Renderer::GetGraphicsApi() == GraphicsApis::OpenGL)
        {
            this->impl = new RED::Opengl::OpenglTexture(data, format, width, height);
        }
    }
    
    Texture::~Texture()
    {
        delete this->impl;
    }

    void Texture::Bind()
    {
        this->Bind();
    }
}
