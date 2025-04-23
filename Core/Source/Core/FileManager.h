#pragma once
#include <fstream>
#include <vector>

namespace Realgar
{
	class FileManager
	{
	public:
		static std::string setResourcePath(std::string path);
		static bool isResourcePathSet() { return !currentResourcePath.empty(); }

		static const char* getResource(std::string path);
		static const char* getLanguageServer();

		static bool fileExists(const std::string& filePath);
		static bool folderExists(const std::string& filePath);
		static std::vector<std::string> getFolderChildren(const std::string& filePath);
	private:
		static std::string currentResourcePath;
	};
} 
