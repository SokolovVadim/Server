//
// Created by vadim on 01.02.20.
//

#ifndef SERVER_THREADPOOL_HPP
#define SERVER_THREADPOOL_HPP

// include thread pool

#include "ctpl_stl.h"
#include <thread>

enum THREAD_POOL_CONST{
    HYPER_TR_MULT = 2
};

void init_thread_pool(ctpl::thread_pool* t_pool);
int ServerRoutine();

#endif //SERVER_THREADPOOL_HPP
