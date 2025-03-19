#include "Gui.h"
#include "Core/graphicsApi/opengl/OpenGlManager.h"

ImGuiWindowFlags windowflags;

Gui::Gui(Realgar::Window* window, Realgar::Scene* scene) : scene(scene)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    if (Realgar::Renderer::GetGraphicsApi() == Realgar::GraphicsApis::Vulkan)
    {
        ImGui_ImplGlfw_InitForVulkan(*window, true);
        ImGui_ImplVulkan_Init(&Realgar::Vulkan::Vulkan::vulkan->init_info);

        Realgar::Vulkan::Vulkan::vulkan->addSceneImages();
    }
    else
    {
        ImGui_ImplGlfw_InitForOpenGL(*window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }


    viewport = ImGui::GetMainViewport();
}

Gui::~Gui()
{
    if (Realgar::Renderer::GetGraphicsApi() == Realgar::GraphicsApis::Vulkan)
        ImGui_ImplVulkan_Shutdown();
    else
        ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::Render()
{
    if (Realgar::Renderer::GetGraphicsApi() == Realgar::GraphicsApis::Vulkan)
        ImGui_ImplVulkan_NewFrame();
    else
        ImGui_ImplOpenGL3_NewFrame();
    
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    MainWindow();
    SceneWindow();
    ProperitiesWindow();
    FilesWindow();

    ImGui::EndFrame();

    ImGui::Render();

    if (Realgar::Renderer::GetGraphicsApi() == Realgar::GraphicsApis::OpenGL)
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool open;

void Gui::MainWindow()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y));

    ImGui::Begin("Main", &open, window_flags);

    ImGuiID dock = ImGui::GetID("Dock");
    ImGui::DockSpace(dock, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();

    ImGui::Begin("Viewport", &open, windowflags);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    Realgar::Window::viewportWidth = viewportPanelSize.x;
    Realgar::Window::viewportHeight = viewportPanelSize.y;

    if (Realgar::Renderer::GetGraphicsApi() == Realgar::GraphicsApis::Vulkan)
    {
        


        uint32_t frame = static_cast<uint32_t>(Realgar::Vulkan::Vulkan::vulkan->currentSceneImage);
        ImGui::Image((ImTextureID)Realgar::Vulkan::Vulkan::vulkan->sceneImages[frame], ImVec2{ viewportPanelSize.x, viewportPanelSize.y });
    }
    else
    {
        ImGui::GetWindowDrawList()->AddImage
        (
            (ImTextureID)Realgar::Opengl::Opengl::framebufferTexture,
            ImVec2(ImGui::GetCursorScreenPos()),
            ImVec2(ImGui::GetCursorScreenPos().x + viewportPanelSize.x, ImGui::GetCursorScreenPos().y + viewportPanelSize.y),
            ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
}
void Gui::SceneWindow()
{
    ImGui::Begin("Scene", &open, windowflags);
    ImGui::Text("Camera :  "); ImGui::SameLine(); ImGui::Button("+");

    ImGui::Bullet();
    if (ImGui::Button("Camera"))
    {
        selected = "Camera";
        selectedType = 1;
    }

    ImGui::NewLine();

    ImGui::Text("GameObjects :  "); ImGui::SameLine(); ImGui::Button("+");

    for (auto object : scene->objects)
    {
        ImGui::Bullet();
        if (ImGui::Button(object.first.c_str())) 
        {
            selected = object.first.c_str();
            selectedType = 2;
        }
    }

    ImGui::NewLine();

    ImGui::Text("Shaders :  "); ImGui::SameLine(); ImGui::Button("+");

    for (auto object : scene->current_shaders)
    {
        ImGui::Bullet();
        if (ImGui::Button(object.first.c_str()))
        {
            selected = object.first.c_str();
            selectedType = 3;
        }
    }

    ImGui::NewLine();

    ImGui::Text("Textures :  "); ImGui::SameLine(); ImGui::Button("+");

    for (auto object : scene->current_textures)
    {
        ImGui::Bullet();
        if (ImGui::Button(object.first.c_str()))
        {
            selected = object.first.c_str();
            selectedType = 4;
        }
    }

    ImGui::NewLine();

    ImGui::Text("Audio :  "); ImGui::SameLine(); ImGui::Button("+");

    for (auto object : scene->current_audio)
    {
        ImGui::Bullet();
        if (ImGui::Button(object.first.c_str()))
        {
            selected = object.first.c_str();
            selectedType = 5;
        }
    }

    ImGui::NewLine();

    ImGui::End();
}
void Gui::ProperitiesWindow()
{
    ImGui::Begin("Properities", &open, windowflags);
    ImGui::Text(selected.c_str());
    ImGui::NewLine();

    switch (selectedType)
    {
    case 1:
        CameraProperities();
        break;
    default:
        break;
    }

    ImGui::End();
}
void Gui::FilesWindow()
{
    ImGui::Begin("Files", &open, windowflags);
    ImGui::End();
}

void Gui::CameraProperities() 
{
    float* pos[3] = { &scene->camera->cameraPosition.y, &scene->camera->cameraPosition.y, &scene->camera->cameraPosition.y };
    float* rot[3] = { &scene->camera->cameraRotation.y, &scene->camera->cameraRotation.y, &scene->camera->cameraRotation.y };
    int* size[2] = { &scene->camera->width, &scene->camera->height};

    ImGui::Text("Position xyz");
    ImGui::InputFloat3("##Position", *pos);

    ImGui::Text("Rotation xyz");
    ImGui::InputFloat3("##Rotation", *rot);

    ImGui::Text("Size xyz");
    ImGui::InputInt2("##Size", *size);
}
void Gui::ObjectProperities() 
{

}
void Gui::ShaderProperities() 
{

}
void Gui::TextureProperities()
{

}
void Gui::AudioProperities()  
{

}