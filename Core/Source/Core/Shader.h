#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <vector>

#include "Renderer.h"

class ShaderImpl;

std::string get_file_contents(const char* filename);

namespace Realgar
{
    class Shader
    {
    private:
        ShaderImpl* impl = nullptr;
    public:

        std::pair<std::string, std::string> shader = { "Resources/Shaders/default.vert.spv", "Resources/Shaders/default.frag.spv" };

        Shader(const char* vertexFile, const char* fragmentFile);
        ~Shader();
        void Activate();
        void Delete();
        unsigned int& ID();
    };
}