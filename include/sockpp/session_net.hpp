#pragma once

namespace pokemon::tcp {

    template<typename T>
    class NETWORK_SOCKPP_POKEMON_API session_net {
        public:
             session_net() = default;

             [[nodiscard]] int server_send_response(T& t, std::shared_ptr<tcp::IConnection> socket, command_session_dispatcher<T>& dispatcher) {
                 if (!socket || !socket->is_open()) {
                     return -1;
                 }

                 char protocol_buf[PROTOCOL_SIZE];

                 // getTrace().print(std::clog, std::format(MSG_SERVER_RECEIVED_CONNECTION, std::format(MSG_NODE_ID, getPort(), SERVER),
                 //                                 socket->address(), getPort()));

                 if (const auto res = socket->read(protocol_buf, sizeof(protocol_buf)); res <= 0) {
                     return -1;
                 }

                 std::string protocol_str(protocol_buf, PROTOCOL_SIZE);
                 //  getTrace().print(std::clog, std::format(MSG_SERVER_RECEIVED_QUERY, std::format(MSG_NODE_ID, getPort(), SERVER),
                 //                                         protocol_str));
                std::cout << "send request: " << protocol_str << std::endl;
                dispatcher.dispatch_send_to_client(t, string_to_protocol(protocol_str), std::move(socket));

                 return 0;
             }

    };
}


