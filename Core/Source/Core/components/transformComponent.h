#pragma once
#include "component.h"

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace RED::Components
{
    class TransformComponent : public Component
    {
    public:
        TransformComponent();
        ~TransformComponent();

        glm::mat4 getMatrix() const;

        glm::vec3 translation = glm::vec3(0.f);
        glm::vec3 rotation = glm::vec3(0.f);
        glm::vec3 scale = glm::vec3(1.f);
    private:
    };
    
} 
