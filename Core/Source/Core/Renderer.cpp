#include "Renderer.h"

namespace Realgar
{
    ApiImpl* Renderer::Api = nullptr;
    const GraphicsApis Renderer::graphicApi = GraphicsApis::Vulkan;

    Renderer::Renderer(std::vector<unsigned int>& indices, std::vector<float>& vertices)
    {
        switch (this->graphicApi)
        {
        case GraphicsApis::OpenGL:
            this->impl = new Realgar::Opengl::OpenglRenderer(indices, vertices);
            break;
        case GraphicsApis::Vulkan:
            this->impl = new Realgar::Vulkan::VulkanRenderer(indices, vertices);
            break;
        default:
            this->impl = new Realgar::Opengl::OpenglRenderer(indices, vertices);
            break;
        }
    }
    
    Renderer::~Renderer()
    {
        delete this->impl;
    }

    void Renderer::Render(Shader* shader, Camera* camera, glm::mat4 model)
    {
        this->impl->Render(shader, camera, model);
    }

    void Renderer::InitApi(GLFWwindow* window)
    {
        switch (graphicApi)
        {
        case GraphicsApis::OpenGL:
            Api = new Realgar::Opengl::Opengl(window);
            break;
        case GraphicsApis::Vulkan:
            Api = new Realgar::Vulkan::Vulkan(window);
            break;
        default:
            Api = new Realgar::Opengl::Opengl(window);
            break;
        }
    }

    void Renderer::RenderGraphics()
    {
        if (graphicApi == GraphicsApis::Vulkan)
            Api->render();
    }
}
