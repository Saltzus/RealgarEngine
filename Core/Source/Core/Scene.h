#pragma once
#include <json.hpp>
using json = nlohmann::json;

#include "GameObject.h"

namespace Realgar
{
    class Scene
    {
    public:
        Scene(const char* filepath);
        ~Scene();

        static std::map<std::string, Texture*> current_textures;
        static std::map<std::string, Shader*> current_shaders;

        static void registerScene(lua_State* L);
        static Scene* currentScene;

        std::string addObject(std::string);
        GameObject* getObject(std::string);

        void RenderScene();
    private:
        void addComponentsFromJson(json& componentData, GameObject* object, Scene* scene);
        json sceneData;

        std::map<std::string, GameObject*> objects;
        std::map<std::string, Texture*> textures;
        std::map<std::string , Shader*> shaders;

        Camera* camera = nullptr;
    };
    
} 
