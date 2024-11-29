#pragma once

#include "RenderImpl.h"
#include "Renderer.h"

namespace RED
{
    class Texture
    {
    private:
        TextureImpl* impl = nullptr;
    public:
        Texture() {};
        Texture(const char* filePath);
        Texture(unsigned char* data, int format, int width, int height);
        ~Texture();
        void Bind();
    };
} 