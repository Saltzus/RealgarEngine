#include "FileEditor.h"

#include <filesystem>
#include <fstream>

LanguageServer FileEditor::server;

FileEditor::FileEditor()
{
    std::ifstream t(fileToEdit);
    if (t.good())
    {
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        editor.SetText(str);
    }

    editor.SetLanguageDefinition(lang);
    server.startServer("LanguageServer\\bin\\server");
}

void FileEditor::ChangeFile(const char* file)
{
    fileToEdit = file;
    std::ifstream t(file);
    if (t.good())
    {
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        editor.SetText(str);
    }
}

FileEditor::~FileEditor()
{
    server.stopServer();
}

std::string ReplaceTabsWithSpaces(const std::string& input) 
{
    std::string result = input;
    size_t pos = 0;

    while ((pos = result.find("\t", pos)) != std::string::npos) 
    {
        result.replace(pos, 1, "    ");
        pos += 4;
    }

    return result;
}

void Save(TextEditor& editor, std::string fileToEdit)
{
    auto textToSave = editor.GetText();

    // Remove trailing newline if it exists.
    if (!textToSave.empty() && textToSave.back() == '\n')
    {
        textToSave.pop_back();
    }

    std::ofstream outFile(fileToEdit);
    if (outFile.is_open())
    {
        outFile << textToSave;
        outFile.close();
        editor.SetText(textToSave);
        std::cout << "File saved successfully to " << fileToEdit << std::endl;
    }
    else
    {
        std::cerr << "Error: Could not open file for saving." << std::endl;
    }
}

void FileEditor::Render()
{
    editor.SetHandleKeyboardInputs(true);

    auto cpos = editor.GetCursorPosition();
    ImGui::Begin("Text Editor Demo", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

    ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save", "Ctrl-S"))
                Save(editor, fileToEdit);
            if (ImGui::MenuItem("Quit", "Alt-F4"))
                return;
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            bool ro = editor.IsReadOnly();
            if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                editor.SetReadOnly(ro);
            ImGui::Separator();

            if (ImGui::MenuItem("Undo", "Ctrl-Z", nullptr, !ro && editor.CanUndo()))
                editor.Undo();
            if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
                editor.Redo();

            ImGui::Separator();

            if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
                editor.Copy();
            if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
                editor.Cut();
            if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
                editor.Delete();
            if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
                editor.Paste();

            ImGui::Separator();

            if (ImGui::MenuItem("Select all", nullptr, nullptr))
                editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Dark palette"))
                editor.SetPalette(TextEditor::GetDarkPalette());
            if (ImGui::MenuItem("Light palette"))
                editor.SetPalette(TextEditor::GetLightPalette());
            if (ImGui::MenuItem("Retro blue palette"))
                editor.SetPalette(TextEditor::GetRetroBluePalette());
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
        editor.IsOverwrite() ? "Ovr" : "Ins",
        editor.CanUndo() ? "*" : " ",
        editor.GetLanguageDefinition().mName.c_str(), fileToEdit.c_str());

    
    if (ImGui::IsKeyPressed(ImGuiKey_Space) && ImGui::GetIO().KeyCtrl)
    {
        editor.SetHandleKeyboardInputs(false);
        server.complete("Resources/Scripts/test.lua", cpos.mLine, cpos.mColumn);
        tooltip = true; 
    }

    if (ImGui::IsKeyPressed(ImGuiKey_S) && ImGui::GetIO().KeyCtrl && ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
        Save(editor, fileToEdit);


    std::string test = ReplaceTabsWithSpaces(editor.GetCurrentLineText());
    if (test.size() > 0 && cpos.mColumn > 0 && test[cpos.mColumn - 1] == '.')
    {
       server.complete("Resources/Scripts/test.lua", cpos.mLine, cpos.mColumn);
       tooltip = true;
    }

    if (tooltip && ImGui::BeginTooltip())
    {
        if (ImGui::IsKeyPressed(ImGuiKey_Space) || ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            tooltip = false;

        ImGui::BeginChild("SuggestionsChild", ImVec2(300, 200), true);
        for (auto& suggestion : server.suggestions) 
        {
            if (ImGui::Selectable(suggestion.label.c_str()))
            {
 
            }
        }
        ImGui::EndChild();

        ImGui::EndTooltip();
    }

    editor.Render("TextEditor");
    ImGui::End();
}