#include "Core/Core.h"
#include <iostream>

#include <lua.hpp>

// settings
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 360;

int main(int argc, char** argv)
{
    Realgar::Window window("RED - Test", SCR_WIDTH, SCR_HEIGHT);
    Realgar::Renderer::InitApi(window); // TODO : add automatically run compile.bat and add switch to change from opengl to vulkan

    Realgar::Input::SetWindow(window);
    Realgar::LuaSystem::Init();
    Realgar::AudioSystem::Init();

    Realgar::Scene scene(Realgar::FileManager::getResource("Scenes/Menu.rdsc"));


    while (!glfwWindowShouldClose(window))
    {
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 0, 0, 120);

        scene.RenderScene(&window);

        Realgar::Renderer::RenderGraphics();
        window.Display();
    }

    Realgar::LuaSystem::Close();
    Realgar::AudioSystem::Close();

    glfwTerminate();
    return 0;
}

