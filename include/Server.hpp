//
// Created by vadim on 15.08.19.
//

#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include "Inet.hpp"
#include "Subroutine.hpp"
#include "PythonEmbedding.hpp"
#include "dirent.h"

// void serverRoutine();

std::string ReadDir              (const std::string& client_name);
std::string CreatePathName       (const std::string& client_name, const std::string& script_name);
std::string CreateWorkingDirName (const std::string& path_name,   const std::string& name_with_timestamp);
std::string CreateExecutable     (const std::string& working_dir, const std::string& name_with_timestamp);
std::string CreateName           (const std::string& script_name);
std::string ReadDirEntry         (const std::string& dir_name);
std::string FindScript           (const std::string& client_name, const std::string& script_name);

void CreateDir                   (const std::string&  client_name);
void server_func                 (int sockfd);
void DownloadScript              (int tid, const std::string& filename,    int sockfd);
void TransferDataToProgrammer    (int tid, const std::string& filename,    int sockfd);
void ShowScriptList              (int tid, const std::string& client_name, int sockfd);

bool IsClientOld(const std::string& client_name);
bool IsScriptOld(const std::string& client_name, const std::string& script_name);

class Server
{
public:
    Server();
    ~Server();
    int Init();
    int BindSocket();
    int Listen();
    int Accept(SA* client_addr);
    int Dump();
    void AssignPort();
    std::string listen_request(int sockfd);
    char* DetermineClientIP(SA* client_addr);
private:
    int sockfd_;
    struct sockaddr_in * servaddr_;
};

#endif //SERVER_SERVER_HPP
