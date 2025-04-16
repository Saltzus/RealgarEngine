#include "FileManager.h"
#include <iostream>

bool fileExists(const std::string& filePath) {
	std::ifstream file(filePath);
	return file.good();
}



namespace Realgar
{
    std::string editorPath = "";
    std::string currentPath = "";
    std::string ownPath = "";
    std::string currentReturnPath = "";

    std::string FileManager::currentResourcePath = "";

	const char* FileManager::getResource(std::string path)
	{
        if (!currentResourcePath.empty())
        {
            currentReturnPath = currentResourcePath + path;
            return currentReturnPath.c_str();
        }

        editorPath = "Game/Resources/" + path;
        currentPath = "../Game/Resources/" + path;
        ownPath = "Resources/" + path;

        if (fileExists(editorPath)) {
            return editorPath.c_str();
        }
        else if (fileExists(currentPath)) {
            return currentPath.c_str();
        }
        else if (fileExists(ownPath)) {
            return ownPath.c_str();
        }
        else {
            std::cout << "FileManager : could not find file - " << path << " from " << editorPath << " or " << currentPath << "\n";
            return "";
        }
	}

    std::string FileManager::setResourcePath(std::string path)
    {
        if (fileExists(path)) {
            return editorPath.c_str();
        }
        else {
            return "Path is invalid!";
        }
    }

    const char* FileManager::getLanguageServer()
    {
        editorPath = "LanguageServer/bin/server";
        currentPath = "Editor/LanguageServer/bin/server";

        if (fileExists(editorPath)) {
            return editorPath.c_str();
        }
        else if (fileExists(currentPath)) {
            return currentPath.c_str();
        }
        else {
            return "";
        }
    }
}