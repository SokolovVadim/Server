//
// Created by vadim on 15.08.19.
//

#ifndef SERVER_CLIENT_HPP
#define SERVER_CLIENT_HPP

#include "Inet.hpp"
#include "Subroutine.hpp"

void ClientRoutine(const std::string & script_name);
void ExecuteScript(const std::string & script_name);

class Client
{
public:
    Client();
    ~Client();
    void Init();
    void AssignPort();
    int  Connect();
    void UploadData(const char* filename, int sockfd);
    sockaddr_in* getServAddr() const;
private:
    int sockfd_;
    struct sockaddr_in * servaddr_;
};

#endif //SERVER_CLIENT_HPP
