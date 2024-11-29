#include "Renderer.h"

namespace RED
{
    Renderer::Renderer(std::vector<unsigned int>& indices, std::vector<float>& vertices)
    {
        if (this->graphicApi == GraphicsApis::OpenGL)
        {
            this->impl = new RED::Opengl::OpenglRenderer(indices, vertices);
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
}
