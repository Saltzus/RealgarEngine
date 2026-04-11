#include "Scene.h"
#include "Texture.h"

namespace Realgar
{
    std::map<std::string, Texture*> Scene::current_textures;
    std::map<std::string, Shader*> Scene::current_shaders;
    std::map<std::string, Audio*> Scene::current_audio;

    Scene* Scene::currentScene;

    static auto startTime = std::chrono::high_resolution_clock::now();

    Scene::Scene(const char* filepath, bool askPath = false) : path(filepath), askPath(askPath)
    {
        startTime = std::chrono::high_resolution_clock::now();

        currentScene = this;

        unsigned int SCR_WIDTH = 800;
        unsigned int SCR_HEIGHT = 600;

        std::ifstream jsonFile(filepath);
     
        if (askPath)
            return;
        
        sceneData = json::parse(jsonFile);

        json cameraData = sceneData["camera"];
        glm::vec3 cameraTranslation(cameraData["translation"][0], cameraData["translation"][1], cameraData["translation"][2]);
        glm::vec3 cameraRotation(cameraData["rotation"][0], cameraData["rotation"][1], cameraData["rotation"][2]);
        float fov = cameraData["fov"];
        float nearPlane = cameraData["nearPlane"];
        float farPlane = cameraData["farPlane"];

        camera = new Camera
        (
            cameraTranslation,
            cameraRotation,
            fov,
            nearPlane,
            farPlane,
            true
        );

        for (auto& [name, shdr] : sceneData["shaders"].items())
        {
            std::string vertex = Realgar::FileManager::getResource(shdr["vertex"].get<std::string>());
            std::string fragment = Realgar::FileManager::getResource(shdr["fragment"].get<std::string>());
            Shader* shader = new Shader(vertex, fragment);

            shaders[name] = shader;
        }

        current_shaders = shaders;

        for (auto& [name, txtr] : sceneData["textures"].items())
        {
            std::string path = txtr.get<std::string>();
            Texture* texture = new Texture(Realgar::FileManager::getResource(path));

            textures[name] = texture;
        }

        current_textures = textures;

        for (auto& [name, aud] : sceneData["audio"].items())
        {
            std::string file = aud["file"].get<std::string>();
            bool spatialized = aud["spatialized"].get<bool>();
            Audio* audio = new Audio(Realgar::FileManager::getResource(file), spatialized);

            audio_map[name] = audio;
        }

        current_audio = audio_map;

        for (json obj : sceneData["gameObjects"])
        {
            GameObject* object = new GameObject;
            object->name = obj["name"].get<std::string>();
            
            addComponentsFromJson(obj["components"], object, this);
            objects[obj["name"].get<std::string>()] = object;
        }
    }

    void Scene::reloadSceneLater()
    {
        startTime = std::chrono::high_resolution_clock::now();

        std::ifstream jsonFile(reloadPath);
        sceneData = json::parse(jsonFile);

        json cameraData = sceneData["camera"];
        glm::vec3 cameraTranslation(cameraData["translation"][0], cameraData["translation"][1], cameraData["translation"][2]);
        glm::vec3 cameraRotation(cameraData["rotation"][0], cameraData["rotation"][1], cameraData["rotation"][2]);
        float fov = cameraData["fov"];
        float nearPlane = cameraData["nearPlane"];
        float farPlane = cameraData["farPlane"];

        if (camera == nullptr)
        {
            camera = new Camera
            (
                cameraTranslation,
                cameraRotation,
                fov,
                nearPlane,
                farPlane,
                true
            );
        }
        else
        {
            camera->cameraPosition = -cameraTranslation;
            camera->cameraRotation = cameraRotation;
            camera->fov = fov;
            camera->nearPlane = nearPlane;
            camera->farPlane = farPlane;
        }

        for (auto object : objects)
            delete object.second;

        for (auto texture : textures)
            delete texture.second;

        for (auto shader : shaders)
            delete shader.second;

        for (auto audio : audio_map)
            delete audio.second;


        shaders.clear();
        for (auto& [name, shdr] : sceneData["shaders"].items())
        {
            std::string vertex = Realgar::FileManager::getResource(shdr["vertex"].get<std::string>());
            std::string fragment = Realgar::FileManager::getResource(shdr["fragment"].get<std::string>());
            Shader* shader = new Shader(vertex, fragment);

            shaders[name] = shader;
        }

        current_shaders = shaders;

        textures.clear();
        for (auto& [name, txtr] : sceneData["textures"].items())
        {
            std::string path = txtr.get<std::string>();
            Texture* texture = new Texture(Realgar::FileManager::getResource(path));

            textures[name] = texture;
        }

        current_textures = textures;

        audio_map.clear();
        for (auto& [name, aud] : sceneData["audio"].items())
        {
            std::string file = aud["file"].get<std::string>();
            bool spatialized = aud["spatialized"].get<bool>();
            Audio* audio = new Audio(Realgar::FileManager::getResource(file), spatialized);

            audio_map[name] = audio;
        }

        current_audio = audio_map;

        objects.clear();
        for (json obj : sceneData["gameObjects"])
        {
            GameObject* object = new GameObject;
            object->name = obj["name"].get<std::string>();

            addComponentsFromJson(obj["components"], object, this);
            objects[obj["name"].get<std::string>()] = object;
        }
    }
    void Scene::reloadScene(const char* filepath)
    {
        path = filepath;

        askPath = false;
        reloadPath = filepath;
        reload = true;
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

        for (auto audio : audio_map)
            delete audio.second;
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
            object->addComponent<Components::ScriptComponent>(object, scene, Realgar::FileManager::getResource(components["script"].get<std::string>()));
        }
        if (components.contains("audioListener"))
        {
            object->addComponent<Components::AudioListenerComponent>();
            json transformData = components["audioListener"];
           
            Components::TransformComponent* transformComponent = object->getComponent<Components::TransformComponent>();
            Components::AudioListenerComponent* audioListenerComponent = object->getComponent<Components::AudioListenerComponent>();

            if (transformComponent)
            {
                audioListenerComponent->parentTranslation = &transformComponent->translation;
                audioListenerComponent->parentRotation = &transformComponent->rotation;
            }

            audioListenerComponent->translation = glm::vec3(transformData["translation"][0], transformData["translation"][1], transformData["translation"][2]);
            audioListenerComponent->rotation = glm::vec3(transformData["rotation"][0], transformData["rotation"][1], transformData["rotation"][2]);
        }
        if (components.contains("audioPlayer"))
        {
            object->addComponent<Components::AudioPlayerComponent>();
            json transformData = components["audioPlayer"];

            Components::TransformComponent* transformComponent = object->getComponent<Components::TransformComponent>();
            Components::AudioPlayerComponent* audioPlayerComponent = object->getComponent<Components::AudioPlayerComponent>();
            
            if (transformComponent)
                audioPlayerComponent->parentTranslation = &transformComponent->translation;
            
            audioPlayerComponent->translation = glm::vec3(transformData["translation"][0], transformData["translation"][1], transformData["translation"][2]);
            
            if (audio_map.find(transformData["audio"].get<std::string>()) != audio_map.end())
                audioPlayerComponent->sound = &audio_map[transformData["audio"].get<std::string>()]->sound;
            else
                audioPlayerComponent->sound = nullptr;
        }
    }

    void Scene::setStatus(bool status)
    {
        startTime = std::chrono::high_resolution_clock::now();
        this->status = status;

        if (status)
        {
            for (auto object : objects)
            {
                for (auto comp : object.second->components)
                    comp.second->init();
            }
        }
    }

    void Scene::RenderScene(Window* window)
    {
        if (reload)
        {
            reload = false;
            reloadSceneLater();
        }

        if (!window->editor && !init)
        {
            init = true;
            for (auto object : objects)
            {
                for (auto comp : object.second->components)
                    comp.second->init();
            }
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        // TODO: change different place or put here from json
        camera->updateMatrix(*window);

        if (Realgar::Renderer::GetGraphicsApi() == Realgar::GraphicsApis::OpenGL && Window::editor)
        {
            Realgar::Opengl::Opengl::RescaleFramebuffer();

            glBindFramebuffer(GL_FRAMEBUFFER, Realgar::Opengl::Opengl::FBO);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        for (auto object : objects)
        {
            if (!window->editor || status) object.second->update(time);
            object.second->render(shaders["default"], camera);
        }



        if (Realgar::Renderer::GetGraphicsApi() == Realgar::GraphicsApis::OpenGL && Window::editor)
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    std::string Scene::addObject(std::string name)
    {
        name.erase(name.begin(), std::find_if(name.begin(), name.end(), [](unsigned char ch) 
        {
            return !std::isspace(ch);
        }));
        name.erase(std::find_if(name.rbegin(), name.rend(), [](unsigned char ch) 
        {
            return !std::isspace(ch);
        }).base(), name.end());


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

    std::string Scene::addShader(std::string name, std::string vert, std::string frag)
    {
        name.erase(name.begin(), std::find_if(name.begin(), name.end(), [](unsigned char ch)
        {
            return !std::isspace(ch);
        }));
        name.erase(std::find_if(name.rbegin(), name.rend(), [](unsigned char ch)
        {
            return !std::isspace(ch);
        }).base(), name.end());


        if (shaders.find(name) == shaders.end())
        {
            current_shaders[name] = new Realgar::Shader(vert,frag);
        }
        else
        {
            return "Shader already exists";
        }

        return " ";
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