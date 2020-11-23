//
// Created by vadim on 06.11.19.
//

//#include "PythonEmbedding.hpp"
//#include "Server.hpp"
#include <cstdio>
#include <string>
#include "Python.h"

#include <cstdlib>

// -------------------------------------------------------------------------------

void ExecuteScrypt(int tid, const std::string& filename)
{

    FILE* file_pointer = fopen(filename.c_str(), "r");

    Py_Initialize();

    // PyRun_SimpleString("print('Hello from Python!')");
    PyRun_SimpleFile(file_pointer, filename.c_str());


    Py_Finalize();
}
