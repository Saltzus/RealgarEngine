#pragma once
#include <fstream>

namespace Realgar
{
	class FileManager
	{
	public:
		static std::string setResourcePath(std::string path);

		static const char* getResource(std::string path);
		static const char* getLanguageServer();
	private:
		static std::string currentResourcePath;
	};
} 
