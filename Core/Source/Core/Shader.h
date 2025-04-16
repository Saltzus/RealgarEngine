#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <vector>

#include "Renderer.h"
#include "FileManager.h"

class ShaderImpl;

std::string get_file_contents(const char* filename);

namespace Realgar
{
    class Shader
    {
    public:
        std::pair<std::string, std::string> shader = { FileManager::getResource("Shaders/default.vert"), FileManager::getResource("Shaders/default.frag") };

        Shader(std::string vertexFile, std::string fragmentFile);
        ~Shader();
        void Activate();
        void Delete();
        unsigned int& ID();
    private:
        ShaderImpl* impl = nullptr;
    };
}