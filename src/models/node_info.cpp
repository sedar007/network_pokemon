#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>   // Nécessaire pour fcntl
#include <string>
#include <iostream>

#include "pch.h"

namespace pokemon {
    Node_Info::Node_Info(std::string_view id, std::string_view nodeName, std::string_view ip, int port ) noexcept
        : id_s(id)
        , nodeName_s(nodeName)
        , ip_s(ip)
        , port_i(port){
        isConnected_b = is_node_online(ip_s);
    }


bool Node_Info::is_node_online(std::string_view ip) const noexcept {
    std::string ip_str(ip);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_i);

    if (inet_pton(AF_INET, ip_str.c_str(), &server_addr.sin_addr) <= 0) {
        close(sock);
        return false;
    }

    int res = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (res < 0) {
        if (errno == EINPROGRESS) {

            fd_set myset;
            FD_ZERO(&myset);
            FD_SET(sock, &myset);

            struct timeval tv;
            tv.tv_sec = 2;  // Timeout de 2 secondes
            tv.tv_usec = 0;

            res = select(sock + 1, NULL, &myset, NULL, &tv);

            if (res > 0) {
                int so_error;
                socklen_t len = sizeof(so_error);
                getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);

                if (so_error == 0) {
                    // SUCCÈS : Connecté !
                    close(sock);
                    return true;
                }
            }
        }
    } else {
        close(sock);
        return true;
    }

    close(sock);
    return false;
}
}
