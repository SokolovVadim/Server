//
// Created by vadim on 01.02.20.
//

// add testing framework

#include "TestAPI.hpp"
#include "gtest.h"

/*
namespace {
    struct ClientTest: public testing::Test
    {
        ClientTest():
            client(new Client)
        {
            client->Init();
        }
        ~ClientTest()
        {
            delete client;
        }
        bool TestAssign(const sockaddr_in* serverAddr)
        {
            sockaddr_in testServerAddr {
                    .sin_family = AF_INET,
                    .sin_port = htons(PORT)
            };
            if(serverAddr->sin_family == testServerAddr.sin_family)
                return true;
            else
                return false;
        }
        Client* client;
    };

    TEST_F(ClientTest, AssignPort)
    {
        // Act

        client->AssignPort();
        bool isEqual = TestAssign(client->getServAddr());

        // Assert

        EXPECT_EQ(isEqual, true);
    }

    TEST_F(ClientTest, Connect)
    {
        // Act

        client->AssignPort();
        int sockfd = client->Connect();

        // Assert

        EXPECT_LT(sockfd, 0);
    }



}
*/

/*namespace {

    struct ProgrammerTest: public testing::Test
    {
        ProgrammerTest():
                progr(new Programmer)
        {
            progr->Init();
        }
        ~ProgrammerTest()
        {
            delete progr;
        }
        bool TestAssign(const sockaddr_in* serverAddr)
        {
            sockaddr_in testServerAddr {
                    .sin_family = AF_INET,
                    .sin_port = htons(PORT)
            };
            if(serverAddr->sin_family == testServerAddr.sin_family)
                return true;
            else
                return false;
        }
        Programmer* progr;
    };

    TEST_F(ProgrammerTest, AssignPort)
    {
        // Act

        progr->AssignPort();
        bool isEqual = TestAssign(progr->getServAddr());

        // Assert

        EXPECT_EQ(isEqual, true);
    }

    TEST_F(ProgrammerTest, Connect)
    {
        // Act

        progr->AssignPort();
        int sockfd = progr->Connect();

        // Assert

        EXPECT_LT(sockfd, 0);
    }
}*/

/*namespace
{
    struct GuiTest: public testing::Test
    {
        GuiTest():
            screen(new Screen)
        {}
        ~GuiTest()
        {
            delete screen;
        }
        Screen* screen;
    };

    TEST_F(GuiTest, menu_upload)
    {
        // Act

        bool isRExit = screen->menu();

        // Assert
        // Upload
        EXPECT_EQ(isRExit, false);
    }

    TEST_F(GuiTest, menu_download)
    {
        // Act

        bool isRExit = screen->menu();

        // Assert
        // Download
        EXPECT_EQ(isRExit, false);
    }

    TEST_F(GuiTest, menu_show_script_list)
    {
        // Act

        bool isRExit = screen->menu();

        // Assert
        // show script list

        EXPECT_EQ(isRExit, false);
    }

    TEST_F(GuiTest, menu_execute_script)
    {
        // Act

        bool isRExit = screen->menu();

        // Assert
        // execute script

        EXPECT_EQ(isRExit, false);
    }

    TEST_F(GuiTest, menu_exit)
    {
        // Act

        bool isRExit = screen->menu();

        // Assert
        // exit

        EXPECT_EQ(isRExit, true);
    }

    TEST_F(GuiTest, getstring)
    {
        // Act

        std::string msg = getstring();

        // Assert

        EXPECT_STREQ(msg.c_str(), "script.py");
    }
}*/

namespace
{
    struct ServerTest: public testing::Test
    {
        ServerTest():
            server(new Server)
        {}
        ~ServerTest()
        {
            delete server;
        }
        Server* server;
    };

    TEST_F(ServerTest, dump)
    {
        // Act

        int dumpVal = server->Dump();

        // Assert

        EXPECT_EQ(dumpVal, 0);
    }

    TEST_F(ServerTest, listen_request)
    {
        // Act

        std::string msg = server->listen_request(0);

        // exit

        EXPECT_STREQ("null", msg.c_str());
    }

    TEST_F(ServerTest, init)
    {
        // Act

        int initVal = server->Init();

        // exit

        EXPECT_GE(initVal, 0);
    }

    TEST_F(ServerTest, bind)
    {
        // Act

        server->Init();
        server->AssignPort();
        int bindVal = server->BindSocket();

        // exit

        EXPECT_EQ(bindVal, 0);
    }

    TEST_F(ServerTest, listen)
    {
        // Act

        server->Init();
        server->AssignPort();
        server->BindSocket();
        int listenVal = server->Listen();

        // exit

        EXPECT_EQ(listenVal, 0);
    }

    TEST_F(ServerTest, accept)
    {
        // Act

        ClientRoutine("script.py");

        server->Init();
        server->AssignPort();
        server->BindSocket();
        server->Listen();

        SA client_addr = {};
        int fd = server->Accept(&client_addr);

        // exit

        EXPECT_GE(fd, 0);
    }
}




