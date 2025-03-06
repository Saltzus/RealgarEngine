#include "Shader.h"

#include "graphicsApi/opengl/OpenglShader.h"
#include "graphicsApi/vulkan/VulkanShader.h"

namespace Realgar
{
    Shader::Shader(const char* vertexFile, const char* fragmentFile) 
    {
        shader = { vertexFile, fragmentFile };

        switch (Renderer::GetGraphicsApi())
        {
        case GraphicsApis::OpenGL:
            this->impl = new Realgar::Opengl::OpenglShader(vertexFile, fragmentFile);
            break;
        case GraphicsApis::Vulkan:
            this->impl = new Realgar::Vulkan::VulkanShader(vertexFile, fragmentFile);
            break;
        default:
            this->impl = new Realgar::Opengl::OpenglShader(vertexFile, fragmentFile);
            break;
        }
    }
    Shader::~Shader()
    {
        delete this->impl;
    }

    void Shader::Activate()
    {
        this->impl->Activate();
    }
    void Shader::Delete()
    {
        this->impl->Delete();
    }
    unsigned int& Shader::ID()
    {
        return this->impl->Id();
    }
}
