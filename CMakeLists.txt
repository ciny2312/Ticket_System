cmake_minimum_required(VERSION 3.16)
project(Ticket_System)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(file)
include_directories(users)
include_directories(train)
include_directories(ticket)

add_executable(code main.cpp)