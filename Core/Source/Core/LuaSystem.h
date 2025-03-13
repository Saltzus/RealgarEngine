#pragma once
#include "components/components.h"

#include <lua.hpp>
#include "Scene.h"

namespace Realgar 
{
	class LuaSystem
	{
	public:
		LuaSystem();
		~LuaSystem();

		static void Init();
		static void Close();
		static void getGlobals(GameObject* object, Scene* scn);

		static lua_State* state;
	private:
		static void registerValues();
	};
}