#pragma once
#include <json.hpp>
using json = nlohmann::json;

#include "GameObject.h"
#include "Audio.h"

namespace Realgar
{
    class Scene
    {
    public:
        Scene(const char* filepath);
        ~Scene();

        static std::map<std::string, Texture*> current_textures;
        static std::map<std::string, Shader*> current_shaders;
        static std::map<std::string, Audio*> current_audio;

        static void registerScene(lua_State* L);
        static Scene* currentScene;

        std::string addObject(std::string);
        GameObject* getObject(std::string);

        Camera* camera = nullptr;
        void RenderScene();
    private:
        void addComponentsFromJson(json& componentData, GameObject* object, Scene* scene);
        json sceneData;

        std::map<std::string, GameObject*> objects;
        std::map<std::string, Texture*> textures;
        std::map<std::string, Shader*> shaders;
        std::map<std::string , Audio*> audio_map;
    };
    
} 
