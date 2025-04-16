#pragma once

#include "LanguageServer.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <TextEditor.h> // https://github.com/BalazsJako/ImGuiColorTextEdit

#include "Core/FileManager.h"

class FileEditor
{
public:
	FileEditor();
	~FileEditor();

	void ChangeFile(const char* file);
	TextEditor editor;

	void Render();
private:
	std::string fileToEdit = Realgar::FileManager::getResource("Scripts/test.lua");
	static LanguageServer server;

	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::Lua();

	bool tooltip = false;
};
