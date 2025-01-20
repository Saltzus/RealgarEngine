#include "Renderer.h"

namespace RED
{
    ApiImpl* Renderer::Api = nullptr;
    const GraphicsApis Renderer::graphicApi = GraphicsApis::Vulkan;

    Renderer::Renderer(std::vector<unsigned int>& indices, std::vector<float>& vertices)
    {
        switch (this->graphicApi)
        {
        case GraphicsApis::OpenGL:
            this->impl = new RED::Opengl::OpenglRenderer(indices, vertices);
            break;
        case GraphicsApis::Vulkan:
            this->impl = new RED::Vulkan::VulkanRenderer(indices, vertices);
            break;
        default:
            this->impl = new RED::Opengl::OpenglRenderer(indices, vertices);
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
            Api = new RED::Opengl::Opengl(window);
            break;
        case GraphicsApis::Vulkan:
            Api = new RED::Vulkan::Vulkan(window);
            break;
        default:
            Api = new RED::Opengl::Opengl(window);
            break;
        }
    }
}
