cmake_minimum_required(VERSION 3.10)
project(Server)

# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS  "-Wl,--no-as-needed -ldl -lutil -lncurses -lstdc++fs -lcrypt -lpam -lpam_misc")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall")

################################
# GTest
################################
ADD_SUBDIRECTORY (googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include/gtest/ ${gtest_SOURCE_DIR})

################################
# CTPL
################################

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CTPL)

add_executable(Server src/main.cpp src/Client.cpp src/Server.cpp src/Programmer.cpp src/Subroutine.cpp src/PythonEmbedding.cpp src/ThreadPool.cpp src/TestAPI.cpp src/Gui.cpp src/User.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

################################
# Python embedding
################################

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(Server ${PYTHON_LIBRARIES} gtest gtest_main)
target_link_libraries(Server stdc++fs)
#include_directories(/home/vadim/Documents/Python-3.6.1/Include/ googletest/googletest/include/gtest/ /home/vadim/Documents/Python-3.6.1/)
#target_link_libraries(Server ${PYTHON_LIBRARIES} gtest gtest_main)
#target_link_libraries(Server /home/vadim/Documents/Python-3.6.1/Modules/ /home/vadim/Documents/Python-3.6.1/Python/ gtest gtest_main)

