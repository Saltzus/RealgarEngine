#include "LuaSystem.h"
#include "GameObject.h"
#include "Scene.h"

namespace Realgar 
{
	lua_State* LuaSystem::state = nullptr;

	LuaSystem::LuaSystem() {}
	LuaSystem::~LuaSystem() {}

    void LuaSystem::registerValues()
    {
		Scene::registerScene(state);
        GameObject::registerGameObject(state);

        Components::RenderComponent::registerRenderComponent(state);
		Components::TransformComponent::registerTransformComponent(state);
    }

	void LuaSystem::getGlobals(GameObject* object, Scene* scn)
	{
		GameObject** obj = (GameObject**)lua_newuserdata(state, sizeof(GameObject*));
		*obj = object;

		luaL_getmetatable(state, "GameObject");
		lua_setmetatable(state, -2);
		lua_setglobal(state, "currentObject");

		Scene** scene = (Scene**)lua_newuserdata(state, sizeof(Scene*));
		*scene = scn;

		luaL_getmetatable(state, "Scene");
		lua_setmetatable(state, -2);
		lua_setglobal(state, "Scene");
	}

	void LuaSystem::Init()
	{
		state = luaL_newstate();
		luaL_openlibs(state);

        registerValues();
	}

	void LuaSystem::Close()
	{
		lua_close(state);
	}
}