#include "Core/Core.h"
#include "Gui.h"



// settings
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 360;

int main()
{
    Realgar::Window window ("Editor", SCR_WIDTH, SCR_HEIGHT, true);
    Realgar::Renderer::InitApi(window); // TODO : add automatically run compile.bat and add switch to change from opengl to vulkan

    Realgar::Input::SetWindow(window);
    Realgar::LuaSystem::Init();
    Realgar::AudioSystem::Init();

    Realgar::Scene scene("", true);

    Gui gui(&window,&scene);

    while (!glfwWindowShouldClose(window))
    {
        if (scene.askPath)
            gui.RenderProjectSelect();
        else
        {
            scene.RenderScene(&window);
            gui.Render();
        }

        Realgar::Renderer::RenderGraphics();
        window.Display();
    }

    Realgar::LuaSystem::Close();
    Realgar::AudioSystem::Close();
    glfwTerminate();
	return 0;
}