cmake_minimum_required(VERSION 3.10)
project(SnakeNN)

include_directories(./simplenn/include)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/simplenn/lib/libsimplenn.a ncurses)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(snakeAI main.cpp)
