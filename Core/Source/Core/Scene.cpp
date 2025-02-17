#include "Scene.h"
#include "Texture.h"

namespace RED
{
    

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

        for (auto& [name, txtr] : sceneData["textures"].items())
        {
            std::string path = txtr.get<std::string>();
            Texture* texture = new Texture(path.c_str());

            textures[name] = texture;
        }

        for (json obj : sceneData["gameObjects"])
        {
            GameObject* object = new GameObject;
            object->name = obj["name"].get<std::string>();
            
            addComponentsFromJson(obj["components"], object);
            objects.push_back(object);
        }
    }
    
    Scene::~Scene()
    {
        delete camera;

        for (auto* object : objects)
            delete object;

        for (auto texture : textures)
            delete texture.second;

        for (auto shader : shaders)
            delete shader.second;
    }

    void Scene::addComponentsFromJson(json& components, GameObject* object)
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
    }

    void Scene::RenderScene()
    {
        // TODO: change different place or put here from json
        camera->updateMatrix(45.0f, 0.01f, 10000.0f, true);

        for (auto object : objects)
        {
            Shader* shader = shaders["default"];

            object->update(0.016f);
            object->render(shaders["default"], camera);
        }






























    }
}