#pragma once
#include <json.hpp>
using json = nlohmann::json;

#include <subprocess.h>

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>
#include <sstream>

struct CompletionSuggestion
{
    std::string label = "";
    std::string insertText = "";
    // Optionally, you can include insertTextFormat, kind, etc.
};

class LanguageServer
{
public:
    void startServer(const std::string& serverPath);
    void stopServer();
    void hover(const char* file, int line, int character);
    void complete(const char* file, int line, int character);

    std::vector<CompletionSuggestion> suggestions;
private:
    json read_lsp_response(FILE* output);

    FILE* input;
    FILE* output;

    struct subprocess_s process;
    int outReturnCode;

    json result;
};

