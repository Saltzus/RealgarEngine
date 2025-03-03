#include "renderComponent.h"
#include "../GameObject.h"
#include "../Texture.h"
#include "../Scene.h"

namespace RED::Components
{
    RenderComponent::RenderComponent(GameObject* object)
    {
        gameObject = object;

        texture = Texture::GetDefaultTexture();
        this->renderer = new Renderer(indices, vertices);
    }
    RenderComponent::~RenderComponent()
    {
        delete texture;
        delete renderer;
    }

    void RenderComponent::render(Shader* shader, Camera* camera)
    {
        TransformComponent* transformComponent = gameObject->getComponent<TransformComponent>();
        glm::mat4 modelMatrix = transformComponent != nullptr ? transformComponent->getMatrix() : glm::mat4(1.f);

        this->shader->Activate();
        texture->Bind();
            
        this->renderer->Render(this->shader, camera,  modelMatrix);
    }


    int setShader(lua_State* L) {
        auto* renderComponent = *(Components::RenderComponent**)luaL_checkudata(L, 1, "RenderComponent");
        const char* shaderName = lua_tostring(L, 2);

        if (renderComponent && shaderName) {
            renderComponent->shader = Scene::current_shaders[shaderName]; // Assuming `shaders` is accessible here
        }
        return 0;
    }
    int setTexture(lua_State* L) {
        auto* renderComponent = *(Components::RenderComponent**)luaL_checkudata(L, 1, "RenderComponent");
        const char* textureName = lua_tostring(L, 2);

        if (renderComponent && textureName) {
            renderComponent->texture = Scene::current_textures[textureName]; // Assuming `textures` is accessible
        }
        return 0;
    }
    void registerRenderComponent(lua_State* L) {
        luaL_newmetatable(L, "RenderComponent");

        lua_pushstring(L, "__index");
        lua_newtable(L);

        lua_pushstring(L, "setShader");
        lua_pushcfunction(L, setShader);
        lua_settable(L, -3);

        lua_pushstring(L, "setTexture");
        lua_pushcfunction(L, setTexture);
        lua_settable(L, -3);

        lua_settable(L, -3);
        lua_pop(L, 1);
    }

    void RenderComponent::registerRenderComponent(lua_State* L)
    {
        Components::registerRenderComponent(L);
    }
}