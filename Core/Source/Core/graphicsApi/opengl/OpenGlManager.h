#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <vector>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "../../RenderImpl.h"

namespace RED::Opengl
{
    // Structure to standardize the vertices used in the sprites
    struct Vertex
    {
    	glm::vec3 position;
    	glm::vec2 texUV;
    };
    
    class OpenglTexture : public TextureImpl
    {
    public:
        OpenglTexture(const char* filePath);
        OpenglTexture(unsigned char* data, int format, int width, int height);
        ~OpenglTexture();  
        void Bind() override;
    private:
        GLuint texture;
    };

    class OpenglRenderer : public RendererImpl
    {
    public:
        OpenglRenderer(std::vector<GLuint>& indices, std::vector<GLfloat>& vertices);
        ~OpenglRenderer();    

        void Render(Shader* shader, Camera* camera, glm::mat4 model) override; // Declare draw

    private:
        GLuint VAO;
	    GLuint VBO;
    	GLuint EBO;

        GLuint UBO;
    };
}
    