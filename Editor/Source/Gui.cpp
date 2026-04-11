
#include "Gui.h"

#include "Core/graphicsApi/opengl/OpenGlManager.h"
#include "Core/GameObject.h"
#include "Core/Texture.h"
#include "Core/FileManager.h"

#include <string>

ImGuiWindowFlags windowflags;

std::vector<File> files;

std::vector<File> generateFileChildren(std::string path)
{
    std::vector<File> children;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        File file;
        file.name = entry.path().filename().string();
        file.path = entry.path().string();
        file.extention = entry.path().filename().extension().string();
        file.isDirectory = entry.is_directory();
        
        if (file.isDirectory)
            file.children = generateFileChildren(file.path);

        children.emplace_back(file);
    }

    return children;
}

void generateFiles(std::string rootPath)
{
    for (const auto& entry : std::filesystem::directory_iterator(rootPath))
    {
        File file;
        file.name = entry.path().filename().string();
        file.path = entry.path().string();
        file.extention = entry.path().filename().extension().string();
        file.isDirectory = entry.is_directory();

        if (file.isDirectory)
            file.children = generateFileChildren(file.path);

        files.emplace_back(file);
    }
}

Gui::Gui(Realgar::Window* window, Realgar::Scene* scene) : scene(scene), window(window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg].w = 0.0f;

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

    generateFiles("Resources");
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
    fileEditor.Render();
    ProperitiesWindow();
    FilesWindow();

    ImGui::EndFrame();

    ImGui::Render();

    if (Realgar::Renderer::GetGraphicsApi() == Realgar::GraphicsApis::OpenGL)
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool open = true;
std::vector<std::string> errors;
std::vector<std::string> scenes;
std::string selectedLocation = "";
std::string selectedScene;

void Gui::RenderProjectSelect()
{
    if (Realgar::Renderer::GetGraphicsApi() == Realgar::GraphicsApis::Vulkan)
        ImGui_ImplVulkan_NewFrame();
    else
        ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glfwSetWindowSize(*window, 640, 360 / 2);
    ImGui::SetNextWindowSize(ImVec2(640, 360 / 2));
    ImGui::SetNextWindowPos(ImVec2(0,0));

    ImGui::Begin("SelectProject", &open, ImGuiWindowFlags_NoDecoration);

    char buffer[256 * 2];
    strncpy(buffer, selectedLocation.c_str(), sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    ImGui::Dummy({1,1});
    ImGui::Text("Select path to desired resource location");
    if (ImGui::InputText("Path", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        selectedLocation = buffer;

        errors.clear();
        scenes.clear();

        if (!Realgar::FileManager::folderExists(selectedLocation))
            errors.push_back(selectedLocation + " not found!");


        if (!Realgar::FileManager::folderExists(selectedLocation + "/Scenes"))
            errors.push_back(selectedLocation + "/Scenes not found!");
        else
        {
            scenes = Realgar::FileManager::getFolderChildren(selectedLocation + "/Scenes");
            if (scenes.empty())
                errors.push_back(selectedLocation + "/Scenes is empty!");
        }
    }

    ImGui::Dummy({ 1,1 });

    if (errors.empty() && !scenes.empty() && ImGui::BeginCombo("Scene", selectedScene.c_str()))
    {
        for (auto scene : scenes)
        {
            if (ImGui::Selectable(scene.c_str()))
                selectedScene = scene.c_str();
        }

        ImGui::EndCombo();
    }

    ImGui::Dummy({ 1,1 });

    if (!selectedScene.empty())
    {
        if (ImGui::Button("Confirm"))
        {
            glfwSetWindowSize(*window, 640, 360);
            Realgar::FileManager::setResourcePath(selectedLocation);
            scene->reloadScene((selectedLocation + "/Scenes/" + selectedScene).c_str());
        }
    }

    for (auto error : errors)
        ImGui::Text(error.c_str());

    ImGui::End();

    ImGui::EndFrame();

    ImGui::Render();

    if (Realgar::Renderer::GetGraphicsApi() == Realgar::GraphicsApis::OpenGL)
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Gui::Menu()
{
    const char* text = "Start";
    if (scene->getStatus())
        text = "Stop";

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Folder"))
        {
            if (ImGui::MenuItem("Open"))
            {
            }
            ImGui::EndMenu();
        }
        if (ImGui::Button(text))
        {
            if (text == "Start")
                scene->setStatus(true);
            else
            {
                scene->reloadScene(scene->getPath().c_str());
                scene->setStatus(false);
            }
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();
}

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

    Menu();

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
            ImVec2(0, 1), ImVec2(1, 0)
        );
    }
    ImGui::End();
}

bool CircleButton(float radius = 5.3f)
{
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    ImVec2 center = ImVec2(cursorPos.x + radius, cursorPos.y + radius + 2);
    ImVec2 size = ImVec2(radius * 2, radius * 2);

    // Create an invisible button to handle interaction
    ImGui::SetCursorScreenPos(cursorPos);
    ImGui::InvisibleButton("##plus_button", size);

    // Get interaction state
    bool hovered = ImGui::IsItemHovered();
    bool held = ImGui::IsItemActive();
    bool clicked = ImGui::IsItemClicked();

    // Draw the circle
    ImU32 color = IM_COL32(255, 255, 255, held ? 80 : hovered ? 60 : 30);
    ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, color, 20);

    // Draw the plus sign
    float plusSize = radius * 0.5f;
    ImU32 plusColor = IM_COL32(255, 255, 255, 200);
    ImGui::GetWindowDrawList()->AddLine(
        ImVec2(center.x - plusSize, center.y),
        ImVec2(center.x + plusSize, center.y),
        plusColor, 0.005f
    );
    ImGui::GetWindowDrawList()->AddLine(
        ImVec2(center.x, center.y - plusSize),
        ImVec2(center.x, center.y + plusSize),
        plusColor, 0.005f
    );

    return clicked;
}

void Gui::Popups(std::string id)
{
    char buffer[256];

    strncpy(buffer, selected.c_str(), sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';


    if (id == "GameObjects" && ImGui::BeginPopup("GameObjects ##popup"))
    {
        ImGui::Text("Object Name:");
        if (ImGui::InputText("##ObjectName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            scene->addObject(buffer);

        ImGui::EndPopup();
    }

    if (id == "Shaders" && ImGui::BeginPopup("Shaders ##popup"))
    {
        ImGui::Text("Shader Name:");

        std::string shaderName = "";
        std::string vertShader = "";
        std::string fragShader = "";

        if (ImGui::InputText("##ShadersName", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            scene->addShader(buffer, "Resources/Shaders/default.vert", "Resources/Shaders/default.frag");

        ImGui::EndPopup();
    }
}

bool Gui::CustomTreeNode(std::string label)
{
    bool treeOpen = ImGui::TreeNodeEx(label.c_str(), ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_SpanAvailWidth);
    bool treeHovered = ImGui::IsItemHovered();

    if (treeHovered)
    {
        ImGui::SameLine(0, 15);
        if (CircleButton())
        {
            if (label == "GameObjects")
                ImGui::OpenPopup("GameObjects ##popup", ImGuiPopupFlags_AnyPopupId);
            else if (label == "Shaders")
                ImGui::OpenPopup("Shaders ##popup");
        }
    }

    Popups(label);
    return treeOpen;
}

void Gui::SceneWindow()
{
    // Set custom button style colors for transparency.
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.05f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f, 1.0f, 1.0f, 0.1f));

    ImGui::Begin("Scene", &open, windowflags);

    if (ImGui::TreeNode("Camera"))
    {
        if (ImGui::Selectable(" Camera"))
        {
            selected = "Camera";
            selectedType = 1;
        }

        ImGui::TreePop();
    }

    if (CustomTreeNode("GameObjects"))
    {
        for (auto object : scene->objects)
        {
            if (ImGui::Selectable((" " + object.first + "##object").c_str()))
            {
                selected = object.first.c_str();
                selectedType = 2;
            }
        }

        ImGui::TreePop();
    }

    if (CustomTreeNode("Shaders"))
    {
        for (auto object : scene->current_shaders)
        {
            if (ImGui::Selectable((" " + object.first + "##shader").c_str()))
            {
                selected = object.first.c_str();
                selectedType = 3;
            }
        }
        ImGui::TreePop();
    }

    if (CustomTreeNode("Textures"))
    {
        for (auto object : scene->current_textures)
        {
            if (ImGui::Selectable((" " + object.first + "##texture").c_str()))
            {
                selected = object.first.c_str();
                selectedType = 4;
            }
        }
        ImGui::TreePop();
    }

    if (CustomTreeNode("Audio"))
    {
        for (auto object : scene->current_audio)
        {
            if (ImGui::Selectable((" " + object.first + "##audio").c_str()))
            {
                selected = object.first.c_str();
                selectedType = 5;
            }
        }
        ImGui::TreePop();
    }


    ImGui::PopStyleColor(3);
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
        break;
    case 3:
        ShaderProperities();
        break;
    case 4:
        TextureProperities();
        break;
    case 5:
        AudioProperities();
        break;
    default:
        break;
    }

    ImGui::End();
}

void Gui::showFiles(std::vector<File> files)
{
    for (File file : files)
    {  
        if (file.isDirectory)
        {
            if (ImGui::TreeNodeEx(file.name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
            {
                showFiles(file.children);
                ImGui::TreePop();
            }
        }
        else
        {
            if (ImGui::Selectable(file.name.c_str()))
            {
                if (!fileEditor.editor.CanUndo())
                    fileEditor.ChangeFile(file.path.c_str());
                else
                    ImGui::OpenPopup(("##" + file.name).c_str());
            }

            if (ImGui::BeginPopup(("##" + file.name).c_str()))
            {
                ImGui::Text("Unsaved changes will be discarded!");
                if (ImGui::Button("Ok")) { fileEditor.ChangeFile(file.path.c_str()); ImGui::CloseCurrentPopup(); }
                ImGui::SameLine();
                if (ImGui::Button("Stop")){ ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }
        }
    }
}
void Gui::FilesWindow()
{
    ImGui::Begin("Files", &open, windowflags);

    showFiles(files);

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
    if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth))
    {
        char buffer[256];

        strncpy(buffer, selected.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0';

        ImGui::SetCursorPosX(4);
        if (ImGui::BeginTable("table8", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable, ImVec2(ImGui::GetContentRegionAvail().x + 4, 0)))
        {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Name");
            ImGui::TableSetColumnIndex(1);
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::InputText("##Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                auto object = scene->objects.extract(selected);
                object.key() = buffer;
                scene->objects.insert(std::move(object));

                selected = buffer;
            }

            ImGui::EndTable();
        }
    }

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

    Realgar::Components::AudioListenerComponent* listener = object->getComponent<Realgar::Components::AudioListenerComponent>();
    if (listener != nullptr)
    {
        if (ImGui::CollapsingHeader("AudioListenerComponent", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth))
        {
            float* pos[3] = { &listener->translation.x, &listener->translation.y, &listener->translation.z };
            float* rot[3] = { &listener->rotation.x, &listener->rotation.y, &listener->rotation.z };

            ImGui::SetCursorPosX(4);
            if (ImGui::BeginTable("table4", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable, ImVec2(ImGui::GetContentRegionAvail().x + 4, 0)))
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

                ImGui::EndTable();
            }
        }
    }

    Realgar::Components::AudioPlayerComponent* speaker = object->getComponent<Realgar::Components::AudioPlayerComponent>();
    if (speaker != nullptr)
    {
        if (ImGui::CollapsingHeader("AudioPlayerComponent", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth))
        {
            float* pos[3] = { &speaker->translation.x, &speaker->translation.y, &speaker->translation.z };

            static int item_selected_idx = 0;
            const char* combo_preview_value = "null";

            std::vector<const char*> audioItems;
            for (auto& audio : scene->current_audio)
            {
                audioItems.push_back(audio.first.c_str());
                if (&audio.second->sound == speaker->sound)
                {
                    item_selected_idx = audioItems.size() - 1;
                    combo_preview_value = audio.first.c_str();
                }
            }

            ImGui::SetCursorPosX(4);
            if (ImGui::BeginTable("table4", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable, ImVec2(ImGui::GetContentRegionAvail().x + 4, 0)))
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Position");
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::InputFloat3("##Position", *pos);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Audio");
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::BeginCombo("Select audio", combo_preview_value))
                {
                    for (int n = 0; n < audioItems.size(); n++)
                    {
                        const bool is_selected = (item_selected_idx == n);
                        if (ImGui::Selectable(audioItems[n], is_selected))
                        {
                            speaker->sound = &scene->current_audio[audioItems[n]]->sound;
                            item_selected_idx = n;
                        }

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                ImGui::EndTable();
            }
        }
    }

    Realgar::Components::ScriptComponent* script = object->getComponent<Realgar::Components::ScriptComponent>();
    if (script != nullptr)
    {
        char buffer[256];
        strncpy(buffer, script->luaFile.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0';

        if (ImGui::CollapsingHeader("ScriptComponent", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth))
        {
            ImGui::SetCursorPosX(4);
            if (ImGui::BeginTable("table4", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable, ImVec2(ImGui::GetContentRegionAvail().x + 4, 0)))
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Path");
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::InputText("##Path", buffer, sizeof(buffer)))
                    script->luaFile = buffer;


                ImGui::EndTable();
            }
        }
    }
}
void Gui::ShaderProperities() 
{
    if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth))
    {
        char buffer[256];
        char buffer1[256];
        char buffer2[256];

        strncpy(buffer, selected.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0';

        strncpy(buffer1, scene->current_shaders[selected]->shader.first.c_str(), sizeof(buffer1));
        buffer1[sizeof(buffer1) - 1] = '\0';

        strncpy(buffer2, scene->current_shaders[selected]->shader.second.c_str(), sizeof(buffer2));
        buffer2[sizeof(buffer2) - 1] = '\0';

        ImGui::SetCursorPosX(4);
        if (ImGui::BeginTable("table8", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable, ImVec2(ImGui::GetContentRegionAvail().x + 4, 0)))
        {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Name");
            ImGui::TableSetColumnIndex(1);
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::InputText("##Path", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                auto shader = scene->current_shaders.extract(selected);
                shader.key() = buffer;
                scene->current_shaders.insert(std::move(shader));

                selected = buffer;
            }

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Vertex shader");
            ImGui::TableSetColumnIndex(1);
            if (ImGui::InputText("##Vertex", buffer1, sizeof(buffer1), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                Realgar::Shader* shader = scene->current_shaders[selected];
                const char* vertexshader = buffer1;
                Realgar::Shader* newShader = new Realgar::Shader(vertexshader, shader->shader.second.c_str());

                scene->current_shaders[selected] = newShader;
                delete shader;
            }


            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Fragment shader");
            ImGui::TableSetColumnIndex(1);
            if (ImGui::InputText("##Fragment", buffer2, sizeof(buffer2), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                Realgar::Shader* shader = scene->current_shaders[selected];
                const char* fragmentshader = buffer2;
                Realgar::Shader* newShader = new Realgar::Shader(shader->shader.first.c_str(), fragmentshader);

                scene->current_shaders[selected] = newShader;
                delete shader;
            }

            ImGui::EndTable();
        }
    }
}
void Gui::TextureProperities()
{
    if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth))
    {
        char buffer[256];
        char buffer1[256];

        strncpy(buffer, selected.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0';

        strncpy(buffer1, scene->current_textures[selected]->path.c_str(), sizeof(buffer1));
        buffer1[sizeof(buffer1) - 1] = '\0';

        ImGui::SetCursorPosX(4);
        if (ImGui::BeginTable("table8", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable, ImVec2(ImGui::GetContentRegionAvail().x + 4, 0)))
        {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Name");
            ImGui::TableSetColumnIndex(1);
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::InputText("##Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                auto texture = scene->current_textures.extract(selected);
                texture.key() = buffer;
                scene->current_textures.insert(std::move(texture));

                selected = buffer;
            }

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Path");
            ImGui::TableSetColumnIndex(1);
            if (ImGui::InputText("##Path", buffer1, sizeof(buffer1), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                Realgar::Texture* texture = scene->current_textures[selected];
                const char* path = buffer1;
                Realgar::Texture* newTexture = new Realgar::Texture(path);

                scene->current_textures[selected] = newTexture;
                delete texture;
            }

            ImGui::EndTable();
        }
    }
}
void Gui::AudioProperities()  
{
    if (ImGui::CollapsingHeader("General", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth))
    {
        char buffer[256];
        char buffer1[256];

        strncpy(buffer, selected.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0';

        strncpy(buffer1, scene->current_audio[selected]->path.c_str(), sizeof(buffer1));
        buffer1[sizeof(buffer1) - 1] = '\0';

        ImGui::SetCursorPosX(4);
        if (ImGui::BeginTable("table11", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable, ImVec2(ImGui::GetContentRegionAvail().x + 4, 0)))
        {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Name");
            ImGui::TableSetColumnIndex(1);
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::InputText("##Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                auto audio = scene->current_audio.extract(selected);
                audio.key() = buffer;
                scene->current_audio.insert(std::move(audio));

                selected = buffer;
            }

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Path");
            ImGui::TableSetColumnIndex(1);
            if (ImGui::InputText("##Path", buffer1, sizeof(buffer1), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                Realgar::Audio* audio = scene->current_audio[selected];
                const char* path = buffer1;
                Realgar::Audio* newAudio = new Realgar::Audio(path, scene->current_audio[selected]->spatialized);

                scene->current_audio[selected] = newAudio;
                delete audio;
            }

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Spatialized");
            ImGui::TableSetColumnIndex(1);
            ImGui::Checkbox("##spatial", &scene->current_audio[selected]->spatialized);

            ImGui::TableNextRow();

            ImGui::EndTable();
        }
    }
}