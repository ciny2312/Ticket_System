cmake_minimum_required(VERSION 3.16)
project(Ticket_System)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -fsanitize=address -fsanitize=undefined -fsanitize=leak")

include_directories(.)
include_directories(file)
include_directories(users)
include_directories(train)
include_directories(ticket)
#include_directories(test)


add_executable(code main.cpp main)

#include(CTest)
#enable_testing()
#include(test/ctest_config)