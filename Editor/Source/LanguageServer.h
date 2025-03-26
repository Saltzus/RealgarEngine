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


class LanguageServer
{
public:
    void startServer(const std::string& serverPath);
    void stopServer();
    void hover(const char* file, int line, int character);
    void complete(const char* file, int line, int character);
private:
    FILE* input;
    FILE* output;

    struct subprocess_s process;
    int outReturnCode;
};

