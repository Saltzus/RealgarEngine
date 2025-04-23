#include "FileManager.h"

#include <iostream>
#include <filesystem>


namespace Realgar
{
    bool FileManager::fileExists(const std::string& filePath) 
    {
        std::ifstream file(filePath);
        return file.good();
    }

    bool FileManager::folderExists(const std::string& filePath)
    {
        return std::filesystem::is_directory(filePath);
    }

    std::vector<std::string> FileManager::getFolderChildren(const std::string& filePath)
    {
        std::vector<std::string> children;
        std::filesystem::path dirPath(filePath);

        try 
        {
            if (std::filesystem::exists(dirPath) && std::filesystem::is_directory(dirPath)) 
            {
                for (const auto& entry : std::filesystem::directory_iterator(dirPath)) 
                {
                    children.push_back(entry.path().filename().string());
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e) 
        {
            std::cerr << "Filesystem error: " << e.what() << "\n";
        }

        return children;
    }

    std::string editorPath = "";
    std::string currentPath = "";
    std::string ownPath = "";
    std::string currentReturnPath = "";

    std::string FileManager::currentResourcePath = "";

	const char* FileManager::getResource(std::string path)
	{
        if (!currentResourcePath.empty())
        {
            currentReturnPath = currentResourcePath + "/" + path;
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
        if (folderExists(path)) {
            currentResourcePath = path;
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