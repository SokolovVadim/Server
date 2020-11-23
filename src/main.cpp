#include <iostream>
#include "Server.hpp"
#include "PythonEmbedding.hpp"
#include "ThreadPool.hpp"
#include "gtest.h"
#include "Gui.hpp"


int main(int argc, char** argv) {
    // Generate_SHA256("data.txt");
    int retVal(0);

    // part of Server routine
    if((argc == 2) && (!strcmp(argv[1], "server")))// server
    {
        std::cout << "argv = server\n";
        retVal = ServerRoutine();
    }
    if((argc == 2) && (!strcmp(argv[1], "client"))) // client
    {
        bool isExit(false);
        while(!isExit){
            Screen screen{};
            isExit = screen.authentificate();
        }
    }
    if((argc == 2) && (!strcmp(argv[1], "test")))// server
    {
        std::cout << "argv = test\n";
        // GOOGLE_TEST

        testing::InitGoogleTest();
        retVal = RUN_ALL_TESTS();
    }

    // return RUN_ALL_TESTS();
    return retVal;
}
