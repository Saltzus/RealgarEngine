#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <vector>

#include <vulkan/vulkan.hpp>
#include "VulkanVertex.h"

namespace RED::Vulkan
{
    class VulkanGraphicsPipeline
    {
    public:
        VulkanGraphicsPipeline(const char* vertexFile, const char* fragmentFile, VkDevice& device, VkDescriptorSetLayout& descriptorSetLayout, VkRenderPass& renderPass, VkPipelineLayout& pipelineLayout, VkPipeline& graphicsPipeline);

        void Bind();
        void Delete();
        unsigned int& Id() { return ID; };

    private:


        unsigned int ID;
        void compileErrors(unsigned int shader, const char* type);
    };
}