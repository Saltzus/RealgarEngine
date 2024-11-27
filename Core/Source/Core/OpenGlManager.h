#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <vector>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/rotate_vector.hpp>

#include "Shader.h"
#include "Camera.h"

namespace RED::Opengl
{
    // Structure to standardize the vertices used in the sprites
    struct Vertex
    {
    	glm::vec3 position;
    	glm::vec2 texUV;
    };
    
    class OpenglRenderer
    {
    public:
        OpenglRenderer(std::vector<GLuint>& indices, std::vector<GLfloat>& vertices);
        ~OpenglRenderer();    

	    glm::mat4 model = glm::mat4(1.0f);
	    glm::vec3 translation = glm::vec3(0.f, 0.f, 0.f);
	    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	    glm::vec3 scale = glm::vec3(0.7f, 0.7f, 0.7f);

	    void Render(Shader* shader, Camera* camera, glm::mat4 model = glm::mat4(1.0f));

        private:
            GLuint VAO;
	        GLuint VBO;
    	    GLuint EBO;

            GLuint texture;
    };
}
    