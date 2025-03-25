#pragma once

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <TextEditor.h> // https://github.com/BalazsJako/ColorTextEditorDemo/blob/master/main.cpp

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

	void EditorWindow();
	const char* fileToEdit = "Resources/Scripts/test.lua";
	TextEditor editor;
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::CPlusPlus();

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
