#include "ScriptComponent.h"
#include "../GameObject.h"
#include "../LuaSystem.h"
#include "../Scene.h"

namespace RED::Components
{
    LuaSystem ScriptComponent::L;

    ScriptComponent::ScriptComponent(GameObject* object, Scene* scene, std::string luaFile) : object(object), scene(scene), luaFile(luaFile)
    {

    }

    ScriptComponent::~ScriptComponent() {}

    void ScriptComponent::update(float deltaTime)
    {
        if (errorState) return;

        if (L.state != nullptr) {
            lua_getglobal(L.state, "update");

            if (lua_isfunction(L.state, -1)) {
                lua_pushnumber(L.state, deltaTime);
                if (lua_pcall(L.state, 1, 0, 0)) {
                    std::cerr << "Error calling Lua update function: " << lua_tostring(L.state, -1) << std::endl;
                    errorState = true;
                    lua_pop(L.state, 1); // Pop error message
                }
            }
            else {
                std::cerr << "Lua function 'update' not found!" << std::endl;
                errorState = true;
                lua_pop(L.state, 1); // Pop non-function value
            }
        }
    }
    void ScriptComponent::init()
    {
        if (luaL_loadfile(L.state, luaFile.c_str())) {
            std::cerr << "Something went wrong loading the chunk (syntax error?)" << std::endl;
            std::cerr << lua_tostring(L.state, -1) << std::endl;
            lua_pop(L.state, 1);
        }

        LuaSystem::getGlobals(object, scene);

        if (lua_pcall(L.state, 0, LUA_MULTRET, 0)) {                  //Run code from lua
            std::cerr << "Something went wrong during execution" << std::endl;
            std::cerr << lua_tostring(L.state, -1) << std::endl;
            lua_pop(L.state, 1);
        }
    }
}