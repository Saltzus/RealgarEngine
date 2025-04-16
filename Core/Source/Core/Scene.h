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
        Scene(const char* filepath, bool askPath);
        ~Scene();

        static std::map<std::string, Texture*> current_textures;
        static std::map<std::string, Shader*> current_shaders;
        static std::map<std::string, Audio*> current_audio;

        void reloadScene(const char* filepath);
        static void registerScene(lua_State* L);
        static Scene* currentScene;

        std::string addObject(std::string);
        GameObject* getObject(std::string);

        std::map<std::string, GameObject*> objects;

        void setStatus(bool status);
        bool getStatus() { return status; }
        std::string getPath() { return path; }

        bool askPath = false;


        Camera* camera = nullptr;
        void RenderScene(Window* window);
    private:
        void addComponentsFromJson(json& componentData, GameObject* object, Scene* scene);
        json sceneData;

        bool status = false;
        std::string path;

        std::map<std::string, Texture*> textures;
        std::map<std::string, Shader*> shaders;
        std::map<std::string , Audio*> audio_map;

        bool init = false;

        bool reload = false;
        std::string reloadPath;
        void reloadSceneLater();
    };
    
} 
