//
// Created by vadim on 15.08.19.
//

#include "Server.hpp"
#include "Programmer.hpp"
#include <iostream>
#include <experimental/filesystem>
#include <ctime>
#include <algorithm>

// ---------------------------------------------------
// C-style Server routine
// ---------------------------------------------------

// -------------------------------------------------------------------------------

void DownloadScript(int tid, const std::string& filename, int sockfd)
{
    // std::system("ls -l .");
    std::cout << "filename server = " << filename << std::endl;
    std::cout << "Thread pool thread_id = " << tid << std::endl;
    int fd(0);
    OpenFile(filename.c_str(), &fd);
    ssize_t f_block_size(0);
    ssize_t written_size(0);
    ssize_t bytes_read_counter(0);
    ssize_t bytes_written_counter(0);
    char buf[BUF_LENGTH] = {};
    do
    {
        errno = 0;
        // read from socket
        f_block_size = read(sockfd, buf, BUF_LENGTH);
        bytes_read_counter += f_block_size;
        if((f_block_size < 0) && (errno == EAGAIN)) {
            perror("read");
            break;
        } else if(f_block_size < 0) {
            std::cout << "Receive failed" << std::endl;
            break;
        }
        // std::cout << buf << std::endl;

        // write data from buffer to file.out on server
        written_size = write(fd, buf, BUF_LENGTH);
        bytes_written_counter += written_size;
        if(written_size < f_block_size)
        {
            std::cout << "Write failed!" << std::endl;
            break;
        } else if(written_size == 0) {
            perror("read");
            break;
        }
        bzero(buf, BUF_LENGTH);

    } while(f_block_size > 0);
    close(fd); 
    if(fd != 0)
    {
        perror("close");
        return;
    }
}


// -------------------------------------------------------------------------------

void TransferDataToProgrammer(int tid, const std::string& filename, int sockfd)
{
    int fd(0);
    OpenFile(filename.c_str(), &fd);
    // std::cout << "fd = " << fd << std::endl;
    ssize_t bytes_read(0);
    ssize_t bytes_written(0);
    char buffer[BUF_LENGTH] = {};
    do {
        // std::cout << "Server before read from file" << std::endl;
        bytes_read = read(fd, buffer, BUF_LENGTH);
        if(bytes_read > 0) {
            // std::cout << "Server read " << buffer << std::endl;
            bytes_written = write(sockfd, buffer, BUF_LENGTH);
            if(bytes_written < 0)
            {
                perror("write");
                break;
            }
        }
        else if(bytes_read == 0)
        {
            // std::cout << "Nothing to read from file (server side)" << std::endl;
            perror("read");
            break;
        } else if(bytes_read < 0)
        {
            perror("read");
            break;
        }
        bzero(buffer, BUF_LENGTH);
    } while(bytes_read > 0);
    close(fd);
    close(sockfd);
}



// ---------------------------------------------------
// CLASS SERVER MEMBER FUNCTIONS
// ---------------------------------------------------

Server::Server():
    sockfd_(0),
    servaddr_(new sockaddr_in)
{
    std::cout << "Server created!" << std::endl;
}

Server::~Server()
{
    delete servaddr_;
    close(sockfd_); // check
}


// -------------------------------------------------------------------------------

int Server::Init()
{
    try
    {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        std::cout << "Socket successfully created" << std::endl;
        return sockfd_;
    }
    catch(std::exception& e)
    {
        std::cout << "Socket creation failed" << e.what() << std::endl;
        return -1;
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

void Server::AssignPort()
{
    bzero(servaddr_, sizeof(*servaddr_));
    servaddr_->sin_family = AF_INET;
    servaddr_->sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr_->sin_port = htons(PORT);
    std::cout << "Port assigned" << std::endl;
}

// -------------------------------------------------------------------------------

int Server::BindSocket()
{
    try
    {
        int err = bind(sockfd_, (SA*)servaddr_, sizeof(*servaddr_));
        if(err == -1)
        {
            std::cerr << "Bind failed!\n";
            perror("bind");
            return err;
        }
        std::cout << "socket bound" << std::endl;
        return err;
    }
    catch (std::exception& ex)
    {
        std::cout << "socket bind failed" << std::endl;
        return -1;
    }



    /*
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
     */
}

// -------------------------------------------------------------------------------

int Server::Listen()
{
    try
    {
        this->Dump();
        int err = listen(sockfd_, 5);
        std::cout << "Server listens" << std::endl;
        if(err < 0)
        {
            std::cerr << "Listen failed!\n";
            perror("listen");
        }
        return err;
    }
    catch(std::exception& ex)
    {
        std::cout << "Listen failed" << std::endl;
        return -1;
    }

}

// -------------------------------------------------------------------------------

int Server::Accept(SA* client_addr)
{
    try
    {
        // SA* clientaddr = nullptr;
        socklen_t len = sizeof(client_addr);
        int connfd = accept(sockfd_, client_addr, &len);

        std::cout << "Client accepted" << std::endl;
        return connfd;
    }
    catch(std::exception& ex)
    {
        std::cout << "Client accepting failed" << std::endl;
        return EXIT_FAILURE;
    }
}

// -------------------------------------------------------------------------------

int Server::Dump()
{
    std::cout << "sockfd =\t"  << sockfd_ << std::endl;
    std::cout << "sin_family =\t" << servaddr_->sin_family << std::endl;
    std::cout << "sin_addr =\t"   << servaddr_->sin_addr.s_addr   << std::endl;
    std::cout << "sin_port =\t"   << servaddr_->sin_port   << std::endl;
    return 0;
}

// -------------------------------------------------------------------------------

void ShowScriptList(int tid, const std::string& client_name, int sockfd)
{
    std::string msg = ReadDirEntry(client_name);
    Send_request(sockfd, msg);
}

// -------------------------------------------------------------------------------

std::string Server::listen_request(int sockfd)
{
    auto buf = new char[MSG_LENGTH];


    ssize_t  f_block_size = read(sockfd, buf, MSG_LENGTH);
    if(f_block_size < 0) {
        perror("read");
        return "null";
    }
    std::string str(buf);
    delete[] buf;
    return str;
}

// -------------------------------------------------------------------------------

/*
 * This code is platform dependant
 */

std::string ReadDir(const std::string& client_name)
{
    DIR* d = {};
    struct dirent * dir = nullptr;
    std::string msg{};
    std::string dir_name = "./" + client_name;
    d = opendir(dir_name.c_str());
    if(d){
        std::cout << "List of scrypts:" << std::endl;
        while((dir = readdir(d)) != nullptr)
        {
            if(dir->d_type == DT_REG)
            {
                msg += dir->d_name;
                msg += "\n";
                std::cout << "\t" << dir->d_name << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Can't open directory!" << std::endl;
    }
    return msg;
}

// -------------------------------------------------------------------------------

char* Server::DetermineClientIP(SA* client_addr)
{
    auto pV4Addr = (struct sockaddr_in*)&client_addr;
    struct in_addr ipAddr = pV4Addr->sin_addr;
    auto str = new char[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
    std::cout << str << std::endl;
    return str;
}

// -------------------------------------------------------------------------------
namespace fs = std::experimental::filesystem;

void CreateDir(const std::string& client_name)
{
    std::string path(client_name);
    // path = "./" + path;

    fs::path dir(path.c_str());
    fs::create_directories(dir);
    // fs::remove_all(dir);
}

// -------------------------------------------------------------------------------

fs::path AppendTimeToFileName(const fs::path& fileName)
{
    std::time_t result = std::time(nullptr);
    std::string cur_time = std::asctime(std::localtime(&result));
    return fileName.stem().string() + "_" + cur_time + fileName.extension().string();
}

// -------------------------------------------------------------------------------

std::string CreateName(const std::string& script_name)
{
    std::time_t result = std::time(nullptr);
    std::asctime(std::localtime(&result));
    std::string name = std::to_string(result) + script_name;

    return name;
}

// clientname0/scriptname0/scriptname0+timestamp0.py
// clientname0/scriptname0/scriptname0+timestamp1.py
// clientname0/scriptname1/scriptname1+timestamp0.py
// clientname0/scriptname1/scriptname1+timestamp1.py
// clientname1/scriptname0/scriptname0+timestamp0.py
// clientname1/scriptname1/scriptname0+timestamp0.py

// -------------------------------------------------------------------------------

bool demo_exists(const fs::path& p, fs::file_status s = fs::file_status{})
{
    std::cout << p;
    if(fs::status_known(s) ? fs::exists(s) : fs::exists(p))
    {
        std::cout << " exists\n";
        return true;
    }
    else
    {
        std::cout << " does not exist\n";
        return false;
    }
}

// -------------------------------------------------------------------------------

bool IsClientOld(const std::string& client_name)
{
    std::string path_to_client_name("./");
    fs::path cur_dir(path_to_client_name.c_str());
    path_to_client_name += client_name;
    std::cout << "path_to_client_name: " << path_to_client_name << std::endl;
    fs::path path(path_to_client_name.c_str());
    for(auto it = fs::directory_iterator(cur_dir); it != fs::directory_iterator(); ++it)
    {
        if(*it == path)
            if(demo_exists(*it, it->status())) // use cached status from directory entry
                return true;
    }
    return false;
}

// -------------------------------------------------------------------------------

bool IsScriptOld(const std::string& client_name, const std::string& script_name)
{
    std::string path_to_client_name_str = std::string("./") + client_name;
    fs::path path_to_client_name(path_to_client_name_str.c_str());

    std::string path_to_script_name_str = path_to_client_name_str + "/" + script_name;
    std::cout << "path_to_script_name_str: " << path_to_script_name_str << std::endl;

    fs::path path_to_script_name(path_to_script_name_str.c_str());
    for(auto it = fs::directory_iterator(path_to_client_name); it != fs::directory_iterator(); ++it)
    {
        if(*it == path_to_script_name)
            if(demo_exists(*it, it->status()))
                return true;
    }
    return false;
}

// -------------------------------------------------------------------------------

std::string CreatePathName(const std::string& client_name, const std::string& script_name)
{
    std::string path = "./" + client_name + "/" + script_name;
    return path;
}

// -------------------------------------------------------------------------------

std::string CreateWorkingDirName(const std::string& path_name, const std::string& name_with_timestamp)
{
    std::string path = path_name + "/" + name_with_timestamp;
    return path;
}

// -------------------------------------------------------------------------------

std::string CreateExecutable(const std::string& working_dir, const std::string& name_with_timestamp)
{
    std::string path = working_dir + "/" + name_with_timestamp;
    return path;
}

// -------------------------------------------------------------------------------

// find the latest script by script_name for user client_name

std::string FindScript(const std::string& client_name, const std::string& script_name)
{
    std::cout << "Finding script name: ";
    std::string cur_dir_name = "./";
    fs::path cur_dir_path(cur_dir_name.c_str());

    std::string client_dir_name = cur_dir_name + client_name;
    fs::path client_dir_path(client_dir_name.c_str());

    std::string script_dir_name = client_dir_name + "/" + script_name;
    fs::path script_dir_path(script_dir_name.c_str());

    for(auto it = fs::directory_iterator(client_dir_path); it != fs::directory_iterator(); ++it)
    {
        if(*it == script_dir_path)
        {
            std::cout << "client found!\n";
            std::vector<fs::path> dir_v;
            auto it1 = fs::directory_iterator(script_dir_path);
            for(; it1 != fs::directory_iterator(); ++it1)
            {
                dir_v.push_back(it1->path());
                // std::cout << it1->path() << std::endl;
            }
            for(const fs::path& i:dir_v)
            {
                std::cout << i << "\n";
            }
            std::sort(dir_v.begin(), dir_v.end());
            std::cout << "After sort:\n";
            for(const fs::path& i:dir_v)
            {
                std::cout << i << "\n";
            }
            std::vector<fs::path> entry_v;
            for (const auto & entry : fs::directory_iterator(std::string(dir_v.back().c_str())))
            {
                entry_v.push_back(entry.path());
            }
            return entry_v.back().c_str();
        }

    }
    return "";
}

// -------------------------------------------------------------------------------

std::string ReadDirEntry(const std::string& dir_name)
{
    std::string msg{};
    fs::path dir_path(dir_name.c_str()); // 255.127.0.0
    for (const auto & entry : fs::directory_iterator(dir_path))
    {
        // std::cout << "Entry: " << entry << "filename: " << entry.path().filename() << std::endl;
        msg += std::string(entry.path()) + "\n"; // 255.127.0.0/script.py

        for (const auto & inner_entry : fs::directory_iterator(entry.path()))
        {
            // std::cout << "inner entry: " << inner_entry.path() << std::endl; // 254.127.0.0/script.py/1600769484script.py
            for (const auto & file_entry : fs::directory_iterator(inner_entry.path()))
            {
                // std::cout << file_entry.path() << std::endl; // 252.127.0.0/script.py/1600769767script.py/1600769767script.py
                msg += "- " + std::string(file_entry.path().filename()) + "\n";
            }
        }
    }
    return msg;
}

// -------------------------------------------------------------------------------

