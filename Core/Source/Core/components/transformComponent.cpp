#include "transformComponent.h"

namespace RED::Components
{
    TransformComponent::TransformComponent() {}
    TransformComponent::~TransformComponent() {}

    glm::mat4 TransformComponent::getMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, translation);

        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::scale(model, scale);

        return model;
    }
}