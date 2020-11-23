//
// Created by vadim on 20.08.19.
//

#ifndef SERVER_PROGRAMMER_HPP
#define SERVER_PROGRAMMER_HPP
#include "Inet.hpp"
#include <string>
#include <sys/user.h>
#include "Subroutine.hpp"

class Programmer
{
public:
    Programmer();
    ~Programmer();

    void Init();
    void AssignPort();
    int  Connect();
    void ReadFromServer(const char* filename, int sockfd);
    void UploadScrypt(const char* filename, int sockfd);
    std::string ReadScriptList(int sockfd);
    sockaddr_in* getServAddr() const;
    void Dump();
private:
    int sockfd_;
    struct sockaddr_in * servaddr_;
};

void ProgrammerRoutine(const std::string & script_name);
void ShowScriptList();

#endif //SERVER_PROGRAMMER_HPP
