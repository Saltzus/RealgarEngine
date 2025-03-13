#include "LuaSystem.h"
#include "GameObject.h"
#include "Scene.h"
#include "Input.h"

namespace Realgar 
{
	lua_State* LuaSystem::state = nullptr;

	LuaSystem::LuaSystem() {}
	LuaSystem::~LuaSystem() {}

    void LuaSystem::registerValues()
    {
		Scene::registerScene(state);
        GameObject::registerGameObject(state);
		Camera::registerCamera(state);
		Input::registerInput(state);
		
        Components::RenderComponent::registerRenderComponent(state);
		Components::TransformComponent::registerTransformComponent(state);

		Components::AudioListenerComponent::registerAudioListenerComponent(state);
		Components::AudioPlayerComponent::registerAudioPlayerComponent(state);
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

		Camera** camera = (Camera**)lua_newuserdata(state, sizeof(Camera*));
		*camera = scn->camera;
		luaL_getmetatable(state, "Camera");
		lua_setmetatable(state, -2);
		lua_setglobal(state, "currentCamera");

		Input** input = (Input**)lua_newuserdata(state, sizeof(Input*));
		luaL_getmetatable(state, "Input");
		lua_setmetatable(state, -2);
		lua_setglobal(state, "Input");
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