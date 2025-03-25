#pragma once

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <TextEditor.h> // https://github.com/BalazsJako/ColorTextEditorDemo/blob/master/main.cpp

class FileEditor
{
public:
	FileEditor();
	~FileEditor();

	void Render();
private:
	const char* fileToEdit = "Resources/Scripts/test.lua";
	TextEditor editor;
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::CPlusPlus();
};
