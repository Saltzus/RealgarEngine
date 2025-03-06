#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Realgar
{
    class Shader;
    class Camera;

    class RendererImpl 
    {
    public:
        virtual ~RendererImpl() {}
        virtual void Render(Shader* shader, Camera* camera, glm::mat4 model) = 0;
    };

    class ApiImpl
    {
    public:
        virtual ~ApiImpl() {}
        virtual void render() {}
    };

    class TextureImpl
    {
    public:
        virtual ~TextureImpl() {}
        virtual void Bind() = 0;
    };

	class ShaderImpl
	{
    public:
        virtual ~ShaderImpl() {}
        virtual void Activate() = 0;
        virtual void Delete() = 0;
        virtual unsigned int& Id() = 0;
	};
}