set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(sockpp CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(INCROOT "${PROJECT_SOURCE_DIR}/include/networkPokemon")
set(SRCROOT "${PROJECT_SOURCE_DIR}/src/networkPokemon")

set(SRC
	"${SRCROOT}/pch.h"
    "${SRCROOT}/json.cpp"
    "${SRCROOT}/node.cpp"
	"${SRCROOT}/helper.cpp"
    "${SRCROOT}/networkNode.cpp"
    "${SRCROOT}/server.cpp"
    "${SRCROOT}/resourceManager.cpp"
    "${SRCROOT}/trace.cpp"
    "${SRCROOT}/listen.cpp"
    "${SRCROOT}/test.cpp"
    "${SRCROOT}/thread_pool.cpp"
    "${SRCROOT}/server.cpp"
    "${SRCROOT}/main.cpp"
	"${SRCROOT}/client.cpp"




        #"${SRCROOT}/pch.cpp"

	#"${SRCROOT}/main.cpp"
        #"${SRCROOT}/client.cpp"

	#"${SRCROOT}/helper.cpp"


 )

add_library(${PROJECT_NAME}-network-pokemon
	STATIC
	        ${SRC}
)

target_include_directories(${PROJECT_NAME}-network-pokemon
	PUBLIC
        ${PROJECT_SOURCE_DIR}/include
		${sockpp_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}-network-pokemon
		PUBLIC
		Sockpp::sockpp-static
	PRIVATE
		Threads::Threads
        nlohmann_json::nlohmann_json
        ${PROJECT_NAME}-model
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
