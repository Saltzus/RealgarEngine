#pragma once
#include <json.hpp>
using json = nlohmann::json;

#include "GameObject.h"

namespace RED
{
    class Scene
    {
    public:
        Scene(const char* filepath);
        ~Scene();

        void RenderScene();
    private:
        void addComponentsFromJson(json& componentData, GameObject* object);
        json sceneData;

        std::vector<GameObject*> objects;
        std::map<std::string, Texture*> textures;
        std::map<std::string , Shader*> shaders;

        Camera* camera = nullptr;
    };
    
} 
