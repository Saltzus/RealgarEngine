#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#include <stb_image/stb_image.h>
#include <vector>
#include <optional>
#include <set>
#include <chrono>

#include <GLFW/glfw3.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "../../RenderImpl.h"
#include "../../Window.h"
#include "VulkanGraphicsPipeline.h"

namespace RED::Vulkan
{
    //// Structure to standardize the vertices used in the sprites
    //struct Vertex {
    //    glm::vec3 pos;
    //    glm::vec3 color;
    //    glm::vec2 texCoord;

    //    static VkVertexInputBindingDescription getBindingDescription() {
    //        VkVertexInputBindingDescription bindingDescription{};
    //        bindingDescription.binding = 0;
    //        bindingDescription.stride = sizeof(Vertex);
    //        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    //        return bindingDescription;
    //    }

    //    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
    //        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

    //        attributeDescriptions[0].binding = 0;
    //        attributeDescriptions[0].location = 0;
    //        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    //        attributeDescriptions[0].offset = offsetof(Vertex, pos);

    //        attributeDescriptions[1].binding = 0;
    //        attributeDescriptions[1].location = 1;
    //        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    //        attributeDescriptions[1].offset = offsetof(Vertex, color);

    //        attributeDescriptions[2].binding = 0;
    //        attributeDescriptions[2].location = 2;
    //        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    //        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    //        return attributeDescriptions;
    //    }
    //};

    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
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

        void renderEnd();
        void render();

    private:
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
        std::vector<VkFramebuffer> swapChainFramebuffers;

        VkRenderPass renderPass;
        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        VkCommandPool commandPool;

        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;

        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

        std::vector<VkCommandBuffer> commandBuffers;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        uint32_t currentFrame = 0;

        bool framebufferResized = false;


        void createInstance();
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        
        struct QueueFamilyIndices;
        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        void createSurface(GLFWwindow* window);

        struct SwapChainSupportDetails;
        void createSwapChain();
        void cleanupSwapChain();
        void recreateSwapChain();
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        
        void createImageViews();
        void createFramebuffers();

        void createTextureImage();
        void createTextureImageView();
        void createTextureSampler();
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        void createVertexBuffer();
        void createIndexBuffer();
        void createUniformBuffers();
        void updateUniformBuffer(uint32_t currentImage);

        void createDescriptorPool();
        void createDescriptorSets();

        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void createDescriptorSetLayout();
        void createGraphicsPipeline();
        VkShaderModule createShaderModule(const std::vector<char>& code);

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void createCommandPool();
        void createCommandBuffers();

        void createDepthResources();
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat findDepthFormat();
        bool hasStencilComponent(VkFormat format);

        void drawFrame();
        void createSyncObjects();

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
        Vulkan* vulkan;

        GLuint VAO;
	    GLuint VBO;
    	GLuint EBO;
    };
}
    