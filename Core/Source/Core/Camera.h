#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"
#include <lua.hpp>

namespace Realgar
{
    class Camera
    {
    public:
    	// Stores the main vectors of the camera
    	glm::vec3 cameraPosition;
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        glm::vec3 cameraRotation;

    	int width;
    	int height;

        float fov;
        float nearPlane;
        float farPlane;

    	Camera(int width, int height, glm::vec3 position, glm::vec3 rotation, float fov,  float nearPlane, float farPlane, bool ortho);

    	void updateMatrix();
    	void Matrix(Shader& shader, const char* uniform);

        static void registerCamera(lua_State* L);

    private:
    	bool perspective;
    	bool perspectiveCheck = false;

        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    };

}