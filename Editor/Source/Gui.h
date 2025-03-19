#pragma once

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "Core/Scene.h"

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

	void MainWindow();

	void SceneWindow();

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
