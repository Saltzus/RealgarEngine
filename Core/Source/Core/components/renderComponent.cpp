#include "renderComponent.h"
#include "GameObject.h"
#include "Texture.h"

namespace RED::Components
{
    RenderComponent::RenderComponent(GameObject* object)
    {
        gameObject = object;

        texture = Texture::GetDefaultTexture();
        this->renderer = new Renderer(indices, vertices);
    }
    RenderComponent::~RenderComponent()
    {
        delete texture;
        delete renderer;
    }

    void RenderComponent::render(Shader* shader, Camera* camera)
    {
        TransformComponent* transformComponent = gameObject->getComponent<TransformComponent>();
        glm::mat4 modelMatrix = transformComponent != nullptr ? transformComponent->getMatrix() : glm::mat4(1.f);

        this->shader->Activate();
        texture->Bind();
            
        this->renderer->Render(this->shader, camera,  modelMatrix);
    }
}