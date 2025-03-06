#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <vector>

#include <vulkan/vulkan.hpp>
#include "VulkanVertex.h"

namespace Realgar::Vulkan
{
    class VulkanGraphicsPipeline
    {
    public:
        VulkanGraphicsPipeline(std::string vertexFile, std::string fragmentFile, VkDevice& device, VkDescriptorSetLayout& descriptorSetLayout, VkRenderPass& renderPass, VkPipelineLayout& pipelineLayout, VkPipeline& graphicsPipeline);

        void Bind();
        void Delete();
        unsigned int& Id() { return ID; };

        std::vector<char> vertShaderCode;
        std::vector<char> fragShaderCode;

    private:


        unsigned int ID;
        void compileErrors(unsigned int shader, const char* type);
    };
}