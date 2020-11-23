//
// Created by vadim on 26.10.20.
//

#include "Proxy.hpp"

class Proxy
{
public:
    Proxy();
    ~Proxy();

private:
    int sockets[2];
};

Proxy::Proxy()
{

}

Proxy::~Proxy()
{

}
