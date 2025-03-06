#include "VulkanShader.h"
#include "VulkanManager.h"

namespace Realgar::Vulkan
{

	// Reads a text file and outputs a string with everything in the text file
	std::string get_file_contents(const char* filename)
	{
		std::ifstream in(filename, std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return(contents);
		}
		throw(errno);
	}

	// Constructor that build the Shader Program from 2 different shaders
	VulkanShader::VulkanShader(const char* vertexFile, const char* fragmentFile)
	{
		Vulkan::vulkan->createGraphicsPipeline(vertexFile, fragmentFile);
	}

	// Activates the Shader Program
	void VulkanShader::Activate()
	{

	}

	// Deletes the Shader Program
	void VulkanShader::Delete()
	{

	}

	// Checks if the different Shaders have compiled properly
	void VulkanShader::compileErrors(unsigned int shader, const char* type)
	{

	}
}