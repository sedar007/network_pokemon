cmake_minimum_required(VERSION 3.22)
project(peer_to_peer_project)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_subdirectory(src)
