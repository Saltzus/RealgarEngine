#include "Core/Core.h"

#include <glm/common.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main(int argc, char** argv)
{
    RED::Window window("RED - Test", 800, 600);
    RED::Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.f,0.f,-8.f));
    RED::Renderer::InitApi(window);

    //RED::Shader shader("Game/Resources/Shaders/default.vert", "Game/Resources/Shaders/default.frag");
    RED::Shader shader("Resources/Shaders/defaultVulkan.vert", "Resources/Shaders/defaultVulkan.frag");


    RED::Audio audio;
    audio.PlayAudio("Game/Resources/Audio/BIG-Trouble.ogg");

    int sus = 0;

    RED::Sprite sprite(window);
    RED::Texture* texture = new RED::Texture("Resources/Textures/modelface.png");
    RED::Sprite sprite1(window);

    while (!glfwWindowShouldClose(window))
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        sus ++;
        camera.updateMatrix(45.0f, 0.01f, 10000.0f, true);


        sprite.scale = glm::vec3(1, 1, 1);
        sprite.rotation.y = time * 180.0f;
        sprite.rotation.x = 45;
        sprite.translation = { 2, 0, -1 };
        sprite.texture = texture;

        sprite.Render(&shader, &camera);

        sprite1.scale = glm::vec3(1, 1, 1);
        sprite1.rotation.y = time * 180.0f;
        sprite1.rotation.x = 45;
        sprite1.translation = { -2, 0, -1 };
        sprite1.texture = texture;

        sprite1.Render(&shader, &camera);
        


        RED::Input::processInput(window);

        RED::Renderer::RenderGraphics();
        window.Display();
    }

    glfwTerminate();
    return 0;
}

