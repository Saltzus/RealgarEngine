#pragma once

#include "Renderer.h"
#include "Texture.h"

namespace RED
{
    class Sprite
    {
    public:
        Sprite(GLFWwindow* window);
        ~Sprite();

        void Render(Shader* shader, Camera* camera, glm::mat4 model = glm::mat4(1.0f));

		Texture* texture;  

        glm::mat4 model = glm::mat4(1.0f);
	    glm::vec3 translation = glm::vec3(0.f, 0.f, 0.f);
	    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	    glm::vec3 scale = glm::vec3(0.7f, 0.7f, 0.7f);

    private:
        Renderer* renderer;

        std::vector<GLfloat> vertices =
        {
            -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
             0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f
        };
	    //Indices for vertices order
	    std::vector<GLuint> indices =
	    {
	    	0, 1, 3, // first triangle
	    	1, 2, 3  // second triangle
	    };
    };
    
} 
