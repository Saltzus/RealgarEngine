#pragma once
#include "component.h"

namespace Realgar
{
    class Texture;
    class GameObject;
}

namespace Realgar::Components
{
    class RenderComponent : public Component
    {
    public:
        RenderComponent(GameObject* object);
        ~RenderComponent();

        Texture* texture;
        Shader* shader = nullptr;

        static void registerRenderComponent(lua_State* L);
        virtual void render(Shader* shader, Camera* camera) override;
    private:
        GameObject* gameObject;
        
        Renderer* renderer;


        std::vector<GLfloat> vertices =
        {
            -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
             0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f
        };
        //Indices for vertices order
        std::vector<GLuint> indices =
        {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };
    };
} 
