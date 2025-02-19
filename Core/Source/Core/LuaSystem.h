#pragma once
#include "components/components.h"

#include <lua.hpp>
#include "Scene.h"

namespace RED 
{
	class LuaSystem
	{
	public:
		LuaSystem();
		~LuaSystem();

		static void Init();
		static void Close();
		static void getGlobals(GameObject* object, Scene* scene);

		static lua_State* state;
	private:
		static void registerValues();
	};
}