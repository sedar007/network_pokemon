#pragma once


#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")

    #define CLOSE_SOCKET closesocket
    #define SOCKET_TYPE SOCKET
    #define VALID_SOCKET(s) ((s) != INVALID_SOCKET)
    #define GET_SOCKET_ERROR() WSAGetLastError()
    #define IS_IN_PROGRESS(e) ((e) == WSAEWOULDBLOCK || (e) == WSAEINPROGRESS)
    typedef int socklen_t;
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <fcntl.h>
    #include <cerrno>

    #define CLOSE_SOCKET close
    #define SOCKET_TYPE int
    #define VALID_SOCKET(s) ((s) >= 0)
    #define GET_SOCKET_ERROR() errno
    #define IS_IN_PROGRESS(e) ((e) == EINPROGRESS)
#endif


#include <models.hpp>

