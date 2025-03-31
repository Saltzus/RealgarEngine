#pragma once

#include "LanguageServer.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <TextEditor.h> // https://github.com/BalazsJako/ColorTextEditorDemo/blob/master

class FileEditor
{
public:
	FileEditor();
	~FileEditor();

	void Render();
private:
	static LanguageServer server;

	const char* fileToEdit = "Resources/Scripts/test.lua";
	TextEditor editor;
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::Lua();
};
