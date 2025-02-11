#include "Core/Core.h"
#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char** argv)
{
    RED::Window window("RED - Test", SCR_WIDTH, SCR_HEIGHT);
    RED::Renderer::InitApi(window); // TODO : add automatically run compile.bat and add switch to change from opengl to vulkan

    RED::Shader defaultShader("Resources/Shaders/default.vert", "Resources/Shaders/default.frag");
    RED::Shader reversedShader("Resources/Shaders/halftransparent.vert", "Resources/Shaders/halftransparent.frag");
    RED::Texture* texture = new RED::Texture("Resources/Textures/modelface.png");


    RED::Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.f, 0.f, -8.f)); // TODO : add camera shader
    
    RED::Sprite sprite(window);
    RED::Sprite sprite1(window);



    RED::Audio audio;
    audio.PlayAudio("Game/Resources/Audio/BIG-Trouble.ogg");

    while (!glfwWindowShouldClose(window))
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        camera.updateMatrix(45.0f, 0.01f, 10000.0f, true);


        sprite.scale = glm::vec3(1, 1, 1);
        sprite.rotation.y = time * 180.0f;
        sprite.rotation.x = 45;
        sprite.translation = { 2, 0, -1 };
        sprite.texture = texture;

        sprite.Render(&defaultShader, &camera);

        sprite1.scale = glm::vec3(1, 1, 1);
        sprite1.rotation.y = time * 180.0f;
        sprite1.rotation.x = 45;
        sprite1.translation = { -2, 0, -1 };
        //sprite1.texture = texture;

        sprite1.Render(&reversedShader, &camera);
        


        RED::Input::processInput(window);

        RED::Renderer::RenderGraphics();
        window.Display();
    }

    glfwTerminate();
    return 0;
}

