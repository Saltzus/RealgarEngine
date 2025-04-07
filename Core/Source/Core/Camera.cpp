#include "Camera.h"

namespace Realgar
{
    Camera::Camera(glm::vec3 position, glm::vec3 rotation, float fov, float nearPlane, float farPlane, bool ortho)
    {
    	cameraPosition = -position;
        cameraRotation = rotation;

        this->fov = fov;
        this->nearPlane = nearPlane;
        this->farPlane = farPlane;
        this->ortho = ortho;
    }

    void Camera::updateMatrix(GLFWwindow* window)
    {
    	view = glm::mat4(1.0f);
    	projection = glm::mat4(1.0f);

        if (Window::editor)
        {
            width = Window::viewportWidth;
            height = Window::viewportHeight;
        }
        else
            glfwGetFramebufferSize(window, &width, &height);

        if (Renderer::GetGraphicsApi() == GraphicsApis::OpenGL && ortho)
        {
            float scale = 0.024f; // or any desired scale factor
            float target_width = 640.0f * scale;
            float target_height = 360.0f * scale;
            float target_aspect = target_width / target_height;
            float aspect = (float)width / height;

            if (aspect > target_aspect) {
                float view_width = target_height * aspect;
                projection = glm::ortho(
                    -view_width / 2.0f, view_width / 2.0f,
                    -target_height / 2.0f, target_height / 2.0f,
                    nearPlane, farPlane
                );
            }
            else {
                float view_height = target_width / aspect;
                projection = glm::ortho(
                    -target_width / 2.0f, target_width / 2.0f,
                    -view_height / 2.0f, view_height / 2.0f,
                    nearPlane, farPlane
                );
            }
        }

        if (!ortho) projection = glm::perspective(glm::radians(fov), (float)width / height, nearPlane, farPlane);



        glm::vec3 front;
        front.x = cos(glm::radians(cameraRotation.x)) * cos(glm::radians(cameraRotation.y + 90));
        front.y = sin(glm::radians(cameraRotation.x));
        front.z = cos(glm::radians(cameraRotation.x)) * sin(glm::radians(cameraRotation.y + 90));
        cameraFront = glm::normalize(front);

        glm::vec3 right = glm::normalize(glm::cross(cameraFront, worldUp));
        glm::vec3 up = glm::normalize(glm::cross(right, cameraFront));
        cameraUp = glm::rotate(up, glm::radians(cameraRotation.z), cameraFront);

        view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
    }


    int setCameraTranslation(lua_State* L) 
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
    int getCameraTranslation(lua_State* L)
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
    int setCameraTranslationX(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float x = (float)luaL_checknumber(L, 2);
        if (pos && *pos)
        {
            (*pos)->x = x;
        }
        return 0;
    }
    int getCameraTranslationX(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->x);

        return 1;
    }
    int setCameraTranslationY(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float y = (float)luaL_checknumber(L, 2);
        if (pos && *pos)
        {
            (*pos)->y = y;
        }
        return 0;
    }
    int getCameraTranslationY(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->y);

        return 1;
    }
    int setCameraTranslationZ(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        float z = (float)luaL_checknumber(L, 2);
        if (pos && *pos)
        {
            (*pos)->z = z;
        }
        return 0;
    }
    int getCameraTranslationZ(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "position");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->z);

        return 1;
    }
    int cameraPosition(lua_State* L) {
        auto* camera = *(Camera**)luaL_checkudata(L, 1, "Camera");

        // Allocate userdata to hold a pointer to glm::vec3
        glm::vec3** pos = (glm::vec3**)lua_newuserdata(L, sizeof(glm::vec3*));
        *pos = &camera->cameraPosition;  // store the address of translation

        luaL_getmetatable(L, "position");
        lua_setmetatable(L, -2);

        return 1;
    }
    const luaL_Reg position_methods[] = {
        {"set",  setCameraTranslation},
        {"get",  getCameraTranslation},
        {"setX", setCameraTranslationX},
        {"getX", getCameraTranslationX},
        {"setY", setCameraTranslationY},
        {"getY", getCameraTranslationY},
        {"setZ", setCameraTranslationZ},
        {"getZ", getCameraTranslationZ},
        {NULL, NULL}
    };
    void registerCameraPosition(lua_State* L)
    {
        luaL_newmetatable(L, "position");

        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        luaL_setfuncs(L, position_methods, 0);

        lua_pop(L, 1);
    }

    int setCameraRotation(lua_State* L)
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
    int getCameraRotation(lua_State* L)
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
    int setCameraRotationX(lua_State* L)
    {
        glm::vec3** rot = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        float x = (float)luaL_checknumber(L, 2);
        if (rot && *rot)
        {
            (*rot)->x = x;
        }
        return 0;
    }
    int getCameraRotationX(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->x);

        return 1;
    }
    int setCameraRotationY(lua_State* L)
    {
        glm::vec3** rot = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        float y = (float)luaL_checknumber(L, 2);
        if (rot && *rot)
        {
            (*rot)->y = y;
        }
        return 0;
    }
    int getCameraRotationY(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->y);

        return 1;
    }
    int setCameraRotationZ(lua_State* L)
    {
        glm::vec3** rot = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        float z = (float)luaL_checknumber(L, 2);
        if (rot && *rot)
        {
            (*rot)->z = z;
        }
        return 0;
    }
    int getCameraRotationZ(lua_State* L)
    {
        glm::vec3** pos = (glm::vec3**)luaL_checkudata(L, 1, "rotation");
        lua_newtable(L);

        lua_pushnumber(L, (*pos)->z);

        return 1;
    }
    int cameraRotation(lua_State* L) {
        auto* camera = *(Camera**)luaL_checkudata(L, 1, "Camera");

        // Allocate userdata to hold a pointer to glm::vec3
        glm::vec3** rot = (glm::vec3**)lua_newuserdata(L, sizeof(glm::vec3*));
        *rot = &camera->cameraRotation;  // store the address of translation

        luaL_getmetatable(L, "rotation");
        lua_setmetatable(L, -2);

        return 1;
    }
    const luaL_Reg rotation_methods[] = {
        {"set",  setCameraRotation},
        {"get",  getCameraRotation},
        {"setX", setCameraRotationX},
        {"getX", getCameraRotationX},
        {"setY", setCameraRotationY},
        {"getY", getCameraRotationY},
        {"setZ", setCameraRotationZ},
        {"getZ", getCameraRotationZ},
        {NULL, NULL}
    };
    void registerCameraRotation(lua_State* L)
    {
        luaL_newmetatable(L, "rotation");

        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        luaL_setfuncs(L, rotation_methods, 0);

        lua_pop(L, 1);
    }

    int cameraSetFov(lua_State* L)
    {
        auto* camera = *(Camera**)luaL_checkudata(L, 1, "Camera");
        float fov = (float)luaL_checknumber(L, 2);

        if (camera && fov)
            camera->fov = fov;

        return 0;
    }

    int Camera_index(lua_State* L)
    {
        // [1] is the TransformComponent userdata, [2] is the key
        const char* key = luaL_checkstring(L, 2);
        if (strcmp(key, "position") == 0) {
            return cameraPosition(L); // push the position userdata
        }
        if (strcmp(key, "rotation") == 0) {
            return cameraRotation(L); // push the position userdata
        }

        // Fallback: look up the key in the metatable
        lua_getmetatable(L, 1);
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        return 1;
    }
    void registerCamera(lua_State* L)
    {
        luaL_newmetatable(L, "Camera");

        lua_pushcfunction(L, Camera_index);
        lua_setfield(L, -2, "__index");

        lua_pushstring(L, "setFov");
        lua_pushcfunction(L, cameraSetFov);
        lua_settable(L, -3);

        lua_pop(L, 1);
    }

    void Camera::registerCamera(lua_State* L)
    {
        registerCameraPosition(L);
        registerCameraRotation(L);
        Realgar::registerCamera(L);
    }
}