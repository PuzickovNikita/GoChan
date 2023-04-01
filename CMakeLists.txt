cmake_minimum_required(VERSION 3.25)
project(GoChan)

set(CMAKE_CXX_STANDARD 17)

add_executable(GoChan main.cpp GoChan.cpp GoChan.h semaphore.cpp semaphore.h)
