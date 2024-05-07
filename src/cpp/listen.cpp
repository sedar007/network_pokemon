#include "../hpp/listen.hpp"
#include "../sockpp/tcp_acceptor.h"
#include "../sockpp/tcp_connector.h"
#include "../sockpp/tcp_socket.h"
#include "hpp/resourceManager.hpp"
#include <iostream>

namespace pokemon {

    Listen::Listen(const in_port_t port) : port(port) {
        auto listenThread = connect();
        listenThread.detach();
    }

    std::thread Listen::connect() {
        return std::thread([this] { listening(); });
    }

    int Listen::listening() {
        std::string id_str("[node - " + std::to_string(port) + "] - Listen");

        trace.print(std::clog, id_str + " - listening server started on port" + std::to_string(port));

        // Create acceptor
        sockpp::error_code ec;
        sockpp::tcp_acceptor acc{port, 40, ec}; // Create acceptor
        if (ec) {
            trace.print(std::cerr, id_str + " - listening server - Error creating the acceptor: " + ec.message());
            return 1;
        }

        trace.print(std::clog,
                    id_str + " - listening server - Awaiting connections on port " + std::to_string(port) + "...");
        while (true) {
            sockpp::inet_address address;
            if (auto res = acc.accept(&address); !res) {
                trace.print(std::cerr, id_str + "- error " + res.error_message());
                std::this_thread::sleep_for(threadSleep_s(1000, 2000));
            } else {
                trace.print(std::clog, id_str + " - listening Server started on port " + std::to_string(port));
                sockpp::tcp_socket socket = res.release();
                Server server(port);
                auto serverThread = server.run(socket);
                serverThread.join();
            }
        }
    }
}