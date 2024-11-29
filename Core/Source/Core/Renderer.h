#pragma once

#include <vector>
#include "OpenGlManager.h"
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
        RendererImpl* impl = nullptr;
        static const GraphicsApis graphicApi = GraphicsApis::OpenGL;

    public:
        Renderer(std::vector<unsigned int>& indices, std::vector<float>& vertices);
        ~Renderer();

        void Render(Shader* shader, Camera* camera, glm::mat4 model = glm::mat4(1.0f));
        static GraphicsApis GetGraphicsApi() {return graphicApi;};
    };
}
