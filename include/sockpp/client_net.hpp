#pragma once

namespace pokemon::tcp {

    template<typename T>
    class NETWORK_SOCKPP_POKEMON_API client_net {
        public:
             client_net() = default;

            [[nodiscard]] int client_ask_to_server(T& t, command_client_dispatcher<T>& dispatcher, std::string_view ip, int port, std::string_view msg) {

                     std::string knowPortStr = std::to_string(port);
                     std::string ip_str(ip);

                    if (ip_str.empty() || port == 0 || msg.empty() || !tcp::is_valid_ip_address(ip_str)) {
                        return -1;
                    }
                     auto connector = std::make_shared<tcp_connector>();

                 //    getTrace().print(std::clog, std::format(MSG_CLIENT_TRYING_TO_CONNECT,
                   //                              std::format(MSG_NODE_ID, getPort(), CLIENT), knowPortStr));


                     if (!connector->connect(ip_str, port)) {
                         std::cout << "[client_net] echec de connexion a " << ip_str << ":" << knowPortStr << std::endl;
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

                         const size_t prefix_len = msg.size() < PROTOCOL_SIZE ? msg.size() : PROTOCOL_SIZE;
                         const std::string_view protocol_prefix = msg.substr(0, prefix_len);
                         PROTOCOL protocol = string_to_protocol(protocol_prefix);

                         if (protocol == PROTOCOL::UNKNOWN) {
                  //           connector->shutdown(SHUT_RDWR);
                             return 1;
                         }

                        dispatcher.dispatch_client_read(t, protocol, std::move(connector));
                 return 0;
            }




    };
}


