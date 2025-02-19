#include "GameObject.h"

namespace RED
{
	GameObject::GameObject() {}

	GameObject::~GameObject()
	{
		for (auto& comp : components)
		{
			components.erase(comp.first);
			delete comp.second;
		}
	}

	void GameObject::update(float deltaTime) {
		for (auto comp : components)
			comp.second->update(deltaTime);
	}

	void GameObject::render(Shader* shader, Camera* camera)
	{
		Components::RenderComponent* renderComponent = getComponent<Components::RenderComponent>();
		if (renderComponent != nullptr)
			renderComponent->render(shader, camera);
	}

    int addComponent(lua_State* L)
    {
        GameObject* object = *(GameObject**)luaL_checkudata(L, 1, "GameObject");

        const char* componentType = luaL_checkstring(L, 2);

        if (strcmp(componentType, "RenderComponent") == 0)
        {
            object->addComponent<Components::RenderComponent>(object);
        }
        else
        {
            lua_pushstring(L, "Unknown component type");
            lua_error(L);
        }

        return 0;
    }
    int getComponent(lua_State* L)
    {
        RED::GameObject* object = *(RED::GameObject**)luaL_checkudata(L, 1, "GameObject");
        const char* componentType = luaL_checkstring(L, 2);

        if (strcmp(componentType, "TransformComponent") == 0)
        {
            auto component = object->getComponent<Components::TransformComponent>();
            if (component)
            {
                Components::TransformComponent** compPtr = (Components::TransformComponent**)lua_newuserdata(L, sizeof(Components::TransformComponent*));
                *compPtr = component;
                luaL_getmetatable(L, "TransformComponent");
                lua_setmetatable(L, -2);
            }
            else
            {
                lua_pushnil(L);
            }
        }
        else if (strcmp(componentType, "RenderComponent") == 0)
        {
            auto component = object->getComponent<Components::RenderComponent>();
            if (component)
            {
                Components::RenderComponent** compPtr = (Components::RenderComponent**)lua_newuserdata(L, sizeof(Components::RenderComponent*));
                *compPtr = component;
                luaL_getmetatable(L, "RenderComponent");
                lua_setmetatable(L, -2);
            }
            else
            {
                lua_pushnil(L);
            }
        }
        else
        {
            lua_pushstring(L, "Unknown component type");
            lua_error(L);
        }
        return 1; // Return the component userdata
    }
    void registerGameObject(lua_State* L) {
        luaL_newmetatable(L, "GameObject");

        lua_pushstring(L, "__index");
        lua_pushvalue(L, -2);  // Set __index to itself
        lua_settable(L, -3);

        lua_pushstring(L, "addComponent");
        lua_pushcfunction(L, addComponent);
        lua_settable(L, -3);

        lua_pushstring(L, "getComponent");
        lua_pushcfunction(L, getComponent);
        lua_settable(L, -3);

        lua_pop(L, 1);  // Pop the metatable
    }

    void GameObject::registerGameObject(lua_State* L)
    {
        RED::registerGameObject(L);
    }
}