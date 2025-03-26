#include "LanguageServer.h"

#define BUFFER_SIZE 4096

std::string get_file_uri(const std::string& path) 
{
    std::filesystem::path abs_path = std::filesystem::absolute(path);
    std::string uri = "file:///" + abs_path.string();
    std::replace(uri.begin(), uri.end(), '\\', '/');
    return uri;
}

void send_lsp_message(FILE* input, const char* json) 
{
    int length = strlen(json);
    fprintf(input, "Content-Length: %d\r\n\r\n%s", length, json);
    fflush(input);
}

void read_lsp_response(FILE* output)
{
    std::string text;
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), output)) {

        text += buffer;

        std::cout << buffer;
        if (text.compare(0, 16, "Content-Length: ") == 0) 
        {
            fgets(buffer, sizeof(buffer), output);
            text += buffer;

            std::string::size_type start = 16; // Start after the prefix
            std::string::size_type end = text.find("\r\n\r\n", start);

            if (end != std::string::npos) 
            {
                std::vector<int> numbers;

                std::string numberPart = text.substr(start, end - start);
                int number = std::stoi(numberPart);

                fgets(buffer, number + 1, output);
                std::cout << buffer << "\n\n";
            }

            break;
        }
    }
}

void LanguageServer::startServer(const std::string& serverPath)
{
    const char* server_cmd[] = { serverPath.c_str(), "--log-level debug", NULL};

    int result = subprocess_create(server_cmd, 0, &process);
    if (0 != result) 
    {
        fprintf(stderr, "Failed to start LSP server\n");
    }

    // Open pipes to communicate with the subprocess
    input = subprocess_stdin(&process);
    output = subprocess_stdout(&process);

    if (!input || !output) {
        fprintf(stderr, "Failed to open pipes\n");
        subprocess_destroy(&process);
    }

    read_lsp_response(output);

    // LSP "initialize" request
    const char* initialize_request =
    "{"
    "  \"jsonrpc\": \"2.0\","
    "  \"id\" : 1,"
    "  \"method\" : \"initialize\","
    "  \"params\" : {"
    "    \"processId\": null,"
    "    \"rootUri\": null,"
    "    \"capabilities\": {"
    "      \"textDocumentSync\": 1,"
    "      \"hoverProvider\" : true,"
    "      \"completionProvider\" : {"
    "        \"resolveProvider\": false,"
    "        \"triggerCharacters\" : [\".\", \":\", \" \"]"
    "      },"
    "      \"signatureHelpProvider\" : {"
    "        \"triggerCharacters\": [\"(\", \",\"]"
    "      }"
    "    }"
    "  }"
    "}";

    printf("Sending initialize request...\n");
    send_lsp_message(input, initialize_request);

    printf("Waiting for response...\n");
    read_lsp_response(output);

    std::string didChangeWorkspaceFolders_request =
    {
        "{"
        "  \"jsonrpc\": \"2.0\","
        "  \"method\" : \"workspace/didChangeWorkspaceFolders\","
        "  \"params\" : {"
        "    \"event\": {"
        "      \"added\": ["
        "        {"
        "          \"uri\": \"" + get_file_uri("Resources/Scripts") + "\""
        "        }"
        "      ] ,"
        "      \"removed\" : []"
        "    }"
        "  }"
        "}"
    };
    
    printf("Sending workspace request...\n");
    send_lsp_message(input, didChangeWorkspaceFolders_request.c_str());

    //std::string file = get_file_uri("Resources/Scripts/test.lua");
    //std::string didOpen_request =
    //    "{"
    //    "\"jsonrpc\": \"2.0\","
    //    "\"method\": \"textDocument/didOpen\","
    //    "\"params\": {"
    //        "\"textDocument\": {"
    //            "\"uri\": \"" + file + "\","
    //            "\"languageId\": \"lua\","
    //            "\"version\": 1,"
    //            "\"text\": \"print('Hello, World!')\""
    //        "}"
    //    "}"
    //    "}";

    //printf("Sending fileOpen request...\n");
    //send_lsp_message(input, didOpen_request.c_str());

}
void LanguageServer::stopServer()
{
    // Shutdown the language server gracefully
    const char* shutdown_request =
        "{"
        "\"jsonrpc\": \"2.0\","
        "\"id\": 2,"
        "\"method\": \"shutdown\","
        "\"params\": {}"
        "}";

    send_lsp_message(input, shutdown_request);
    read_lsp_response(output);

    // Exit LSP
    const char* exit_request =
        "{"
        "\"jsonrpc\": \"2.0\","
        "\"method\": \"exit\","
        "\"params\": {}"
        "}";

    send_lsp_message(input, exit_request);

    // Cleanup
    subprocess_join(&process, &outReturnCode);
    subprocess_destroy(&process);

    printf("LSP server exited.\n");
}

void LanguageServer::hover(const char* filePath, int line, int character)
{
    static int lastLine;
    static int lastCharacter;

    if (lastLine == line && lastCharacter == character) return;

    lastLine = line;
    lastCharacter = character;

    std::string file = get_file_uri(filePath);

    std::string hover_request =
        "{"
        "\"jsonrpc\": \"2.0\","
        "\"id\" : 1,"
        "\"method\": \"textDocument/hover\","
        "\"params\": {"
        "\"textDocument\": {"
        "\"uri\": \"" + file + "\""
        "},"
        "\"position\": {"
        "\"line\": " + std::to_string(line) + ", "
        "\"character\": " + std::to_string(character) + ""
        "}"
        "}"
        "}";

    printf("Sending hover request...\n");
    send_lsp_message(input, hover_request.c_str());

    printf("Waiting for response...\n");
    read_lsp_response(output);
}

void LanguageServer::complete(const char* filePath, int line, int character)
{
    static int lastLine2;
    static int lastCharacter2;

    if (lastLine2 == line && lastCharacter2 == character) return;

    lastLine2 = line;
    lastCharacter2 = character;

    std::string file = get_file_uri(filePath);

    std::string completion_request =
        "{"
        "\"jsonrpc\": \"2.0\","
        "\"id\" : 3,"
        "\"method\": \"textDocument/completion\","
        "\"params\": {"
        "\"textDocument\": {"
        "\"uri\": \"" + file + "\""
        "},"
        "\"position\": {"
        "\"line\": " + std::to_string(line) + ","
        "\"character\": " + std::to_string(character) +
        "}"
        "}"
        "}";

    printf("Sending completion request...\n");
    send_lsp_message(input, completion_request.c_str());

    printf("Waiting for response...\n");
    read_lsp_response(output);
}