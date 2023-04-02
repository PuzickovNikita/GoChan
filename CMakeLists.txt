cmake_minimum_required(VERSION 3.25)
project(GoChan)

set(CMAKE_CXX_STANDARD 17)

include_directories(buffers)
ADD_LIBRARY(Buffer buffers/Buffer.cpp buffers/Buffer.h buffers/QueueBuffer.cpp buffers/QueueBuffer.h)

ADD_LIBRARY(Channel GoChan.cpp GoChan.h semaphore.cpp semaphore.h)
target_link_libraries(Channel Buffer)

find_package (Threads REQUIRED)
add_executable(GoChan main.cpp)
target_link_libraries(GoChan Channel Buffer -pthread)
