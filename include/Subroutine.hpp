//
// Created by vadim on 29.08.19.
//

#ifndef SERVER_SUBROUTINE_HPP
#define SERVER_SUBROUTINE_HPP

#include <cerrno>
#include <fcntl.h>
#include <cassert>
#include <iostream>
#include <cstring>
#include <unistd.h>

enum CHOICES
{
    CHOICES_NUM      = 5,
    UPLOAD_SCRIPT    = 0,
    DOWNLOAD_SCRIPT  = 1,
    SHOW_SCRIPT_LIST = 2,
    EXECUTE_SCRIPT   = 3,
    EXIT             = 4
};

enum AUTH
{
    OPTIONS_NUM = 3,
    SIGN_IN     = 0,
    SIGN_UP     = 1,
    EXIT_AUTH   = 2
};

static std::string options_list[OPTIONS_NUM] = {"Sign in", "Sign up", "Exit"};
static std::string command_list[CHOICES_NUM] = {"Upload Script", "Download Script", "Show script list", "Execute script", "Exit"};

const unsigned long BUF_LENGTH = 64;
const unsigned long MSG_LENGTH = 1024; // PAGE_SIZE
int OpenFile(const char* filename, int* fd);
int Send_request(int sockfd, const std::string& msg);

#endif //SERVER_SUBROUTINE_HPP
