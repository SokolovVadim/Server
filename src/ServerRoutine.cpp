//
// Created by vadim on 17.11.20.
//

#include <string>
#include "Subroutine.hpp"
namespace SR {
    void ServerRoutine(const std::string& request_msg)
    {
        if(request_msg == command_list[UPLOAD_SCRIPT])
        {

        }
        if(request_msg == command_list[DOWNLOAD_SCRIPT])
        {

        }
        if(request_msg == command_list[SHOW_SCRIPT_LIST])
        {

        }
        if(request_msg == command_list[EXECUTE_SCRIPT])
        {

        }
    }
}

/*int main(int argc, char* argv[])
{
    std::string request_msg{};
    SR::ServerRoutine(request_msg);
    return 0;
}*/

