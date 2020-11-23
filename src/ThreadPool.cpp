//
// Created by vadim on 01.02.20.
//

#include "ThreadPool.hpp"
#include "Server.hpp"
#include "Client.hpp"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"*/

int ServerRoutine()
{
    // Thread pool

    /*ctpl::thread_pool* t_pool = nullptr;
    init_thread_pool(t_pool);*/
    unsigned int core_num = std::thread::hardware_concurrency();
    std::cout << "Core num = " << core_num << std::endl;
    ctpl::thread_pool t_pool(core_num * HYPER_TR_MULT);

    // Server starts work here
    Server server;
    server.Init();
    server.AssignPort();
    server.BindSocket();
    while(true) {
        server.Listen();
        SA client_addr = {};
        int fd = server.Accept(&client_addr);
        char* client_name = server.DetermineClientIP(&client_addr);

        // CreateDir(client_name);

        // test IsClientNew()

        // IsClientNew(client_name);

        std::string request_msg = server.listen_request(fd);
        std::cout << request_msg << std::endl;

        if(request_msg == command_list[UPLOAD_SCRIPT])
        {
            std::cout << "Uploading script from client to server\n";
            std::string script_name = server.listen_request(fd);
            std::cout << "Script name: " << script_name << std::endl;

            std::string name_with_timestamp = CreateName(script_name);
            std::cout << "name with timestamp:\n" << name_with_timestamp << std::endl;

            std::string path_name = CreatePathName(client_name, script_name);
            std::cout << "Path name: " << path_name << std::endl;

            if(!IsClientOld(client_name))
            {
                std::cout << "Client is new!\n";
                CreateDir(client_name);
            }
            else
                std::cout << "Client is old!\n";
            if(!IsScriptOld(client_name, script_name))
            {
                std::cout << "Script is new!\n";
                CreateDir(path_name);
            }
            else
                std::cout << "Script is old!\n";

            // create folder like: ./253.127.0.0/data.txt/1600717486data.txt/1600717486data.txt
            std::string working_dir = CreateWorkingDirName(path_name, name_with_timestamp);
            std::cout << "Working dir: " << working_dir << std::endl;
            CreateDir(working_dir);
            std::string executable = CreateExecutable(working_dir, name_with_timestamp);
            std::cout << "executable: " << executable << std::endl;

            // TP intro
            t_pool.push(DownloadScript, executable, fd);
        }
        if(request_msg == command_list[DOWNLOAD_SCRIPT])
        {
            std::cout << "Uploading script from server to client\n";
            std::string script_name = server.listen_request(fd);
            std::cout << "Script name: " << script_name << std::endl;

            if(!IsClientOld(client_name))
            {
                std::cout << "Client is new!\n";
                continue;
            }

            std::string script_path = FindScript(client_name, script_name);
            std::cout << "script path: " << script_path << std::endl;

            // TP intro
            t_pool.push(TransferDataToProgrammer, script_path, fd);
        }
        if(request_msg == command_list[SHOW_SCRIPT_LIST])
        {
            std::cout << "Showing script list\n";
            // TP intro
            t_pool.push(ShowScriptList, client_name, fd);
        }
        if(request_msg == command_list[EXECUTE_SCRIPT])
        {
            std::cout << "Executing script on server\n";
            std::string script_name = server.listen_request(fd);
            std::cout << "Script name: " << script_name << std::endl;

            if(!IsClientOld(client_name))
            {
                std::cout << "Client is new!\n";
                continue;
            }

            std::string script_path = FindScript(client_name, script_name);
            std::cout << "script path: " << script_path << std::endl;

            // TP intro
            t_pool.push(ExecuteScrypt, script_path);
        }

        // break statement is necessary
    }
    return 0;
}

void init_thread_pool(ctpl::thread_pool* t_pool)
{
    // THREAD POOL

    unsigned int core_num = std::thread::hardware_concurrency();
    std::cout << "Core num = " << core_num << std::endl;
    t_pool = new ctpl::thread_pool(core_num * HYPER_TR_MULT);
}

/*int main(int argc, char** argv) {
    // GOOGLE_TEST

    testing::InitGoogleTest();

    // THREAD POOL

    unsigned int core_num = std::thread::hardware_concurrency();
    std::cout << "Core num = " << core_num << std::endl;
    ctpl::thread_pool t_pool(core_num * HYPER_TR_MULT);

    // part of Server routine
    if((argc == 2) && (!strcmp(argv[1], "server")))// server
    {
        ServerRoutine("rep.txt", t_pool);
    }
    if((argc == 2) && (!strcmp(argv[1], "client"))) // client
    {
        ClientRoutine("data.txt");
    }

    return RUN_ALL_TESTS();

}*/
#pragma clang diagnostic pop



