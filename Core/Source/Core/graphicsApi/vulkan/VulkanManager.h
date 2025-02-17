#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#include <stb_image/stb_image.h>
#include <vector>
#include <map>
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

    class VulkanRenderer;

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
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageViewTex;
    };

    class Vulkan : public ApiImpl
    {
    public:
        Vulkan(GLFWwindow* GLFW_Window);
        ~Vulkan();

        void renderEnd();
        void render() override;

        static Vulkan* vulkan;

        std::vector<UniformBufferObject> ubo;

        VkDevice device;
        VkImageView textureImageView;
        VkSampler textureSampler;

        std::map<std::pair<std::string, std::string>,VkPipeline> graphicsPipelines;

        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void createGraphicsPipeline(std::string vertexFile, std::string fragmentFile);


        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        std::pair<VkBuffer, VkDeviceMemory> createVertexBuffer(std::vector<GLfloat>& vertices);
        std::pair<VkBuffer, VkDeviceMemory> createIndexBuffer(std::vector<uint16_t> indices);
        std::pair<std::vector<VkBuffer>, std::vector<void*>> createUniformBuffers();
        void updateUniformBuffer(uint32_t currentImage, VulkanRenderer* object);

        void createDescriptorPool();
        std::vector<VkDescriptorSet> createDescriptorSets();

        uint32_t getCurrentFrame() { return currentFrame; }

    private:
        GLFWwindow* window;

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

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

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;



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

        //void createTextureImage();
        void createTextureImageView();
        void createTextureSampler();


        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void createDescriptorSetLayout();
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
        unsigned int id;
        int PipelineID = 0;

        VulkanRenderer(std::vector<GLuint>& indices, std::vector<GLfloat>& vertices);
        ~VulkanRenderer();

        std::pair<std::string, std::string>* shader;
        const char* vertexShader;
        const char* fragmentShader;
        
        std::pair<VkBuffer, VkDeviceMemory> vertexBuffer_vertexBufferMemory;
        std::pair<VkBuffer, VkDeviceMemory> indexBuffer_indexBufferMemory;
        std::pair <std::vector<VkBuffer>, std::vector<void*>> uniformBuffers_uniformBuffersMapped;

        std::vector<VkDescriptorSet> descriptorSets;
        UniformBufferObject ubo;

        virtual void Render(Shader* shader, Camera* camera, glm::mat4 model) override; // Declare draw
    private:
        void* lastTexture;

        GLuint VAO;
	    GLuint VBO;
    	GLuint EBO;
    };
}
    