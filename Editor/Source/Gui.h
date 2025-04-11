#pragma once

#include "FileEditor.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "Core/Scene.h"

struct File
{
	std::string name;
	std::string path;
	std::string extention;

	std::vector<File> children;
	bool isDirectory;
};

class Gui
{
public:
	Gui(Realgar::Window* window, Realgar::Scene* scene);
	~Gui();

	void Render();
private:
	ImGuiIO* io;
	ImGuiViewport* viewport;

	Realgar::Scene* scene;

	void Menu();
	void MainWindow();
	void SceneWindow();

	FileEditor fileEditor;

	void showFiles(std::vector<File> files);

	void ProperitiesWindow();
	std::string selected = " ";
	int selectedType = 0;
	void CameraProperities();
	void ObjectProperities();
	void ShaderProperities();
	void TextureProperities();
	void AudioProperities();

	void FilesWindow();
};
