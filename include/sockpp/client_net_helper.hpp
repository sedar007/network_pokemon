#pragma once

namespace pokemon::tcp {

    template<typename T>
    class NETWORK_SOCKPP_POKEMON_API ClientNetHelper {
        public:
             ClientNetHelper() = default;


             [[nodiscard]] int server_send_response(T& t, std::shared_ptr<tcp::IConnection> socket, command_dispatcher<T>& dispatcher) {
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


            [[nodiscard]] int client_ask_to_server(T& t, std::shared_ptr<tcp::IConnection> socket, command_client_dispatcher<T>& dispatcher, std::string_view ip, int port, std::string_view msg) {

                     std::string knowPortStr = std::to_string(port);
                     std::string ip_str(ip);
                     auto connector = std::make_shared<tcp_connector>();

                 //    getTrace().print(std::clog, std::format(MSG_CLIENT_TRYING_TO_CONNECT,
                   //                              std::format(MSG_NODE_ID, getPort(), CLIENT), knowPortStr));


                     if (!connector->connect(ip, port)) {
                     //    getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_CONNECTING,
                       //                              std::format(MSG_NODE_ID, getPort(), CLIENT), neighbour_ip, neighbour_port));
                        return -1;
                    }

                  //       getTrace().print(std::clog, std::format(MSG_CLIENT_CONNECTED,
                    //                                 std::format(MSG_NODE_ID, getPort(), CLIENT), neighbour_ip, neighbour_port));


                         if (!connector->write(msg)) {
                 //            getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_WRITING_TCP_STREAM,
               //                                          std::format(MSG_NODE_ID, getPort(), CLIENT), connector->last_error_str()));
                  //           connector->shutdown(SHUT_RDWR);
                             return 1;
                         }

                         PROTOCOL protocol = string_to_protocol(msg);

                         if (protocol == PROTOCOL::UNKNOWN) {
                  //           connector->shutdown(SHUT_RDWR);
                             return 1;
                         }

                        dispatcher.dispatch_client_read(t, protocol, std::move(connector));
                 return 0;
            }




    };
}


