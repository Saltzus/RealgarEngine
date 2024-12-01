#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <vector>

#include "../../RenderImpl.h"

std::string get_file_contents(const char* filename);

namespace RED::Opengl
{
    class OpenglShader : public ShaderImpl
    {
    public:
        OpenglShader(const char* vertexFile, const char* fragmentFile);

        void Activate() override;
        void Delete() override;
        unsigned int& Id() override { return ID; };

    private:
        unsigned int ID;
        void compileErrors(unsigned int shader, const char* type);
    };
}