include_directories(${CMAKE_SOURCE_DIR}/src)

set(sockpp_hpp sockpp/acceptor.h sockpp/connector.h sockpp/inet_address.h sockpp/sock_address.h sockpp/tcp6_connector.h sockpp/tcp_socket.h sockpp/unix_acceptor.h sockpp/unix_stream_socket.h
        sockpp/can_address.h sockpp/datagram_socket.h sockpp/platform.h sockpp/socket.h sockpp/tcp6_socket.h sockpp/types.h sockpp/unix_address.h
        sockpp/can_frame.h sockpp/error.h sockpp/raw_socket.h sockpp/stream_socket.h sockpp/tcp_acceptor.h sockpp/udp6_socket.h sockpp/unix_connector.h
        sockpp/can_socket.h sockpp/inet6_address.h sockpp/result.h sockpp/tcp6_acceptor.h sockpp/tcp_connector.h sockpp/udp_socket.h sockpp/unix_dgram_socket.h)

set(sockpp_cpp sockpp/acceptor.cpp sockpp/connector.cpp sockpp/datagram_socket.cpp sockpp/error.cpp sockpp/inet6_address.cpp
        sockpp/inet_address.cpp sockpp/socket.cpp sockpp/stream_socket.cpp  sockpp/unix/unix_address.cpp)

set(pokemon_hpp hpp/client.hpp hpp/main.hpp hpp/listen.hpp hpp/node.hpp hpp/resourceManager.hpp hpp/server.hpp hpp/trace.hpp hpp/helper.hpp hpp/const.hpp)

set(pokemon_cpp cpp/client.cpp cpp/main.cpp cpp/listen.cpp cpp/node.cpp cpp/resourceManager.cpp cpp/server.cpp cpp/trace.cpp cpp/helper.cpp)

add_library(pokemon ${pokemon_hpp} ${pokemon_cpp})

target_link_libraries(pokemon)


add_executable(main main.cpp ${sockpp_hpp} ${sockpp_cpp})

target_link_libraries(main Threads::Threads pokemon)



