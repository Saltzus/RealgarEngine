#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#include <vector>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "../../RenderImpl.h"

namespace RED::Vulkan
{
    // Structure to standardize the vertices used in the sprites
    struct Vertex
    {
    	glm::vec3 position;
    	glm::vec2 texUV;
    };
    
    class VulkanTexture : public TextureImpl
    {
    public:
        VulkanTexture(const char* filePath);
        VulkanTexture(unsigned char* data, int format, int width, int height);
        ~VulkanTexture();
        void Bind() override;
    private:
        GLuint texture;
    };

    class Vulkan
    {
    public:
        Vulkan();
        ~Vulkan();

    private:
        void createInstance();

        VkInstance instance;
    };

    class VulkanRenderer : public RendererImpl
    {
    public:
        VulkanRenderer(std::vector<GLuint>& indices, std::vector<GLfloat>& vertices);
        ~VulkanRenderer();

        void Render(Shader* shader, Camera* camera, glm::mat4 model) override; // Declare draw

    private:
        GLuint VAO;
	    GLuint VBO;
    	GLuint EBO;
    };
}
    