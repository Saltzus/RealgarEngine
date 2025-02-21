#include "Core/Core.h"
#include <iostream>

#include <lua.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char** argv)
{
    RED::Window window("RED - Test", SCR_WIDTH, SCR_HEIGHT);
    RED::Renderer::InitApi(window); // TODO : add automatically run compile.bat and add switch to change from opengl to vulkan

    RED::LuaSystem::Init();
    RED::Scene scene("Resources/Scenes/Menu.rdsc");


    // TODO: change miniaudio to Openal-soft 
    //RED::Audio audio;
    //audio.PlayAudio("Resources/Audio/BIG-Trouble.ogg");

    while (!glfwWindowShouldClose(window))
    {
        scene.RenderScene();

        RED::Input::processInput(window);

        RED::Renderer::RenderGraphics();
        window.Display();
    }

    RED::LuaSystem::Close();
    glfwTerminate();
    return 0;
}

