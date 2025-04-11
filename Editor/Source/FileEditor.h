#pragma once

#include "LanguageServer.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <TextEditor.h> // https://github.com/BalazsJako/ImGuiColorTextEdit

class FileEditor
{
public:
	FileEditor();
	~FileEditor();

	void ChangeFile(const char* file);

	void Render();
private:
	std::string fileToEdit = "Resources/Scripts/test.lua";
	static LanguageServer server;

	TextEditor editor;
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::Lua();

	bool tooltip = false;
};
