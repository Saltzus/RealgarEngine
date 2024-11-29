#pragma once

#include "Shader.h"
#include "Camera.h"

namespace RED
{
    class RendererImpl 
    {
    public:
        virtual ~RendererImpl() {}
        virtual void Render(Shader* shader, Camera* camera, glm::mat4 model) = 0;
    };

    class TextureImpl
    {
    public:
        virtual ~TextureImpl() {}
        virtual void Bind() = 0;
    };
}