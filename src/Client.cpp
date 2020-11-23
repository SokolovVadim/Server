//
// Created by vadim on 15.08.19.
//

#include "Client.hpp"
#include <iostream>

// -------------------------------------------------------------------------------

Client::Client():
    sockfd_(0),
    servaddr_(new sockaddr_in)
{
    std::cout << "Client created!" << std::endl;
}

// -------------------------------------------------------------------------------

Client::~Client()
{
    delete servaddr_;
    close(sockfd_); // check
}

// -------------------------------------------------------------------------------

void Client::Init()
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

    /* C-style
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    */
}

// -------------------------------------------------------------------------------

void Client::AssignPort()
{
    bzero(servaddr_, sizeof(*servaddr_));
    servaddr_->sin_family = AF_INET;
    servaddr_->sin_addr.s_addr = inet_addr(host_addr.c_str());
    servaddr_->sin_port = htons(PORT);
    std::cout << "Port assigned: sin_port = " << servaddr_->sin_port << std::endl;
}

// -------------------------------------------------------------------------------

sockaddr_in* Client::getServAddr() const
{
    return this->servaddr_;
}

// -------------------------------------------------------------------------------

int Client::Connect()
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

void Client::UploadData(const char *filename, int sockfd)
{
    int fd(0);
    OpenFile(filename, &fd);
    ssize_t bytes_read(0);
    ssize_t bytes_written(0);
    ssize_t bytes_read_counter(0);
    ssize_t bytes_written_counter(0);
    char buf[BUF_LENGTH] = {};
    do {

        bytes_read = read(fd, buf, BUF_LENGTH);
        bytes_read_counter += bytes_read;
        // std::cout << "Client: read from data.txt: " << bytes_read << std::endl;
        // std::cout << buf; // !!!!!!!!!!!!
        if(bytes_read > 0) {

            bytes_written = write(sockfd, buf, BUF_LENGTH);
            bytes_written_counter += bytes_written;
            if(bytes_written < 0)
            {
                perror("write");
                break;
            }
        }
        else if(bytes_read == 0)
        {
            perror("read");
            break; // no errors, just nothing to read from that side
        } else if(bytes_read < 0)
        {
            perror("read");
            break;
        }
        bzero(buf, BUF_LENGTH);
    } while(bytes_read > 0);
    std::cout << "bytes_read_counter = " << bytes_read_counter << std::endl;
    std::cout << "bytes_written_counter = " << bytes_written_counter << std::endl;
    close(fd);
}

// -------------------------------------------------------------------------------

/*int Send_request(int sockfd, int request_num)
{
    ssize_t bytes_written(0);
    char buffer[BUF_LENGTH] = {};
    if(!strcpy(buffer, command_list[request_num].c_str()))
    {
        perror("strcpy");
        return EXIT_FAILURE;
    }
    bytes_written = write(sockfd, buffer, BUF_LENGTH);
    if(bytes_written < 0)
    {
        perror("write");
        return EXIT_FAILURE;
    }
    return 0;
}*/

// -------------------------------------------------------------------------------

void ClientRoutine(const std::string & script_name)
{
    Client client;
    client.Init();
    client.AssignPort();
    int sockfd = client.Connect();
    if(sockfd < 0)
        return;

    // Send request for uploading data
    Send_request(sockfd, command_list[UPLOAD_SCRIPT]);
    // Send script name to be uploaded to server
    Send_request(sockfd, script_name);
    client.UploadData(script_name.c_str(), sockfd);
}

// -------------------------------------------------------------------------------

void ExecuteScript(const std::string & script_name)
{
    Client client;
    client.Init();
    client.AssignPort();
    int sockfd = client.Connect();

    // Send request for executing script
    Send_request(sockfd, command_list[EXECUTE_SCRIPT]);
    // Send script name to be executed on server
    Send_request(sockfd, script_name);
}

// -------------------------------------------------------------------------------
