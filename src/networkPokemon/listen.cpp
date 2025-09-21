#include "pch.h"

namespace pokemon {

    Listen::Listen(const in_port_t port) noexcept : port(port)  {
        auto listenThread = connect();
        listenThread.detach();
    }

    std::thread Listen::connect() {
        return std::thread([this] { listening(); });
    }

    void Listen::listening() {
        trace.print(std::clog, std::format(MSG_LISTENING_START, std::format(MSG_NODE_ID, port, LISTEN)));

        // Create acceptor
        sockpp::tcp_acceptor acc(port);
        if (!acc) {
            trace.print(std::cerr, std::format(MSG_LISTENING_ERROR_CREATING_ACCEPTOR, std::format(MSG_NODE_ID, port, LISTEN)));
            throw std::runtime_error(MSG_ERROR_CREATING_ACCEPTOR);
        }

        while (true) {
            trace.print(std::cerr, std::format(MSG_LISTENING_AWAITING_CONNECTION, std::format(MSG_NODE_ID, port, LISTEN), acc.last_error_str()));
            std::unique_ptr<sockpp::tcp_socket> sock = std::make_unique<sockpp::tcp_socket>(acc.accept()) ;

            if (!sock) {
                trace.print(std::cerr, std::format(MSG_LISTENING_ERROR_ACCEPTING_ACCEPTOR, std::format(MSG_NODE_ID, port, LISTEN), acc.last_error_str()));
                std::this_thread::sleep_for(threadSleep_s(LISTEN_ERROR_CONNECTION_SLEEP_RANGE_BEGIN, LISTEN_ERROR_CONNECTION_SLEEP_RANGE_END));
            }
            else {
                trace.print(std::clog, std::format(MSG_LISTENING_ACCEPTING_CONNECTION, std::format(MSG_NODE_ID, port, LISTEN)));
                Server server(port);
                auto serverThread = server.run(std::move(sock));
                serverThread.detach();
            }
        }
    }
}
