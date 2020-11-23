//
// Created by vadim on 29.08.19.
//

#include "Subroutine.hpp"


// -------------------------------------------------------------------------------

int OpenFile(const char* filename, int* fd)
{
    assert(fd != nullptr);
    errno = 0;
    *fd = open(filename, O_RDWR | O_CREAT, 0644);
    if(*fd < 0)
    {
        std::cout << "Open failed!" << std::endl;
        perror("open");
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "open succeed, fd = " << *fd << std::endl;
        return *fd;
    }
}

// -------------------------------------------------------------------------------

int Send_request(int sockfd, const std::string& msg)
{
    ssize_t bytes_written(0);
    char buffer[MSG_LENGTH] = {};
    if(!strcpy(buffer, msg.c_str()))
    {
        perror("strcpy");
        return EXIT_FAILURE;
    }
    bytes_written = write(sockfd, buffer, MSG_LENGTH);
    if(bytes_written < 0)
    {
        std::cout << "write failed!\n";
        perror("write");
        return EXIT_FAILURE;
    }
    return 0;
}

// -------------------------------------------------------------------------------