#include "Camera.h"

namespace Realgar
{
    Camera::Camera(int width, int height, glm::vec3 position)
    {
    	//Loads information
    	Camera::width = width;
    	Camera::height = height;
    	cameraPosition = -position;
    }

    void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane, bool ortho)
    {
    	view = glm::mat4(1.0f);
    	projection = glm::mat4(1.0f);

    	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

        glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));
        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

        view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    }
}