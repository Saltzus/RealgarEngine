#include "Sprite.h"

namespace RED
{
    

    Sprite::Sprite(GLFWwindow* window)
    {
        texture = Texture::GetDefaultTexture();
        this->renderer = new Renderer(indices, vertices);
    }
    
    Sprite::~Sprite()
    {
        delete this->renderer;
        delete this->texture;
    }

    void Sprite::Render(Shader* shader, Camera* camera, glm::mat4 model)
    {
        shader->Activate();  
        texture->Bind();

        model = glm::translate(model, translation);

        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::scale(model, scale);

        this->renderer->Render(shader, camera, model);
    }
}