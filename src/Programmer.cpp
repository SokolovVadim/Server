//
// Created by vadim on 20.08.19.
//

#include "Programmer.hpp"
#include "Subroutine.hpp"
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>


// ------------------------------------------------------------------------------
/*
 * Programmer can upload script to server
 * See previous scripts
 * download script
 */
// --------------------------------------------------------------------------------
/*
 * 1 Read file from Server
 * 2 Upload file onto Server
 * 3 Show the list of available files
 * 4 Choose one and read
 */
// -------------------------------------------------------------------------------

Programmer::Programmer():
    sockfd_(0),
    servaddr_(new sockaddr_in)
{
    std::cout << "Programmer created!" << std::endl;
}

// -------------------------------------------------------------------------------

Programmer::~Programmer()
{
    delete this->servaddr_;
    close(sockfd_);
    std::cout << "Programmer deleted" << std::endl;
}

// -------------------------------------------------------------------------------

void Programmer::UploadScrypt(const char* filename, int sockfd)
{
    std::cout << "Programmer filename = " << filename << std::endl;
    int fd(0);
    OpenFile(filename, &fd);
    ssize_t bytes_read(0);
    ssize_t bytes_written(0);
    char buf[BUF_LENGTH] = {};
    do {

        bytes_read = read(fd, buf, BUF_LENGTH);
        if(bytes_read > 0) {

            bytes_written = write(sockfd, buf, BUF_LENGTH);
            if(bytes_written < 0)
            {
                perror("write");
                break;
            }
        }
        else if(bytes_read == 0)
        {
            break; // no errors, just nothing to read from that side
        } else if(bytes_read < 0)
        {
            perror("read");
            break;
        }
        bzero(buf, BUF_LENGTH);
    } while(bytes_read > 0);
    close(fd);
}


// -------------------------------------------------------------------------------

void Programmer::ReadFromServer(const char* filename, int sockfd)
{
    int fd(0);
    OpenFile(filename, &fd);
    ssize_t f_block_size(0);
    ssize_t written_size(0);
    char buf[BUF_LENGTH] = {};
    
    do
    {
        errno = 0;
        f_block_size = read(sockfd, buf, BUF_LENGTH);
        
        if((f_block_size < 0) && (errno == EAGAIN)) {
            break;
        } else if(f_block_size < 0) {
            perror("read");
            std::cout << "Receive failed" << std::endl;
            break;
        }

        written_size = write(fd, buf, BUF_LENGTH);
        if(written_size < f_block_size)
        {
            std::cout << "Write failed!" << std::endl;
            break;
        } else if(written_size == 0)
            break;
        bzero(buf, BUF_LENGTH);

    } while(f_block_size > 0);
    close(fd);
    if(fd != 0)
    {
        perror("close");
    }
}

// -------------------------------------------------------------------------------

std::string Programmer::ReadScriptList(int sockfd)
{
    auto buf = new char[MSG_LENGTH];


    ssize_t  f_block_size = read(sockfd, buf, MSG_LENGTH);
    if(f_block_size < 0) {
        perror("read");
        return nullptr;
    }
    std::string str(buf);
    delete[] buf;
    return str;
}

// -------------------------------------------------------------------------------

void ShowScriptList()
{
    Programmer prog;
    prog.Init();
    prog.AssignPort();
    int fd = prog.Connect();
    if(fd < 0)
        return;

    // Send request for showing script list
    Send_request(fd, command_list[SHOW_SCRIPT_LIST]);
    // read script list from server
    std::string script_list = prog.ReadScriptList(fd);
    std::cout << "Script list:\n" <<  script_list;
}

// -------------------------------------------------------------------------------

void Programmer::Dump()
{
    std::cout << "-----------Programmer DUMP-----------" << std::endl;
    std::cout << "sockfd = " << this->sockfd_ << std::endl;
    std::cout << "-------------------------------------" << std::endl;
}

// -------------------------------------------------------------------------------

void Programmer::Init()
{
    try
    {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        std::cout << "Socket successfully created" << std::endl;
    }
    catch(std::exception& e)
    {
        std::cout << "Socket creation failed" << e.what() << std::endl;
    }
}

// -------------------------------------------------------------------------------

void Programmer::AssignPort()
{
    bzero(servaddr_, sizeof(*servaddr_));
    servaddr_->sin_family = AF_INET;
    servaddr_->sin_addr.s_addr = inet_addr(host_addr.c_str());
    servaddr_->sin_port = htons(PORT);
    std::cout << "Port assigned" << std::endl;
}

// -------------------------------------------------------------------------------

sockaddr_in* Programmer::getServAddr() const
{
    return this->servaddr_;
}

// -------------------------------------------------------------------------------

int Programmer::Connect()
{
    try
    {
        int retval = connect(sockfd_, (sockaddr*)servaddr_, sizeof(*servaddr_));
        if(retval < 0)
        {
            std::cerr << "Connect failed!\n";
            perror("connect");
            return -1;
        }
        return sockfd_;
    }
    catch(std::exception& e)
    {
        std::cout << "Connect failed " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

// -------------------------------------------------------------------------------

void ProgrammerRoutine(const std::string & script_name)
{
    Programmer prog;
    prog.Init();
    prog.AssignPort();
    int fd = prog.Connect();
    if(fd < 0)
        return;
    // Send request for downloading data
    Send_request(fd, command_list[DOWNLOAD_SCRIPT]);

    // Send script name to be downloaded from server
    Send_request(fd, script_name);
    prog.ReadFromServer(script_name.c_str(), fd);

    // prog.Dump();
    // prog.ShowFilesInDir();
}

// -------------------------------------------------------------------------------
