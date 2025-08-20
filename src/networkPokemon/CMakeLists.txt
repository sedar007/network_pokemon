set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(sockpp CONFIG REQUIRED)
find_package(Threads REQUIRED)

set(INCROOT "${PROJECT_SOURCE_DIR}/include/networkPokemon")
set(SRCROOT "${PROJECT_SOURCE_DIR}/src/networkPokemon")

set(SRC
	"${SRCROOT}/pch.h"
	"${SRCROOT}/helper.cpp"
	"${SRCROOT}/trace.cpp"
	"${SRCROOT}/resourceManager.cpp"
	"${SRCROOT}/main.cpp"
		"${SRCROOT}/test.cpp"

	#"${SRCROOT}/client.cpp"
	"${SRCROOT}/node.cpp"


	        "${INCROOT}/modbus_master.h"

	"${SRCROOT}/pch.cpp"
	"${SRCROOT}/modbus_master.cpp"

	#"${SRCROOT}/main.cpp"
	"${SRCROOT}/client.cpp"
	"${SRCROOT}/server.cpp"
	#"${SRCROOT}/helper.cpp"
		"${SRCROOT}/listen.cpp"

 )

add_library(${PROJECT_NAME}
	SHARED
	        ${SRC}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	        ${PROJECT_SOURCE_DIR}/include
		${sockpp_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
		PUBLIC
		Sockpp::sockpp-static
	PRIVATE

		Threads::Threads
)

target_compile_options(${PROJECT_NAME}
	PRIVATE
	        $<$<CXX_COMPILER_ID:MSVC>:${MSVC_WARNINGS}>
		$<$<CXX_COMPILER_ID:GNU>:${GCC_WARNINGS}>
		$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_DEBUG_OPTIMISATION}>
		$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:${GCC_DEBUG_OPTIMISATION}>
		$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_RELEASE_OPTIMISATION}>
		$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:${GCC_RELEASE_OPTIMISATION}>
)

target_precompile_headers(${PROJECT_NAME}
	PRIVATE
	        "${SRCROOT}/pch.h"
)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
	        NETWORK_POKEMON_CLASS_EXPORTS
)
