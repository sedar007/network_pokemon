#cmake_minimum_required(VERSION 3.22)
#project(peer_to_peer_project)

#set(CMAKE_CXX_STANDARD 17)

#set(THREADS_PREFER_PTHREAD_FLAG ON)

#find_package(Threads REQUIRED)

#add_subdirectory(src)
#------------------
cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME "NetworkPokemon")
set(PROJECT_DESCRIPTION "A peer-to-peer network application for sharing Pokémon data.")
set(PROJECT_VERSION "1.0.0")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

project(${PROJECT_NAME}
        LANGUAGES CXX
        DESCRIPTION ${PROJECT_DESCRIPTION}
        VERSION ${PROJECT_VERSION}
)

#if(${PROJECT_NAME}_BUILD_TESTS)
enable_testing()
add_subdirectory(tests)
#endif()

add_subdirectory(src)
add_subdirectory(exemple)

#add_subdirectory(Ui)





