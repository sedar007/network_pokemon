set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(sockpp CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)


set(INCROOT "${PROJECT_SOURCE_DIR}/include/networkPokemon")
set(SRCROOT "${PROJECT_SOURCE_DIR}/src/networkPokemon")

set(SRC
	"${SRCROOT}/pch.h"
    "${SRCROOT}/node.cpp"
	"${SRCROOT}/helper.cpp"
    "${SRCROOT}/networkNode.cpp"
    "${SRCROOT}/resourceManager.cpp"
    "${SRCROOT}/trace.cpp"
    "${SRCROOT}/server.cpp"
    "${SRCROOT}/test.cpp"
    "${SRCROOT}/thread_pool.cpp"
    "${SRCROOT}/session.cpp"
    "${SRCROOT}/main.cpp"
	"${SRCROOT}/client.cpp"
    "${SRCROOT}/command/ip_command.cpp"
    "${SRCROOT}/pictures_command.cpp"
    "${SRCROOT}/alive_command.cpp"
    "${SRCROOT}/command/add_new_node_command.cpp"
    "${SRCROOT}/image_data_command.cpp"
    "${SRCROOT}/utils.cpp"

    "${SRCROOT}/command/command_dispatcher.cpp"
    "${SRCROOT}/peer_registry.cpp"
    "${SRCROOT}/image_repository.cpp"
 )

add_library(${PROJECT_NAME}-network-pokemon
	STATIC
	        ${SRC}
)

target_include_directories(${PROJECT_NAME}-network-pokemon
	PUBLIC
        ${PROJECT_SOURCE_DIR}/include
		${sockpp_INCLUDE_DIRS}
        ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}-network-pokemon
		PUBLIC
		Sockpp::sockpp-static
	PRIVATE
		Threads::Threads
        nlohmann_json::nlohmann_json
        OpenSSL::SSL
        OpenSSL::Crypto
        ${PROJECT_NAME}-model
        ${PROJECT_NAME}-storage
        ${PROJECT_NAME}-sockpp
)

target_compile_options(${PROJECT_NAME}-network-pokemon
	PRIVATE
	        $<$<CXX_COMPILER_ID:MSVC>:${MSVC_WARNINGS}>
		$<$<CXX_COMPILER_ID:GNU>:${GCC_WARNINGS}>
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall -Wextra>
		$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_DEBUG_OPTIMISATION}>
		$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:${GCC_DEBUG_OPTIMISATION}>
		$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_RELEASE_OPTIMISATION}>
		$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:${GCC_RELEASE_OPTIMISATION}>
)

target_precompile_headers(${PROJECT_NAME}-network-pokemon
	PRIVATE
	        "${SRCROOT}/pch.h"
)

target_compile_definitions(${PROJECT_NAME}-network-pokemon
	PRIVATE
        NETWORK_POKEMON_EXPORTS
)
