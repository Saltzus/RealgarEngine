#include "Core/Core.h"
#include <iostream>

#include <lua.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char** argv)
{
    Realgar::Window window("RED - Test", SCR_WIDTH, SCR_HEIGHT);
    Realgar::Renderer::InitApi(window); // TODO : add automatically run compile.bat and add switch to change from opengl to vulkan

    Realgar::LuaSystem::Init();
    Realgar::Scene scene("Resources/Scenes/Menu.rdsc");


    // TODO: change miniaudio to Openal-soft 
    //RED::Audio audio;
    //audio.PlayAudio("Resources/Audio/BIG-Trouble.ogg");

    while (!glfwWindowShouldClose(window))
    {
        scene.RenderScene();

        Realgar::Input::processInput(window);

        Realgar::Renderer::RenderGraphics();
        window.Display();
    }

    Realgar::LuaSystem::Close();
    glfwTerminate();
    return 0;
}

