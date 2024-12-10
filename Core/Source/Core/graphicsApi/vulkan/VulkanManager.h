#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#include <stb_image/stb_image.h>
#include <vector>
#include <optional>
#include <set>

#include <GLFW/glfw3.h>

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
        Vulkan(GLFWwindow* GLFW_Window);
        ~Vulkan();

    private:
        void createInstance();
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        
        struct QueueFamilyIndices;
        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        void createSurface(GLFWwindow* window);

        struct SwapChainSupportDetails;
        void createSwapChain();
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        
        void createImageViews();

        void createGraphicsPipeline();
        VkShaderModule createShaderModule(const std::vector<char>& code);

        GLFWwindow* window;

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;

        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;

        void createRenderPass();

        void createLogicalDevice();

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void setupDebugMessenger();
        std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    };

    class VulkanRenderer : public RendererImpl
    {
    public:
        VulkanRenderer(std::vector<GLuint>& indices, std::vector<GLfloat>& vertices, GLFWwindow* window);
        ~VulkanRenderer();

        void Render(Shader* shader, Camera* camera, glm::mat4 model) override; // Declare draw

    private:
        GLuint VAO;
	    GLuint VBO;
    	GLuint EBO;
    };
}
    