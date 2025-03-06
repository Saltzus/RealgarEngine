#pragma once

#include "RenderImpl.h"
#include "Renderer.h"

namespace Realgar
{
    class Texture
    {
    private:
        TextureImpl* impl = nullptr;
    public:
        Texture(const char* filePath);
        Texture(unsigned char* data, int format, int width, int height);
        ~Texture();
        void virtual Bind();

        static Texture* GetDefaultTexture();
    };

    
} 