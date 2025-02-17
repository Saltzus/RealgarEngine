#include "Core/Core.h"
#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char** argv)
{
    RED::Window window("RED - Test", SCR_WIDTH, SCR_HEIGHT);
    RED::Renderer::InitApi(window); // TODO : add automatically run compile.bat and add switch to change from opengl to vulkan


    RED::Scene scene("Resources/Scenes/Menu.rdsc");

    RED::Audio audio;
    audio.PlayAudio("Game/Resources/Audio/BIG-Trouble.ogg");

    while (!glfwWindowShouldClose(window))
    {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        scene.RenderScene();

        RED::Input::processInput(window);

        RED::Renderer::RenderGraphics();
        window.Display();
    }

    glfwTerminate();
    return 0;
}

