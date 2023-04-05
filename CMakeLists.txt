cmake_minimum_required(VERSION 3.23)
project(ShellB C)

set(CMAKE_C_STANDARD 99)
include_directories(inc)

add_executable(ShellB src/shellb.c src/shellb_platform.c src/main.c)

if(WIN32)
    target_link_libraries(ShellB pthread)
endif(WIN32)

if (UNIX)
    target_link_libraries(ShellB pthread)
endif(UNIX)