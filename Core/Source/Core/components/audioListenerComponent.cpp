#include "audioListenerComponent.h"

namespace Realgar::Components
{
    AudioListenerComponent::AudioListenerComponent() 
    {
        ma_engine_listener_set_position(engine, 0, translation.x, translation.y, translation.z);
    }
    AudioListenerComponent::~AudioListenerComponent() {}

    void AudioListenerComponent::update(float deltaTime)
    {
        if (parentTranslation == nullptr) return;
        ma_engine_listener_set_position(engine, 0, parentTranslation->x + translation.x, parentTranslation->x + translation.y, parentTranslation->x + translation.z);
    }


    int setListenerTranslation(lua_State* L) 
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float x = (float)luaL_checknumber(L, 2);
        float y = (float)luaL_checknumber(L, 3);
        float z = (float)luaL_checknumber(L, 4);

        if (pos && *pos) 
        {
            (*pos)->x = x;
            (*pos)->y = y;
            (*pos)->z = z;
        }
        return 0;
    }
    int getListenerTranslation(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushstring(L, "x");
        lua_pushnumber(L, (*pos)->x);
        lua_settable(L, -3);

        lua_pushstring(L, "y");
        lua_pushnumber(L, (*pos)->y);
        lua_settable(L, -3);

        lua_pushstring(L, "z");
        lua_pushnumber(L, (*pos)->z);
        lua_settable(L, -3);
        return 1;
    }
    int setListenerTranslationX(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float x = (float)luaL_checknumber(L, 2);
        if (pos && *pos) 
        {
            (*pos)->x = x;
        }
        return 0;
    }
    int getListenerTranslationX(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->x);

        return 1;
    }
    int setListenerTranslationY(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float y = (float)luaL_checknumber(L, 2);
        if (pos && *pos)
        {
            (*pos)->y = y;
        }
        return 0;
    }
    int getListenerTranslationY(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->y);

        return 1;
    }
    int setListenerTranslationZ(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float z = (float)luaL_checknumber(L, 2);
        if (pos && *pos)
        {
            (*pos)->z = z;
        }
        return 0;
    }
    int getListenerTranslationZ(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->z);

        return 1;
    }
    int listenerTransformPosition(lua_State* L) {
        auto* audioListenerComponent = *(Components::AudioListenerComponent**)luaL_checkudata(L, 1, "AudioListenerComponent");

        // Allocate userdata to hold a pointer to glm::vec3
        glm::vec3** pos = (glm::vec3**)lua_newuserdata(L, sizeof(glm::vec3*));
        *pos = &audioListenerComponent->translation;  // store the address of translation

        luaL_getmetatable(L, "position");
        lua_setmetatable(L, -2);

        return 1;
    }
    const luaL_Reg position_methods[] = {
        {"set",  setListenerTranslation},
        {"get",  getListenerTranslation},
        {"setX", setListenerTranslationX},
        {"getX", getListenerTranslationX},
        {"setY", setListenerTranslationY},
        {"getY", getListenerTranslationY},
        {"setZ", setListenerTranslationZ},
        {"getZ", getListenerTranslationZ},
        {NULL, NULL}
    };
    void registerAudioListenerComponentPosition(lua_State* L)
    {
        luaL_newmetatable(L, "position");

        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        luaL_setfuncs(L, position_methods, 0);

        lua_pop(L, 1);
    }

    int setListenerRotation(lua_State* L)
    {
        glm::vec3** rot = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        float x = (float)luaL_checknumber(L, 2);
        float y = (float)luaL_checknumber(L, 3);
        float z = (float)luaL_checknumber(L, 4);

        if (rot && *rot)
        {
            (*rot)->x = x;
            (*rot)->y = y;
            (*rot)->z = z;
        }
        return 0;
    }
    int getListenerRotation(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        lua_newtable(L);

        lua_pushstring(L, "x");
        lua_pushnumber(L, (*pos)->x);
        lua_settable(L, -3);

        lua_pushstring(L, "y");
        lua_pushnumber(L, (*pos)->y);
        lua_settable(L, -3);

        lua_pushstring(L, "z");
        lua_pushnumber(L, (*pos)->z);
        lua_settable(L, -3);
        return 1;
    }
    int setListenerRotationX(lua_State* L)
    {
        glm::vec3** rot = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        float x = (float)luaL_checknumber(L, 2);
        if (rot && *rot)
        {
            (*rot)->x = x;
        }
        return 0;
    }
    int getListenerRotationX(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->x);

        return 1;
    }
    int setListenerRotationY(lua_State* L)
    {
        glm::vec3** rot = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        float y = (float)luaL_checknumber(L, 2);
        if (rot && *rot)
        {
            (*rot)->y = y;
        }
        return 0;
    }
    int getListenerRotationY(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->y);

        return 1;
    }
    int setListenerRotationZ(lua_State* L)
    {
        glm::vec3** rot = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        float z = (float)luaL_checknumber(L, 2);
        if (rot && *rot)
        {
            (*rot)->z = z;
        }
        return 0;
    }
    int getListenerRotationZ(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->z);

        return 1;
    }
    int listenerTransformRotation(lua_State* L) {
        auto* transformComponent = *(Components::AudioListenerComponent**)luaL_checkudata(L, 1, "AudioListenerComponent");

        // Allocate userdata to hold a pointer to glm::vec3
        glm::vec3** rot = (glm::vec3**)lua_newuserdata(L, sizeof(glm::vec3*));
        *rot = &transformComponent->rotation;  // store the address of translation

        luaL_getmetatable(L, "rotation");
        lua_setmetatable(L, -2);

        return 1;
    }
    const luaL_Reg rotation_methods[] ={
        {"set",  setListenerRotation},
        {"get",  getListenerRotation},
        {"setX", setListenerRotationX},
        {"getX", getListenerRotationX},
        {"setY", setListenerRotationY},
        {"getY", getListenerRotationY},
        {"setZ", setListenerRotationZ},
        {"getZ", getListenerRotationZ},
        {NULL, NULL}
    };
    void registerAudioListenerComponentRotation(lua_State* L)
    {
        luaL_newmetatable(L, "rotation");

        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        luaL_setfuncs(L, rotation_methods, 0);

        lua_pop(L, 1);
    }

    int AudioListenerComponent_index(lua_State* L)
    {
        // [1] is the TransformComponent userdata, [2] is the key
        const char* key = luaL_checkstring(L, 2);
        if (strcmp(key, "position") == 0) {
            return listenerTransformPosition(L); // push the position userdata
        }
        if (strcmp(key, "rotation") == 0) {
            return listenerTransformRotation(L); // push the position userdata
        }
        // Fallback: look up the key in the metatable
        lua_getmetatable(L, 1);
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        return 1;
    }
    void registerAudioListenerComponent(lua_State* L)
    {
        luaL_newmetatable(L, "AudioListenerComponent");

        lua_pushcfunction(L, AudioListenerComponent_index);
        lua_setfield(L, -2, "__index");

        lua_pop(L, 1);
    }

    void AudioListenerComponent::registerAudioListenerComponent(lua_State* L)
    {
        registerAudioListenerComponentPosition(L);
        registerAudioListenerComponentRotation(L);
        Components::registerAudioListenerComponent(L);
    }
}