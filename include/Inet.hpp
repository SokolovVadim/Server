//
// Created by vadim on 19.08.19.
//

#ifndef SERVER_INET_HPP
#define SERVER_INET_HPP

#include <netdb.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <string>

#include <netinet/in.h>
#include <arpa/inet.h>
#define MAX 80
#define PORT 8000
typedef struct sockaddr SA;
const std::string host_addr = "127.0.0.1";

#endif //SERVER_INET_HPP
