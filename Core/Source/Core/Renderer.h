#pragma once

#include <vector>
#include "graphicsApi/opengl/OpenGlManager.h"
#include "graphicsApi/vulkan/VulkanManager.h"
#include "Shader.h"
#include "Camera.h"

#include "RenderImpl.h"

namespace RED
{
    enum class GraphicsApis 
    {
        OpenGL,
        DirectX,
        Vulkan
    };

    class Renderer
    {
    private:
        static ApiImpl* Api;
        RendererImpl* impl = nullptr;
        const static GraphicsApis graphicApi;
    public:
        Renderer(std::vector<unsigned int>& indices, std::vector<float>& vertices);
        ~Renderer();

        void Render(Shader* shader, Camera* camera, glm::mat4 model = glm::mat4(1.0f));
        static GraphicsApis GetGraphicsApi() {return graphicApi;};
        static void InitApi(GLFWwindow* window); 
        static void RenderGraphics();
    };
}
