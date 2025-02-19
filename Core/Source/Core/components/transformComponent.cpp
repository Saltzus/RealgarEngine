#include "transformComponent.h"

namespace RED::Components
{
    TransformComponent::TransformComponent() {}
    TransformComponent::~TransformComponent() {}

    glm::mat4 TransformComponent::getMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, translation);

        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::scale(model, scale);

        return model;
    }

    int setTranslation(lua_State* L) 
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
    int getTranslation(lua_State* L)
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
    int setTranslationX(lua_State* L) 
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float x = (float)luaL_checknumber(L, 2);
        if (pos && *pos) 
        {
            (*pos)->x = x;
        }
        return 0;
    }
    int getTranslationX(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->x);

        return 1;
    }
    int setTranslationY(lua_State* L) 
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float y = (float)luaL_checknumber(L, 2);
        if (pos && *pos)
        {
            (*pos)->y = y;
        }
        return 0;
    }
    int getTranslationY(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->y);

        return 1;
    }
    int setTranslationZ(lua_State* L) 
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float z = (float)luaL_checknumber(L, 2);
        if (pos && *pos)
        {
            (*pos)->z = z;
        }
        return 0;
    }
    int getTranslationZ(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->z);

        return 1;
    }
    int transformPosition(lua_State* L) {
        auto* transformComponent = *(Components::TransformComponent**)luaL_checkudata(L, 1, "TransformComponent");

        // Allocate userdata to hold a pointer to glm::vec3
        glm::vec3** pos = (glm::vec3**)lua_newuserdata(L, sizeof(glm::vec3*));
        *pos = &transformComponent->translation;  // store the address of translation

        luaL_getmetatable(L, "position");
        lua_setmetatable(L, -2);

        return 1;
    }
    const luaL_Reg position_methods[] = {
        {"set",  setTranslation},
        {"get",  getTranslation},
        {"setX", setTranslationX},
        {"getX", getTranslationX},
        {"setY", setTranslationY},
        {"getY", getTranslationY},
        {"setZ", setTranslationZ},
        {"getZ", getTranslationZ},
        {NULL, NULL}
    };
    void registerTransformComponentPosition(lua_State* L)
    {
        luaL_newmetatable(L, "position");

        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        luaL_setfuncs(L, position_methods, 0);

        lua_pop(L, 1);
    }

    int setRotation(lua_State* L)
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
    int getRotation(lua_State* L)
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
    int setRotationX(lua_State* L)
    {
        glm::vec3** rot = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        float x = (float)luaL_checknumber(L, 2);
        if (rot && *rot)
        {
            (*rot)->x = x;
        }
        return 0;
    }
    int getRotationX(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->x);

        return 1;
    }
    int setRotationY(lua_State* L)
    {
        glm::vec3** rot = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        float y = (float)luaL_checknumber(L, 2);
        if (rot && *rot)
        {
            (*rot)->y = y;
        }
        return 0;
    }
    int getRotationY(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->y);

        return 1;
    }
    int setRotationZ(lua_State* L)
    {
        glm::vec3** rot = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        float z = (float)luaL_checknumber(L, 2);
        if (rot && *rot)
        {
            (*rot)->z = z;
        }
        return 0;
    }
    int getRotationZ(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->z);

        return 1;
    }
    int transformRotation(lua_State* L) {
        auto* transformComponent = *(Components::TransformComponent**)luaL_checkudata(L, 1, "TransformComponent");

        // Allocate userdata to hold a pointer to glm::vec3
        glm::vec3** rot = (glm::vec3**)lua_newuserdata(L, sizeof(glm::vec3*));
        *rot = &transformComponent->rotation;  // store the address of translation

        luaL_getmetatable(L, "rotation");
        lua_setmetatable(L, -2);

        return 1;
    }
    const luaL_Reg rotation_methods[] ={
        {"set",  setRotation},
        {"get",  getRotation},
        {"setX", setRotationX},
        {"getX", getRotationX},
        {"setY", setRotationY},
        {"getY", getRotationY},
        {"setZ", setRotationZ},
        {"getZ", getRotationZ},
        {NULL, NULL}
    };
    void registerTransformComponentRotation(lua_State* L)
    {
        luaL_newmetatable(L, "rotation");

        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        luaL_setfuncs(L, rotation_methods, 0);

        lua_pop(L, 1);
    }

    const luaL_Reg transform_methods[] = {
        {"position", transformPosition},
        {NULL, NULL}
    };
    int transformComponent_index(lua_State* L) 
    {
        // [1] is the TransformComponent userdata, [2] is the key
        const char* key = luaL_checkstring(L, 2);
        if (strcmp(key, "position") == 0) {
            return transformPosition(L); // push the position userdata
        }
        if (strcmp(key, "rotation") == 0) {
            return transformRotation(L); // push the position userdata
        }
        // Fallback: look up the key in the metatable
        lua_getmetatable(L, 1);
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        return 1;
    }
    void registerTransformComponent(lua_State* L) 
    {
        luaL_newmetatable(L, "TransformComponent");

        lua_pushcfunction(L, transformComponent_index);
        lua_setfield(L, -2, "__index");

        lua_pop(L, 1);
    }

    
    void TransformComponent::registerTransformComponent(lua_State* L)
    {
        registerTransformComponentPosition(L);
        registerTransformComponentRotation(L);
        Components::registerTransformComponent(L);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//#include <lua.hpp>
//#include <iostream>
//
//// Define the Position class
//struct Position {
//    float x, y, z;
//
//    Position(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
//
//    void set(float x, float y, float z) {
//        this->x = x;
//        this->y = y;
//        this->z = z;
//    }
//};
//
//// Define the Transform class
//struct Transform {
//    Position position;
//};
//
//// Lua C API functions for Position
//static int l_position_new(lua_State* L) {
//    float x = luaL_optnumber(L, 1, 0.0f);
//    float y = luaL_optnumber(L, 2, 0.0f);
//    float z = luaL_optnumber(L, 3, 0.0f);
//
//    Position* pos = (Position*)lua_newuserdata(L, sizeof(Position));
//    new (pos) Position(x, y, z);
//
//    luaL_getmetatable(L, "Position");
//    lua_setmetatable(L, -2);
//
//    return 1;
//}
//
//static int l_position_set(lua_State* L) {
//    Position* pos = (Position*)luaL_checkudata(L, 1, "Position");
//    float x = luaL_checknumber(L, 2);
//    float y = luaL_checknumber(L, 3);
//    float z = luaL_checknumber(L, 4);
//
//    pos->set(x, y, z);
//
//    return 0;
//}
//
//static int l_position_gc(lua_State* L) {
//    Position* pos = (Position*)luaL_checkudata(L, 1, "Position");
//    pos->~Position();
//    return 0;
//}
//
//// Lua C API functions for Transform
//static int l_transform_new(lua_State* L) {
//    Transform* transform = (Transform*)lua_newuserdata(L, sizeof(Transform));
//    new (transform) Transform();
//
//    luaL_getmetatable(L, "Transform");
//    lua_setmetatable(L, -2);
//
//    return 1;
//}
//
//static int l_transform_position(lua_State* L) {
//    Transform* transform = (Transform*)luaL_checkudata(L, 1, "Transform");
//
//    Position* pos = (Position*)lua_newuserdata(L, sizeof(Position));
//    new (pos) Position(transform->position.x, transform->position.y, transform->position.z);
//
//    luaL_getmetatable(L, "Position");
//    lua_setmetatable(L, -2);
//
//    return 1;
//}
//
//static int l_transform_gc(lua_State* L) {
//    Transform* transform = (Transform*)luaL_checkudata(L, 1, "Transform");
//    transform->~Transform();
//    return 0;
//}
//
//// Register functions and metatables
//static const luaL_Reg position_methods[] = {
//    {"set", l_position_set},
//    {NULL, NULL}
//};
//
//static const luaL_Reg transform_methods[] = {
//    {"position", l_transform_position},
//    {NULL, NULL}
//};
//
//static void register_position(lua_State* L) {
//    luaL_newmetatable(L, "Position");
//
//    lua_pushvalue(L, -1);
//    lua_setfield(L, -2, "__index");
//
//    luaL_setfuncs(L, position_methods, 0);
//
//    lua_pushcfunction(L, l_position_gc);
//    lua_setfield(L, -2, "__gc");
//
//    lua_pop(L, 1);
//}
//
//static void register_transform(lua_State* L) {
//    luaL_newmetatable(L, "Transform");
//
//    lua_pushvalue(L, -1);
//    lua_setfield(L, -2, "__index");
//
//    luaL_setfuncs(L, transform_methods, 0);
//
//    lua_pushcfunction(L, l_transform_gc);
//    lua_setfield(L, -2, "__gc");
//
//    lua_pop(L, 1);
//}
//
//int main() {
//    lua