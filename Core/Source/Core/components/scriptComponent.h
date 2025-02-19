#pragma once
#include "component.h"

#include <lua.hpp>

namespace RED
{
    class Texture;
    class GameObject;
    class LuaSystem;
    class Scene;
}

namespace RED::Components
{
    class ScriptComponent : public Component
    {
    public:
        ScriptComponent(GameObject* object, Scene* scene,  std::string luaFile);
        ~ScriptComponent();

        virtual void update(float deltaTime) override;
        virtual void init() override;
    private:
        static LuaSystem L;
        bool errorState = false;

        GameObject* object = nullptr;
        Scene* scene = nullptr;
        std::string luaFile;
    };
    
} 
