#include "Texture.h"

namespace Realgar
{
    Texture::Texture(const char* filePath)
    {
        path = filePath;

        switch (Renderer::GetGraphicsApi())
        {
        case GraphicsApis::OpenGL:
            this->impl = new Realgar::Opengl::OpenglTexture(filePath);
            break;
        case GraphicsApis::Vulkan:
            this->impl = new Realgar::Vulkan::VulkanTexture(filePath);
            break;
        default:
            this->impl = new Realgar::Opengl::OpenglTexture(filePath);
            break;
        }
    }
    Texture::Texture(unsigned char* data, int format, int width, int height)
    {
        switch (Renderer::GetGraphicsApi())
        {
        case GraphicsApis::OpenGL:
            this->impl = new Realgar::Opengl::OpenglTexture(data, format, width, height);
            break;
        case GraphicsApis::Vulkan:
            this->impl = new Realgar::Vulkan::VulkanTexture(data, format, width, height);
            break;
        default:
            this->impl = new Realgar::Opengl::OpenglTexture(data, format, width, height);
            break;
        }
    }
    
    Texture::~Texture()
    {
        delete impl;
    }

    void Texture::Bind()
    {
        impl->Bind();
    }

    Texture* Texture::GetDefaultTexture()
    {
        static Texture* DEFAULT_TEXTURE = new Texture(Realgar::FileManager::getResource("Textures/PixelText.png"));
        return DEFAULT_TEXTURE;
    }
}
