#include "Gui.h"
#include "Core/graphicsApi/opengl/OpenGlManager.h"
#include "Core/GameObject.h"

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
    ImGui::Text("%s", selected.c_str());                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
    ImGui::Separator();

    switch (selectedType)
    {
    case 1:
        CameraProperities();
        break;
    case 2:
        ObjectProperities();
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

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth))
    {
        ImGui::SetCursorPosX(4);
        if (ImGui::BeginTable("table1", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable, ImVec2(ImGui::GetContentRegionAvail().x + 4, 0)))
        {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Position");
            ImGui::TableSetColumnIndex(1);
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputFloat3("##Position", *pos);

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Rotation");
            ImGui::TableSetColumnIndex(1);
            ImGui::InputFloat3("##Rotation", *rot);

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Size");
            ImGui::TableSetColumnIndex(1);
            ImGui::InputInt2("##Size", *size);

            ImGui::EndTable();
        }
    }
}
void Gui::ObjectProperities() 
{
    Realgar::GameObject* object = scene->getObject(selected);

    Realgar::Components::TransformComponent* transform = object->getComponent<Realgar::Components::TransformComponent>();
    if (transform != nullptr)
    {
        if (ImGui::CollapsingHeader("TransformComponent", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth))
        {
            float* pos[3] = { &transform->translation.x, &transform->translation.y, &transform->translation.z };
            float* rot[3] = { &transform->rotation.x, &transform->rotation.y, &transform->rotation.z };
            float* size[3] = { &transform->scale.x, &transform->scale.y, &transform->scale.z };

            ImGui::SetCursorPosX(4);
            if (ImGui::BeginTable("table2", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable, ImVec2(ImGui::GetContentRegionAvail().x + 4, 0)))
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Position");
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::InputFloat3("##Position", *pos);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Rotation");
                ImGui::TableSetColumnIndex(1);
                ImGui::InputFloat3("##Rotation", *rot);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Size");
                ImGui::TableSetColumnIndex(1);
                ImGui::InputFloat3("##Size", *size);

                ImGui::EndTable();
            }
        }
    }

    Realgar::Components::RenderComponent* render = object->getComponent<Realgar::Components::RenderComponent>();
    if (render != nullptr)
    {
        if (ImGui::CollapsingHeader("RenderComponent", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth))
        {
            static int item_selected_idx1 = 0; // Here we store our selection data as an index.
            static int item_selected_idx2 = 0; // Here we store our selection data as an index.

            // Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
            const char* combo_preview_value1;
            const char* combo_preview_value2;

            std::vector<const char*> shaderItems;
            for (auto& shader : scene->current_shaders)
            {
                shaderItems.push_back(shader.first.c_str());
                if (shader.second == render->shader)
                {
                    item_selected_idx1 = shaderItems.size() - 1;
                    combo_preview_value1 = shader.first.c_str();
                }
            }

            std::vector<const char*> textureItems;
            for (auto& texture : scene->current_textures)
            {
                textureItems.push_back(texture.first.c_str());
                if (texture.second == render->texture)
                {
                    item_selected_idx2 = textureItems.size() - 1;
                    combo_preview_value2 = texture.first.c_str();
                }
            }

            ImGui::SetCursorPosX(4);
            if (ImGui::BeginTable("table3", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable, ImVec2(ImGui::GetContentRegionAvail().x + 4, 0)))
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Shader");
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::BeginCombo("Select shader", combo_preview_value1))
                {
                    for (int n = 0; n < shaderItems.size(); n++)
                    {
                        const bool is_selected = (item_selected_idx1 == n);
                        if (ImGui::Selectable(shaderItems[n], is_selected))
                        {
                            render->shader = scene->current_shaders[shaderItems[n]];
                            item_selected_idx1 = n;
                        }

                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Texture");
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::BeginCombo("Select texture", combo_preview_value2))
                {
                    for (int n = 0; n < textureItems.size(); n++)
                    {
                        const bool is_selected = (item_selected_idx2 == n);
                        if (ImGui::Selectable(textureItems[n], is_selected))
                        {
                            render->texture = scene->current_textures[textureItems[n]];
                            item_selected_idx2 = n;
                        }

                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                ImGui::EndTable();
            }
        }
    }
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