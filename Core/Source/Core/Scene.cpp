#include "Scene.h"
#include "Texture.h"

namespace Realgar
{
    std::map<std::string, Texture*> Scene::current_textures;
    std::map<std::string, Shader*> Scene::current_shaders;

    Scene::Scene(const char* filepath)
    {
        unsigned int SCR_WIDTH = 800;
        unsigned int SCR_HEIGHT = 600;

        std::ifstream jsonFile(filepath);
        sceneData = json::parse(jsonFile);

        json cameraData = sceneData["camera"];
        glm::vec3 cameraTranslation(cameraData["translation"][0], cameraData["translation"][1], cameraData["translation"][2]);
        camera = new Camera
        (
            SCR_WIDTH, SCR_HEIGHT,
            cameraTranslation
        );

        // TODO: ADD TO CAMERA
        //glm::vec3 cameraRotation(cameraData["rotation"][0], cameraData["rotation"][1], cameraData["rotation"][2]);
        //float fov = cameraData["fov"];
        //float nearClip = cameraData["nearClip"];
        //float farClip = cameraData["farClip"];

        // TODO: ADD TEXTURES

        for (auto& [name, shdr] : sceneData["shaders"].items())
        {
            std::string vertex = shdr["vertex"].get<std::string>();
            std::string fragment = shdr["fragment"].get<std::string>();
            Shader* shader = new Shader(vertex.c_str(), fragment.c_str());

            shaders[name] = shader;
        }

        current_shaders = shaders;

        for (auto& [name, txtr] : sceneData["textures"].items())
        {
            std::string path = txtr.get<std::string>();
            Texture* texture = new Texture(path.c_str());

            textures[name] = texture;
        }

        current_textures = textures;

        for (json obj : sceneData["gameObjects"])
        {
            GameObject* object = new GameObject;
            object->name = obj["name"].get<std::string>();
            
            addComponentsFromJson(obj["components"], object, this);
            objects[obj["name"].get<std::string>()] = object;
        }

        for (auto object : objects)
        {
            for (auto comp : object.second->components)
                comp.second->init();
        }
    }
    
    Scene::~Scene()
    {
        delete camera;

        for (auto object : objects)
            delete object.second;

        for (auto texture : textures)
            delete texture.second;

        for (auto shader : shaders)
            delete shader.second;
    }

    void Scene::addComponentsFromJson(json& components, GameObject* object, Scene* scene)
    {
        if (components.contains("transform"))
        {
            object->addComponent<Components::TransformComponent>();
            json transformData = components["transform"];

            Components::TransformComponent* transformComponent = object->getComponent<Components::TransformComponent>();
            transformComponent->translation = glm::vec3(transformData["translation"][0], transformData["translation"][1], transformData["translation"][2]);
            transformComponent->scale = glm::vec3(transformData["scale"][0], transformData["scale"][1], transformData["scale"][2]);
            transformComponent->rotation = glm::vec3(transformData["rotation"][0], transformData["rotation"][1], transformData["rotation"][2]);
        }
        if (components.contains("render"))
        {
            object->addComponent<Components::RenderComponent>(object);
            json renderData = components["render"];


            Components::RenderComponent* renderComponent = object->getComponent<Components::RenderComponent>();
            renderComponent->shader = shaders[renderData["shader"].get<std::string>()];
            renderComponent->texture = textures[renderData["texture"].get<std::string>()];
        }
        if (components.contains("script"))
        {
            object->addComponent<Components::ScriptComponent>(object, scene, components["script"].get<std::string>());
        }
    }

    void Scene::RenderScene()
    {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        // TODO: change different place or put here from json
        camera->updateMatrix(45.0f, 0.01f, 10000.0f, true);

        for (auto object : objects)
        {
            Shader* shader = shaders["default"];

            object.second->update(time);
            object.second->render(shaders["default"], camera);
        }
    }

    std::string Scene::addObject(std::string name)
    {
        if (objects.find(name) == objects.end()) 
        {
            objects[name] = new Realgar::GameObject();
        }
        else 
        {
            return "Object already exists";
        }

        return " ";
    }
    GameObject* Scene::getObject(std::string name)
    {
        if (objects.find(name) != objects.end())
        {
            return objects[name];
        }
        else
        {
            return nullptr;
        }
    }

    int addObject(lua_State* L)
    {
        Scene* scene = *(Scene**)luaL_checkudata(L, 1, "Scene");

        const char* componentType = luaL_checkstring(L, 2);
        std::string ret = scene->addObject(componentType);
        
        if (ret != " ")
        {
            lua_pushstring(L, ret.c_str());
            lua_error(L);
        }

        return 0;
    }

    int getObject(lua_State* L)
    {
        Scene* scene = *(Scene**)luaL_checkudata(L, 1, "Scene");
        const char* componentType = luaL_checkstring(L, 2);
        GameObject* object = scene->getObject(componentType);


        if (object)
        {
            GameObject** objectPtr = (GameObject**)lua_newuserdata(L, sizeof(GameObject*));
            *objectPtr = object;
            luaL_getmetatable(L, "GameObject");
            lua_setmetatable(L, -2);
        }
        else
        {
            lua_pushnil(L);
            lua_pushstring(L, "Could not find Object");
            lua_error(L);
        }

        return 1; // Return the component userdata
    }

    void registerScene(lua_State* L) {
        luaL_newmetatable(L, "Scene");

        lua_pushstring(L, "__index");
        lua_pushvalue(L, -2);
        lua_settable(L, -3);

        static const luaL_Reg sceneMethods[] = {
            {"addObject", addObject},
            {"getObject", getObject},
            {NULL, NULL}
        };

        luaL_setfuncs(L, sceneMethods, 0);
        lua_pop(L, 1); 
    }


    void Scene::registerScene(lua_State* L)
    {
        Realgar::registerScene(L);
    }
}