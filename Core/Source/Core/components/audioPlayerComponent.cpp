#include "audioPlayerComponent.h"
#include "../Scene.h"

namespace Realgar::Components
{
    AudioPlayerComponent::AudioPlayerComponent() {}

    void AudioPlayerComponent::init()
    {
        if (sound == nullptr) return;

        ma_sound_set_position(sound, translation.x, translation.y, translation.z);
        ma_sound_start(sound);
    }
    
    AudioPlayerComponent::~AudioPlayerComponent() 
    {
    }

    void AudioPlayerComponent::update(float deltaTime)
    {
        if (parentTranslation == nullptr || sound == nullptr) return;
        ma_sound_set_position(sound, parentTranslation->x + translation.x, parentTranslation->y + translation.y, parentTranslation->z + translation.z);
    }

    int setPlayerTranslation(lua_State* L) 
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
    int getPlayerTranslation(lua_State* L)
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
    int setPlayerTranslationX(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float x = (float)luaL_checknumber(L, 2);
        if (pos && *pos) 
        {
            (*pos)->x = x;
        }
        return 0;
    }
    int getPlayerTranslationX(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->x);

        return 1;
    }
    int setPlayerTranslationY(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float y = (float)luaL_checknumber(L, 2);
        if (pos && *pos)
        {
            (*pos)->y = y;
        }
        return 0;
    }
    int getPlayerTranslationY(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->y);

        return 1;
    }
    int setPlayerTranslationZ(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float z = (float)luaL_checknumber(L, 2);
        if (pos && *pos)
        {
            (*pos)->z = z;
        }
        return 0;
    }
    int getPlayerTranslationZ(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->z);

        return 1;
    }
    int playerTransformPosition(lua_State* L) {
        auto* audioPlayerComponent = *(Components::AudioPlayerComponent**)luaL_checkudata(L, 1, "AudioPlayerComponent");

        // Allocate userdata to hold a pointer to glm::vec3
        glm::vec3** pos = (glm::vec3**)lua_newuserdata(L, sizeof(glm::vec3*));
        *pos = &audioPlayerComponent->translation;  // store the address of translation

        luaL_getmetatable(L, "position");
        lua_setmetatable(L, -2);

        return 1;
    }
    const luaL_Reg position_methods[] = {
        {"set",  setPlayerTranslation},
        {"get",  getPlayerTranslation},
        {"setX", setPlayerTranslationX},
        {"getX", getPlayerTranslationX},
        {"setY", setPlayerTranslationY},
        {"getY", getPlayerTranslationY},
        {"setZ", setPlayerTranslationZ},
        {"getZ", getPlayerTranslationZ},
        {NULL, NULL}
    };
    void registerAudioPlayerComponentPosition(lua_State* L)
    {
        luaL_newmetatable(L, "position");

        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        luaL_setfuncs(L, position_methods, 0);

        lua_pop(L, 1);
    }

    int playerSetTexture(lua_State* L) {
        auto* audioPlayerComponent = *(Components::AudioPlayerComponent**)luaL_checkudata(L, 1, "AudioPlayerComponent");
        const char* audioName = lua_tostring(L, 2);

        if (audioPlayerComponent && audioName) {
            audioPlayerComponent->sound = &Scene::current_audio[audioName]->sound;
        }

        return 0;
    }

    int AudioPlayerComponent_index(lua_State* L)
    {
        // [1] is the TransformComponent userdata, [2] is the key
        const char* key = luaL_checkstring(L, 2);
        if (strcmp(key, "position") == 0) {
            return playerTransformPosition(L); // push the position userdata
        }

        // Fallback: look up the key in the metatable
        lua_getmetatable(L, 1);
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        return 1;
    }
    void registerAudioPlayerComponent(lua_State* L)
    {
        luaL_newmetatable(L, "AudioPlayerComponent");

        lua_pushcfunction(L, AudioPlayerComponent_index);
        lua_setfield(L, -2, "__index");

        lua_pushstring(L, "setTexture");
        lua_pushcfunction(L, playerSetTexture);
        lua_settable(L, -3);

        lua_pop(L, 1);
    }

    
    void AudioPlayerComponent::registerAudioPlayerComponent(lua_State* L)
    {
        registerAudioPlayerComponentPosition(L);
        Components::registerAudioPlayerComponent(L);
    }
}
